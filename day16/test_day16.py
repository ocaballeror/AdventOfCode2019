import pytest

from day16 import CircularList, simulate, sim2


@pytest.mark.parametrize(
    "factor,idx,value",
    [
        (1, 0, 1),
        (1, 1, 0),
        (1, 2, -1),
        (1, 3, 0),
        (2, 0, 0),
        (2, 1, 1),
        (2, 2, 1),
        (2, 3, 0),
        (2, 4, 0),
        (2, 5, -1),
        (2, 6, -1),
        (2, 7, 0),
        (2, 8, 0),
        (3, 0, 0),
        (3, 1, 0),
        (3, 2, 1),
        (3, 3, 1),
        (3, 4, 1),
        (3, 5, 0),
        (3, 6, 0),
        (3, 7, 0),
        (3, 8, -1),
        (3, 9, -1),
        (3, 10, -1),
        (3, 11, 0),
        (3, 12, 0),
    ],
)
def test_pattern_getitem(factor, idx, value):
    pat = CircularList([0, 1, 0, -1])
    pat._factor = factor
    assert pat[idx] == value

    it = iter(pat)
    [next(it) for _ in range(idx)]
    assert next(it) == value


@pytest.mark.parametrize(
    "signal,phases,expect",
    [
        ("12345678", 4, "01029498"),
        ("80871224585914546619083218645595", 100, "24176176"),
        ("19617804207202209144916044189917", 100, "73745418"),
        ("69317163492948606335995924319873", 100, "52432133"),
    ],
)
def test_examples(signal, phases, expect):
    got = simulate(signal, phases)
    assert got.startswith(expect)


@pytest.mark.parametrize(
    "signal,phases,expect",
    [
        ("03036732577212944063491565474664", 100, 84462026),
        ("02935109699940807407585447034323", 100, 78725270),
        ("03081770884921959731165446850517", 100, 53553731),
    ],
)
def test_examples2(signal, phases, expect):
    got = sim2(signal, phases)
    assert got.startswith(expect)
