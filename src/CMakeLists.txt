cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME PicShot)
set(PROJECT_VERSION 0.2)
set(_version ${PROJECT_VERSION})
#set(PROJECT_VERSION_MAJOR 0)
#set(PROJECT_VERSION_MINOR 2)
#set(PROJECT_VERSION_PATCH 0)

project(${PROJECT_NAME})   

#CMAKE_BINARY_DIR   CMAKE_CURRENT_SOURCE_DIR
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

# Sources files
# main
set(SRCS_MAIN
    "xglobal.h"
    "main.cpp"
    )

# core
set(SRCS_CORE
    "core/isingleton.h"
    "core/xlog.h"
    )

# platform
set(SRCS_PLATFORM
    "platform/xtype.h"
    "platform/xtype.cpp"
    "platform/iwininfo.h"
    "platform/iwininfo.cpp"
    "platform/wininfo.h"
    "platform/wininfo.cpp"

    # 特定平台实现的文件列举在此，会自动剔除和分类归组。分类标准 *_win.* *_x11.*
    "platform/wininfo_win.h"
    "platform/wininfo_win.cpp"
    "platform/wininfo_x11.h"
    "platform/wininfo_x11.cpp"
    )


# 正则 http://blog.icodeten.com/cmake/2015/01/22/cmake-experience/
# 删除特定平台相关的 file
foreach(_rm_file ${SRCS_PLATFORM})
    string(REGEX MATCH ".*/*_win.h|.*/*_win.cpp|.*/*_x11.h|.*/*_x11.cpp" _need_remove_source ${_rm_file})
    if(_need_remove_source)
        list(REMOVE_ITEM SRCS_PLATFORM ${_need_remove_source})
    endif(_need_remove_source)
endforeach(_rm_file)

##----------- Test begin -----------
#foreach(_rm_file ${SRCS_PLATFORM})
#    message(${_rm_file})
#endforeach(_rm_file)
##----------- Test end -----------

# custom widget
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
    "widget/xlabel.h"
    "widget/xlabel.cpp"
    "widget/xframewidget.h"
    "widget/xframewidget.cpp"
    "widget/xcombobox.h"
    "widget/xcombobox.cpp"
    )

# tool
set(SRCS_TOOL
    "tool/base/colorparabar.h"
    "tool/base/colorparabar.cpp"
    "tool/base/widthparabar.h"
    "tool/base/widthparabar.cpp"
    "tool/base/managebar.h"
    "tool/base/managebar.cpp"
    "tool/selectsize/selectsize.h"
    "tool/selectsize/selectsize.cpp"
    "tool/magnifier/magnifier.h"
    "tool/magnifier/magnifier.cpp"
    "tool/selectbar.h"
    "tool/selectbar.cpp"
    "tool/parameterbar.h"
    "tool/parameterbar.cpp"
    )

# tool
set(SRCS_PERFERENCE
    "preference/preference.h"
    "preference/preference.cpp"
    "preference/resetbtn.h"
    "preference/resetbtn.cpp"
    )

# screen
set(SRCS_WINSCREEN
    "screen/drawhelper.h"
    "screen/drawhelper.cpp"
    "screen/rectcalcu.h"
    "screen/rectcalcu.cpp"
    "screen/screenshot.h"
    "screen/screenshot.cpp"
    "screen/tray.h"
    "screen/tray.cpp"
    "screen/winresetbtn.h"
    "screen/winresetbtn.cpp"
    "screen/winsetting.h"
    "screen/winsetting.cpp"
)

# 测试模块
set(SRCS_EXAMPLE
    "example/exwidget.h"
    "example/exwidget.cpp")


# 插件模块
set(SRCS_PLUGIN_INTERFACE
    "pluginsinterface/iplugininterface.h"
    "pluginsinterface/iplugininterface.cpp")

