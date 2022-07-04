add_library(Animation
    STATIC
    Animation.cpp
)

target_link_libraries(Animation
    PUBLIC
    Resources
)

target_include_directories(Animation
    PUBLIC
    include
)