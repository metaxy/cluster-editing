# cluster-editing
A Solver for Cluster Editing

## Compile
mkdir build
cd build
cmake ..
make

## Run Tests
ruby k.rb

## Run SMAC
cd ~/bin
wget http://www.cs.ubc.ca/labs/beta/Projects/SMAC/smac-v2.08.00-master-731.tar.gz
extract smac-v2.08.00-master-731.tar.gz
~/bin/smac-v2.08.00-master-731/smac  --scenario-file smac/2k/2k-scenario.txt  - -shared-model-mode 

### Generate Training Set
ruby k.rb > current_2k.txt
ruby gen_train.rb < current_2k.txt > smac/instances/train.k