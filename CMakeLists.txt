cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror")

set(SDL3IMAGE_AVIF OFF)

project(pong C)

add_subdirectory(external EXCLUDE_FROM_ALL)

set(SOURCES src/main.c)

add_executable(pong ${SOURCES})
target_link_libraries(pong external)

add_custom_command(
  TARGET pong POST_BUILD COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL3::SDL3>
  $<TARGET_FILE:SDL3_image::SDL3_image>
  $<TARGET_FILE_DIR:pong>
)

add_custom_command(
  TARGET pong POST_BUILD COMMAND
  ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/images/
  $<TARGET_FILE_DIR:pong>/images
)
