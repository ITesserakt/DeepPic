set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(Boost 1.40.0 REQUIRED system)
find_package(Boost COMPONENTS log log_setup REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

find_package(nlohmann_json 3.2.0 REQUIRED)

set(WITH_SANITIZER ON)
if (WITH_SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()


set(SOURCES src/MainWindow.cpp src/PaintScene.cpp src/Palette.cpp src/Changer.cpp
        src/ToolsPanel.cpp src/ParametersPanel.cpp src/Brush.cpp src/Canvas.cpp
        src/Connector.cpp src/ServerConnection.cpp src/Base64.cpp)
set(HEADER_FILES include/Palette.h include/MainWindow.h include/PaintScene.h include/Changer.h
        include/ToolsPanel.h include/ParametersPanel.h include/Brush.h include/Canvas.h
        include/Connector.h include/ServerConnection.h ${Boost_INCLUDE_DIR} include/Base64.h)

#include_directories(include ${Boost_INCLUDE_DIR})
#aux_source_directory(src SRC)

add_executable(client src/main.cpp ${SOURCES} ${HEADER_FILES})
target_link_libraries(client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        boost_thread
        pthread
        ${BOOST_LIBRARIES}
        nlohmann_json::nlohmann_json
        )

target_include_directories(client PUBLIC include)

#add_executable(test src/main_window_test.cpp ${SOURCES} ${HEADER_FILES})
#find_package(GTest)

#target_link_libraries(test PRIVATE
#        GTest::Main
#        Qt5::Core
#        Qt5::Gui
#        Qt5::Widgets
#        )

#add_test(NAME test COMMAND test)
#target_include_directories(test PUBLIC include)

