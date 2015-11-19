add_library(common
    ray.cpp
    )

add_executable(rt main.cpp)
target_link_libraries(rt
    common
    render
    scene
    ${SDL2_LIBRARY}
    )

set_property(TARGET rt PROPERTY CXX_STANDARD 14)
set_property(TARGET rt PROPERTY CXX_STANDARD_REQUIRED ON)

add_subdirectory("./render")
add_subdirectory("./scene")
