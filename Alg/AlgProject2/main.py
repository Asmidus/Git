shiftTable = {}
wordToFind = ""
stringToSearch = ""
mispelledWords = []
index = 0

def createShiftTable():
    shiftTable.clear()
    for i in range(len(wordToFind) - 2, -1, -1):
        if wordToFind[i] not in shiftTable:
            shiftTable[wordToFind[i]] = len(wordToFind) - 1 - i

def shift():
    shiftChar = index + len(wordToFind) - 1
    for i in range(len(wordToFind)):
        if stringToSearch[shiftChar - i] in shiftTable:
            index += shiftTable[stringToSearch[shiftChar - i]] - i
            return
    index + len(wordToFind)
    return

def matchFound():
    numMatches = 0
    if index + len(wordToFind) > len(stringToSearch):
        return 0
    for i in range(0, len(wordToFind)):
        if stringToSearch[i + index] == wordToFind[i]:
            numMatches += 1
    if numMatches < len(wordToFind) - 1:
        return 0
    if numMatches == len(wordToFind) - 1:
        return 1
    if numMatches == len(wordToFind):
        return 2


stringToSearch = " taco tacotacotaco taco taco ttttaco"
wordToFind = "tado"
createShiftTable()
index = 0
found = matchFound()
while found != 2 and  < len(stringToSearch):
    shift()
    found = matchFound()
    if found == 1:
        mispelledWords.append()
         += len(wordToFind)
if found == 0:
    if mispelledWords:
        found = 1
    else:
        print("String not found")
if found == 1:
    print("String found, but mispelled, at:")
    for word in mispelledWords:
        print(word, end=" ")
if found == 2:
    print("String found at", )


