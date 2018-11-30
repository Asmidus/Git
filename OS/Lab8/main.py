import fileinput

FRAME_SIZE = 512
NUM_FRAMES = 1024
f = 0
processNum = 1

#this function takes a file (frame list) and attempts to attach a process to it
def allocatePages(fileName, pages):
    global processNum                   #use our global process number
    counter = NUM_FRAMES                #initialize the counter
    pageArr = []                        #create an array for the pages
    data = open(fileName, "r").readlines()
    for line in data:                   #for every line in the file
        if line[0] == "0":              #if the page is empty
            pageArr.append(NUM_FRAMES - counter)    #append the page to the array
        if len(pageArr) == pages:       #if we reached the required number of pages, finish this loop
            print("There is enough space, loading executable into memory")
            break
        counter -= 1
        if counter == 0:                #if we reached the end of the file and don't have enough pages, return
            print("There is not enough space")
            return
    counter = 0
    for page in pageArr:                    #for each page in the array
        data[page] = str(processNum) + " "  #attach the process number this frame belongs to
        start = pageArr[0]                  #record the page range data for the header frame
        finish = pageArr[0]
        ranges = []
        if page == pageArr[0]:              #if this frame is the header frame of the process
            data[page] = str(processNum) + " P "    #label it for ease of access
            for i in range(len(pageArr) - 1):       #save the frames that belong to it in the header
                if pageArr[i + 1] == pageArr[i] + 1:
                    finish = pageArr[i + 1]
                else:
                    if start != finish:
                        ranges.append(str(start) + "-" + str(finish))
                    else:
                        ranges.append(str(start))
                    start = finish = pageArr[i + 1]
            if start != finish:
                ranges.append(str(start) + "-" + str(finish))
            else:
                ranges.append(str(start))
            for r in ranges:
                data[page] += r + " "
        while len(data[page]) < FRAME_SIZE - 1:     #fill out the rest of the frame until it hits 512 bytes
            data[page] += "0"
        data[page] += "\n"
    open(fileName, "w").writelines(data)            #write the updated data to the file
    processNum += 1                                 #increment the global process number
    return


while True:
    processNum = 1                                  #initialize process number
    fileName = ""
    fileName = input("Enter the path of the data file or enter x to exit: ")
    if fileName == "x":                             #if x, exit the program
        break
    try:                                            #otherwise, try to open an existing file
        f = open(fileName)
        for line in open(fileName, "r").readlines():#find the starting process number we should have for the file
            procNum = int(line.split(" ")[0])
            if int(procNum >= processNum):
                processNum = procNum + 1
    except IOError:                                 #if no existing file is found, make a new one and initialize it
        print("File with that name not found, creating a new one...")
        f = open(fileName, "w+")
        line = "0"*511 + "\n"
        for i in range(NUM_FRAMES):
            f.write(line)
    if fileName == "x":
        break
    validInput = False
    while True:             #while the user is operating on the file, get input
        select = input("E to load an executable\nD to dump\nL to list\nX to enter new data file\n")
        if select == "E" or select == "e":          #if e, prompt user for the number of pages the executable will take
            numPages = int(input("How many pages does this executable take?"))
            allocatePages(fileName, numPages)
        elif select == "L" or select == "l":        #if l, list all executables and the pages they take up
            data = open(fileName, "r").readlines()
            for line in data:
                if line[0] != "0" and line.split(" ")[1] == "P":
                    ranges = line.split(" ")
                    print("Executable", ranges[0] , "located - occupies pages:", end = " ")
                    for i in range(2,len(ranges) - 1):
                        print(ranges[i], end = " ")
                    print()
        elif select == "D" or select == "d":        #if d, list all frames and the executables they belong to
            data = open(fileName, "r").readlines()
            for i in range(len(data)):
                if data[i][0]  != "0":
                    print("Frame", i, "is nonempty, belongs to process #", end = "")
                    print(data[i].split(" ")[0])
        elif select == "X" or select == "x":        #if x, exit the menu
            break
