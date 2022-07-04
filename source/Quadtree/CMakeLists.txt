add_library(Quadtree
    STATIC
    Quadtree.cpp
)

target_link_libraries(Quadtree
    PUBLIC
    Resources
    Camera
)

target_include_directories(Quadtree
    PUBLIC
    include
)