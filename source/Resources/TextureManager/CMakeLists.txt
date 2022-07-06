add_library(TextureManager
    STATIC
    TextureManager.cpp
)

target_link_libraries(TextureManager
    PUBLIC
    SDL2::Image
    Engine
    Camera
)

target_include_directories(TextureManager
    PUBLIC
    include
)
