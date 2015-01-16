BIN_PATH=./build
INSTANCES_PATH=../../programs/instances
names=("ce-lp-2k-it-rel")
PROGCORRECT="ce-lp-2k-it"
TYPE="synthetic"
FILES=$(ls ../../programs/instances/$TYPE/*.txt)
cd build
make
cd ..
for GRAPH in ${FILES[@]}; do
    g=$(basename $GRAPH)
    kfull=$(grep $g ../../programs/instances/$TYPE/k.txt)
    kfull2=($kfull)
    kcorrect=${kfull2[1]}

    for PROG in ${names[@]}; do
        echo -ne "$TYPE/$g $PROG "
        kb=$($BN_PATH/$PROG < "$INSTANCES_PATH/$GRAPH" | grep -ve "^#" | grep -v '^$' | wc -l)
        abs=$(expr $kb - $kcorrect)
        echo "$kcorrect $abs"
    done
done

