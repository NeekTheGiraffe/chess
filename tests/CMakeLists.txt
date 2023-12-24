enable_testing()

add_executable(chess_test "test.cpp")
target_link_libraries(chess_test PRIVATE chess_lib)

add_test(NAME "test_castle_kingside" COMMAND "chess_test" "test_castle_kingside.txt")
add_test(NAME "test_new_board" COMMAND "chess_test" "test_new_board.txt")
add_test(NAME "test_pawn_attack" COMMAND "chess_test" "test_pawn_attack.txt")
add_test(NAME "test_rook_pin" COMMAND "chess_test" "test_rook_pin.txt")

file(GLOB TEST_CASES "test_*.txt")
file(COPY ${TEST_CASES} DESTINATION "${CMAKE_BINARY_DIR}/tests")
