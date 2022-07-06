add_library(Engine
    STATIC
    Engine.cpp
)

target_link_libraries(Engine
    PUBLIC
    SDL2::TTF
)

target_include_directories(Engine
    PUBLIC
    include
)