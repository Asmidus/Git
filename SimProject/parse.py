import random
import pathfinding
import math

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

def GetResponseTime(grid, startTile, endTile, travelSpeed):
    path = pathfinding.astar(grid, startTile, endTile)
    distance = 0
    #The height and width of each tile is 1m^2
    for i in range(len(path)-1):
        if path[i][0] == path[i+1][0] or path[i][1] == path[i+1][1]:
            distance += 1
        else:
            distance += math.sqrt(2)
    return (distance/travelSpeed)*60.0 #convert hours to minutes

grid, weight = ParseGrid("stockton.csv")
i, j = SelectCrimeTile(grid, weight)
start = (7, 7)
end = (26, 13)
speed = 45 #mph
time = GetResponseTime(grid, start, end, speed)

print(time)