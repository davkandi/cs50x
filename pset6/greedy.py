import cs50

q = 25
d = 10
n = 5
p = 1

while True:
    owed = cs50.get_float('How much change is owed?: ')
    if owed > 0:
        break

change = owed * 100
cents = round(change)
counter = 0

while (cents >= q):
    cents -= q
    counter += 1

while (cents >= d):
    cents -= d
    counter += 1

while (cents >= n):
    cents -= n
    counter += 1

while (cents >= p):
    cents -= p
    counter += 1

print(counter)