# 资源模块
set(SRCS_RESOURCES
    "Resources.qrc"
    "logo.rc"
    )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT5_LIBS_LINK
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        Qt5::Xml
        )

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        Xml
        REQUIRED)

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
    # set(MACOSX_BUNDLE_ICON_FILE "xxx.icns")
    # set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    # set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${SRCS_MAIN}
    ${SRCS_PLATFORM}
    ${SRCS_CORE}
    ${SRCS_WIDGET}
    ${SRCS_TOOL}
    ${SRCS_PERFERENCE}
    ${SRCS_WINSCREEN}
    ${SRCS_EXAMPLE}
    ${SRCS_PLUGIN_INTERFACE}
    ${SRCS_RESOURCES}
    )
else()
    add_executable(${PROJECT_NAME} WIN32
    ${SRCS_MAIN}
    ${SRCS_PLATFORM}
    ${SRCS_CORE}
    ${SRCS_WIDGET}
    ${SRCS_TOOL}
    ${SRCS_PERFERENCE}
    ${SRCS_WINSCREEN}
    ${SRCS_EXAMPLE}
    ${SRCS_PLUGIN_INTERFACE}
    ${SRCS_RESOURCES}
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

#target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)

# 文件归类, ref:
# https://stackoverflow.com/questions/33808087/cmake-how-to-create-visual-studio-filters
# https://blog.csdn.net/gzj2013/article/details/102619480
set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR}) # default root path curr path （CMakeList.txt）

file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")

#message( "_source_list----------"  ${_source_list})

source_group(TREE ${_src_root_path} FILES ${_source_list})  # [ source_group方法一， 会按照实际的目录结构进行组织， .h.cpp 放在一起]

# 特定平台的文件，进行过滤分组和添加得到编译中
foreach(_source IN ITEMS ${_source_list})
    string(REGEX MATCH ".*/*_win.h|.*/*_win.cpp" _plat_win_source ${_source})  # 这个的匹配最后不能是 .c* 表示 .cpp
    string(REGEX MATCH ".*/*_x11.h|.*/*_x11.cpp" _plat_x11_source ${_source})

#   #----------- Test begin -----------
#    message( "_source -->"  ${_source})
#    message( "_plat_win_source -->"  ${_plat_win_source})
#    message( "_plat_x11_source -->"  ${_plat_x11_source})
#   #----------- Test end -----------

    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")

    if(_plat_win_source OR _plat_x11_source)
        if(APPLE)
        elseif(WIN32)
            target_sources(${PROJECT_NAME} PRIVATE "${_plat_win_source}")
            source_group("${_group_path}" FILES "${_plat_win_source}")
        else()
            target_sources(${PROJECT_NAME} PRIVATE "${_plat_x11_source}")
            source_group("${_group_path}" FILES "${_plat_x11_source}")
        endif()
    else()
#        source_group("${_group_path}" FILES "${_source}") # [source_group 方法二， 自行分组 .h 和最后的 .cpp 是分开放在两个大分组下]
    endif(_plat_win_source OR _plat_x11_source)

endforeach()

# https://www.ljjyy.com/archives/2021/03/100653.html
# CPU是32位还是64位
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_bit_arch "x64")
    message(STATUS "Target is 64 bits")
else()
    set(_bit_arch "x86")
    message(STATUS "Target is 32 bits")
endif()

# 主机处理器架构
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
    message(STATUS "i386 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
    message(STATUS "i686 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "x86_64 architecture detected")
else()
    message(STATUS "host processor architecture is unknown")
endif()

# 定义一些变量，可以在 CPP 中使用
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_NAME="${PROJECT_NAME}")           # PicShot
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_VERSION="${_version}")    # 0.2 这个没打印出来，很奇怪，改用下面这个
target_compile_definitions(${PROJECT_NAME} PUBLIC _BIT_ARCH="${_bit_arch}")                  # x64/x86
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC

message("==========BRGIN============")
message("_PROJECT_NAME:${PROJECT_NAME}")
message("_PROJECT_VERSION:" ${_version})
message("_BIT_ARCH:" ${_bit_arch})
message("_COMPILER:" ${CMAKE_HOST_SYSTEM_PROCESSOR})
message("_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message("==========END============")

#  CMAKE_INSTALL_PREFIX:   /usr/local   或 c:/Program Files/${PROJECT_NAME}
if(APPLE)

elseif(WIN32)
    # install(TARGETS ${PROJECT_NAME} RUNTIME)
else()
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
endif()
