#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

echo -e "\e[0;32mcompiling...\e[0m"

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"

if [ "$1" = "quicksort" ]; then
    icc $FLAGS -I framework -o harness algorithms/quicksort/quicksort_harness.cpp algorithms/quicksort/QuicksortProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning quicksort...\e[0m"
    ./harness

elif [ "$1" = "mergesort" ]; then
    icc $FLAGS -I framework -o harness algorithms/mergesort/mergesort_harness.cpp algorithms/mergesort/MergesortProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning mergesort...\e[0m"
    ./harness

elif [ "$1" = "carma" ]; then
    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma/carma_harness.cpp algorithms/carma/CarmaProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning CARMA...\e[0m"
    MIN_K=1024
    MAX_K=1024
    ITERATIONS=2
    for (( k=$MIN_K; k<=$MAX_K; k*=2 )); do
        for (( i=1; i<=$ITERATIONS; i+=1 )); do
            ./harness 1024 $k 1024
        done
    done

elif [ "$1" = "strassen" ]; then
    icc $FLAGS -I framework -o harness algorithms/strassen/strassen_harness.cpp algorithms/strassen/StrassenProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning STRASSEN...\e[0m"
    ./harness 1024 1024 1024

elif [ "$1" = "trsm" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/trsm/trsm_harness.cpp algorithms/trsm/TrsmProblem.cpp algorithms/mult/MultProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning TRSM...\e[0m"
    ./harness

elif [ "$1" = "syrk" ]; then
    icc $FLAGS -I framework -I algorithms/mult -o harness algorithms/syrk/syrk_harness.cpp algorithms/syrk/SyrkProblem.cpp algorithms/mult/MultProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning SYRK...\e[0m"
    ./harness

elif [ "$1" = "cholesky" ]; then
    icc $FLAGS -I framework -I algorithms/mult -I algorithms/trsm -I algorithms/syrk -o harness algorithms/cholesky/cholesky_harness.cpp algorithms/trsm/TrsmProblem.cpp algorithms/mult/MultProblem.cpp algorithms/cholesky/CholeskyProblem.cpp algorithms/syrk/SyrkProblem.cpp $FRAMEWORK
    echo -e "\e[0;32mrunning CHOLESKY...\e[0m"
    ./harness

elif [ "$1" = "delaunay" ]; then
    DEL_DIR="algorithms/delaunay"
    icc $FLAGS -I framework -o harness $DEL_DIR/delaunay_harness.cpp $DEL_DIR/DelaunayProblem.cpp $DEL_DIR/edge.cpp $DEL_DIR/library.cpp $DEL_DIR/mypred.cpp $DEL_DIR/predicates.c $FRAMEWORK
    echo -e "\e[0;32mrunning DELAUNAY...\e[0m"
    ./harness
    # To benchmark the sequential version: make && ./main -t 1 -r 1000000 (the last part is the problem size)

else
    echo -e "\e[0;31mERROR: Algorithm not found\e[0m"
    exit
fi

rm -rf harness

# /reserve/unreserve.me
