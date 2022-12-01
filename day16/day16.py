import math


class CircularList(list):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._iter = 0
        self._factor = 1

    def __iter__(self):
        self._iter = 0
        return self

    def __next__(self):
        val = self[self._iter]
        self._iter += 1
        return val

    def __getitem__(self, i):
        if isinstance(i, int):
            return super().__getitem__(((i + 1) // self._factor) % len(self))
        elif isinstance(i, slice):
            return CircularList(super().__getitem__(i))

    def replicate(self):
        self._factor += 1


def simulate(signal, phases):
    pattern = CircularList([0, 1, 0, -1])

    signal = list(map(int, signal))

    for phase in range(phases):
        print(phase)
        newsignal = []
        for digit in signal:
            # total = sum(a if pattern[idx] == 1 else -a for idx, a in enumerate(signal) if pattern[idx])
            total = sum(a * b for a, b in zip(signal, pattern))
            newsignal.append(abs(total) % 10)
            pattern.replicate()

        signal = newsignal
        pattern._factor = 1

    return ''.join(map(str, signal))


def sim2(signal, phases):
    offset = int(''.join(signal[:7]))
    pattern = CircularList([0, 1, 0, -1])

    signal = CircularList(map(int, signal))

    for phase in range(phases):
        newsignal = []
        for digit in signal:
            copy = math.lcm(len(signal), len(pattern * pattern._factor))
            total = sum(a * b for a, b in zip(signal*copy, pattern))
            newsignal.append(abs(total) % 10)
            pattern._factor += 1

        signal = newsignal
        pattern._factor = 1

    return signal[offset:offset+8]


def read_input():
    with open('input') as f:
        return f.read().strip()


if __name__ == '__main__':
    print("Part 1:", simulate(read_input(), 100))
