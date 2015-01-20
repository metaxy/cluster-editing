BIN_PATH=./build
INSTANCES_PATH=../programs/instances
names=("ce-lp-round")
TYPE="synthetic"
FILES=$(ls $INSTANCES_PATH/$TYPE/*.txt)

cd build
make
cd ..

for GRAPH in ${FILES[@]}; do
    g=$(basename $GRAPH)
    kfull=$(grep $g $INSTANCES_PATH/$TYPE/k.txt)
    kfull2=($kfull)
    kcorrect=${kfull2[1]}

    for PROG in ${names[@]}; do
        echo -ne "$TYPE/$g $PROG "
        kb=$($BIN_PATH/$PROG < "$GRAPH" | grep -ve "^#" | grep -v '^$' | wc -l)
        abs=$(expr $kb - $kcorrect)
        echo "$kcorrect $abs"
    done
done

