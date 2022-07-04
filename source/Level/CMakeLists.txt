add_subdirectory(Map)
add_subdirectory(XMLParser)

add_library(Level
    STATIC
    Level.cpp
)

target_link_libraries(Level
    PUBLIC
    tinyxml2
    Map
    XMLParser
    Quadtree
)

target_include_directories(Level
    PUBLIC
    include
)