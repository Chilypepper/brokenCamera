#!/bin/sh
echo "Compiling...\n"
cppfile=$1
g++ $cppfile -o programs/${cppfile%.*} `pkg-config --cflags --libs opencv`
echo "\nFinished!"