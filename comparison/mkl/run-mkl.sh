#!/bin/bash

# /reserve/reserve.me

export MKL_NUM_THREADS=32

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

function mkl_sweep {
    outfile=$1
    for (( n=1024; n<8192; n+=1024 )); do
        for (( i=1; i<=10; i+=1 )); do
            ./mkl_harness $n $n $n $outfile
        done
    done
    for (( n=8192; n<=25600; n+=1024 )); do
        for (( i=1; i<=5; i+=1 )); do
            ./mkl_harness $n $n $n $outfile
        done
    done
}

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"

icc $FLAGS -o mkl_harness mkl_harness-single.cpp
echo -e "\e[01;34mRunning MKL Single\e[0m"
mkl_sweep "mkl-single.csv"

icc $FLAGS -o mkl_harness mkl_harness-double.cpp
echo -e "\e[01;34mRunning MKL Double\e[0m"
mkl_sweep "mkl-double.csv"

rm -rf mkl_harness
echo -e "\e[01;32mThis trial took" $SECONDS "seconds\e[0m"
echo "This trial took" $SECONDS "seconds" > timing.txt

# /reserve/unreserve.me
