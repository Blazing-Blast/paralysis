import random

highest = 0
procs = 0
TURN_TOTAL = 231
TURN_SAFE = 177
ATTEMPTS = 1_000_000

for _i in range(ATTEMPTS):
    procs = 0
    for _j in range(TURN_TOTAL):
        if random.getrandbits(2) == 0:  # 1 in 2^2 chance
            procs += 1
    if procs > 177:
        procs = 177
    highest = max(highest, procs)
print(f"The most amount of procs is {highest}")
