import math
import random

with open('data.txt', 'w') as f:
    for i in range(20):
        data = random.uniform(1, 10)
        result = math.pi ** data
        f.write(str(data) + ' ' + str(result) + '\n')

