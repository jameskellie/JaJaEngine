add_library(Factory
    STATIC
    EntityCreator.cpp
)

target_link_libraries(Factory
    PUBLIC
    Entity
)

target_include_directories(Factory
    PUBLIC
    include
)