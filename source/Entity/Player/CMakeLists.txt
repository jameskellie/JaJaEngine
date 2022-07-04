add_library(Player
    STATIC
    Player.cpp
)

target_link_libraries(Player
    PUBLIC
    Animation
    Entity
    Quadtree
)

target_include_directories(Player
    PUBLIC
    include
)