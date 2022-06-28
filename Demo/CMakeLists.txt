cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME Demo)
set(PROJECT_VERSION 1.0)
project(${PROJECT_NAME} LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../bin) # 更改二进制输出路径

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

# msvc multicore compilation
if (WIN32)
    if(MSVC)
        add_definitions(-DUNICODE -D_UNICODE)   # 设置 DUNICODE 编码

                OPTION(USE_MP "use multiple" ON)
                OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP
                        "Set The Global Option COMPILE_FLAGS /MP to target." ON)
                if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
                        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
                endif()
                set(VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif(MSVC)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(SRCS_FILES
        main.cpp
        exwidget.cpp
        exwidget.h)

if (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.XMuli.PicShot")
    set(MACOSX_BUNDLE_INFO_STRING "A cross-platform screenshot software")
    # set(MACOSX_BUNDLE_ICON_FILE "xxx.icns")
    # set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${SRCS_FILES})
else()
    add_executable(${PROJECT_NAME} WIN32
        ${SRCS_FILES})
endif()

set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR}) # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")
source_group(TREE ${_src_root_path} FILES ${_source_list})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

