import random
import pathfinding
import math
from enum import Enum

lv1 = 1
lv2 = 4.5
lv3 = 7
lv4 = 10

def ParseGrid(fileName):
    f = open(fileName, 'r')
    grid = []
    totalWeight = 0
    for line in f:
        row = []
        line = line.split(',')
        for cell in line:
            num = int(cell)
            if num == 0:
                row.append(0)
            elif num == 1:
                row.append(lv1)
                totalWeight += lv1
            elif num == 2:
                row.append(lv2)
                totalWeight += lv2
            elif num == 3:
                row.append(lv3)
                totalWeight += lv3
            elif num == 4:
                row.append(lv4)
                totalWeight += lv4
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
    #The height and width of each tile is 1m
    for i in range(len(path)-1):
        if path[i][0] == path[i+1][0] or path[i][1] == path[i+1][1]:
            distance += 1
        else:
            distance += math.sqrt(2)
    return (distance/travelSpeed)*60.0 #convert hours to minutes

def testWeights(grid, totalWeight):
    results = []
    for i in range(0, 100):
        row, col = SelectCrimeTile(grid, totalWeight)
        results.append(grid[row][col])
    print(results.count(lv1))
    print(results.count(lv2))
    print(results.count(lv3))
    print(results.count(lv4))

grid, weight = ParseGrid("stockton.csv")
i, j = SelectCrimeTile(grid, weight)
start = (7, 7)
end = (26, 13)
speed = 45 #mph
time = GetResponseTime(grid, start, end, speed)
testWeights(grid, weight)