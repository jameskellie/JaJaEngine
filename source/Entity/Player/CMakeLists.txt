add_library(Player
    STATIC
    Player.cpp
)

target_link_libraries(Player
    PUBLIC
    Animation
    Entity
    Level
)

target_include_directories(Player
    PUBLIC
    include
)