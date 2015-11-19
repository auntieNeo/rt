add_library(scene STATIC
    camera.cpp
    drawableObject.cpp
    perspectiveCamera.cpp
    scene.cpp
    sceneObject.cpp
    sphere.cpp
    )

set_property(TARGET scene PROPERTY CXX_STANDARD 14)
set_property(TARGET scene PROPERTY CXX_STANDARD_REQUIRED ON)
