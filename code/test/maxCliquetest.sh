#!/bin/bash
in_dir="../../gen_dataset/dataset"
out_dir="../../results/runtime_nongeo_result"

mkdir ${out_dir}

num_node_arr=(100 133 178 237 316 422 562 745 1000 1330 1780 2370 3160 4220 5620 7450 10000 13300 17800 23700 31600 42200 56200 74500 100000 133000 178000 237000 316000 422000 562000 745000 1000000)
# num_node_arr=(100 133 178 237 316 422 562 745 1000 1330 1780 2370 3160 4220 5620 7450 10000 13300 17800 23700 31600 42200 56200 74500 100000)
alphas=("0.75")
deg="10"
versions=(1 2)

max_idx="100"

mkdir $save_dir

for num_node in "${num_node_arr[@]}"; do
    for alpha in "${alphas[@]}"; do
        for version in "${versions[@]}"; do
            alpha_str=${alpha/./}
            idx=1
            echo "Calculate maximum clique of random HRG: ${num_node} nodes, alpha=${alpha}, deg=${deg}, version=${version}"
            while(( "${idx}" <= "${max_idx}" )); do
                echo -e -n "\r ${idx}/100"

                in_target="${in_dir}/${num_node}_${alpha_str}/${idx}"
                out_target_w="${out_dir}/${num_node}_${alpha_str}_v${version}_wgeo$"
                out_target_wo="${out_dir}/${num_node}_${alpha_str}_v${version}_wogeo$"

                # w/ geometry
                # ../build/solve_geo_test ${in_target}.txt ${in_target}.hyp ${out_target_w}.txt ${version}
                
                # w/o geometry
                ../build/solve_nongeo_test ${in_target}.txt ${out_target_wo}.txt ${version}

                (( idx = "${idx}" + 1 ))
            done
            echo " done!"
        done
    done
done

echo "all done"