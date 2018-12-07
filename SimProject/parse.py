import random
import pathfinding

def ParseGrid(fileName):
    f = open(fileName, 'r')
    grid = []
    totalWeight = 0
    for line in f:
        row = []
        line = line.split(',')
        for num in line:
            row.append(float(num))
            totalWeight += float(num)
        grid.append(row)
    return (grid, totalWeight)

def SelectCrimeTile(grid, totalWeight):
    lottery = random.uniform(0, totalWeight)
    i = 0
    j = 0
    while True:
        lottery -= grid[i][j]
        if lottery < 0:
            break
        j += 1
        if j == len(grid[i]):
            j = 0
            i += 1
    return (i, j)


grid, weight = ParseGrid("test.txt")
i, j = SelectCrimeTile(grid, weight)
start = (1, 0)
end = (1, 2)

path = pathfinding.astar(grid, start, end)
print(path)