BIN_PATH=./build
INSTANCES_PATH=../programs/instances
#INSTANCES_PATH=/home/paul/Cloud/studium/Cluster/programs/instances/
#names=("ce-approx3" "ce-approx3 --reduceZero" "ce-approx3 --reduce2K")
names=("ce-blp-rel")
#names=("ce-branch --reduceAll")
#TYPE="synthetic"
TYPE="synthetic"
FILES=$(ls $INSTANCES_PATH/$TYPE/*.*)

cd build
make
cd ..
echo "GRAPH_NAME PROGRAM CORRECT_K APPROX_K DIFF_K"
for GRAPH in ${FILES[@]}; do
    g=$(basename $GRAPH)
    if [[ "$g" == "k.txt" ]]; then
        continue
    fi
    if [[ "$g" == "k-cvd.txt" ]]; then
        continue
    fi

    kfull=$(grep $g $INSTANCES_PATH/$TYPE/k.txt)
    kfull2=($kfull)
    kcorrect=${kfull2[1]}


    for IX in ${!names[*]}; do
        PROG=${names[$IX]}
        kb=$($BIN_PATH/$PROG < "$GRAPH" | grep -ve "^#" | grep -v '^$' | wc -l)
        echo "$TYPE;$kcorrect;$kb;$TYPE/$g;$PROG"
    done
done

