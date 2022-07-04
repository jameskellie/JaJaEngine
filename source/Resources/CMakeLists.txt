add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/InputHandler)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TextureManager)

add_library(Resources
    STATIC
    Resources.cpp
)

target_link_libraries(Resources
    PUBLIC
    Engine
    InputHandler
    TextureManager
)

target_include_directories(Resources
    PUBLIC
    include
)