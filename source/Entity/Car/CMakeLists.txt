add_library(Car
    STATIC
    Car.cpp
)

target_link_libraries(Car
    PUBLIC
    Entity
)

target_include_directories(Car
    PUBLIC
    include
)