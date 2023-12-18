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

def accessible(x, y, grid, opened, seen=(), acc=0):
    if not seen:
        seen = set()

    visible = {}
    for move in moves(x, y, grid, opened):
        if move in seen:
            continue

        other = grid[move]
        if other.islower() and other not in opened:
            if visible.get(move, float('inf')) > acc + 1:
                visible[move] = acc + 1
        else:
            for cost, pos in accessible(*move, grid, opened, seen | {(x, y)}, acc + 1):
                assert grid[pos] not in opened
                if grid[pos] not in opened and visible.get(pos, float('inf')) > cost:
                    visible[pos] = cost

    return [(cost, key) for key, cost in visible.items()]


def dijkstra(grid, keys, start):
    seen = set()
    allkeys = tuple(sorted(keys))
    pending = [(0, start, ())]
    pset = set(pending)  # set copy of `pending` for faster membership checks
    it = 0
    while pending:
        cost, heads, keys = heapq.heappop(pending)
        assert len(keys) == len(set(keys))
        if keys == allkeys:
            return cost

        if it % 1000 == 0:
            print('Pending nodes:', len(pending))
        it += 1

        for idx, node in enumerate(heads):
            acc = accessible(*node, grid, keys)
            what = [(a[0], grid[a[1]]) for a in acc]
            for movecost, move in acc:
                other = grid[move]
                assert other.islower() and other not in keys
                newkeys = tuple(sorted((*keys, other)))
                if (move, newkeys) in seen:
                    continue

                new = (cost + movecost, (*heads[:idx], move, *heads[idx+1:]), newkeys)
                if new not in pset:
                    heapq.heappush(pending, new)
                    pset.add(new)

        seen.add((node, keys))

    return -1


def part2():
    grid, head, keys, doors = read_input()
    headx, heady = head
    grid[head] = "#"
    grid[headx - 1, heady] = "#"
    grid[headx + 1, heady] = "#"
    grid[headx, heady - 1] = "#"
    grid[headx, heady + 1] = "#"
    heads = (
        (headx - 1, heady - 1),
        (headx - 1, heady + 1),
        (headx + 1, heady - 1),
        (headx + 1, heady + 1),
    )
    for x in heads:
        assert grid[x] == "."
    return dijkstra(grid, keys, heads)


print("Part 2 can take a couple minutes")
print("Part 2:", part2())
