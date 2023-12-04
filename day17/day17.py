import subprocess as sp

data = {}


def read_input():
    head = (0, 0)
    face = 0

    with open("input", "r+") as f:
        f.write("1")

    sp.check_output(["make"])
    output = sp.check_output(["./day17"], text=True)
    sp.check_output(["make", "clean"])

    for y, line in enumerate(output.split("\n")):
        for x, char in enumerate(line.strip()):
            if char in "^>v<":
                head = x, y
                face = "^>v<".find(char)
                char = "#"
            data[(x, y)] = char

    return head, face


def adjacent(x, y):
    for otherx, othery in ((-1, 0), (1, 0), (0, -1), (0, 1)):
        other = x + otherx, y + othery
        if other in data:
            yield data[other]


def intersection(x, y):
    return list(adjacent(x, y)) == ["#", "#", "#", "#"]


def part1():
    read_input()
    total = 0
    for (x, y), char in data.items():
        if char == "#" and intersection(x, y):
            total += x * y

    return total


class NoMoreMoves(Exception):
    pass


def move(head, face, seen, instructions):
    headx, heady = head
    for _ in range(4):
        head = [
            (headx, heady - 1),
            (headx + 1, heady),
            (headx, heady + 1),
            (headx - 1, heady),
        ][face]
        if head not in data or data[head] != "#" or head in seen and not intersection(*head):
            face = (face + 1) % 4
            instructions.append("R")
        else:
            if instructions and instructions[-3:] == ["R", "R", "R"]:
                instructions[-3:] = ["L"]
            if instructions and isinstance(instructions[-1], int):
                instructions[-1] += 1
            else:
                instructions.append(1)
            break
    else:
        if instructions[-4:] == ["R", "R", "R", "R"]:
            del instructions[-4:]
        raise NoMoreMoves

    seen.add(head)
    return head, face


def patterns(path, repl="A"):
    for idx, char in enumerate(path):
        end = idx + 1
        while end < len(path):
            chunk = path[idx:end]
            if path.count(chunk) == 1:
                break
            end += 1

        chunk = path[idx : end - 1]
        if "A" in chunk or "B" in chunk or "C" in chunk:
            continue
        new = path.replace(chunk, repl)
        if repl == "A":
            conv = patterns(new, "B")
            if conv:
                return (conv[0], [chunk, *conv[1]])
        if repl == "B":
            conv = patterns(new, "C")
            if conv:
                return (conv[0], [chunk, *conv[1]])
        if repl == "C":
            if set(new) == {"A", "B", "C"}:
                return new, [chunk]
            return None


def part2():
    head, face = read_input()
    seen = set()
    instructions = []
    while True:
        try:
            head, face = move(head, face, seen, instructions)
        except NoMoreMoves:
            break

    commands, functions = patterns("".join(map(str, instructions)))

    sp.check_output(["make"])
    with open("input", "r+") as f:
        f.write("2")
    cmdinput = " 44 ".join(str(ord(c)) for c in commands)
    funcinput = " 10 ".join(
        (
            " 44 ".join(
                " ".join(str(ord(c)) for c in part)
                for part in ",R,".join(",L,".join(fnc.split("L")).split("R")).strip(",").split(",")
            )
        )
        for fnc in functions
    )

    output = sp.check_output(
        ["./day17"], text=True, input=f"{cmdinput} 10 {funcinput} 10 121 10", stderr=sp.STDOUT
    )
    sp.check_output(["make", "clean"])
    return output.split("\n")[-1]


if __name__ == "__main__":
    print("Part 1:", part1())
    print("Part 2:", part2())
