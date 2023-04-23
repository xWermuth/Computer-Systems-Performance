#! bin/bash

# Reduces file size
ffmpeg -i input.mp4 -vcodec libx264 -crf 24 output.mp4