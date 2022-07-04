add_library(TextureManager
    STATIC
    TextureManager.cpp
)

target_link_libraries(TextureManager
    PUBLIC
    tinyxml2
    Engine
    Camera
)

target_include_directories(TextureManager
    PUBLIC
    include
)