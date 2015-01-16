names=("ce-lp-2k-it" "ce-lp-2k-it-rel")
files=(synthetic/k_556_n_071.txt synthetic/k_445_n_028.txt synthetic/k_426_n_031.txt synthetic/k_412_n_053.txt synthetic/k_405_n_068.txt)
cd build
make
cd ..
for GRAPH in ${files[@]}; do
    for EX in ${names[@]}; do
        echo -ne "$EX;$GRAPH;"
        /usr/bin/time -f %e ./build/$EX < "../../programs/instances/$GRAPH" > /dev/null
    done
done

###files=(synthetic/k_039_n_141.txt synthetic/k_048_n_418.txt synthetic/k_070_n_033.txt synthetic/k_080_n_378.txt synthetic/k_100_n_541.txt)
