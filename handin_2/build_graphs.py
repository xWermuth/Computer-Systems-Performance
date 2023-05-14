import matplotlib.pyplot as plt
from monitor import *
import numpy as np

DATA_FOLDER = "./data"
OUT_PATH = "./graphs"

def read_report(path:str):
    rep = open(path, "r")
    lines = rep.readlines()
    times = []
    sm = None
    decoder = None
    encoder = None
    tx = None
    rx = None

    temp_idx = 0
    for idx, line in enumerate(lines):
        words = line.strip().split(" ")
        if words[0] != "PCI:":
            times.append(float(line.strip()))
            continue
        temp = float(words[len(words) - 1])
        if temp_idx == 0:
            tx = temp
        elif temp_idx == 1:
            rx = temp
        elif temp_idx == 2:
            sm = temp
        elif temp_idx == 3:
            encoder = temp
        elif temp_idx == 4:
            decoder = temp
        temp_idx += 1

        
        
        d = line.strip().split(":")
        
        
    rep.close()
    time_avg = np.mean(times)
    return time_avg, sm, decoder, encoder, tx, rx


def build_stress_test_bar_chart(test_type:str, report_name = None, dir = DATA_FOLDER, _from = 1, to = 4, _label = None):
    report_name = test_type if report_name == None else report_name
    pci_width = 16
    times:list[list[int]] = [ []*3 for i in range(3)]
    sms:list[list[int]] = [ []*3 for i in range(3)]
    decoders:list[list[int]] = [ []*3 for i in range(3)]
    encoders:list[list[int]] = [ []*3 for i in range(3)]
    txs:list[list[int]] = [ []*3 for i in range(3)]
    rxs:list[list[int]] = [ []*3 for i in range(3)]
    for pci_gen in range(_from, to):
        gen_idx = pci_gen - 1
        for fold in range(1, FOLDS):
            p = os.path.join(dir, f"report_{report_name}_{pci_gen}_{pci_width}_{fold}.txt")
            time_avg, sm, decoder, encoder, tx, rx = read_report(p)
            times[gen_idx].append(time_avg)
            sms[gen_idx].append(sm)
            decoders[gen_idx].append(decoder)
            encoders[gen_idx].append(encoder)
            txs[gen_idx].append(tx)
            rxs[gen_idx].append(rx)

    bar_width = 0.25
    x = None
    for pci_gen in range(_from - 1, to - 1):
        if x is not None:
            x = [_x + bar_width for _x in x]
        else:
            x = [i + bar_width for i in range(0, FOLDS - 1)]

        color = 'r' if pci_gen == 0 else 'b' if pci_gen == 1 else 'g'
        label = f"PCI-E gen. {pci_gen + 1}" if _label == None else _label

        plt.figure(0)
        plt.title(f"Average time to decode in seconds")
        plt.bar(x, times[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(1)
        plt.title(f"Average sms used in percentage")
        plt.bar(x, sms[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(2)
        plt.title(f"Average decoder used in percentage")
        plt.bar(x, decoders[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(3)
        plt.title(f"Average encoder used in percentage")
        plt.bar(x, encoders[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(4)
        plt.title(f"Average throughput transmitted over PCI-E in MB/s")
        plt.bar(x, txs[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(5)
        plt.title(f"Average throughput recieved over PCI-E in MB/s")
        plt.bar(x, rxs[pci_gen], color=color, width=bar_width, label=label)

        if test_type == "stress_test" or test_type == "gpu" or test_type == "two_process":
            plt.figure(6)
            plt.title(f"Average throughput recieved and transmitted over PCI-E in MB/s")
            plt.bar(x, [a + b for a, b in zip(rxs[pci_gen], txs[pci_gen])], color=color, width=bar_width, label=label)

    names = ["time", "sm", "dec", "enc", "tx", "rx"]


    if test_type == "stress_test" or test_type == "gpu" or "two_process":
        names.append("total_pci")

    for i, n in enumerate(names):
        plt.figure(i)
        plt.legend(loc="lower right")
        plt.grid()
        plt.xticks([r + bar_width for r in range(0, FOLDS - 1)], [i for i in range(1, FOLDS)])
        plt.xlabel("Number of streams")
        if n == "sm" or n == "dec" or n == "enc":
            plt.ylabel("Percentage")
        elif n == "time":
            plt.ylabel("Seconds")
        else:
            plt.ylabel("MB/s")
        plt.savefig(os.path.join(OUT_PATH, f"{test_type}_graph_{n}.png"))
        plt.clf()
        plt.cla()

def build_cpu_time_graph():
    pci_gen = 3
    pci_width = 16
    cpu_times = []
    gpu_times = []
    for fold in range(1, FOLDS):
        cpu_p = os.path.join(DATA_FOLDER, f"report_cpu_{pci_gen}_{pci_width}_{fold}.txt")
        gpu_p = os.path.join(DATA_FOLDER, f"report_gpu_{pci_gen}_{pci_width}_{fold}.txt")
        time_avg_cpu, _, _, _, _, _ = read_report(cpu_p)
        time_avg_gpu, _, _, _, _, _ = read_report(gpu_p)
        cpu_times.append(time_avg_cpu)
        gpu_times.append(time_avg_gpu)

    plt.clf()
    plt.cla()
    bar_width = 0.25
    x = [i + bar_width for i in range(0, FOLDS - 1)]
    plt.bar(x, cpu_times, label="CPU", width = bar_width)
    x = [_x + bar_width for _x in x]
    plt.bar(x, gpu_times, label="GPU", width = bar_width)
    plt.legend(loc="lower right")
    plt.grid()
    plt.title("Average time to decode in seconds on CPU and GPU using PCI gen 3")
    plt.xlabel("Number of streams")
    plt.ylabel("Seconds")
    plt.xticks(x, [i for i in range(1, FOLDS)])
    plt.savefig(os.path.join(OUT_PATH, "cpu_graph_time.png"))


if not os.path.exists(OUT_PATH):
    os.mkdir(OUT_PATH)



args = ["stress_test", "gpu"]

build_stress_test_bar_chart("stress_test")
build_stress_test_bar_chart("gpu")
build_stress_test_bar_chart("two_process", report_name="stress_test", dir = os.path.join("./new_mps_data", "2-procs-mps-12_05_2023"), _from=2, to=3, _label = "PCI gen 3")
build_cpu_time_graph()