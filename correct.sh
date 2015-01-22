export LC_NUMERIC="en_US.UTF-8"

PROG=ce-approx3
BIN_PATH=/home/paul/Cloud/coding/cluster-editing/build
#BIN_PATH=/afs/akt.tu-berlin.de/user/pwalger/cluster-editing/build
PROGRAMS_PATH=/home/paul/Cloud/studium/Cluster/programs
cd $BIN_PATH
make
cd $PROGRAMS_PATH/material
./ce-solver-test "$BIN_PATH/$PROG" 
