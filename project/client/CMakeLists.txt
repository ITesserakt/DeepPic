set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(WITH_SANITIZER ON)
if (WITH_SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

set(SOURCES src/main.cpp src/MainWindow.cpp  src/PaintScene.cpp src/Palette.cpp src/Changer.cpp)
set(HEADER_FILES include/Palette.h include/MainWindow.h include/PaintScene.h include/Changer.h)

add_executable(client ${SOURCES} ${HEADER_FILES})
target_link_libraries(client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

target_include_directories(client PUBLIC include)
