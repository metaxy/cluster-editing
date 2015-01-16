export LC_NUMERIC="en_US.UTF-8"

PROG=ce-lp-2k-it-rel
cd build
make
cd ..
cd ../../programs/material
./ce-solver-test "../../gruppe1/aufgabe4_reduce/build/$PROG" 
