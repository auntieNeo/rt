cmake_minimum_required(VERSION 3.3)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
project(rt)
set(rt_VERSION_MAJOR 0)
set(rt_VERSION_MAJOR 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)
find_package(GLM REQUIRED)

include_directories(SYSTEM
    ${SDL2_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    )

add_subdirectory("./src")
