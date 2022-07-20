add_library(Quadtree
    STATIC
    Quadtree.cpp
)

target_link_libraries(Quadtree
    PUBLIC
    Camera
    Entity
    Resources
)

target_include_directories(Quadtree
    PUBLIC
    include
)