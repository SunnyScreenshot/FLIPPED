cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME Acrylic)
set(PROJECT_VERSION 1.0)
project(${PROJECT_NAME} LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../bin) # 更改二进制输出路径

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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS_MAIN
    "acrylic.h"
    "acrylic.cpp"
    "dllmain.cpp")

add_library(${PROJECT_NAME} SHARED
    ${SRCS_MAIN})

set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR}) # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")
source_group(TREE ${_src_root_path} FILES ${_source_list})

target_compile_definitions(${PROJECT_NAME} PRIVATE ACRYLIC_LIBRARY)
