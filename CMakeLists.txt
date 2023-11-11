cmake_minimum_required (VERSION 3.8)

project ("ChessNov23" VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src/vendor/SDL EXCLUDE_FROM_ALL)

add_executable(mygame src/main.cpp)

target_link_libraries(mygame PRIVATE SDL3::SDL3)
