#!/bin/sh
ffmpeg -framerate 20 -pattern_type glob -i "*.ppm" out.mkv
