cmake_minimum_required(VERSION 3.6)

project(fcitx5-virtualkeyboard VERSION 5.0.0)

find_package(ECM 1.0.0 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(FeatureSummary)
include(GNUInstallDirs)
include(ECMUninstallTarget)

find_package(Fcitx5Core 5.0.14 REQUIRED)
find_package(Gettext REQUIRED)
find_package(PkgConfig REQUIRED)

option(ENABLE_X11 "Enable X11 support" On)
option(ENABLE_WAYLAND "Enable wayland support" On)

if (ENABLE_X11 OR ENABLE_WAYLAND)
    pkg_check_modules(Cairo IMPORTED_TARGET cairo)
    pkg_check_modules(Pango IMPORTED_TARGET pango pangocairo)
    pkg_check_modules(GdkPixbuf IMPORTED_TARGET gdk-pixbuf-2.0)
    pkg_check_modules(GioUnix IMPORTED_TARGET gio-unix-2.0)
endif()

if (ENABLE_X11)
    find_package(Fcitx5Module REQUIRED COMPONENTS XCB)
endif()

if (ENABLE_WAYLAND)
    find_package(Fcitx5Module REQUIRED COMPONENTS Wayland)
endif()

include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")
add_definitions(-DFCITX_GETTEXT_DOMAIN=\"fcitx5-virtualkeyboard\" -D_GNU_SOURCE)
fcitx5_add_i18n_definition()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
