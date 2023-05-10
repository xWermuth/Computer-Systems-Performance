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

    for idx, line in enumerate(lines):
        if idx < NUM_OF_TRIALS:
            times.append(float(line.strip()))
            continue
        words = line.strip().split(" ")
        temp = float(words[len(words) - 1])
        if idx == (NUM_OF_TRIALS - 1) + 1:
            tx = temp
        elif idx == (NUM_OF_TRIALS - 1) + 2:
            rx = temp
        elif idx == (NUM_OF_TRIALS - 1) + 3:
            sm = temp
        elif idx == (NUM_OF_TRIALS - 1) + 4:
            encoder = temp
        elif idx == (NUM_OF_TRIALS - 1) + 5:
            decoder = temp

        
        
        d = line.strip().split(":")
        
        
    rep.close()
    time_avg = np.mean(times)
    return time_avg, sm, decoder, encoder, tx, rx


def build_stress_test_bar_chart(test_type:str):
    pci_width = 16
    times:list[list[int]] = [ []*3 for i in range(3)]
    sms:list[list[int]] = [ []*3 for i in range(3)]
    decoders:list[list[int]] = [ []*3 for i in range(3)]
    encoders:list[list[int]] = [ []*3 for i in range(3)]
    txs:list[list[int]] = [ []*3 for i in range(3)]
    rxs:list[list[int]] = [ []*3 for i in range(3)]
    for pci_gen in range(1, 4):
        gen_idx = pci_gen - 1
        for fold in range(1, FOLDS):
            p = os.path.join(DATA_FOLDER, f"report_{test_type}_{pci_gen}_{pci_width}_{fold}.txt")
            time_avg, sm, decoder, encoder, tx, rx = read_report(p)
            times[gen_idx].append(time_avg)
            sms[gen_idx].append(sm)
            decoders[gen_idx].append(decoder)
            encoders[gen_idx].append(encoder)
            txs[gen_idx].append(tx)
            rxs[gen_idx].append(rx)

    bar_width = 0.25
    x = None
    for pci_gen in range(0, 3):
        if x is not None:
            x = [_x + bar_width for _x in x]
        else:
            x = [i + bar_width for i in range(0, FOLDS - 1)]

        color = 'r' if pci_gen == 0 else 'b' if pci_gen == 1 else 'g'
        label = f"PCI-E x gen: {pci_gen + 1}"

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
        plt.title(f"Average throughput transmitted over PCI-E in GiB/s")
        plt.bar(x, txs[pci_gen], color=color, width=bar_width, label=label)

        plt.figure(5)
        plt.title(f"Average throughput recieved over PCI-E in GiB/s")
        plt.bar(x, rxs[pci_gen], color=color, width=bar_width, label=label)

    names = ["time", "sm", "dec", "enc", "tx", "rx"]
    for i, n in enumerate(names):
        plt.figure(i)
        plt.legend()
        plt.grid()
        plt.xticks([r + bar_width for r in range(0, FOLDS - 1)], [i for i in range(1, FOLDS)])
        plt.xlabel("Number of streams")
        if n == "sm" or n == "dec" or n == "enc":
            plt.ylabel("Percentage")
        elif n == "time":
            plt.ylabel("Seconds")
        else:
            plt.ylabel("GiB/s")
        plt.savefig(os.path.join(OUT_PATH, f"{test_type}_graph_{n}.png"))
        plt.clf()
        plt.cla()

def build_cpu_time_graph():
    pci_gen = 3
    pci_width = 16
    times = []
    for fold in range(1, FOLDS):
        p = os.path.join(DATA_FOLDER, f"report_cpu_{pci_gen}_{pci_width}_{fold}.txt")
        time_avg, _, _, _, _, _ = read_report(p)
        times.append(time_avg)

    plt.clf()
    plt.cla()
    bar_width = 0.25
    x = [i + bar_width for i in range(0, FOLDS - 1)]
    plt.bar(x, times, label="cpu", width = bar_width)
    plt.legend()
    plt.grid()
    plt.title("Average time to decode on CPU in seconds")
    plt.xlabel("Number of streams")
    plt.ylabel("Seconds")
    plt.xticks(x, [i for i in range(1, FOLDS)])
    plt.savefig(os.path.join(OUT_PATH, "cpu_graph_time.png"))


if not os.path.exists(OUT_PATH):
    os.mkdir(OUT_PATH)



args = ["stress_test", "gpu"]

build_stress_test_bar_chart("stress_test")
build_stress_test_bar_chart("gpu")
build_cpu_time_graph()