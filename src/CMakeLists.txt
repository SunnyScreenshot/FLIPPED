cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME PicShot)
# SET(PROJECT_VERSION 0.0.13)
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
set(SRCS_MAIN
    "xglobal.h"
    "main.cpp"
    )

# core
FILE(GLOB_RECURSE SRCS_CORE
    "core/isingleton.h"
    "core/xlog/xlog.h"
    )

# wininfo
set(SRCS_WININFO
    "wininfo/iwininfo.h"
    "wininfo/iwininfo.cpp"
    )

# widget
set(SRCS_WIDGET
    "widget/xhorizontalline.h"
    "widget/xhorizontalline.cpp"
    "widget/xverticalline.h"
    "widget/xverticalline.cpp"
    "widget/xkeysequenceedit.h"
    "widget/xkeysequenceedit.cpp"
    "widget/xroundwidget.h"
    "widget/xroundwidget.cpp"
    "widget/xtextwidget.h"
    "widget/xtextwidget.cpp"
    )

set(SRCS_RESOURCES
    "Resources.qrc"
    "logo.rc"
    )

# screen
FILE(GLOB_RECURSE SRCS_WINSCREEN
    "./screen/*.h"
    "./screen/*.cpp"
)

set(CMAKE_CXX_STANDARD 17)
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

#add_compile_definitions(QT_MESSAGELOGCONTEXT  # Release 版本下也能输出文件名、行号等
#	QT_NO_DEBUG_OUTPUT     # 屏蔽 debug\info\warning 信息
##    QT_NO_INFO_OUTPUT
##    QT_NO_WARNING_OUTPUT
#    )

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/3rdparty/QHotkey/QHotkey
    )
link_directories(${CMAKE_BINARY_DIR}/bin
    ${CMAKE_BINARY_DIR}/3rdparty/QHotkey
    )

if (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.XMuli.PicShot")
    set(MACOSX_BUNDLE_INFO_STRING "A cross-platform screenshot software")
    # set(MACOSX_BUNDLE_ICON_FILE "shadowsocks-qt5.icns")
    # set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
	
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
    ${SRCS_MAIN}
    ${SRCS_WININFO}
    ${SRCS_CORE}
    ${SRCS_WIDGET}
    ${SRCS_WINSCREEN}
    ${SRCS_RESOURCES}
    )
else()
	add_executable(${PROJECT_NAME} WIN32
    ${SRCS_MAIN}
    ${SRCS_WININFO}
    ${SRCS_CORE}
    ${SRCS_WIDGET}
    ${SRCS_WINSCREEN}
    ${SRCS_RESOURCES}
    )
endif()

target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK}
        qhotkey
        )

#target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)

if(APPLE)
#    find_library(CARBON_LIBRARY Carbon)
#    mark_as_advanced(CARBON_LIBRARY)

#    target_sources(qhotkey PRIVATE QHotkey/qhotkey_mac.cpp)
#    target_link_libraries(qhotkey PRIVATE ${CARBON_LIBRARY})
elseif(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE wininfo/wininfo_win.h wininfo/wininfo_win.cpp)
else()
#    find_package(X11 REQUIRED)
#    if(QT_MAJOR GREATER_EQUAL 6)
#        target_link_libraries(qhotkey PRIVATE ${X11_LIBRARIES})
#    else()
#        find_package(Qt${QT_MAJOR} COMPONENTS X11Extras REQUIRED)
#        target_link_libraries(qhotkey
#            PRIVATE
#                ${X11_LIBRARIES}
#                Qt${QT_MAJOR}::X11Extras)
#    endif()

#    include_directories(${X11_INCLUDE_DIR})
#    target_sources(qhotkey PRIVATE QHotkey/qhotkey_x11.cpp)
endif()

source_group("" FILES ${SRCS_MAIN})
source_group("core" FILES ${SRCS_CORE})
source_group("wininfo" FILES ${SRCS_WININFO})
source_group("widget" FILES ${SRCS_WIDGET})
source_group("screen" FILES ${SRCS_WINSCREEN})


#  CMAKE_INSTALL_PREFIX:   /usr/local   或 c:/Program Files/${PROJECT_NAME}
if(APPLE)

elseif(WIN32)
    # install(TARGETS ${PROJECT_NAME} RUNTIME)
else()
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
endif()
