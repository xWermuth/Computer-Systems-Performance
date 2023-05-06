import re
import glob, os
import numpy as np
import matplotlib.pyplot as plt

REGEX = "(?P<name>[a-zA-Z][a-zA-Z0-9)( -]*)\#\s*(?P<value>\d+\.\d+)"

OUT_PIC_NAME = "./pictures"
if not os.path.exists(OUT_PIC_NAME):
    os.mkdir(OUT_PIC_NAME)
ALGOS = ["concurrent", "parallel"]
FILES:list[str] = []
stats:dict[str, list[float]] = {}
for algo in ALGOS:
    for root, dirs, files in os.walk(f"./{algo}"):
        for file_path in files:
            if file_path.endswith(".txt"):
                FILES.append(os.path.join(root, file_path))

for file_path in FILES:
    dir_split = file_path.split("/")
    meta = dir_split[3].split("-")
    algo = dir_split[1]
    thread = meta[0]
    hashbit = meta[1]
    with open(file_path) as f:
        lines = f.readlines()
        for l in lines:
            res = re.search(REGEX, l)
            if res is None:
                continue

            name = res.group('name').strip()
            value = res.group('value')
            # print(f"name: {name}, value: {value}")
            key = f"{algo}/{thread}/{hashbit}/{name}"
            if stats.get(key) is None:
                stats[key] = []
            stats[key].append(float(value))
    
data_map:dict[str, list[float]] = {}

for key, val in stats.items():
    meta = key.split("/")
    algo = meta[0]
    thread = meta[1]
    name = meta[3]
    avg = np.mean(val)
    new_key = f"{algo}/{thread}/{name}"
    if data_map.get(new_key) is None:
        data_map[new_key] = []   
    hash_bit = int(meta[2].replace("h", "").strip())
    data_map[new_key].append((hash_bit, avg))

HASH_BITS = list(range(1, 19))
x_ticks = HASH_BITS
map_idx = {}
plot_idx = 0
for a_t, h_avg in data_map.items():
    meta = a_t.split("/")
    algo = meta[0]
    thread = meta[1]
    name = meta[2]
    key = f"{algo}/{name}"
    if map_idx.get(key) is None:
        map_idx[key] = plot_idx
        plot_idx +=1
    plt.figure(map_idx[key])
    # sort by hasbit
    h_avg.sort(key=lambda a: a[0])
    data = list(map(lambda x: x[1], h_avg))
    n_thread = thread.replace("t", "")
    plt.plot(x_ticks[:17], data[:17], label=f"Thread {n_thread}")
    plt.xticks(x_ticks)
    plt.legend()

# Plot graphs
for key, idx in map_idx.items():
    meta = key.split("/")
    algo = meta[0]
    name = meta[1]
    plt.figure(idx)
    plt.title(f"{algo} // {name}")
    plt.savefig(f"{OUT_PIC_NAME}/{algo}-{name}.png")
    plt.close()