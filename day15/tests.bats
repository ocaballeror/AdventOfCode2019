#!/usr/bin/env bats


setup() {
	make >/dev/null
}

teardown() {
	make clean >/dev/null
}

@test "Day 15" {
	run ./day15
	[ "$status" = 0 ]
	[ "${lines[0]}" = "Found the oxygen at (-19, 18), dist: 269" ]
	[ "${lines[1]}" = "Total minutes: 269" ]
}
@test "Day 15 leaks" {
	run valgrind ./day15
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
