#! /bin/bash

# Reduces file size - aims to reduce bit rate
ffmpeg -i input.mp4 -vcodec NVENC -crf 24 output.mp4 --enable-nvenc

# ffmpeg -i input.mp4 -f null - 

# ffmpeg -hwaccel cuda -hwaccel_output_format cuda -extra_hw_frames 4 -i cut.mp4 -c:v hevc_nvenc -f null -

ffmpeg -hwaccel cuda -hwaccel_output_format cuda -extra_hw_frames 4 -i cut.mp4 -c:v hevc_nvenc -crf 24 output.mp4

lspci -vv

dmidecode

sudo lspci -vv | grep -P "[0-9a-f]{2}:[0-9a-f]{2}\.[0-9a-f]|LnkSta:"
