add_library(render STATIC
    debug.cpp
    pixelSampleDistribution.cpp
    poissonDiskSampleDistribution.cpp
    )

set_property(TARGET render PROPERTY CXX_STANDARD 11)
set_property(TARGET render PROPERTY CXX_STANDARD_REQUIRED ON)
