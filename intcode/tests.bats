#!/usr/bin/env bats


setup() {
	cp input $BATS_TMPDIR
	gcc -lm sol1.c -o sol1
}

teardown() {
	rm -f sol1
	cp $BATS_TMPDIR/input .
}

test_true_false() {
	for i in $1; do
		out=$(echo $i | ./sol1)
		echo "$out"
		[ "$out" = "Input: Out: 1" ]
	done
	for i in $2; do
		out=$(echo $i | ./sol1)
		echo "$out"
		[ "$out" = "Input: Out: 0" ]
	done
}

@test "position equal" {
	echo "3,9,8,9,10,9,4,9,99,-1,8" > input
	test_true_false "8" "1 2 3 4 5 6 7 9 10"
}

@test "position lessthan" {
	echo "3,9,7,9,10,9,4,9,99,-1,8" > input
	test_true_false "1 2 3 4 5 6 7" "8 9 10"
}

@test "immediate equal" {
	echo "3,3,1108,-1,8,3,4,3,99" > input
	test_true_false "8" "1 2 3 4 5 6 7 9 10"
}

@test "immediate lessthan" {
	echo "3,3,1107,-1,8,3,4,3,99" > input
	test_true_false "1 2 3 4 5 6 7" "8 9 10"
}

@test "immediate truthy" {
	echo "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9" > input
	test_true_false "1 2 3 4 5 6 7 8 9 10" "0"
}

@test "position truthy" {
	echo "3,3,1105,-1,9,1101,0,0,12,4,12,99,1" > input
	test_true_false "1 2 3 4 5 6 7 8 9 10" "0"
}

@test "comparer" {
	echo "3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,"\
		"1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,"\
		"999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99" > input

	for i in 1 2 3 4 5 6 7; do
		out=$(echo $i | ./sol1)
		echo "$out"
		[ "$out" = "Input: Out: 999" ]
	done
	for i in 8; do
		out=$(echo $i | ./sol1)
		echo "$out"
		[ "$out" = "Input: Out: 1000" ]
	done
	for i in 9 10 11 12; do
		out=$(echo $i | ./sol1)
		echo "$out"
		[ "$out" = "Input: Out: 1001" ]
	done
}

@test "long output" {
	echo "104,1125899906842624,99" > input
	out="$(./sol1)"
	echo "$out"
	[ "$out" = "Out: 1125899906842624" ]
}

@test "long multiplication" {
	echo "1102,34915192,34915192,7,4,7,99,0" > input
	out="$(./sol1)"
	echo "$out"
	[ "$out" = "Out: 1219070632396864" ]
}

@test "leaks" {
	cp $BATS_TMPDIR/input .
	out=$(echo 1 | valgrind ./sol1 2>&1)
	echo "$out"
	echo "$out" | grep -q "All heap blocks were freed -- no leaks are possible"
}
