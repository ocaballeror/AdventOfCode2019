import heapq

def read_input():
    grid = {}
    keys = []
    doors = []
    with open("input") as f:
        for y, line in enumerate(f):
            for x, char in enumerate(line.strip()):
                grid[x, y] = char
                if char == "@":
                    head = x, y
                    grid[x, y] = "."
                elif char.islower():
                    keys.append(char)
                elif char.isupper():
                    doors.append(char)

    return grid, head, keys, doors


def moves(x, y, grid, opened):
    for move in [
        (x - 1, y),
        (x + 1, y),
        (x, y - 1),
        (x, y + 1),
    ]:
        if move not in grid:
            continue
        other = grid[move]
        if other == "." or other.islower() or (other.isupper() and other.lower() in opened):
            yield move


def accessible(x, y, grid, opened, acc=0):
    for move in moves(x, y, grid, opened):
        other = grid[move]
        if other.islower():
            yield acc + 1, move

        if other == "@":
            continue

        yield from accessible(*move, grid, opened, acc + 1)


def dijkstra(grid, keys, start=(0, 0)):
    seen = set()
    allkeys = tuple(sorted(keys))
    pending = [(0, start, ())]
    pset = set(pending)  # set copy of `pending` for faster membership checks
    it = 0
    while pending:
        cost, node, keys = heapq.heappop(pending)
        assert len(keys) == len(set(keys))
        if keys == allkeys:
            return cost

        if it % 100000 == 0:
            print("Pending nodes:", len(pending))
        it += 1

        for move in moves(*node, grid, keys):
            if (move, keys) in seen:
                continue

            other = grid[move]
            if other.islower() and other not in keys:
                newkeys = tuple(sorted((*keys, other)))
            else:
                newkeys = keys
            new = (cost + 1, move, newkeys)

            if new not in pset:
                heapq.heappush(pending, new)
                pset.add(new)

        seen.add((node, keys))

    return -1


def part1():
    grid, head, keys, doors = read_input()
    return dijkstra(grid, keys, head)


print("Part 1:", part1())
