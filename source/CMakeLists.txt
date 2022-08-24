set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2       REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf   REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    
    ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(Camera)
add_subdirectory(Entity)
add_subdirectory(Factory)
add_subdirectory(Level)
add_subdirectory(Menu)
add_subdirectory(Quadtree)
add_subdirectory(Resources)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL2::Main
    SDL2::Image
    SDL2::TTF

    tinyxml2

    Camera
    Entity
    Factory
    Level
    Menu
    Quadtree
    Resources
)
