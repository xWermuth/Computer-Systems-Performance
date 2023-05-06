import csv
import numpy as np
from os import path


DATA_FILE_PATH = "./"
DATA_FILE_NAME = "2023-05-03T14:36:32+02:00.data"
ALGORITHMS = ["concurrent","parallel"]

tuple_count = int(pow(2, 24))

data_file = path.join(DATA_FILE_PATH, DATA_FILE_NAME)
file = open(data_file)

csv_data = csv.reader(file, delimiter=',')

data: dict[str, dict[str, dict[str, list[int]]]] = {}
for algo in ALGORITHMS:
    data[algo] = {}

# Millions of tuples per second
def mts(ms: float) -> float:
    return tuple_count / ms / 1000

next(csv_data, None) # skip first line
for line in csv_data:
    algo = line[0]
    ms = float(line[1])
    t = line[2]
    h = line[3]
    
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
for algo in ALGORITHMS:
    print()
    print(f">>> {algo} <<<")
    algo_data = data[algo]
    for t in algo_data:
        print(f"Thread {t}: ", end="")
        for h in algo_data[t]:
            print(np.mean(algo_data[t][h]), end=" ")
        print()
