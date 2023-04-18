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
