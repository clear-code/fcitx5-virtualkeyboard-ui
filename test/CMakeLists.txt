set(VIRTUALKEYBOARD_LAYOUT_TEST
    testkeyboardanthy
    testkeyboardchewing
    testkeyboardlayout
    testkeyboardmetadata
    testkeyboardpinyin)

set(VIRTUALKEYBOARD_LAYOUT_SRC
    ../src/virtualkeyboardlayout.cpp
    ../src/virtualkeyboard.cpp
    ../src/virtualkeyboardanthy.cpp
    ../src/virtualkeyboardchewing.cpp
    ../src/virtualkeyboardhangul.cpp
    ../src/virtualkeyboardi18n.cpp
    ../src/virtualkeyboardpinyin.cpp
    ../src/virtualkeyboardrussian.cpp
    ../src/virtualkeyboardus.cpp
    ../src/virtualkeygeneral.cpp
    testkeyboardutils.cpp)

foreach(TESTCASE ${VIRTUALKEYBOARD_LAYOUT_TEST})
    add_executable(${TESTCASE} ${TESTCASE}.cpp ${VIRTUALKEYBOARD_LAYOUT_SRC})
    target_link_libraries(${TESTCASE}
        Fcitx5::Core Fcitx5::Utils
	PkgConfig::Cairo PkgConfig::Pango
	PkgConfig::GdkPixbuf PkgConfig::GioUnix
	PkgConfig::JsonC
	${${TESTCASE}_LIBS})
    target_include_directories(${TESTCASE} PRIVATE ../src)
    set(FCITX_INSTALL_PKGDATADIR "${CMAKE_INSTALL_PREFIX}/share/fcitx5")
    target_compile_definitions(${TESTCASE} PUBLIC FCITX_INSTALL_PKGDATADIR="${FCITX_INSTALL_PKGDATADIR}")
    target_compile_definitions(${TESTCASE} PUBLIC VIRTUALKEYBOARDUI_TEST_DIR="${CMAKE_SOURCE_DIR}/test/")
    target_compile_definitions(${TESTCASE} PUBLIC USE_CUSTOM_LAYOUT=1)
    target_compile_definitions(${TESTCASE} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
    add_test(NAME ${TESTCASE}
             COMMAND ${TESTCASE} ${${TESTCASE}_ARGS})
endforeach()
