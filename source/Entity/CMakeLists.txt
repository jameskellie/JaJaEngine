add_subdirectory(Animation)
add_subdirectory(Player)

add_library(Entity
    STATIC
    Entity.cpp
)

target_link_libraries(Entity
    INTERFACE
    Animation
    Player
)

target_include_directories(Entity
    PUBLIC
    include
)
