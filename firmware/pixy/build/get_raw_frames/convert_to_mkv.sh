#!/bin/sh
ffmpeg -framerate 30 -pattern_type glob -i "*.ppm" out.mkv
