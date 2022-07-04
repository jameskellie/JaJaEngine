add_library(XMLParser
    STATIC
    XMLParser.cpp
)

target_link_libraries(XMLParser
    PUBLIC
    tinyxml2
    Resources
    Map
)

target_include_directories(XMLParser
    PUBLIC
    include
)