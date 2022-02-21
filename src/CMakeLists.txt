add_subdirectory(themes)

set(VIRTUALKEYBOARDUI_WAYLAND_SRCS)
set(VIRTUALKEYBOARDUI_WAYLAND_LIBS)
if (WAYLAND_FOUND)
    set(VIRTUALKEYBOARDUI_WAYLAND_SRCS ${VIRTUALKEYBOARDUI_WAYLAND_SRCS}
        waylandui.cpp waylandwindow.cpp waylandeglwindow.cpp waylandshmwindow.cpp
        waylandpointer.cpp buffer.cpp waylandinputwindow.cpp)
    set(VIRTUALKEYBOARDUI_WAYLAND_LIBS ${VIRTUALKEYBOARDUI_WAYLAND_LIBS}
        Fcitx5::Module::Wayland Wayland::Client Fcitx5::Wayland::Core
        Fcitx5::Wayland::InputMethod)
else()
endif()

if (ENABLE_X11)
    set(VIRTUALKEYBOARDUI_WAYLAND_SRCS ${VIRTUALKEYBOARDUI_WAYLAND_SRCS}
        xcbui.cpp xcbwindow.cpp xcbtraywindow.cpp xcbinputwindow.cpp xcbmenu.cpp)
    set(VIRTUALKEYBOARDUI_WAYLAND_LIBS ${VIRTUALKEYBOARDUI_WAYLAND_LIBS} PkgConfig::CairoXCB Fcitx5::Module::XCB
        XCB::AUX XCB::ICCCM XCB::XINERAMA XCB::RANDR XCB::EWMH)
endif()

add_library(virtualkeyboardui MODULE
    classicui.cpp window.cpp theme.cpp inputwindow.cpp virtualkeyboard.cpp virtualkeygeneral.cpp
    virtualkeyboardi18n.cpp virtualkeyboardanthy.cpp virtualkeyboardpinyin.cpp virtualkeyboardrussian.cpp
    virtualkeyboardhangul.cpp virtualkeyboardchewing.cpp virtualkeyboardus.cpp ${VIRTUALKEYBOARDUI_WAYLAND_SRCS}
    )

if (CAIRO_EGL_FOUND)
set(CAIRO_EGL_LIBRARY PkgConfig::CairoEGL Wayland::Egl EGL::EGL)
else()
set(CAIRO_EGL_LIBRARY)
endif()

target_link_libraries(virtualkeyboardui
    Fcitx5::Core
    PkgConfig::Cairo PkgConfig::Pango
    PkgConfig::GdkPixbuf PkgConfig::GioUnix
    Fcitx5::Module::NotificationItem
    ${CAIRO_EGL_LIBRARY}
    ${VIRTUALKEYBOARDUI_WAYLAND_LIBS}
    ${FMT_TARGET})
install(TARGETS virtualkeyboardui DESTINATION "${FCITX_INSTALL_ADDONDIR}")
configure_file(virtualkeyboardui.conf.in.in virtualkeyboardui.conf.in @ONLY)
fcitx5_translate_desktop_file(${CMAKE_CURRENT_BINARY_DIR}/virtualkeyboardui.conf.in virtualkeyboardui.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/virtualkeyboardui.conf" DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")

add_subdirectory(fcitx-wayland)
