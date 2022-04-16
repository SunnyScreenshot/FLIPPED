
set(PLUGIN_NAME "watemark")
project(${PLUGIN_NAME})

#set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin/plugins) # 更改输出路径

# Sources files
set(SRCS_FILES
    "pluginwatemark.h"
    "pluginwatemark.cpp")

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

add_library(${PLUGIN_NAME} SHARED ${SRCS_FILES})

#add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
#set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
#target_include_directories(${PLUGIN_NAME} PUBLIC
#  ${DtkWidget_INCLUDE_DIRS}
#  ${DFrameworkDBus_INCLUDE_DIRS}
#  ${QGSettings_INCLUDE_DIRS}
#  ../../src/pluginsinterface)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    ${QT5_LIBS_LINK})

#install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION plugins)
