import fileinput
import os.path
import copy
import time

puzzleList = []
comSpots = []
times = []

#simple function to print a grid in a pleasing way
def printGrid(grid):
	for row in grid:
		for tile in row:
			print(tile, end = "  ")
		print("\n")

def findBestSpace():
	numOfIntersections = []
	for i in range(len(puzzleList)):
		numOfIntersections.append(0)
		for spot in puzzleList[i]:
			if spot in comSpots:
				numOfIntersections[i] += 1
	maxVal = max(numOfIntersections)
	maxIndex = numOfIntersections.index(maxVal)
	return maxIndex
#this function returns all spaces that intersect with a given space and 
def intersectingSpaces(space, filledSpaces):
	returnList = []
	for spot in space:
		if spot in comSpots:
			for puzSpace in puzzleList:
				if spot in puzSpace and puzSpace not in filledSpaces:
					returnList.append(puzSpace)
					break
	return returnList

#this function takes the grid, a space, and a word list and returns a list of words
#that satisfy the spaces conditions
def possibleWords(grid, space, wordList):
	newList = []
	letters = []
	indices = []
	for i in range(len(space)):
		if grid[space[i][0]][space[i][1]] != "*":
			letters.append(grid[space[i][0]][space[i][1]])
			indices.append(i)
	for word in wordList:
		addWord = True
		if len(word) == len(space):
			for i in range(len(letters)):
				if word[indices[i]] != letters[i]:
					addWord = False
					break
			if addWord:
				newList.append(word)
	return newList

def solve(grid, space, wordList, filledSpaces):
	possWords = possibleWords(grid, space, wordList)
	for word in possWords:								#for each word in the wordlist
		tempSpaces = copy.deepcopy(filledSpaces)		#copy the occupied spaces and grid (this serves as both a "reset" for these variables as well as allows us to use trial and error without causing data damage)
		tempGrid = copy.deepcopy(grid)
		for i in range(len(word)):							#for each character in the word
			tempGrid[space[i][0]][space[i][1]] = word[i]		#place the character of the word into the grid spot
		if len(wordList) == 1:						#if the word was placed successfully and it was the last word, return 2 (tell the program to wrap up, it's done)
			return (2, tempGrid, wordList)
		tempSpaces.append(space)				#add the space the word took to the cloned occupied spaces list
		tempWordList = copy.deepcopy(wordList)
		tempWordList.remove(word)
		cont = 1
		for common in intersectingSpaces(space, tempSpaces):		#for each intersecting word
			#call the solve function again with the new trial-and-error temporary variables
			(cont, tempGrid, tempWordList) = solve(tempGrid, common, tempWordList, tempSpaces)
			if cont == 0:						#if the recursive solve came back bad
				break							#break and try a new word
			if cont == 2:						#if the recursive solve came back as a success
				return (2, tempGrid, tempWordList)	#pass it on
		if cont:								#if the recursive solve solved all intersecting spaces, but the puzzle is not yet solved
			return (1, tempGrid, tempWordList)	#return the new values but with a 1 to tell the program to continue
	return (0, grid, wordList)							#if the function tried every word in the list and nothing fits, return 0


for puznum in range(1, sum([len(files) for r, d, files in os.walk("puzzles")]) + 1):		#for each file in the puzzle folder
	#initialize variables
	start = time.clock()
	puzzleList = []
	comSpots = []
	filename = "puzzles/puzzle" + str(puznum) + ".txt"
	rows = 0
	cols = 0
	wordCount = 0
	words = []
	grid = []
	#count rows, columns, words, and store words in array
	for line in fileinput.input(filename):
		if '.' in line or '*' in line:
			grid.append(list(line))
		else:
			wordCount += 1
			words.append(line.strip())
	rows = len(grid)
	cols = len(grid[0])
	
	#search the grid for spots to fill and store their locations in a list
	currList = []
	#find horizontal spaces
	for i in range(rows):
		for j in range(cols):
			if grid[i][j] == '*':
				currList.append((i, j))
			if grid[i][j] == '.':
				if len(currList) > 1:
					puzzleList.append(currList)
				currList = []
		if len(currList) > 1:
			puzzleList.append(currList)
		currList = []

	# find vertical spaces
	for j in range(cols):
		for i in range(rows):
			if grid[i][j] == '*':
				currList.append((i, j))
			if grid[i][j] == '.':
				if len(currList) > 1:
					puzzleList.append(currList)
				currList = []
		if len(currList) > 1:
			puzzleList.append(currList)
		currList = []

	comSpots = []
	# find coordinates of shared spaces
	for i in range(len(puzzleList)):
		for j in range(i + 1, len(puzzleList)):
			for coord1 in puzzleList[i]:
				for coord2 in puzzleList[j]:
					if coord1[0] == coord2[0] and coord1[1] == coord2[1] and coord1 not in comSpots:
						comSpots.append(coord1)

	solveSpace = findBestSpace()
	spacesOccupied = []
	(result, grid, x) = solve(grid, puzzleList[solveSpace], words, spacesOccupied)
	end = time.clock()
	times.append(end - start)

	if result:
		print("Puzzle", puznum, ":")
		printGrid(grid)
	else:
		print("Puzzle", puznum, "is not possible")

for time in times:
	print("Puzzle", times.index(time) + 1, "took", time*1000.0, "ms")
print("Total time: ", sum(times)*1000, "ms")