import fileinput
import random


def test_word(word, letter_list):
    for letter in word:
        i = 0
        while i < len(letter_list):
            if letter_list[i] == letter:
                letter_list[i] = '*'
                break
            i += 1
        if i == len(letter_list):
            return False
    return True

count = 0
for line in fileinput.input("sixLetters.txt"):
    count += 1
selected = random.randrange(0, count)
selectedWord = open("sixLetters.txt").readlines()[selected - 1]
wordsLeft = []
for line in fileinput.input("dictionary.txt"):
    if test_word(line, list(selectedWord)):
        wordsLeft.append(line.strip())
jumbledWord = ''.join(random.sample(selectedWord.strip(), len(selectedWord.strip())))
jumbledWord
ans = ""
print("Your letters are: ", jumbledWord)
while ans != "qqq" and len(wordsLeft) > 0:
    print("There are ",  len(wordsLeft),  " words left. Enter your guess: ")
    ans = input()
    for x in wordsLeft:
        if ans.lower() == x.lower():
            wordsLeft.remove(x)
            break