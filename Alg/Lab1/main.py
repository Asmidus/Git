import fileinput
import random

count = 0
for line in fileinput.input("capitals.txt"):
    count += 1
selected = random.randrange(0, count)
ans = open("capitals.txt").readlines()[selected - 1]
anshidden = ""
guess = ""
for i in range(len(ans)):
    if ans[i].isalpha():
        anshidden += "-"
    else:
        anshidden += ans[i]
while guess != "stop":
    print(anshidden)
    guess = "initialize"
    while len(guess) > 2 or not guess[0].isalpha():
            guess = input("Guess a letter: ")
            if guess == "stop":
                break
            guess += "."
    found = False
    anscheck = anshidden
    anshidden = ""
    for i in range(len(ans)):
        if ans[i] == guess[0].upper():
            anshidden += guess[0].upper()
        else:
            anshidden += anscheck[i]
    if anshidden == ans:
        print(ans + "\nYou win")
        break