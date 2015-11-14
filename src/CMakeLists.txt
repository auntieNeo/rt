add_executable(rt main.cpp)
target_link_libraries(rt
    render
    ${SDL2_LIBRARY}
    )

set_property(TARGET rt PROPERTY CXX_STANDARD 11)
set_property(TARGET rt PROPERTY CXX_STANDARD_REQUIRED ON)

add_subdirectory("./render")
