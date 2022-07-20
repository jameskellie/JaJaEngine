add_library(Map
    STATIC
    Map.cpp
    Layer.cpp
)

target_link_libraries(Map
    PUBLIC
    Object
    Resources
)

target_include_directories(Map
    PUBLIC
    include
)