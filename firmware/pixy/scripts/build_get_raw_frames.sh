#!/bin/bash

function WHITE_TEXT {
  printf "\033[1;37m"
}
function NORMAL_TEXT {
  printf "\033[0m"
}
function GREEN_TEXT {
  printf "\033[1;32m"
}
function RED_TEXT {
  printf "\033[1;31m"
}

WHITE_TEXT
echo "########################################################################################"
echo "# Building Get Raw Frames...                                                            #"
echo "########################################################################################"
NORMAL_TEXT

uname -a

TARGET_BUILD_FOLDER=../build

mkdir $TARGET_BUILD_FOLDER
mkdir $TARGET_BUILD_FOLDER/get_raw_frames

rm $TARGET_BUILD_FOLDER/get_raw_frames/get_raw_frames
cd ../src/host/libpixyusb2_examples/get_raw_frames
make
mv ./get_raw_frames ../../../../build/get_raw_frames

if [ -f ../../../../build/get_raw_frames/get_raw_frames ]; then
  GREEN_TEXT
  printf "SUCCESS "
else
  RED_TEXT
  printf "FAILURE "
fi
echo ""
