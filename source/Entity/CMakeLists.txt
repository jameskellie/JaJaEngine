add_subdirectory(Animation)
add_subdirectory(Car)
add_subdirectory(Object)
add_subdirectory(OldMan)
add_subdirectory(Player)

add_library(Entity
    STATIC
    Entity.cpp
)

target_link_libraries(Entity
    PUBLIC
    Animation
)

target_link_libraries(Entity
    INTERFACE
    Car
    Level
    Object
    OldMan
    Player
)

target_include_directories(Entity
    PUBLIC
    include
)
