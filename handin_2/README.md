# Measure PCI througput when encoding vidoes with FFMPEG

## Hypothesis

**FFMPEG video encoding throughput is increased as the PCI-E link speed is increased**

## Synthetic data

We encode a 100GB video recorded on Iphone 14 Pro.

## Workload

The Iphone video has a high bitrate. Hence we want to use FFMPEG's compression to compress the video into lower bitrates while preserving video quality. The desired result can be achvieved by running FFMPEG with the following flags:

```bash
# Some ffmpeg cmd
```

## Benchmarking

### Tools

- Nvidia-smi

To get PCI-E current width:

```bash
nvidia-smi --query-gpu="pcie.link.width.current" --format=csv
```

### View GPU usage

```bash
nvidia-smi -l 1
```

### Discuss

Where is the bottleneck?
- Sending many, small data packets over PCIe
- We checked
  - Disk
  - CPU
  - GPU decoder and SM
  - GPU Memory (only ~10% in use)
  - Host Memory (only ~1.3% in use)
  - Timing of PCI-E gen 2 vs gen 3
    - Gen 3 is faster than gen 2 and gen 1 (about 260% faster)

- Is the above analysis sufficient thorrough?

### Smart nvidia guy

https://forums.developer.nvidia.com/t/why-is-pcie-data-transfer-so-absolutely-slow-discuss-please/22176

#### Timing results of PCI-E Gen2 and Gen3

**1 min Gen 3 x16**: 16s
- TX ~3 GB/S
- RX ~25 MB/S

**1 min Gen 2 x16**: 42s
- TX ~ 1.305 GB/S
- RX ~ 11 MB/S

**1 min Gen 1 x16**: 42s
- TX ~ 1.305 GB/S
- RX ~ 11 MB/S

## Maybe solutions

### Load a larger file

To see if we see more throughput

### Feed data faster to ffmpeg

### Python wrapper


### USE DCGM.

SM Active metric - nvidia smi

Put file in memory
linux: /dev/shm
