FLEX_TARGET(plyLexer plyLexer.ll "${CMAKE_CURRENT_BINARY_DIR}/plyLexer.cpp"
    COMPILE_FLAGS "--header-file='${CMAKE_CURRENT_BINARY_DIR}/plyLexer.h'")
BISON_TARGET(plyParser plyParser.ypp "${CMAKE_CURRENT_BINARY_DIR}/plyParser.cpp"
    COMPILE_FLAGS --verbose)
ADD_FLEX_BISON_DEPENDENCY(plyLexer plyParser)
add_executable(ply_test
    plyLexer.cpp
    plyParser.cpp
    )
target_link_libraries(ply_test
    )

add_library(scene STATIC
    camera.cpp
    drawableObject.cpp
    materialProperties.cpp
    perspectiveCamera.cpp
    plane.cpp
    pointLight.cpp
    scene.cpp
    sceneObject.cpp
    sphere.cpp
    )

set_property(TARGET scene PROPERTY CXX_STANDARD 14)
set_property(TARGET scene PROPERTY CXX_STANDARD_REQUIRED ON)
