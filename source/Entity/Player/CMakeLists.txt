add_library(Player
    STATIC
    Player.cpp
)

target_link_libraries(Player
    PUBLIC
    Entity
)

target_include_directories(Player
    PUBLIC
    include
)