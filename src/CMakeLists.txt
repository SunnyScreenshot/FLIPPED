cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME PicShot)
project(${PROJECT_NAME} VERSION 0.4.8 LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
file(COPY config/config.ini DESTINATION ${CMAKE_BINARY_DIR}/bin)

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
    tool/parameterbar.cpp
    tool/testbtstyle.h
    tool/testbtstyle.cpp)

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

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_MODULES Core Gui Widgets Svg Xml)
set(QT_VERSION_MODULES)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_MODULES})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_MODULES} LinguistTools) # LinguistTools No link

foreach(_qt_module ${QT_MODULES})
    list(APPEND QT_VERSION_MODULES Qt${QT_VERSION_MAJOR}::${_qt_module})
endforeach()

# Translation
option(PICSHOT_GENERATE_TS "Regenerate translation source files" OFF)
set(PICSHOT_TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/data/translations/zh_CN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/data/translations/zh_TW.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/data/translations/en_US.ts)

if (PICSHOT_GENERATE_TS)
    message("PICSHOT_GENERATE_TS is ON")
    qt5_create_translation(PICSHOT_QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${PICSHOT_TS_FILES} OPTIONS -source-language en_US -no-obsolete)
else ()
    message("PICSHOT_GENERATE_TS is OFF")
    qt5_add_translation(PICSHOT_QM_FILES ${PICSHOT_TS_FILES})   # Avoid deleting *.ts files during project cleanup
endif ()

message(@1--> ${PICSHOT_TS_FILES})
message(@2--> ${PICSHOT_QM_FILES})

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/3rdparty/QHotkey/QHotkey)
link_directories(${CMAKE_BINARY_DIR}/bin
    ${CMAKE_BINARY_DIR}/3rdparty/QHotkey)

add_executable(${PROJECT_NAME}
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
    ${PICSHOT_QM_FILES}  # Note: https://www.cnblogs.com/Paoyao/p/15752116.html
)

#if (APPLE)
#    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
#    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.XMuli.PicShot")
#    set(MACOSX_BUNDLE_INFO_STRING "A cross-platform screenshot software")
#    # set(MACOSX_BUNDLE_ICON_FILE "xxx.icns")
#    # set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
#    # set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
#    # set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
#endif()

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${QT_VERSION_MODULES} qhotkey)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} ${QT_VERSION_MODULES} qhotkey)
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} ${QT_VERSION_MODULES} qhotkey X11)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER org.XMuli.PicShot
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE)

if (MSVC)
    #set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif(MSVC)

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
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_VERSION="${PROJECT_VERSION}")
target_compile_definitions(${PROJECT_NAME} PUBLIC _BIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # 4/8
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        UNICODE
        _UNICODE)
endif()

foreach (FILE ${PICSHOT_QM_FILES})
    get_filename_component(F_NAME ${FILE} NAME)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${F_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/../bin/translations/${F_NAME})

    message("Copy *.qm Path: " ${CMAKE_CURRENT_BINARY_DIR}/../bin/translations/${F_NAME})
endforeach ()

message("==========BRGIN============")
message("_PROJECT_NAME:${PROJECT_NAME}")
message("_PROJECT_VERSION:" ${PROJECT_VERSION})
message("_BIT_ARCH:" ${CMAKE_SIZEOF_VOID_P})
message("_COMPILER:" ${CMAKE_HOST_SYSTEM_PROCESSOR})
message("_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message("CMake Path"
    "\n PROJECT_NAME:"  ${PROJECT_NAME}
    "\n CMAKE_INSTALL_PREFIX: "  ${CMAKE_INSTALL_PREFIX}
    "\n PROJECT_BINARY_DIR: "  ${PROJECT_BINARY_DIR}
    "\n CMAKE_INSTALL_BINDIR: "  ${CMAKE_INSTALL_BINDIR}
    "\n CMAKE_INSTALL_LIBDIR: "  ${CMAKE_INSTALL_LIBDIR}
    "\n CMAKE_INSTALL_INCLUDEDIR: "  ${CMAKE_INSTALL_INCLUDEDIR})
message("==========END============")

#=========================================================================================
# Install settings   CMAKE_NSTALL_PREFIX: "/usr/local" or "c:/Program Files/${PROJECT_NAME}"
if (WIN32)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
elseif (APPLE)
elseif (UNIX)
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/bin")
    endif ()

    # Install Translations
    install(FILES ${PICSHOT_QM_FILES} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/data/translations)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}    # bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})   # lib
endif()

## CPACK: General Settings
#set(CPACK_PACKAGE_NAME "PicShot")
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lightweight, beautiful and cross-platform screenshot tool")
#set(CPACK_PACKAGE_CONTACT "https://github.com/XMuli/PicShot/issues")
##set (CPACK_GENERATOR "TBZ2")
#set(CPACK_SET_DESTDIR ON)                             # ON 安装指定路径，设置下一个; OFF 则使用 CMAKE_INSTALL_PREFIX
#set(CPACK_INSTALL_PREFIX "C:/Program Files(x86)/PicShot")   # 安装到的目录路径
#set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})         # 生成的安装的版本号信息
#set(CPACK_RPM_PACKAGE_GROUP "XMuli")
#set(CPACK_PACKAGE_VENDOR "XMuli")
#set(CPACK_RPM_PACKAGE_LICENSE "MIT")

#if (WIN32)
#    # see: https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
#    set(CPACK_GENERATOR NSIS)
##    set(CPACK_NSIS_MUI_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeLogo.ico")
##    set(CPACK_NSIS_MUI_UNIICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeLogo.ico")
##    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\cmake-gui.exe")
#    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME} ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")
##    set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeInstall.bmp")
##    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")

#    set(CPACK_NSIS_MODIFY_PATH ON)
#    set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME})

##    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
#    set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}" )
#    set(CPACK_NSIS_MENU_LINKS
#        "doc/${PROJECT_NAME}-${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}/${PROJECT_NAME}.html"
#        "${PROJECT_NAME} Help" "https://github.com/XMuli/PicShot" "${PROJECT_NAME} Web Site")

#elseif (APPLE)
#    # ...
#elseif (UNIX)
#    set (CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
#    set (CPACK_DEBIAN_PACKAGE_SHILIBDEPS ON)
##    set (CPACK_DEBIAN_PACKAGE_RECOMMENDS "kimageformat-plugins")
#endif()

#include(CPack)
