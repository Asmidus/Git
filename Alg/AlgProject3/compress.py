from PIL import Image
import bisect
import os
import time
MAX_COLORS = 999999999
globalDict = {}
class Node:
    weight = 0
    leftNode = 0
    rightNode = 0
    value = 0
    isLeaf = True
    codeWord = 0
    def __lt__(self, other):
        return self.weight < other.weight

def combine(nodes):
    while len(nodes) > 1:
        min1 = nodes[0]
        min2 = nodes[1]
        newNode = Node()
        newNode.leftNode = min1
        newNode.rightNode = min2
        newNode.weight = min1.weight + min2.weight
        newNode.isLeaf = False
        nodes.remove(min1)
        nodes.remove(min2)
        bisect.insort(nodes, newNode)
    return nodes

def generateCodes(head):
    if head.leftNode:
        head.leftNode = genCodes(head.leftNode, '10')
    if head.rightNode:
        head.rightNode = genCodes(head.rightNode, '11')
    return head

def genCodes(node, currentCode):
    global globalDict
    if node.isLeaf:
        node.codeWord = currentCode
        globalDict[node.value] = node.codeWord
        globalDict[node.codeWord] = node.value
    if node.leftNode:
        genCodes(node.leftNode, currentCode + '0')
    if node.rightNode:
        genCodes(node.rightNode, currentCode + '1')
    return node


def makeTree(image):
    nodes = []
    colors = image.getcolors(MAX_COLORS)
    for color in colors:
        newNode = Node()
        newNode.weight = color[0]
        newNode.value = color[1]
        bisect.insort(nodes, newNode)
    head = combine(nodes)[0]
    head = generateCodes(head)
    return head

userInput = ""
while userInput != "x":
    globalDict = {}
    userInput = input("Which file would you like to compress (x to exit)? ")
    try:
        img = Image.open(userInput)
    except IOError:
        continue
    initialSize = os.path.getsize(userInput)
    userInput = input("Name of compressed file: ")
    px = img.load()
    width, height = img.size
    initialTime = time.clock()
    makeTree(img)
    compressedImage = []
    f = open(userInput, "wb+")
    lastPix = 0
    output = ""
    for x in range(width):
        for y in range(height):
            thisPix = globalDict[(px[x,y][0], px[x,y][1], px[x,y][2])]
            if thisPix == lastPix:
                output += '0'
            else:
                output += globalDict[(px[x,y][0], px[x,y][1], px[x,y][2])]
                lastPix = thisPix
    b = bytearray()
    while len(output)%8 != 0:
        output += '1'
    for i in range(0, len(output), 8):
        test1 = output[i:i+8]
        test2 = int(output[i:i+8], 2)
        b.append(int(output[i:i+8], 2))
    f.write(b)
    f.close()
    finalTime = time.clock()
    finalSize = os.path.getsize(userInput)
    print("Size before:", initialSize, "\nSize after: ", finalSize)
    print("Compression ratio: ", initialSize/finalSize, "\nCompression %: ", 100*(finalSize/initialSize))
    print("Time taken: ", (finalTime - initialTime)*1000.0)
    userInput = ""
    while True:
        userInput = input("Would you like to decompress an image using the generated data structure (y/n)? ")
        if userInput == "n":
            break
        elif userInput != "y":
            continue
        userInput = input("Enter the file you would like to decompress: ")
        try:
            f = open(userInput, "rb")
        except IOError:
            continue
        userInput = input("Enter the name of the file to save to: ")
        byteStr = ""
        initialTime = time.clock()
        byte = f.read(1)
        finalImage = Image.new('RGB', (width,height), "black") # Create a new black image
        pixels = finalImage.load() # Create the pixel map
        x = 0
        y = 0
        while byte != b'':
            bitStr = format(int.from_bytes(byte, "little"), '08b')
            byteStr += bitStr
            byte = f.read(1)
        currString = ""
        lastColor = 0
        for char in byteStr:
            currString += char
            if currString in globalDict or currString == "0":
                if currString == "0":
                    pixels[x, y] = lastColor
                else:
                    pixels[x, y] = lastColor = (globalDict[currString][0], globalDict[currString][1], globalDict[currString][2])
                currString = ""
                y += 1
                if y == height:
                    y = 0
                    x += 1
                    if x == width:
                        break
        finalTime = time.clock()
        print("Time to decompress: ", (finalTime - initialTime)*1000.0)
        finalImage.save(userInput)