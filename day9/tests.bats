#!/usr/bin/env bats


setup() {
	make >/dev/null
	cp input $BATS_TMPDIR
}

teardown() {
	make clean >/dev/null
	cp $BATS_TMPDIR/input .
}

assert_output() {
	echo "Assert status 0"
	[ "$status" = 0 ]

	IFS=' ' read -r -a input <<< $1
	len=${#input[@]}
	lines[0]="$(echo ${lines[0]} | sed 's/^Input: //')"
	for i in $(seq 0 $((len - 1))); do
		echo "${lines[$i]} == ${input[$i]}"
		[ "${lines[$i]}" = "Out: ${input[$i]}" ]
	done
}

@test "Example 1" {
	test="109 1 204 -1 1001 100 1 100 1008 100 16 101 1006 101 0 99"
	echo "$test" | tr -s ' ' , > input
	run ./day9
	assert_output $test
}
@test "Example 2" {
	test="1102 34915192 34915192 7 4 7 99 0"
	echo "$test" | tr -s ' ' , > input
	run ./day9
	assert_output 1219070632396864
}
@test "Example 3" {
	test="104 1125899906842624 99"
	echo "$test" | tr -s ' ' , > input
	run ./day9
	assert_output 1125899906842624
}
@test "Part 1" {
	cp $BATS_TMPDIR/input .
	run bash -c 'echo 1 | ./day9'
	assert_output 3454977209
}
@test "Part 2" {
	cp $BATS_TMPDIR/input .
	run bash -c 'echo 2 | ./day9'
	assert_output 50120
}

@test "Leaks" {
	run bash -c 'echo 1 | valgrind ./day9'
	echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"
}
