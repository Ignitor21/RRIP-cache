import random

size = int(input("Input cache size: "))
n = int(input("Input number of requests: "))
list = []

for i in range(n):
        list.append(random.randint(1, size + 200000))

list_str = [str(i) for i in list]

with open("random_test3.in", "w") as f:
    f.write(str(size) + " " + str(n) + " ")
    f.write(" ".join(list_str))