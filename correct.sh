export LC_NUMERIC="en_US.UTF-8"

PROG=ce-lp-round
cd build
make
cd ~/programs/material
./ce-solver-test "../../gruppe1/aufgabe4_reduce/build/$PROG" 
