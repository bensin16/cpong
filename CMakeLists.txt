cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pong C)

add_subdirectory(external EXCLUDE_FROM_ALL)

set(SOURCES src/main.c)

add_executable(pong ${SOURCES})
target_link_libraries(pong external)

add_custom_command(
  TARGET pong POST_BUILD COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL3::SDL3>
  $<TARGET_FILE_DIR:pong>
)
