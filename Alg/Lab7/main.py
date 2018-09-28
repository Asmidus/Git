import fileinput
import os

grid = []


def printGrid():
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            print(grid[i][j], end = "  ")
        print("\n")


def findSources():
    sources = []
    for j in range(len(grid[0])):
        for i in range(len(grid)):
            if grid[i][j] != 0:
                break
            if i == len(grid) - 1:
                sources.append(j)
    return sources


def removeSources(sources):
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            for k in sources:
                if i == k and i != j and grid[i][j] == 1:
                    grid[i][j] = 0
    for k in sources:
        grid[k][k] = 1
    return 1               

for graphnum in range(1, sum([len(files) for r, d, files in os.walk("graphs")]) + 1):
    grid = []
    for line in fileinput.input("graphs/graph" + str(graphnum) + ".txt"):
        grid.append([])
        for char in line:
            if char != "\n":
                grid[len(grid) - 1].append(int(char))
    sort = []
    while len(findSources()) > 0:
        sourcesToRemove = findSources()
        for s in sourcesToRemove:
            sort.append(s)
        removeSources(sourcesToRemove)
    printGrid()
    print(sort)
    if(len(sort) != len(grid)):
        print("Could not sort, cyclic")


