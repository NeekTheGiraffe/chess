enable_testing()

add_executable(chess_test "test.cpp")
target_link_libraries(chess_test PRIVATE chess_lib)
target_link_libraries(chess_test PRIVATE yaml-cpp::yaml-cpp)

add_test(NAME "test_castle_kingside" COMMAND "chess_test" "test_castle_kingside.yaml")
add_test(NAME "test_castle_queenside" COMMAND "chess_test" "test_castle_queenside.yaml")
add_test(NAME "test_double_check" COMMAND "chess_test" "test_double_check.yaml")
add_test(NAME "test_moved_pawn" COMMAND "chess_test" "test_moved_pawn.yaml")
add_test(NAME "test_new_board" COMMAND "chess_test" "test_new_board.yaml")
add_test(NAME "test_pawn_attack" COMMAND "chess_test" "test_pawn_attack.yaml")
add_test(NAME "test_rook_pin" COMMAND "chess_test" "test_rook_pin.yaml")

file(GLOB TEST_CASES "*.yaml")
file(COPY ${TEST_CASES} DESTINATION "${CMAKE_BINARY_DIR}/tests")
