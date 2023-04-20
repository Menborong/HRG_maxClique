#!/bin/bash
in_dir="../../real_dataset"
out_dir="../../results/runtime_general_result"

mkdir $out_dir

versions=("2" "1")
filenames=("soc-Epinions1" "Slashdot0811" "Slashdot0902" "Gnutella31" "web-Stanford" "Wiki-Vote" "com-lj" "web-Google" "web-BerkStan" "com-dblp" "com-amazon" "as-skitter" "soc-pokec" "com-youtube" "WikiTalk" "web-NotreDame" "ca-AstroPh" "ca-CondMat" "ca-HepPh")

for version in "${versions[@]}"; do
    for filename in "${filenames[@]}"; do
        echo "target: ${filename}, version: ${version}"
        ../build/solve_general_test ${in_dir}/${filename}.txt ${out_dir}/${filename}_v${version}.txt
        echo "done!"
    done
done

echo "all done"