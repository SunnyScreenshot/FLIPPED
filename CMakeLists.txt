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

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(SOURCE_CODE_PATH
        main.cpp SysScreen/sysscreen.cpp SysScreen/sysscreen.h)

add_executable(${PROJECT_NAME} ${SOURCE_CODE_PATH})
target_link_libraries(${PROJECT_NAME}
        ${QT5_LIBS_LINK})

