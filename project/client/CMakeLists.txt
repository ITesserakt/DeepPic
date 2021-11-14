set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(client src/main.cpp)
target_link_libraries(client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_subdirectory(widgets/main_window)
target_link_libraries(client MainWindow)
target_include_directories(client PUBLIC widgets/main_window/include)

