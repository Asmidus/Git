import fileinput
import time


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

def matchFound(stringToSearch, wordToFind, index, mispelledWords):
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
    # lastIndex = index + len(wordToFind)
    # compareWord = stringToSearch[index:lastIndex]
    # #test for direct match
    # if compareWord == wordToFind:
    #     return 2
    # stack = list(wordToFind)
    # #test for missing letter
    # copyStack = stack
    # skips = 1
    # i = 0
    # if len(compareWord) < len(wordToFind):
    #     if len(wordToFind) == 2 and len(compareWord) == 1:
    #         if compareWord in wordToFind:
    #             mispelledWords.append(index)
    #     return 0
    # while i < len(compareWord) - 1:
    #     if copyStack.pop(0) != compareWord[i]:
    #         skips -= 1
    #         i -= 1
    #         if skips < 0:
    #             break
    #     i += 1
    # if skips >= 0:
    #     mispelledWords.append(index)
    # if len(compareWord) < len(wordToFind):
    #     return 0
    # #test for adjacent swap
    # stack = list(wordToFind)
    # copyStack = stack
    # skips = 1
    # swappedChar = ""
    # for i in range(len(compareWord)):
    #     if copyStack[0] != compareWord[i] and copyStack[0] != swappedChar:
    #         swappedChar = copyStack[0]
    #         skips -= 1
    #         if skips < 0:
    #             break
    #     else:
    #         swappedChar = ""
    #     copyStack.pop(0)
    # if skips >= 0:
    #     mispelledWords.append(index)
    # #test for random insertion
    # skips = 1
    # compareWord = stringToSearch[index:lastIndex + 1]
    # copyStack = list(compareWord)
    # i = 0
    # while i < len(wordToFind):
    #     if copyStack.pop(0) != wordToFind[i]:
    #         skips -= 1
    #         i -= 1
    #         if skips < 0:
    #             break
    #     i += 1
    # if skips >= 0:
    #     mispelledWords.append(index)
    # return 0
    lastIndex = index + len(wordToFind) + 1
    if wordToFind == stringToSearch[index:lastIndex - 1]:
        return 2
    compareWord = stringToSearch[index:lastIndex]
    if len(compareWord) < len(wordToFind):
        return 0
    doubleCase = 0
    errNum = 0
    stack = list(wordToFind)
    if len(stack) == 2:
        compareWordSmall = compareWord[:2]
        if compareWordSmall == wordToFind:
            return 2
        if compareWordSmall == wordToFind[0]*2 or compareWordSmall == wordToFind[1]*2:
            mispelledWords.append(index)
            return 1
        if wordToFind[1] == compareWordSmall[1]:
            return 1
        if wordToFind[0] == compareWord[0] and wordToFind[1] == compareWord[2]:
            mispelledWords.append(index)
            mispelledWords.append(index)
            return 1
        if wordToFind[0] in compareWordSmall or wordToFind[1] in compareWordSmall:
            return 1
        return 0
    #longer words   
    for i in range(len(compareWord)):
        if stack[0] == compareWord[i]:
            stack.pop(0)
        else:
            errNum += 1
            if errNum == 1:
                if len(stack) > 1 and stack[1] == compareWord[i]:
                    if stack[0] == compareWord[i+1]:
                        stack.pop(1)
                        if i == len(compareWord) - 3:
                            doubleCase = 1
                    elif i == len(compareWord) - 2:
                        doubleCase = 1
                        stack.pop(0)
                        stack.pop(0)
                    else:
                        stack.pop(0)
                        stack.pop(0)
                elif len(stack) > 1 and i < len(compareWord) - 2 and stack[1] == compareWord[i+1]:
                    stack.pop(0)
                elif i == len(wordToFind) - 1 and len(stack) <= 1:
                    mispelledWords.append(index)
                    return 1
            if errNum > 1:
                return 0
        if len(stack) == 0:
            if errNum == 0:
                return 2
            if doubleCase:
                mispelledWords.append(index)
            return 1
    return 1




cont = True
while cont:
    fileName = "search_files/"
    fileName += input("Enter the name of the file to search:")
    wordToFind = input("Enter the word to search for:")
    # wordToFind = "brain"
    start = time.clock()
    shiftTable = createShiftTable(wordToFind)
    mispelledWords = []
    currIndex = 0
    stringToSearch = ""
    for line in fileinput.input(fileName):
        stringToSearch += line
    stringToSearch = stringToSearch.replace("\n", "*")
    while True:
        found = matchFound(stringToSearch, wordToFind, currIndex, mispelledWords)
        if found == 2 or currIndex > len(stringToSearch) - 1:
            break
        if found == 1:
            mispelledWords.append(currIndex)
            currIndex += 1
        else:
            currIndex = shift(stringToSearch, wordToFind, currIndex, shiftTable)
            # currIndex += 1
    end = time.clock()
    if found == 0:
        if mispelledWords:
            found = 1
        else:
            print("String not found")
    if found == 1:
        print("String found, but mispelled, at:")
        for word in mispelledWords:
            print("Mismatch num:", mispelledWords.index(word), "index:", word, "section:", stringToSearch[word:word + len(wordToFind) + 1])
        print("Number of approximate matches:", len(mispelledWords))
    if found == 2:
        print("String found at", currIndex)
    print((end - start)*1000.0)