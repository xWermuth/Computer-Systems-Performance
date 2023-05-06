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
            # print(cache_miss)
            data[meta_data].append(cache_miss)

    grep_in.close()

x_ticks = HASH_BITS

algo_map = {}

for key, cache_misses in data.items():
    # print(f"{key}: {np.average(cache_misses)}")
    meta = key.split("-")
    algo = meta[0]
    thread = meta[1]
    hasbit = meta[2]
    cache_miss_avg = np.average(cache_misses)
    my_key = algo + "-" + thread
    if algo_map.get(my_key) is None:
        algo_map[my_key] = []    
    algo_map[my_key].append(cache_miss_avg)

    # plt.plot(x_ticks, cache_miss)
    # plt.xticks(x_ticks)

# plt.savefig("here.png")
    


# plot
my_max = 0

for a_t, h_avg in algo_map.items():
    try:
        meta = a_t.split("-")
        alg = meta[0]
        thread = meta[1]
        print(len(h_avg))
        print(len(x_ticks))
        # if(len(h_avg) != len(x_ticks)):
        #     print("skip", a_t)
        #     continue
        if alg == "concurrent":
            print("IS OTSSSS")
            plt.figure(0)
        else: 
            print("sssS")
            plt.figure(1)
        plt.plot(x_ticks[:17], h_avg[:17], label=f"Thread {thread}")
        plt.xticks(x_ticks)
        plt.legend()
        my_max = np.max(h_avg)

        # print(f"{a_t}: \nmax: {np.max(h_avg)}, \nmin: {np.min(h_avg)}, \nmid: {np.median(h_avg)}\n\n")
        # plt.yticks([1_000_000,5_000_000, 10_000_000, 20_000_000, 30_000_000,40_000_000,50_000_000, 100_000_000, 200_000_000])
        ax = plt.gca()
        ax.get_xaxis().get_major_formatter().set_useOffset(False)
    except:
        pass

plt.figure(0)
plt.savefig(f"concurrent.png")
plt.figure(1)
plt.savefig(f"parallel.png")
    
