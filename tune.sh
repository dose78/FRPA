#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

OPENTUNERPATH="/home/eecs/deliahu/opentuner/"
export PYTHONPATH=$PYTHONPATH:$OPENTUNERPATH
otpython=$OPENTUNERPATH/venv/bin/python

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"

# STRASSEN
icc $FLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
dim=1024
$otpython algorithms/strassen/strassen_tune.py --test-limit 10 --max_depth 5 --m $dim --k $dim --n $dim

rm -rf harness
echo -e "\e[01;32mTuning took" $SECONDS "seconds\e[0m"
echo "Tuning took" $SECONDS "seconds" > timing.txt

# /reserve/unreserve.me
