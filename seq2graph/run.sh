#!/bin/bash

INPUT_GFA="data/COVID-19-3.gfa"
INPUT_STRING="/public/agis/ruanjue_group/xiangyixin/pangenome/dataset/COVID-19/genome/GCA_009937885.1.fna"
OUT_FOLDER="data"

make clean > build.log 2>&1
make -j >> build.log 2>&1

./seq2graph.exe -g ${INPUT_GFA} -s ${INPUT_STRING} -o ${OUT_FOLDER}