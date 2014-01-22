#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

OPENTUNERPATH="$HOME/opentuner"
export PYTHONPATH=$PYTHONPATH:$OPENTUNERPATH
otpython=$OPENTUNERPATH/venv/bin/python

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"
icc $FLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
dim=4096

outfile='opentuner-strassen-emerald-single-'$dim'.csv'
for (( i=1; i<=100; i+=1 )); do
    rm -rf opentuner.db opentuner.log
    $otpython algorithms/strassen-single/tuner.py --test-limit 100 --max_depth 10 --m $dim --k $dim --n $dim --out_file $outfile
    echo "" >> $outfile
done

rm -rf harness
echo -e "\e[01;32mTuning took" $SECONDS "seconds\e[0m"
echo "Tuning took" $SECONDS "seconds" > timing.txt

# /reserve/unreserve.me
