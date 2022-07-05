add_library(TextureManager
    STATIC
    TextureManager.cpp
)

target_link_libraries(TextureManager
    PUBLIC
    Engine
    Camera
)

target_include_directories(TextureManager
    PUBLIC
    include
)