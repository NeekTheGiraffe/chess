file(GLOB chess_lib_source "*.cpp" "*.h")

add_library(chess_lib STATIC ${chess_lib_source})

target_include_directories(chess_lib PUBLIC ..)
