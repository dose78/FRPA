#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
DFLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer -DDEBUG"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"

function mem_sweep_strassen {
    interleaving=$1
    for (( n=1024; n<=20480; n+=1024 )); do
        for (( i=1; i<=1; i+=1 )); do
            ./harness $n $n $n $interleaving
        done
    done
}

function timing_sweep_strassen {
    interleaving=$1
    for (( n=1024; n<8192; n+=1024 )); do
        for (( i=1; i<=10; i+=1 )); do
            ./harness $n $n $n $interleaving
        done
    done
    for (( n=8192; n<11264; n+=1024 )); do
        for (( i=1; i<=5; i+=1 )); do
            ./harness $n $n $n $interleaving
        done
    done
    for (( n=11264; n<17408; n+=1024 )); do
        for (( i=1; i<=3; i+=1 )); do
            ./harness $n $n $n $interleaving
        done
    done
    for (( n=17408; n<=20480; n+=1024 )); do
        for (( i=1; i<=1; i+=1 )); do
            ./harness $n $n $n $interleaving
        done
    done
}

function mem_sweep_carma {
    interleaving=$1
    for (( n=64; n<=16777216; n*=2 )); do
        for (( i=1; i<=1; i+=1 )); do
            ./harness 64 $n 64 $interleaving
        done
    done
}

function timing_sweep_carma {
    interleaving=$1
    for (( n=16777216; n<=16777216; n*=2 )); do
        for (( i=1; i<=10; i+=1 )); do
            ./harness 64 $n 64 $interleaving
        done
    done
}

if [ "$1" = "strassen" ]; then
    echo -e "\e[0;32mrunning STRASSEN DOUBLE PRECISION...\e[0m"

    icc $DFLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
    mem_sweep_strassen BB
    mem_sweep_strassen DBB
    mem_sweep_strassen BDB
    mem_sweep_strassen BBD
    mem_sweep_strassen DBBB
    mem_sweep_strassen BDBB
    mem_sweep_strassen BBDB
    mem_sweep_strassen BBBD
    mem_sweep_strassen DBDB
    mem_sweep_strassen BDDB
    mem_sweep_strassen BDBDB

    icc $FLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
    timing_sweep_strassen BB
    timing_sweep_strassen DBB
    timing_sweep_strassen BDB
    timing_sweep_strassen BBD
    timing_sweep_strassen DBBB
    timing_sweep_strassen BDBB
    timing_sweep_strassen BBDB
    timing_sweep_strassen BBBD
    timing_sweep_strassen DBDB
    timing_sweep_strassen BDDB
    timing_sweep_strassen BDBDB

elif [ "$1" = "strassen-single" ]; then
    echo -e "\e[0;32mrunning STRASSEN SINGLE PRECISION...\e[0m"

    icc $DFLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
    mem_sweep_strassen BB

    icc $FLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
    timing_sweep_strassen BB

elif [ "$1" = "carma" ]; then
    echo -e "\e[0;32mrunning CARMA...\e[0m"

    icc  -I framework $DFLAGS -I framework -o harness  algorithms/carma/*.cpp $FRAMEWORK
    mem_sweep_carma BBBBB

    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma/*.cpp $FRAMEWORK
    timing_sweep_carma BBBBB


elif [ "$1" = "carma-single" ]; then
    echo -e "\e[0;32mrunning CARMA SINGLE...\e[0m"

    icc  -I framework $DFLAGS -I framework -o harness  algorithms/carma-single/*.cpp $FRAMEWORK
    mem_sweep_carma BBBBB

    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma-single/*.cpp $FRAMEWORK
    timing_sweep_carma BBBBB

elif [ "$1" = "quicksort" ]; then
    icc $FLAGS -I framework -o harness algorithms/quicksort/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning quicksort...\e[0m"
    ./harness BB

elif [ "$1" = "mergesort" ]; then
    icc $FLAGS -I framework -o harness algorithms/mergesort/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning mergesort...\e[0m"
    ./harness BB

elif [ "$1" = "trsm" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/trsm/*.cpp algorithms/mult/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning TRSM...\e[0m"
    ./harness BB

elif [ "$1" = "syrk" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/syrk/*.cpp algorithms/mult/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning SYRK...\e[0m"
    ./harness BB

elif [ "$1" = "cholesky" ]; then
    icc $FLAGS -I framework -I algorithms/mult -I algorithms/trsm -I algorithms/syrk -o harness algorithms/cholesky/*.cpp algorithms/trsm/TrsmProblem.cpp algorithms/mult/MultProblem.cpp  algorithms/syrk/SyrkProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning CHOLESKY...\e[0m"
    ./harness BB

elif [ "$1" = "delaunay" ]; then
    DEL_DIR="algorithms/delaunay"
    icc $FLAGS -I framework -o harness $DEL_DIR/delaunay_harness.cpp $DEL_DIR/DelaunayProblem.cpp $DEL_DIR/edge.cpp $DEL_DIR/library.cpp $DEL_DIR/mypred.cpp $DEL_DIR/predicates.c $FRAMEWORK
    # icc $FLAGS -I framework -o harness algorithms/delaunay/*.cpp algorithms/delaunay/*.c $FRAMEWORK
    echo -e "\e[0;32mrunning DELAUNAY...\e[0m"
    ./harness BB
    # To benchmark the sequential version: make && ./main -t 1 -r 1000000 (the last part is the problem size)

elif [ "$1" = "test" ]; then
    icc  -I framework $FLAGS -I framework -o harness  algorithms/test/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning TEST...\e[0m"
    ./harness 10 BB

else
    echo -e "\e[0;31mERROR: Algorithm not found\e[0m"
    exit
fi

rm -rf harness
echo -e "\e[01;32mThis trial took" $SECONDS "seconds\e[0m"
echo "This trial took" $SECONDS "seconds" > timing.txt

# /reserve/unreserve.me
