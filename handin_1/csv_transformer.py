import csv
import numpy as np
from os import path


DATA_FILE_PATH = "./"
DATA_FILE_NAME = "minus_perf_times.txt"
ALGORITHMS = ["concurrent","parallel"]

tuple_count = int(pow(2, 24))

data_file = path.join(DATA_FILE_PATH, DATA_FILE_NAME)
file = open(data_file)

csv_data = csv.reader(file, delimiter=',')

data: dict[str, dict[str, dict[str, list[int]]]] = {}
for algo in ALGORITHMS:
    data[algo] = {}

temp = {}

# Millions of tuples per second
def mts(ms: float) -> float:
    return tuple_count / ms / 1000

next(csv_data, None) # skip first line
for line in csv_data:
    algo = line[0]
    ms = float(line[1])
    t = line[2]
    h = line[3]

    key = f"{algo}-{t}-{h}"
    if temp.get(key) is None:
        temp[key] = []
    temp[key].append(mts(ms))
    if(data[algo].get(t) == None):
        data[algo][t] = {}

    if(data[algo][t].get(h) == None):
        data[algo][t][h] = []
    
    data[algo][t][h].append(mts(ms))

print("""
########################################
###  PRINTING STDEV                  ###
########################################
""")
for algo in ALGORITHMS:
    print(f"\t >>> {algo} <<<")
    algo_data = data[algo]
    for t in algo_data:
        print(f"Thread {t}: ", end="")
        for h in algo_data[t]:
            print(np.std(algo_data[t][h]), end=" ")
        print()
    print()

print("""
########################################
###  PRINTING AVG (a.k.a. MEAN)      ###
########################################
""")

temp2 = {}
for key, tp in temp.items():
    v = key.split("-")
    algo = v[0]
    t = v[1]
    h = v[2]
    new_key = f"{algo}-{t}"
    if temp2.get(new_key) is None:
        temp2[new_key] = []
    hash_bit = int(h)
    temp2[new_key].append((hash_bit, np.mean(tp)))


for key, tp in temp2.items():
    meta = key.split("-")
    algo = meta[0]
    thread = meta[1]
    print(f">>> {algo} <<<")
    print(f"Thread {thread}: ")
    tp.sort(key=lambda a: a[0])
    data = list(map(lambda x: x[1], tp))
    print(data)
    print()

# for algo in ALGORITHMS:
#     print(f">>> {algo} <<<")
#     algo_data = data[algo]
#     for t in algo_data:
#         print(f"Thread {t}: ", end="")
#         for h in algo_data[t]:
#             print(np.mean(algo_data[t][h]), end=" ")
#         print()
