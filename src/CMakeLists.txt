add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image EXCLUDE_FROM_ALL)

add_subdirectory(chess)
add_subdirectory(chess_app)

add_executable(mygame "main.cpp")

target_link_libraries(mygame PRIVATE chess_app)
