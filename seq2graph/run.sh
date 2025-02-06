#!/bin/bash

INPUT_GFA="data/COVID-19-3.gfa"
INPUT_STRING="/home/yxxiang/data/pangenome/dataset/real/COVID-19/GCA_009937895.1/GCA_009937895.1_ASM993789v1_genomic.fna"
OUT_FOLDER="data"

make clean > build.log 2>&1
make -j >> build.log 2>&1

./seq2graph.exe -g ${INPUT_GFA} -s ${INPUT_STRING} -o ${OUT_FOLDER}