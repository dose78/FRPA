#!/bin/bash

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

m=$1
k=$2
n=$3
interleaving=$4
length=${#interleaving}

if [ $length -eq 0 ]; then
    interleaving='blah' # interleaving doesn't matter, base case is called immediately
fi

echo -n "compiling..."
FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer -std=c99 -wd266"
icc $FLAGS -o harness strassen_harness.c strassen.c

echo "running ($CILK_NWORKERS threads)"
./harness $m $k $n $interleaving $length

rm -rf harness
