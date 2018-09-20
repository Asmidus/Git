import fileinput
import itertools

# count the size of the populations file
rows = 0
cols = 0
wordCount = 0
words = []
wordLengths = [[] for x in range(100)]
print(wordLengths)
# count rows, columns, words, and store words in array
for line in fileinput.input("puzzles/puzzle3.txt"):
	if '.' in line or '*' in line:
		rows += 1
		cols = len(line)-1
	else:
		wordCount += 1
		words.append(line.strip())
# create a grid to match the puzzle
grid = [[0 for x in range(cols)] for y in range(rows)]
r = 0
c = 0
for line in fileinput.input("puzzles/puzzle3.txt"):
	if r < rows:
		for char in range(len(line) - 1):
			if line[char] == '.':
				grid[r][c] = '.'
			else:
				grid[r][c] = '*'
			c += 1
		r += 1
		c = 0
# search the grid for spots to fill and store their locations in a list
puzzleList = []
currList = []
# find horizontal spots
for i in range(rows):
	for j in range(cols):
		if grid[i][j] == '*':
			currList.append([i, j])
		if grid[i][j] == '.':
			if len(currList) > 1:
				puzzleList.append(currList)
			currList = []
	if len(currList) > 1:
		puzzleList.append(currList)
	currList = []
# find vertical spots
for j in range(cols):
	for i in range(rows):
		if grid[i][j] == '*':
			currList.append([i, j])
		if grid[i][j] == '.':
			if len(currList) > 1:
				puzzleList.append(currList)
			currList = []
	if len(currList) > 1:
		puzzleList.append(currList)
	currList = []
for i in range(wordCount):
	words.append(words[i][::-1])
for word in words:
	wordLengths[len(word)].append(word)
print("Finished shit")
attempts = []
add = True
counter = 0
print(wordLengths)
for spot in puzzleList:
	for i in range(wordLengths):
		if len(spot) == i:
			
# for solution in itertools.permutations(words, wordCount):
# 	add = True
# 	for i in range(wordCount):
# 		if len(solution[i]) != len(puzzleList[i]):
# 			add = False
# 			break
# 	if add:
# 		attempts.append(solution)
# 		print(len(attempts))
# 	counter += 1
# 	print(counter)

# for bruteForceAttempt in attempts:
# 	cont = True
# 	for i in range(wordCount):
# 		for j in range(len(bruteForceAttempt[i])):
# 			if grid[puzzleList[i][j][0]][puzzleList[i][j][1]] == '*':
# 				grid[puzzleList[i][j][0]][puzzleList[i][j][1]] = bruteForceAttempt[i][j]
# 			if grid[puzzleList[i][j][0]][puzzleList[i][j][1]] != bruteForceAttempt[i][j]:
# 				cont = False
# 				break
# 		if cont == False:
# 			break
# 	if cont == False:
# 		for spot in puzzleList:
# 			for coord in spot:
# 				grid[coord[0]][coord[1]] = '*'
# 	else:
# 		break

# commonSpots = []
# # find coordinates of shared spaces
# for spot1 in puzzleList:
# 	for spot2 in puzzleList:
# 		if spot1 is not spot2:
# 			for coord1 in spot1:
# 				for coord2 in spot2:
# 					if coord1[0] == coord2[0] and coord1[1] == coord2[1] and coord1 not in commonSpots:
# 						commonSpots.append(coord1)
# print(commonSpots)
for i in range(rows):
	for j in range(cols):
		print(grid[i][j], end = "")
	print("\n")
print(rows, cols)
for word in words:
	print(word)
# # create a dictionary and store the data inside of it
# popDict = {}
# for line in fileinput.input("populations.csv"):
# 	line = line.split(",")
# 	popDict[line[0]] = line[1]
# input("Press enter to run the query")
# # print the queries from the dictionary
# for line in fileinput.input("queries.txt"):
# 	print(line, popDict.get(line.strip()))
# # create a 2D array to store the data in
# popList = [[0] * 2 for i in range(size)]
# count = 0
# for line in fileinput.input("populations.csv"):
# 	line = line.split(",")
# 	popList[count][0] = line[0]
# 	popList[count][1] = line[1]
# 	count += 1
# input("Press enter to run the next query")
# # print the data of the string that matches
# for line in fileinput.input("queries.txt"):
# 	for i in range(size):
# 		if line.strip() == popList[i][0].strip():
# 			print(line, popList[i][1])
# 			break
