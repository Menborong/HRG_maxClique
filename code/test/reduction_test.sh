#!/bin/bash
in_dir="../../gen_dataset/dataset"
out_dir="../../results/reudution_result"
mkdir $out_dir

# num_node_arr=(100 133 178 237 316 422 562 745 1000 1330 1780 2370 3160 4220 5620 7450 10000 13300 17800 23700 31600 42200 56200 74500 100000 133000 178000 237000 316000 422000 562000 745000 1000000 1330000 1780000 2370000 3160000 4220000 5620000 7450000 10000000)
# alphas=("0.55" "0.60" "0.65" "0.70" "0.75" "0.80" "0.85" "0.90")

# num_node_arr=(10000000)
# alphas=("0.55" "0.60" "0.65" "0.70" "0.75" "0.80" "0.85" "0.90")

num_node_arr=(1000 1330 1780 2370 3160 4220 5620 7450 10000 13300 17800 23700 31600 42200 56200 74500 100000)
alphas=("0.75")

# deg="10"
Cs=(9 10 11 12 13)

max_idx="100"

for num_node in "${num_node_arr[@]}"; do
    for alpha in "${alphas[@]}"; do
        for C in "${Cs[@]}"; do
            alpha_str=${alpha/./}
            idx=1
            echo "Calculate reduction size of random HRG: ${num_node} nodes, alpha=${alpha}"
            while(( "${idx}" <= "${max_idx}" )); do
                echo -e -n "\r ${idx}/100"
                # genhrg -n ${num_node} -alpha ${alpha} -deg ${deg} -edge 1 -file ./temp_graph -rseed -1 -aseed -1 -sseed -1 -threads 8 > /dev/null
                ../build/reduction_test ${in_dir}/${num_node}_${alpha_str}_${C}/${idx}.txt ${out_dir}/${num_node}_${alpha_str}_${C}.txt
                (( idx = "${idx}" + 1 ))
            done
            echo " done!"
        done
    done
done

echo "all done"