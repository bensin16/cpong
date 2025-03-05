add_subdirectory(SDL)
add_subdirectory(SDL_image)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE SDL3::SDL3 SDL3_image::SDL3_image)
