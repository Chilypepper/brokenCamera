#!/bin/sh
echo "Compiling...\n"
cppfile=$1
g++ $cppfile -g -o programs/${cppfile%.*} `pkg-config --cflags --libs opencv`
echo "\nFinished!"