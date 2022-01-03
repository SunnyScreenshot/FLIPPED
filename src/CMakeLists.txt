cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME PicShot)
#set(PROJECT_VERSION_MAJOR 0)
#set(PROJECT_VERSION_MINOR 1)

project(${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin) # 更改二进制输出路径

# 此处配合 VMSVC 的 UTF8-BOM 插件，达到跨平台
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message("---using Clang---")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message("---using GCC---")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  message("---using Intel C++---")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        message("---using Visual Studio C++---")
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# Sources files
# main
FILE(GLOB SRCS_MAIN
    "./*.h"
    "./*.cpp"
#    "main.cpp"
#    "xglobal.h"
    )

# syswininfo
FILE(GLOB_RECURSE SRCS_SYSWININFO
    "./syswininfo/*.h"
    "./syswininfo/*.cpp"
#    "syswininfo/syswininfo.h"
#    "syswininfo/syswininfo.cpp"
#    "syswininfo/syswininfo_win.h"
#    "syswininfo/syswininfo_win.cpp"
    )

# widget
FILE(GLOB_RECURSE SRCS_WIDGET
    "./widget/*.h"
    "./widget/*.cpp"
#    "widget/xkeysequenceedit.h"
#    "widget/xkeysequenceedit.cpp"
#    "widget/xroundwidget.h"
#    "widget/xroundwidget.cpp"
#    "widget/xhorizontalline.h"
#    "widget/xhorizontalline.cpp"
#    "widget/xverticalline.h"
#    "widget/xverticalline.cpp"
    )

# winscreen
FILE(GLOB_RECURSE SRCS_WINSCREEN
    "./winscreen/*.h"
    "./winscreen/*.cpp"
#    "winscreen/screenshot.h"
#    "winscreen/screenshot.cpp"
#    "winscreen/rectcalcu.h"
#    "winscreen/rectcalcu.cpp"
#    "winscreen/winmain.h"
#    "winscreen/winmain.cpp"
#    "winscreen/winresetbtn.h"
#    "winscreen/winresetbtn.cpp"
#    "winscreen/tray.h"
#    "winscreen/tray.cpp"
#    "winscreen/drawtoolbar.h"
#    "winscreen/drawtoolbar.cpp"
#    "winscreen/subabstoolbar.h"
#    "winscreen/subabstoolbar.cpp"
#    "winscreen/subgraptoolbar.h"
#    "winscreen/subgraptoolbar.cpp"
#    "winscreen/subrecttoolbar.h"
#    "winscreen/subrecttoolbar.cpp"
#    "winscreen/subellipsetoolbar.h"
#    "winscreen/subellipsetoolbar.cpp"
#    "winscreen/sublinetoolbar.h"
#    "winscreen/sublinetoolbar.cpp"
#    "winscreen/submosaictoolbar.h"
#    "winscreen/submosaictoolbar.cpp"
#    "winscreen/wincolorpick.h"
#    "winscreen/wincolorpick.cpp"
#    "winscreen/drawhelper.h"
#    "winscreen/drawhelper.cpp"
)

set(PICSHOT_RESOURCES
    Resources.qrc
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT5_LIBS_LINK
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

configure_file(${CMAKE_SOURCE_DIR}/config/log4qt.conf ${CMAKE_BINARY_DIR} COPYONLY)

add_compile_definitions(QT_MESSAGELOGCONTEXT  # Release 版本下也能输出文件名、行号等
#	QT_NO_DEBUG_OUTPUT     # 屏蔽 debug\info\warning 信息
#    QT_NO_INFO_OUTPUT
#    QT_NO_WARNING_OUTPUT
    )

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/Log4Qt/include
    ${CMAKE_SOURCE_DIR}/3rdparty/QHotkey/QHotkey
    )
link_directories(${CMAKE_BINARY_DIR}/bin
    ${CMAKE_BINARY_DIR}/3rdparty/QHotkey
    )

add_executable(${PROJECT_NAME}
    ${SRCS_MAIN}
    ${SRCS_SYSWININFO}
    ${SRCS_WIDGET}
    ${SRCS_WINSCREEN}
    ${PICSHOT_RESOURCES}
    )

target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK}
        log4qt
        qhotkey
        )

source_group("" FILES ${SRCS_MAIN})
source_group("syswininfo" FILES ${SRCS_SYSWININFO})
source_group("widget" FILES ${SRCS_WIDGET})
source_group("winscreen" FILES ${SRCS_WINSCREEN})

