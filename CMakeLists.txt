cmake_minimum_required (VERSION 3.8)

project("Desktop Chess" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
