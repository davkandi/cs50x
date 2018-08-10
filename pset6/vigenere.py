import cs50
import sys

if len(sys.argv) != 2:
    print('You can use only two command line')
    exit(1)
else:
    for c in sys.argv[1]:
        if c.isdigit():
            print('You can just use A-Z or a-z characters')
            exit(2)
    k = sys.argv[1]

length = len(k)
pos = 0

plaintext = cs50.get_string('Plaintext: ')
print('Ciphertext: ', end="")

for i in plaintext:
    if i.isupper():
        if k[pos % length].isupper():
            j = ord(k[pos % length]) - 65
        else:
            j = ord(k[pos % length]) - 97
        print(chr((ord(i) - ord('A') + j) % 26 + ord('A')), end="")
        pos += 1
    elif i.islower():
        if k[pos % length].isupper():
            j = ord(k[pos % length]) - 65
        else:
            j = ord(k[pos % length]) - 97
        print(chr((ord(i) - ord('a') + j) % 26 + ord('a')), end="")
        pos += 1
    else:
        print(i, end="")
print()



