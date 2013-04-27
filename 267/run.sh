#!/bin/bash

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1

echo -e "\e[01;34mcompiling...\e[0m"

if [ "$1" = "quicksort" ] || [ "$1" = "quicksort.c" ]; then
    echo "NOT IMPLEMENTED"
    # icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer sort_harness.c quicksort.c
    # echo -e "\e[0;32mrunning quicksort...\e[0m"
    # ./harness

elif [ "$1" = "mergesort" ] || [ "$1" = "mergesort.c" ]; then
    echo "NOT IMPLEMENTED"
    # icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer sort_harness.c mergesort.c
    # echo -e "\e[0;32mrunning mergesort...\e[0m"
    # ./harness

elif [ "$1" = "carma" ] || [ "$1" = "carma.c" ]; then
    icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer carma_harness.cpp CarmaProblem.cpp framework.cpp
    echo -e "\e[0;32mrunning CARMA...\e[0m"
    ./harness 64 262144 64

elif [ "$1" = "strassen" ] || [ "$1" = "strassen.c" ]; then
    icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer test_strassen.cpp framework.cpp StrassenProblem.cpp
    #icc -mkl -o harness  -O3 test_strassen.cpp framework.cpp StrassenProblem.cpp
    echo -e "\e[0;32mrunning STRASSEN...\e[0m"
    ./harness 

else
    echo -e "\e[0;31mERROR: Algorithm not found\e[0m"
    exit
fi

rm -rf harness
