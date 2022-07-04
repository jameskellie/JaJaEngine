add_library(InputHandler
    STATIC
    InputHandler.cpp
)

target_link_libraries(InputHandler
    PUBLIC
    Engine
)

target_include_directories(InputHandler
    PUBLIC
    include
)