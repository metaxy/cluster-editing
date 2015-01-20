export LC_NUMERIC="en_US.UTF-8"

PROG=ce-lp-round
cd build
make
cd ~/programs/material
./ce-solver-test "/afs/akt.tu-berlin.de/user/pwalger/cluster-editing/build/$PROG" 
