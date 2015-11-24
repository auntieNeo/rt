add_library(render STATIC
    debug.cpp
    image.cpp
    pixel.cpp
    pixelSampleDistribution.cpp
    poissonDiskSampleDistribution.cpp
    previewWindow.cpp
    sample.cpp
    simpleHaltingStrategy.cpp
    simpleRayCaster.cpp
    simpleRayTracer.cpp
    simpleSampleDistribution.cpp
    subimage.cpp
    task.cpp
    worker.cpp
    workerPool.cpp
    )
target_link_libraries(render
    common
    )

set_property(TARGET render PROPERTY CXX_STANDARD 14)
set_property(TARGET render PROPERTY CXX_STANDARD_REQUIRED ON)
