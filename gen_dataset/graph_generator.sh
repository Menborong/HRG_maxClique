#!/bin/bash
save_dir="dataset"
# num_node_arr=(100 133 178 237 316 422 562 745 1000 1330 1780 2370 3160 4220 5620 7450 10000 13300 17800 23700 31600 42200 56200 74500 100000 133000 178000 237000 316000 422000 562000 745000 1000000)
num_node_arr=(2370000)

# alphas=("0.60" "0.65" "0.70" "0.75")
alphas=("0.55" "0.60" "0.65" "0.70" "0.75" "0.80" "0.85" "0.90")
deg="10"

max_idx="100"

mkdir $save_dir

for num_node in "${num_node_arr[@]}"; do
    for alpha in "${alphas[@]}"; do
        alpha_str=${alpha/./}
        mkdir ./${save_dir}/${num_node}_${alpha_str}
        idx=1
        echo "Generate random HRG: ${num_node} nodes, alpha=${alpha}, deg=${deg}"
        while(( "${idx}" <= "${max_idx}" )); do
            echo -e -n "\r ${idx}/100"
            # genhrg -n ${num_node} -alpha ${alpha} -deg ${deg} -edge 1 -coord 1 -file ./${save_dir}/${num_node}_${alpha_str}/${idx} -rseed -1 -aseed -1 -sseed -1 -threads 8 > /dev/null
            genhrg -n ${num_node} -alpha ${alpha} -deg ${deg} -edge 1 -coord 0 -file ./${save_dir}/${num_node}_${alpha_str}/${idx} -rseed -1 -aseed -1 -sseed -1 -threads 1 > /dev/null
            # get R
            # ./dataset_get_metadata ${num_node} ${alpha} 0 ${deg} ./${save_dir}/${num_node}_${alpha_str}/${idx}.hyp

            (( idx = "${idx}" + 1 ))
        done
        echo " done!"
    done
done

echo "all done"