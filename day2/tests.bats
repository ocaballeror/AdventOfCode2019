#!/usr/bin/env bats


setup() {
	make >/dev/null
	cp input $BATS_TMPDIR
}

teardown() {
	make clean >/dev/null
	cp $BATS_TMPDIR/input .
}

@test "Part 1" {
	run ./sol1
	[ "$status" = 0 ]
	echo "$output"
	[ "$output" = "Solution: 6087827" ]
}
@test "Part 2" {
	run ./sol2
	[ "$status" = 0 ]
	echo "$output"
	[ "${lines[-1]}" = "Solution: 5379" ]
}

@test "Part 1 leaks" {
	run valgrind ./sol1
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
@test "Part 2 leaks" {
	run valgrind ./sol2
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
