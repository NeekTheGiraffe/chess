file(GLOB chess_app_source "*.cpp" "*.h")

add_library(chess_app STATIC ${chess_app_source})

target_include_directories(chess_app PUBLIC ..)
target_link_libraries(chess_app
	PUBLIC SDL3::SDL3
	PUBLIC SDL3_image::SDL3_image
	PUBLIC chess_lib
)
