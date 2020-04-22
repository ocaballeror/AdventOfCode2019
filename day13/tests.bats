#!/usr/bin/env bats


setup() {
	make >/dev/null
}

teardown() {
	make clean >/dev/null
}

@test "Part 1" {
	run ./sol1
	[ "$status" = 0 ]
	echo "$output"
	[ "$output" = "Number of blocks: 284" ]
}
@test "Part 2" {
	run ./sol2 --no-draw
	[ "$status" = 0 ]
	echo "$output"
	[ "$output" = "Final score: 13581" ]
}

@test "Part 1 leaks" {
	run valgrind ./sol1
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
@test "Part 2 leaks" {
	run valgrind ./sol2 --no-draw
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
