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
	[ "$output" = "Number of blocks: 208" ]
}
@test "Part 2" {
	run ./sol2
	[ "$status" = 0 ]
	echo "$output"
	cat >/tmp/expect <<EOF
.####..##..###..###...##..####...##.#..#...
.#....#..#.#..#.#..#.#..#.#.......#.#.#....
.###..#..#.#..#.###..#....###.....#.##.....
.#....####.###..#..#.#....#.......#.#.#....
.#....#..#.#.#..#..#.#..#.#....#..#.#.#....
.#....#..#.#..#.###...##..#.....##..#..#...
EOF
	[ "$output" = "$(cat /tmp/expect)" ]
}

@test "Part 1 leaks" {
	run valgrind ./sol1
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
@test "Part 2 leaks" {
	run valgrind ./sol2
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
