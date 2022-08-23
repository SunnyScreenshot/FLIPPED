cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME PicShot)
set(PROJECT_VERSION 0.3)
set(_version ${PROJECT_VERSION})

project(${PROJECT_NAME})   
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
file(COPY config/config.ini DESTINATION ${CMAKE_BINARY_DIR}/bin)

# msvc multicore compilation
if(MSVC)
    OPTION(USE_MP "use multiple" ON)
    OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP
            "Set The Global Option COMPILE_FLAGS /MP to target." ON)
    if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()

    set(VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(SRC_MAIN
    xglobal.h
    main.cpp)

set(SRC_CORE
    core/isingleton.h
    core/xlog.h)

set(SRC_PLATFORM
    platform/iwininfo.h
    platform/iwininfo.cpp
    platform/wininfo.h
    platform/wininfo.cpp)

if(APPLE)
elseif(WIN32)
    list(APPEND SRC_PLATFORM
        platform/wininfo_win.h
        platform/wininfo_win.cpp)
else()
    list(APPEND SRC_PLATFORM
        platform/wininfo_x11.h
        platform/wininfo_x11.cpp)
endif()

#----------- Test begin -----------
#foreach(_rm_file ${SRC_PLATFORM})
#    message(${_rm_file})
#endforeach()
#----------- Test end -----------

set(SRC_WIDGET
    widget/xhorizontalline.h
    widget/xhorizontalline.cpp
    widget/xverticalline.h
    widget/xverticalline.cpp
    widget/xkeysequenceedit.h
    widget/xkeysequenceedit.cpp
    widget/xroundwidget.h
    widget/xroundwidget.cpp
    widget/xtextwidget.h
    widget/xtextwidget.cpp
    widget/xlabel.h
    widget/xlabel.cpp
    widget/xframewidget.h
    widget/xframewidget.cpp
    widget/xcombobox.h
    widget/xcombobox.cpp)

set(SRC_TOOL
    tool/base/colorparabar.h
    tool/base/colorparabar.cpp
    tool/base/managebar.h
    tool/base/managebar.cpp
    tool/base/blurwidget.h
    tool/base/blurwidget.cpp
    tool/selectsize/selectsize.h
    tool/selectsize/selectsize.cpp
    tool/magnifier/magnifier.h
    tool/magnifier/magnifier.cpp
    tool/pin/pinwidget.h
    tool/pin/pinwidget.cpp
    tool/selectbar.h
    tool/selectbar.cpp
    tool/parameterbar.h
    tool/parameterbar.cpp)

set(SRC_PERFERENCE
    preference/preference.h
    preference/preference.cpp
    preference/hotkeyswidget.h
    preference/hotkeyswidget.cpp
    preference/appellation.h
    preference/appellation.cpp)

set(SRC_WINSCREEN
    screen/drawhelper.h
    screen/drawhelper.cpp
    screen/rectcalcu.h
    screen/rectcalcu.cpp
    screen/screenshot.h
    screen/screenshot.cpp
    screen/tray.h
    screen/tray.cpp)

set(SRC_EXAMPLE
    example/exwidget.h
    example/exwidget.cpp)

set(SRC_PLUGIN_INTERFACE
    pluginsinterface/iplugininterface.h
    pluginsinterface/iplugininterface.cpp)

set(SRC_RESOURCES
    Resources.qrc
    logo.rc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT5_LIBS_LINK
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        Qt5::Xml)
# I18
set(TS_FILES
    i18n/zh_CN.ts
    i18n/zh_TW.ts
    i18n/es_US.ts)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        Xml
        LinguistTools
        REQUIRED)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})  # .ts transition .qm
# message("#2---->" ${QM_FILES})
#1. 自动批量生成 *.ts 文件
#2. 每次都能够更新 *ts 的增量
#3. 每次 清理 不会删除 .ts 文件
#4. 能够自定义导出 .ts 和 .qm 文件

##编译时更新.ts .qm
#foreach(_ts_file ${TS_FILES})
#    execute_process(COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_SOURCE_DIR} -ts ${_ts_file})
#    execute_process(COMMAND lrelease ${_ts_file})
#endforeach()

#foreach(_qm_file ${QM_FILES})
#    message("#b---->" ${_qm_file})
#    message("#b2---->" ${EXECUTABLE_OUTPUT_PATH}/i18n)
#    if(EXISTS ${_qm_file})
#        message("#a---->" ${_qm_file})
#        message("#a2---->" ${EXECUTABLE_OUTPUT_PATH}/i18n)
#        file(COPY ${_qm_file} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/i18n)
#    endif()
#endforeach()

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/3rdparty/QHotkey/QHotkey)
link_directories(${CMAKE_BINARY_DIR}/bin
    ${CMAKE_BINARY_DIR}/3rdparty/QHotkey)

if (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.XMuli.PicShot")
    set(MACOSX_BUNDLE_INFO_STRING "A cross-platform screenshot software")
    # set(MACOSX_BUNDLE_ICON_FILE "xxx.icns")
    # set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${SRC_MAIN}
    ${SRC_PLATFORM}
    ${SRC_CORE}
    ${SRC_WIDGET}
    ${SRC_TOOL}
    ${SRC_PERFERENCE}
    ${SRC_WINSCREEN}
    ${SRC_EXAMPLE}
    ${SRC_PLUGIN_INTERFACE}
    ${SRC_RESOURCES}
    #${QM_FILES}
    )
else()
    add_executable(${PROJECT_NAME} WIN32
    ${SRC_MAIN}
    ${SRC_PLATFORM}
    ${SRC_CORE}
    ${SRC_WIDGET}
    ${SRC_TOOL}
    ${SRC_PERFERENCE}
    ${SRC_WINSCREEN}
    ${SRC_EXAMPLE}
    ${SRC_PLUGIN_INTERFACE}
    ${SRC_RESOURCES}
    #${QM_FILES}
    )
endif()

IF (WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK}
        qhotkey)
ELSEIF (APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK}
        qhotkey)
ELSEIF (UNIX)
    target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK}
        qhotkey
        X11)
ENDIF ()

# 文件归类, ref:
# https://stackoverflow.com/questions/33808087/cmake-how-to-create-visual-studio-filters
# https://blog.csdn.net/gzj2013/article/details/102619480
set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")

source_group(TREE ${_src_root_path} FILES ${_source_list})                                   # will be organized according to the actual directory structure, .h.cpp is put together


# Define some variables that can be used in *.cpp
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_VERSION="${_version}")
target_compile_definitions(${PROJECT_NAME} PUBLIC _BIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # x64/x86
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        UNICODE
        _UNICODE)
endif()

message("==========BRGIN============")
message("_PROJECT_NAME:${PROJECT_NAME}")
message("_PROJECT_VERSION:" ${_version})
message("_BIT_ARCH:" ${_bit_arch})
message("_COMPILER:" ${CMAKE_HOST_SYSTEM_PROCESSOR})
message("_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message("==========END============")

#  CMAKE_INSTALL_PREFIX:   /usr/local   或 c:/Program Files/${PROJECT_NAME}
#if(WIN32)
#    set (BIN_INSTALL_DIR "") # seems useless, don't know why...
#else()
#    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#        set(CMAKE_INSTALL_PREFIX /usr)
#    endif ()
#
#    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
#endif()
