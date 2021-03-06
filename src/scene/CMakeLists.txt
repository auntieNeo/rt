FLEX_TARGET(plyLexer plyLexer.ll "${CMAKE_CURRENT_BINARY_DIR}/plyLexer.cpp"
    COMPILE_FLAGS "--header-file='${CMAKE_CURRENT_BINARY_DIR}/plyLexer.h'")
BISON_TARGET(plyParser plyParser.ypp "${CMAKE_CURRENT_BINARY_DIR}/plyParser.cpp"
    COMPILE_FLAGS --verbose)
ADD_FLEX_BISON_DEPENDENCY(plyLexer plyParser)

add_library(scene STATIC
    aabb.cpp
    camera.cpp
    drawableObject.cpp
    materialProperties.cpp
    perspectiveCamera.cpp
    plane.cpp
    plyLexer.cpp
    plyParser.cpp
    pointLight.cpp
    scene.cpp
    sceneObject.cpp
    sphere.cpp
    triangleMesh.cpp
    triangleMeshBuilder.cpp
    )
target_include_directories(scene PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    )

set_property(TARGET scene PROPERTY CXX_STANDARD 14)
set_property(TARGET scene PROPERTY CXX_STANDARD_REQUIRED ON)
