#!/bin/sh
filename=$1
if [ -n "$filename" ]; then
    ./extract_green $filename.ppm && ./get_edges2 g_$filename.ppm && convert g_$filename.ppm o$filename.png \
        && rm -r *_$filename.ppm green_bytes_only
else
    echo "filename was not given"
fi
