
import cs50

while True:
    height = cs50.get_int('Height: ');
    if height > 0 and height <= 23:
        break

hashes = 2
spaces = height - 1

for i in range(height):
    for j in range(spaces):
        print(' ', end='')
    spaces -= 1
    for k in range(hashes):
        print('#',end='')
    hashes += 1
    print()