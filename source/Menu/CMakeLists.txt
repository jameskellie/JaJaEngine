add_library(Menu
    STATIC
    Menu.cpp
)

target_link_libraries(Menu
    PUBLIC
    Resources
)

target_include_directories(Menu
    PUBLIC
    include
)