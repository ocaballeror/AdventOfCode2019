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
	run bash -c 'echo 1 | ./day5'
	[ "$status" = 0 ]
	echo "$output"
	[ "${lines[-1]}" = "Out: 12440243" ]
}
@test "Part 2" {
	run bash -c 'echo 5 | ./day5'
	[ "$status" = 0 ]
	echo "$output"
	[ "$output" = "Input: Out: 15486302" ]
}

@test "Part 1 leaks" {
	run bash -c 'echo 1 | valgrind ./day5'
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
@test "Part 2 leaks" {
	run bash -c 'echo 5 | valgrind ./day5'
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
