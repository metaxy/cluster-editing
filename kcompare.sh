BIN_PATH=./build
#INSTANCES_PATH=../programs/instances
INSTANCES_PATH=/home/paul/Cloud/studium/Cluster/programs/instances/
names=("ce-approx3" "ce-approx3 --reduceZero")
TYPE="synthetic"
FILES=$(ls $INSTANCES_PATH/$TYPE/*.txt)

cd build
make
cd ..
echo "GRAPH_NAME PROGRAM CORRECT_K APPROX_K DIFF_K"
for GRAPH in ${FILES[@]}; do
    g=$(basename $GRAPH)
    kfull=$(grep $g $INSTANCES_PATH/$TYPE/k.txt)
    kfull2=($kfull)
    kcorrect=${kfull2[1]}

    for IX in ${!names[*]}; do
        PROG=${names[$IX]}
        echo -ne "$TYPE/$g;$PROG;"
        kb=$($BIN_PATH/$PROG < "$GRAPH" | grep -ve "^#" | grep -v '^$' | wc -l)
        abs=$(expr $kb - $kcorrect)
        echo "$kcorrect;$kb;$abs"
    done
done

