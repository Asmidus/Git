import heapq
import random
import time
import math

def heapSort(inputArray):
    finishArray = []
    heapq.heapify(inputArray)
    for i in range(len(inputArray)):
        finishArray.append(heapq.heappop(inputArray))
    return finishArray

def binarySearch(inputArray, left, right, search):
    if(right >= left):
        mid = left + math.ceil((right-left)/2)
        if inputArray[mid] == search:
            return mid
        if inputArray[mid] > search:
            return binarySearch(inputArray, left, mid-1, search)
        return binarySearch(inputArray, mid+1, right, search)
    return -1

def linearSearch(inputArray, search):
    for i in range(len(inputArray)):
        if inputArray[i] == search:
            return i
    return -1
    

n = int(input("How many searches? "))
searches = []
for i in range(n):
    searches.append(random.randint(0, 1000000))
array = []
for i in range(10000):
    array.append(random.randint(0, 1000000))
start1 = time.clock()
for i in range(n):
    linearSearch(array, searches[i])
end1 = time.clock()
start2 = time.clock()
array = heapSort(array)
for i in range(n):
    binarySearch(array, 0, len(array) - 1, searches[i])
end2 = time.clock()
print("LINEAR: ", end1 - start1, "\nBINARY: ", end2 - start2)
