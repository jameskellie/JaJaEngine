add_library(OldMan
    STATIC
    OldMan.cpp
)

target_link_libraries(OldMan
    PUBLIC
    Entity
)

target_include_directories(OldMan
    PUBLIC
    include
)