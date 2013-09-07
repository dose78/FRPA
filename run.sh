#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

echo -e "\e[0;32mcompiling...\e[0m"

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
# FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer -DDEBUG"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"

if [ "$1" = "strassen" ]; then
    icc $FLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning STRASSEN DOUBLE PRECISION...\e[0m"
    ./harness 1024 1024 1024
    # ./harness 4096 4096 4096
    # ./harness 8192 8192 8192

elif [ "$1" = "strassen-single" ]; then
    icc $FLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning STRASSEN SINGLE PRECISION...\e[0m"
    ./harness 1024 1024 1024
    # ./harness 4096 4096 4096
    # ./harness 8192 8192 8192

elif [ "$1" = "quicksort" ]; then
    icc $FLAGS -I framework -o harness algorithms/quicksort/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning quicksort...\e[0m"
    ./harness

elif [ "$1" = "mergesort" ]; then
    icc $FLAGS -I framework -o harness algorithms/mergesort/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning mergesort...\e[0m"
    ./harness

elif [ "$1" = "carma" ]; then
    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning CARMA...\e[0m"
    MIN_K=1024
    MAX_K=1024
    ITERATIONS=2
    for (( k=$MIN_K; k<=$MAX_K; k*=2 )); do
        for (( i=1; i<=$ITERATIONS; i+=1 )); do
            ./harness 1024 $k 1024
        done
    done

elif [ "$1" = "trsm" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/trsm/*.cpp algorithms/mult/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning TRSM...\e[0m"
    ./harness

elif [ "$1" = "syrk" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/syrk/*.cpp algorithms/mult/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning SYRK...\e[0m"
    ./harness

elif [ "$1" = "cholesky" ]; then
    icc $FLAGS -I framework -I algorithms/mult -I algorithms/trsm -I algorithms/syrk -o harness algorithms/cholesky/*.cpp algorithms/trsm/*.cpp algorithms/mult/*.cpp  algorithms/syrk/*.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning CHOLESKY...\e[0m"
    ./harness

elif [ "$1" = "delaunay" ]; then
    DEL_DIR="algorithms/delaunay"
    icc $FLAGS -I framework -o harness $DEL_DIR/delaunay_harness.cpp $DEL_DIR/DelaunayProblem.cpp $DEL_DIR/edge.cpp $DEL_DIR/library.cpp $DEL_DIR/mypred.cpp $DEL_DIR/predicates.c $FRAMEWORK
    # icc $FLAGS -I framework -o harness algorithms/delaunay/*.cpp algorithms/delaunay/*.c $FRAMEWORK
    echo -e "\e[0;32mrunning DELAUNAY...\e[0m"
    ./harness
    # To benchmark the sequential version: make && ./main -t 1 -r 1000000 (the last part is the problem size)

else
    echo -e "\e[0;31mERROR: Algorithm not found\e[0m"
    exit
fi

rm -rf harness

# /reserve/unreserve.me
