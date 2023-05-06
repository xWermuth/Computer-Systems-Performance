import matplotlib.pyplot as plt
import subprocess as sp
import numpy as np

CACHE_MISSES_ACRO = "'cache-misses'"

DIRS = [
    "2023-05-05T00:02:16+02:00"
]
THREADS = [1,2,4,8,16,32]
HASH_BITS = list(range(1,19))
REPORTS = 10
ALGOS = ["concurrent", "parallel"]

# grep me some data

data = {}

for d in DIRS:
    file = f"grep.{d}.out"
    grep_out = open(file, "w")

    GREP_CMD = ["grep", "-r", "-A", "1", "-F", "'cache-misses'", f"{d}"]
    ret_val = sp.Popen(GREP_CMD, stdout=grep_out).wait()
    if ret_val != 0:
        print("Non-zero exit code for grep", ret_val)
        exit(ret_val)

    grep_out.close()
    grep_in = open(file, "r")
    # continue
    # grep_out.seek(0, 0)

    cache_misses = 0
    lines_arr = grep_in.readlines()
    for idx, l in enumerate(lines_arr):
        if l.strip() == '--':
            continue
        line = l.split("/")
        meta_data = line[1]
        stats = line[2].split()
        # algo = meta_data[0]
        # threads = meta_data[1]
        # bits = meta_data[2]
        if data.get(meta_data) is None:
            data[meta_data] = []
        
        if stats[len(stats) - 1].strip() == CACHE_MISSES_ACRO:
            stats = lines_arr[idx + 1].split()
            cache_miss = int(stats[len(stats) - 1].strip())
            data[meta_data].append(cache_miss)

    grep_in.close()

x_ticks = HASH_BITS
DIVIDE_FACTOR = 1_000_000

algo_map = {}

for key, cache_misses in data.items():
    meta = key.split("-")
    algo = meta[0]
    thread = meta[1]
    hasbit = meta[2]
    cache_miss_avg = np.average(cache_misses) / DIVIDE_FACTOR
    print()
    my_key = algo + "-" + thread
    if algo_map.get(my_key) is None:
        algo_map[my_key] = []    
    algo_map[my_key].append((int(hasbit), cache_miss_avg))

for a_t, h_avg in algo_map.items():
    meta = a_t.split("-")
    alg = meta[0]
    thread = meta[1]
    # if(len(h_avg) != len(x_ticks)):
    #     print("skip", a_t)
    #     continue
    if alg == "concurrent":
        plt.figure(0)
    else: 
        plt.figure(1)
    # sort by hasbit
    h_avg.sort(key=lambda a: a[0])
    data = list(map(lambda x: x[1], h_avg))
    plt.plot(x_ticks[:17], data[:17], label=f"Thread {thread}")
    plt.xticks(x_ticks)
    plt.legend()
    ax = plt.gca()
    ax.get_xaxis().get_major_formatter().set_useOffset(False)


plt.figure(0)
plt.title("Concurrent algorithm cache misses in millions")
plt.savefig("concurrent.png")
plt.figure(1)
plt.title("Parallel algorithm cache misses in millions")
plt.savefig("parallel.png")
    
