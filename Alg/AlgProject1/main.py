import fileinput

# count the size of the populations file
rows = 0
cols = 0
for line in fileinput.input("puzzle.txt"):
	if '.' in line or '*' in line:
		rows += 1
		cols = len(line)-1
grid = [[0 for x in range(cols)] for y in range(rows)]
r = 0
c = 0
for line in fileinput.input("puzzle.txt"):
	if r < rows:
		for char in range(len(line) - 1):
			if line[char] == '.':
				grid[r][c] = '.'
			else:
				grid[r][c] = '*'
			c += 1
		r += 1
		c = 0
for i in range(rows):
	for j in range(cols):
		print(grid[i][j], end = "")
	print("\n")
print(rows, cols)
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
