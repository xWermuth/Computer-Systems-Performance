from functools import reduce
import os 
import subprocess as sp
from datetime import datetime
import matplotlib.pyplot as plt
import numpy as np
import sys
import random

dt_string = datetime.now().strftime("%d_%m_%Y")
DIR_NAME = os.path.join(os.path.dirname(__file__), dt_string)
STD_ERR = os.path.join(DIR_NAME, "errors.txt")
FOLDS = 8


def get_ffmpeg_cmd(out_file:str, in_n: int):
    base = ["time","-f", "%e", "--output", out_file, 
              "ffmpeg", "-loglevel", "error", "-hide_banner"]

    for _ in range(in_n):
        base += ["-c:v", "h264_cuvid", "-i", "/dev/shm/cut.mp4", "-vf", "'hwupload_cuda'", "-t", "60", "-f", "null", "-"] 

    return base

SMI_CMD = ["nvidia-smi",  "dmon", "-s", "pucvmt"]

def get_gpu_stats_path(prefix:str):
    return os.path.join(DIR_NAME, f"gpu_stats{prefix}.txt")

# def create_file(name: str) -> None:
#     open

def generate_report(meta: str, REPORT_OUT_FILE:str, GPU_OUT_FILE:str):
    gpu_stats = open(GPU_OUT_FILE, "r")
    lines = gpu_stats.readlines()
    rxs:list[int] = []
    txs:list[int] = []
    sm:list[int] = []
    encoders:list[int] = []
    decoders:list[int] = []
    for idx, line in enumerate(lines):
        stats = line.split()
        if stats[0] == "#":
            continue
        if len(stats) > 4:
            sm.append(int(stats[4]))
        if len(stats) > 6:
            encoders.append(int(stats[6]))
        if len(stats) > 7:
            decoders.append(int(stats[7]))
        if len(stats) > 15:
            rx_pci = stats[14]
            tx_pci = stats[15]
            rxs.append(int(rx_pci))
            txs.append(int(tx_pci))


    report = open(REPORT_OUT_FILE, "a")
    report.write(f"{meta}: Average transfer rate: {np.average(txs)}\n")
    report.write(f"{meta}: Average recieve rate: {np.average(rxs)}\n")
    report.write(f"{meta}: Average sm in percentage: {np.average(sm)}\n")
    report.write(f"{meta}: Average encoder utilized in percentage: {np.average(encoders)}\n")
    report.write(f"{meta}: Average decoder utilized in percentage: {np.average(decoders)}\n")

    report.close()
    gpu_stats.close()


def rand_color():
    r = random.random()
    b = random.random()
    g = random.random()
    return (r, g, b)

def generate_pci_graph():
    pci_gens = [1, 3]
    for fold in range(1, FOLDS):
        x_ticks = 0
        for pci_gen in pci_gens:
            prefix = f"_{pci_gen}_16_{fold}"
            GPU_OUT_FILE = get_gpu_stats_path(prefix)
            gpu_stats = open(GPU_OUT_FILE, "r")
            lines = gpu_stats.readlines()
            rxs:list[int] = []
            txs:list[int] = []
            for _, line in enumerate(lines):
                stats = line.split()
                if stats[0] == "#":
                    continue
                if len(stats) > 15:
                    rx_pci = stats[14]
                    tx_pci = stats[15]
                    rxs.append(int(rx_pci))
                    txs.append(int(tx_pci))

            _sum = list(map(lambda _x: _x[0] + _x[1], zip(txs, rxs)))
            
            x = [i for i in range(len(_sum))] 
            x_ticks = max(x_ticks, len(x))
            plt.plot(x, _sum, label = f"Gen{pci_gen}x16 throughput", color=rand_color())
            
        plt.legend(fancybox=True, shadow=True)
        # plt.xticks([i for i in range(x_ticks)])
        plt.xlabel("Seconds")
        plt.ylabel("Throughput in MiB")
        noun = "streams" if fold > 1 else "stream"
        plt.title(f"PCIe throughput for {fold} {noun}")
        plt.grid(visible=True, which='both', axis='both', linestyle = '--', linewidth = 0.5)
        plt.savefig(os.path.join(DIR_NAME, f"pci_{prefix}.png"))
        plt.clf()


    gpu_stats.close()





def main():
    if not os.path.exists(DIR_NAME):
        os.mkdir(DIR_NAME, )
    pci_gen = sp.check_output(["nvidia-smi", "--query-gpu", "pcie.link.gen.max", "--format", "csv,noheader,nounits"]).decode(sys.stdout.encoding).strip()
    pci_width = sp.check_output(["nvidia-smi", "--query-gpu", "pcie.link.width.max", "--format", "csv,noheader,nounits"]).decode(sys.stdout.encoding).strip()

    for i in range(1, FOLDS):
        afterfix = f"_{pci_gen}_{pci_width}_{i}"
        print(afterfix)
        REPORT_OUT_FILE = os.path.join(DIR_NAME, f"report{afterfix}.txt")
        GPU_OUT_FILE = get_gpu_stats_path(afterfix)
        FFMPEG_CMD = get_ffmpeg_cmd(REPORT_OUT_FILE, i)
        ffmpeg_out = open(REPORT_OUT_FILE, "w")
        errors_out = open(STD_ERR, "w")
        gpu_out = open(GPU_OUT_FILE, "w")

        ffmpeg = sp.Popen(FFMPEG_CMD, stdout=ffmpeg_out, stderr=errors_out)
        nvi_process = sp.Popen(SMI_CMD, stdout=gpu_out, stderr=errors_out)
        ffmpeg.wait()
        nvi_process.kill()

        gpu_out.close()
        ffmpeg_out.close()
        errors_out.close()

        generate_report(f"PCI: {afterfix}", REPORT_OUT_FILE, GPU_OUT_FILE)
    
    # generate_pci_graph()


if __name__ == '__main__':
    print("hello")
    main()
    
