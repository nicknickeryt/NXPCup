#!/bin/sh

for file in *.ppm; do
    echo "file is: " ${file}
    newfilename="${file/ppm/png}"
    echo "newfilename is: " ${newfilename}
    convert ${file} ${newfilename}
done
