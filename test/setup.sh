#!/bin/bash

set -xe

run() {
    "$@"
    result=$?
    if [ $result -ne 0 ]
    then
	echo "Failed: $@ [$PWD]" >&2
	exit $result
    fi
    return 0
}

build_with_autogen() {
    TARGET=$1
    cd $TARGET
    run ./autogen.sh --prefix=/opt/fcitx508
    run make -j$(nproc)
    run sudo make install
    cd -
}

build_with_cmake() {
    TARGET=$1
    cd $TARGET
    OPTIONS=" -DCMAKE_INSTALL_PREFIX=/opt/fcitx508"
    case $TARGET in
	xcb-imdkit|fcitx5)
	    # use master
	    :
	    ;;
	libime)
	    # newer version can't build with fcitx-5.0.8
	    git submodule update --init
	    run git checkout 1.1.1
	    ;;
	fcitx5-chinese-addons)
	    OPTIONS="$OPTIONS -DCMAKE_PREFIX_PATH=/opt/fcitx508 -DENABLE_GUI=off"
	    run git checkout $FCITX_TARGET_VERSION
	    ;;
	*)
	    run git checkout $FCITX_TARGET_VERSION
	    ;;
    esac
    run git branch -a
    run cmake -B build $OPTIONS
    run make -C build -j$(nproc)
    run sudo make -C build install
    cd -
}

MODULES="
xcb-imdkit
anthy-unicode
fcitx5
libime
fcitx5-lua
fcitx5-anthy
fcitx5-chewing
fcitx5-hangul 
fcitx5-chinese-addons
"

FCITX_TARGET_VERSION=$1

case $FCITX_TARGET_VERSION in
    5.0.8)
	echo "::group::Install dependency packages"
	sudo apt-get build-dep -y fcitx5
	sudo apt-get purge -y libxcb-imdkit*
	sudo apt-get install -y unicode-cldr-core itstool appstream # for fcitx5
	sudo apt-get install -y libboost-program-options-dev libboost-dev libboost-iostreams-dev libboost-filesystem-dev \
	     liblua5.3-dev libopencc-dev # for libime
	sudo apt-get install -y libchewing3-dev # for chewing
	sudo apt-get install -y libhangul-dev # for hangul
	sudo apt-get install -y libcurl4-openssl-dev # for chinese-addons
	echo "::endgroup::"
	for d in $MODULES; do
	    echo "BUILD: $d..."
	    export PKG_CONFIG_PATH=/opt/fcitx508/lib/pkgconfig
	    case $d in
		anthy-unicode)
		    if [ ! -d $d ]; then
			git clone https://github.com/fujiwarat/anthy-unicode.git
		    fi
		    echo "::group::Install anthy-unicode"
		    build_with_autogen $d
		    echo "::endgroup::"
		    ;;
		fcitx5-chinese-addons)
		    if [ ! -d $d ]; then
			git clone https://github.com/fcitx/$d.git
		    fi
		    echo "::group::Install fcitx5-chinese-addons"
		    build_with_cmake $d
		    echo "::endgroup::"
		    ;;
		fcitx5)
		    if [ ! -d $d ]; then
			git clone https://github.com/clear-code/fcitx5.git $d
		    fi
		    if [ ! -f /usr/share/ECM/find-modules/FindWaylandProtocols.cmake ] ; then
			# curl -o FindWaylandProtocols.cmake https://raw.githubusercontent.com/KDE/extra-cmake-modules/master/find-modules/FindWaylandProtocols.cmake 
			sudo curl -o /usr/share/ECM/find-modules/FindWaylandProtocols.cmake \
			     https://raw.githubusercontent.com/KDE/extra-cmake-modules/master/find-modules/FindWaylandProtocols.cmake 
		    fi
		    (cd $d && git checkout 5.0.8+virtual-keyboard-prerequisites)
		    echo "::group::Install fcitx5"
		    build_with_cmake $d
		    echo "::endgroup::"
		    ;;
		fcitx-anthy|fcitx-chewing|fcitx-hangul|fcitx-lua)
		    if [ ! -d $d ]; then
			git clone https://github.com/fcitx/$d.git
		    fi
		    echo "::group::Install $d"
		    build_with_cmake $d
		    echo "::endgroup::"
		    ;;
		*)
		    if [ ! -d $d ]; then
			git clone https://github.com/fcitx/$d.git
		    fi
		    echo "::group::Install $d"
		    build_with_cmake $d
		    echo "::endgroup::"
		    ;;
	    esac
	done
	;;
    *)
	echo "Currently only fcitx-5.0.8 is supported."
	exit 1
	;;
esac
