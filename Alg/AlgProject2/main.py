import fileinput


def createShiftTable(inputStr):
    shiftTable = {}
    for i in range(len(inputStr) - 2, -1, -1):
        if inputStr[i] not in shiftTable:
            shiftTable[inputStr[i]] = len(inputStr) - 1 - i
    return shiftTable

def shift(stringToSearch, wordToFind, index, shiftTable):
    shiftChar = index + len(wordToFind) - 1
    if shiftChar > len(stringToSearch) - 1:
        return shiftChar
    for i in range(2):                                              #shift based on the last two characters in the word
        if stringToSearch[shiftChar - i] in shiftTable:             #this allows the shifting algorithm to work
            shift = index + shiftTable[stringToSearch[shiftChar - i]] - i  #even though the word may be mispelled by one letter
            if shift - index == 0:
                shift = index + 1
            return shift
    return index + len(wordToFind) - 1

def matchFound(stringToSearch, wordToFind, index):
    # numMatches = 0
    # if lastIndex > len(stringToSearch):
    #     return 0
    # for i in range(0, len(wordToFind)):
    #     if stringToSearch[i + index] == wordToFind[i]:
    #         numMatches += 1
    # if numMatches == len(wordToFind) - 2:
    #     if sorted(stringToSearch[index:lastIndex]) == sorted(wordToFind):
    #         return 1
    #     if
    # if numMatches < len(wordToFind) - 1:
    #     return 0
    # if numMatches == len(wordToFind) - 1:
    #     return 1
    # if numMatches == len(wordToFind):
    #     return 2
    lastIndex = index + len(wordToFind)
    compareWord = stringToSearch[index:lastIndex]
    #test for direct match
    if compareWord == wordToFind:
        return 2
    stack = list(wordToFind)
    #test for missing letter
    copyStack = stack
    skips = 1
    i = 0
    while i < len(compareWord) - 1:
        if copyStack.pop(0) != compareWord[i]:
            skips -= 1
            i -= 1
            if skips < 0:
                break
        i += 1
    if skips >= 0:
        return 1
    #test for adjacent swap
    stack = list(wordToFind)
    copyStack = stack
    skips = 1
    swappedChar = ""
    for i in range(len(compareWord)):
        if copyStack[0] != compareWord[i] and copyStack[0] != swappedChar:
            swappedChar = copyStack[0]
            skips -= 1
            if skips < 0:
                break
        else:
            swappedChar = ""
        copyStack.pop(0)
    if skips >= 0:
        return 1
    #test for random insertion
    skips = 1
    compareWord = stringToSearch[index:lastIndex + 1]
    copyStack = list(compareWord)
    i = 0
    while i < len(wordToFind):
        if copyStack.pop(0) != wordToFind[i]:
            skips -= 1
            i -= 1
            if skips < 0:
                break
        i += 1
    if skips >= 0:
        return 1
    return 0



cont = True
while cont:
    fileName = "search_files/"
    fileName += input("Enter the name of the file to search:")
    wordToFind = input("Enter the word to search for:")
    shiftTable = createShiftTable(wordToFind)
    mispelledWords = []
    currIndex = 0
    stringToSearch = ""
    for line in fileinput.input(fileName):
        stringToSearch += line
    while True:
        found = matchFound(stringToSearch, wordToFind, currIndex)
        if found == 2 or currIndex > len(stringToSearch) - 1:
            break
        if found == 1:
            mispelledWords.append(currIndex)
            currIndex += 1
        else:
            currIndex = shift(stringToSearch, wordToFind, currIndex, shiftTable)
    if found == 0:
        if mispelledWords:
            found = 1
        else:
            print("String not found")
    if found == 1:
        print("String found, but mispelled, at:")
        for word in mispelledWords:
            print("index:", word, "section:", stringToSearch[word:word + len(wordToFind)])
        print("Number of approximate matches:", len(mispelledWords))
    if found == 2:
        print("String found at", currIndex)