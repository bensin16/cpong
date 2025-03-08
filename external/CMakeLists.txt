add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
