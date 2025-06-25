cmake_minimum_required(VERSION 3.25)
project(SpinningCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Switch between Desktop OpenGL or OpenGL ES
# cmake .. -DUSE_DESKTOP_GL=ON
option(USE_DESKTOP_GL "Use Desktop OpenGL instead of OpenGL ES" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(SpinningCube
    main.cpp
    cube_widget.cpp
    cube_widget.h
    opengl_config.h
)

if(USE_DESKTOP_GL)
    target_compile_definitions(SpinningCube PRIVATE USE_DESKTOP_GL)
endif()

target_link_libraries(SpinningCube
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
