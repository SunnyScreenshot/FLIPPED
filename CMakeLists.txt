cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME PicShot)
#set(PROJECT_VERSION_MAJOR 0)
#set(PROJECT_VERSION_MINOR 1)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT5_LIBS_LINK
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin) # 更改二进制输出路径

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

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(SOURCE_CODE_PATH
        main.cpp

        WinScreen/winfullscreen.cpp
        WinScreen/winfullscreen.h)

add_executable(${PROJECT_NAME} ${SOURCE_CODE_PATH})
target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK})

