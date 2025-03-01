cmake_minimum_required(VERSION 2.8)
project (X11Tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(X11 REQUIRED)

include_directories(${X11_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

file(GLOB LIB_SOURCES lib/*.cpp)
add_library(X11Wrap ${LIB_SOURCES})

add_executable(main main.cpp)
add_executable(life life.cpp)
add_executable(render render.cpp)
add_executable(tga tga.cpp)
add_definitions(-std=c++11 -g)
target_link_libraries(main X11Wrap ${X11_LIBRARIES})
target_link_libraries(life X11Wrap ${X11_LIBRARIES})
target_link_libraries(render X11Wrap ${X11_LIBRARIES})
target_link_libraries(tga X11Wrap ${X11_LIBRARIES})
