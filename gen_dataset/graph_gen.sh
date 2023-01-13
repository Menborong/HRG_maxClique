#!/bin/bash

save_dir="dataset"
num_node="8000000"
alpha="0.85"
temp="0"
deg="10"

max_idx="100"

mkdir $save_dir
alpha_str=${alpha/./}
save_dir=${save_dir}/${num_node}_${alpha_str}

mkdir $save_dir
g++ dataset_get_metadata.cpp -o dataset_get_metadata

idx=1
while(( "${idx}" <= "${max_idx}" )); do
    echo -e -n "\r ${idx}/100"
    genhrg ./genhrg -n ${num_node} -alpha ${alpha} -t ${temp} -deg ${deg} -edge 1 -coord 1 -file ./${save_dir}/${idx} -rseed -1 -aseed -1 -sseed -1 -threads 8 > /dev/null
    ./dataset_get_metadata ${num_node} ${alpha} ${temp} ${deg} ./${save_dir}/${idx}.meta
    (( idx = "${idx}" + 1 ))
done

echo " done"