import math
import itertools
from functools import cache


PATTERN = [0, 1, 0, -1]


def repeat(times):
    for count in itertools.count(1):
        idx = (count // times) % len(PATTERN)
        yield PATTERN[idx]


def calc(signal, idx):
    return abs(sum(a * b for a, b in zip(signal, repeat(idx)))) % 10


def simphase(signal):
    return tuple(calc(signal, idx) for idx, digit in enumerate(signal, start=1))


def part1(signal, phases=100):
    signal = tuple(map(int, signal))

    for phase in range(phases):
        # print(phase)
        signal = simphase(signal)

    return "".join(map(str, signal[:7]))


def sim2(signal):
    res = sum(signal)
    new = [res % 10]
    for t in signal[:-1]:
        res -= t
        new.append(res % 10)

    return new


def part2(signal, phases=100):
    offset = int(signal[:7])
    signal = tuple(map(int, signal)) * 5000
    breakpoint()
    offset -= len(signal)

    for phase in range(phases):
        signal = sim2(signal)

    # print("out", "".join(map(str, signal)))
    return "".join(map(str, signal[offset : offset + 8]))


def read_input():
    with open("input") as f:
        return f.read().strip()


if __name__ == "__main__":
    print("Part 1:", part1(read_input(), 100))
    print("Part 2:", part2(read_input(), 100))
