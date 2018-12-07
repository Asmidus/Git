import fileinput

frames = []
FRAME_SIZE = 512
NUM_FRAMES = 1024
f = 0

def allocatePages(fileName, pages):
    counter = 1024
    pageArr = []
    # outputFile = open(fileName, "r")
    data = open(fileName, "r").readlines()
    for line in data:
        if line[0] == "0":
            pageArr.append(1024 - counter)
        if len(pageArr) == pages:
            print("There is enough space, loading executable into memory")
            break
        counter -= 1
        if counter == 0:
            print("There is not enough space")
    counter = 0
    for page in pageArr:
        data[page] = "E"
        start = pageArr[0]
        finish = 0
        ranges = []
        if page == pageArr[0]:
            for i in range(len(pageArr) - 1):
                if pageArr[i + 1] == pageArr[i] + 1:
                    finish = pageArr[i + 1]
                else:
                    if start != finish:
                        ranges.append(str(start) + "-" + str(finish))
                    else:
                        ranges.append(start)
                    start = finish = pageArr[i + 1]
            if start != finish:
                ranges.append(str(start) + "-" + str(finish))
            else:
                ranges.append(start)
            for r in ranges:
                data[page] += r + " "
        while len(data[page]) < 511:
            data[page] += "0"
        data[page] += "\n"
    open(fileName, "w").writelines(data)
    return


while True:
    fileName = ""
    fileName = input("Enter the path of the data file or enter x to exit: ")
    if fileName == "x":
        break
    try:
        f = open(fileName)
    except IOError:
        print("File with that name not found, creating a new one...")
        f = open(fileName, "w+")
        line = "0"*511 + "\n"
        print(len(line.encode('utf-8')))
        for i in range(1024):
            f.write(line)
    if fileName == "x":
        break
    validInput = False
    while True:
        select = input("E to load an executable\nD to dump\nL to list\nX to exit\n")
        if select == "E":
            numPages = int(input("How many pages does this executable take?"))
            allocatePages(fileName, numPages)
        elif select == "D":
            data = open(fileName, "r").readlines()
            for line in data:
                if line[0] != "0" and line[1] != "0":
                    ranges = line.split(" ")
                    print("Executable located - occupies pages")
