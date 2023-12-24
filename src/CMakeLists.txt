add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image EXCLUDE_FROM_ALL)

file(GLOB SOURCE "*.cpp" "*.h")
list(REMOVE_ITEM SOURCE "main.cpp")

add_library(chess_lib STATIC ${SOURCE})

target_include_directories(chess_lib PUBLIC ".")
target_link_libraries(chess_lib PUBLIC SDL3::SDL3)
target_link_libraries(chess_lib PUBLIC SDL3_image::SDL3_image)

add_executable(mygame "main.cpp")

target_link_libraries(mygame PRIVATE chess_lib)
