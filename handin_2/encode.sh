#! /bin/bash

# Reduces file size - aims to reduce bit rate
ffmpeg -i input.mp4 -vcodec NVENC -crf 24 output.mp4 --enable-nvenc

# ffmpeg -i input.mp4 -f null - 

# ffmpeg -hwaccel cuda -hwaccel_output_format cuda -extra_hw_frames 4 -i cut.mp4 -c:v hevc_nvenc -f null -

ffmpeg -hwaccel cuda -hwaccel_output_format cuda -extra_hw_frames 4 -i cut.mp4 -c:v hevc_nvenc -crf 24 output.mp4

# Encode
ffmpeg -hwaccel cuda -hwaccel_output_format cuda -i input.mp4 -c:v h264_nvenc -b:v 5M output.mp4
# -hwaccel cuda -hwaccel_output_format cuda
# Decode on CPU and transfer to GPU - max PCI throughput
ffmpeg -hide_banner \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null - \
-c:v h264_cuvid -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 60 -f null -
# https://developer.nvidia.com/blog/nvidia-ffmpeg-transcoding-guide/

lspci -vv

dmidecode

sudo lspci -vv | grep -P "[0-9a-f]{2}:[0-9a-f]{2}\.[0-9a-f]|LnkSta:"

ffmpeg -hide_banner -i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 15 -f null /dev/null -ss 30 -t 15 -f null /dev/null \
-i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 15 -f null - \
-i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 15 -f null - \
-i /dev/shm/cut.mp4 -vf "hwupload_cuda" -t 15 -f null - 