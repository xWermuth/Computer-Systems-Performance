# Measure PCI througput when encoding vidoes with FFMPEG

## Hypothesis

**FFMPEG video decoding throughput is increased as the PCI-E transfer rate is increased**

## Synthetic data

We encode a 100GB video recorded on Iphone 14 Pro.

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
