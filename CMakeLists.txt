cmake_minimum_required (VERSION 3.8)

project("ChessNov23" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src/vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(src/vendor/SDL_image EXCLUDE_FROM_ALL)

file(GLOB SOURCE "src/*.cpp" "src/*.h")
list(REMOVE_ITEM SOURCE "src/main.cpp")

add_library(chess_lib STATIC ${SOURCE})

target_include_directories(chess_lib PUBLIC "src")
target_link_libraries(chess_lib PUBLIC SDL3::SDL3)
target_link_libraries(chess_lib PUBLIC SDL3_image::SDL3_image)

add_executable(mygame "src/main.cpp")

target_link_libraries(mygame PRIVATE chess_lib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(tests)
