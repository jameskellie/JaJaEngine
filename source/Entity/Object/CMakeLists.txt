add_library(Object
    STATIC
    Object.cpp
)

target_link_libraries(Object
    PUBLIC
    Entity
)

target_include_directories(Object
    PUBLIC
    include
)