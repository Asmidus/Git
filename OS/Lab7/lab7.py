import fileinput

#read input from the text file
while True:
    lineNum = 0
    processes = 0
    resourceTypes = 0
    startingResources = []
    allocationMatrix = []
    requestMatrix = []
    validFile = False
    while not validFile:        #continue trying to get input so long as the file name is invalid
        fileName = input("Enter the path of the data file or enter x to exit: ")
        if fileName == "x":
            break
        try:
            open(fileName)
            validFile = True
        except IOError:
            print("Error - file not found")
    if fileName == "x":
        break
    for line in fileinput.input(fileName):
        line = line.replace(" ", "")
        if lineNum == 0:                    #read the number of processes
            processes = int(line[0])
        elif lineNum == 1:                  #read the number of resources
            resourceTypes = int(line[0])
        elif lineNum == 2:                  #read the initial resources
            for i in range(resourceTypes):
                startingResources.append(int(line[i]))
        elif lineNum > 2 and lineNum < processes + 3:               #read the allocation matrix
            allocationMatrix.append([])
            for i in range(resourceTypes):
                allocationMatrix[lineNum - 3].append(int(line[i]))
        elif lineNum > processes + 2 and lineNum < 2*processes + 3: #read the request matrix
            requestMatrix.append([])
            for i in range(resourceTypes):
                requestMatrix[lineNum - 3 - processes].append(int(line[i]))
        lineNum += 1
    #Check for a deadlock
    counter = 0         #The number of processes that finished in the current loop
    finished = []       #The finished processes
    unfinished = list(range(processes))
    cont = True
    while True:
        counter = 0
        for i in unfinished:        #for each unfinished process, check if the request can be granted
            cont = True
            for j in range(resourceTypes):
                if requestMatrix[i][j] > startingResources[j]:
                    cont = False
                    break
            if cont:                #if the request can be granted, allocate the resources and finish the process
                for j in range(resourceTypes):
                    startingResources[j] += allocationMatrix[i][j]
                unfinished.remove(i)
                finished.append(i)
                counter += 1
        if counter == 0:
            break
    if not unfinished:
        print("No deadlock detected using this sequence:", finished)
    else:
        print("Deadlock detected and contains the following processes:", unfinished)