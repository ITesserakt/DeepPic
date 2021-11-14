set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)


add_executable(client src/main.cpp MainWindow.cpp include/MainWindow.h)
target_link_libraries(client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

target_include_directories(client PUBLIC include)
