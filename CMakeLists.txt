cmake_minimum_required(VERSION 3.6)

project(fcitx5-virtualkeyboard-ui VERSION 5.0.0)

find_package(ECM 1.0.0 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(FeatureSummary)
include(GNUInstallDirs)
include(ECMUninstallTarget)

set(REQUIRED_FCITX5_VERSION 5.0.8)
find_package(Fcitx5Core "${REQUIRED_FCITX5_VERSION}" REQUIRED)
find_package(Fcitx5Module REQUIRED COMPONENTS NotificationItem)
find_package(Gettext REQUIRED)
find_package(PkgConfig REQUIRED)

if (NOT (TARGET fmt::fmt OR TARGET fmt::fmt-header-only))
    find_package(fmt REQUIRED)
endif()

if (TARGET fmt::fmt-header-only)
  set(FMT_TARGET fmt::fmt-header-only)
else()
  set(FMT_TARGET fmt::fmt)
endif ()

pkg_check_modules(Cairo IMPORTED_TARGET cairo)
pkg_check_modules(Pango IMPORTED_TARGET pango pangocairo)
pkg_check_modules(GdkPixbuf IMPORTED_TARGET gdk-pixbuf-2.0)
pkg_check_modules(GioUnix IMPORTED_TARGET gio-unix-2.0)

option(ENABLE_X11 "Enable X11 support" On)
option(ENABLE_WAYLAND "Enable wayland support" On)
option(INSTALL_THEME "Install theme files" Off)
option(ENABLE_CUSTOM_LAYOUT "Use custom keyboard layout files (JSON)" Off)

if (ENABLE_X11)
    set(REQUIRED_XKBCOMMON_COMPONENTS ${REQUIRED_XKBCOMMON_COMPONENTS} X11)
    find_package(XCB REQUIRED COMPONENTS XCB AUX XKB XFIXES ICCCM XINERAMA RANDR EWMH KEYSYMS)
    pkg_check_modules(CairoXCB IMPORTED_TARGET cairo-xcb)
    find_package(Fcitx5Module REQUIRED COMPONENTS XCB)
endif()

if (ENABLE_WAYLAND)
    find_package(Wayland REQUIRED COMPONENTS Client Egl)
    find_package(WaylandScanner REQUIRED)
    find_package(WaylandProtocols REQUIRED)
    pkg_get_variable(WaylandProtocols_PKGDATADIR wayland-protocols pkgdatadir)
    mark_as_advanced(WaylandProtocols_PKGDATADIR)
    string(REGEX REPLACE "[\r\n]" "" WaylandProtocols_PKGDATADIR "${WaylandProtocols_PKGDATADIR}")
    set(WAYLAND_PROTOCOLS_PKGDATADIR ${WaylandProtocols_PKGDATADIR})
    find_package(EGL)
    find_package(Fcitx5Module REQUIRED COMPONENTS Wayland)

endif()

include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")
add_definitions(-DFCITX_GETTEXT_DOMAIN=\"fcitx5-virtualkeyboard-ui\" -D_GNU_SOURCE)
fcitx5_add_i18n_definition()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
