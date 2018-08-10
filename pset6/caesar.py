import cs50
import sys


if len(sys.argv) != 2:
    print('Usage: casear.py key')
    exit(1)

for c in sys.argv[1]:
    if c.isalpha():
        print('Key must be an integer')
        exit(2)

if int(sys.argv[1]) < 1:
    print('Key must be a non negative integer')
    exit(2)
else:
    k = int(sys.argv[1])

plaintext = cs50.get_string('Plaintext: ')
print('Ciphertext: ', end="")

for p in plaintext:
    if p.islower():
        print(chr((ord(p) + k - ord('a')) % 26 + ord('a')), end="")
    elif p.isupper():
        print(chr((ord(p) + k - ord('A')) % 26 + ord('A')), end="")
    else:
        print(p, end="")
print()