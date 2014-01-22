#!/bin/bash

# /reserve/reserve.me

export CILK_NWORKERS=32
export MKL_NUM_THREADS=1
echo -e "\e[01;34mRunning with $CILK_NWORKERS threads\e[0m"

if [ `dnsdomainname | tr [:upper:] [:lower:]` = "millennium.berkeley.edu" ]; then
    source /opt/intel/bin/iccvars.sh intel64
fi

FLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer"
DFLAGS="-O3 -mkl -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer -DDEBUG -DTERSE"
FRAMEWORK="framework/framework.cpp framework/Task.cpp framework/Problem.cpp framework/memory.cpp"

function mem_sweep_square {
    a=("$@")
    for interleaving in "${a[@]}" ; do
        for (( n=1024; n<=30720; n+=1024 )); do
            ./harness $n $n $n $interleaving
        done
    done
}

function timing_sweep_square {
    a=("$@")
    for interleaving in "${a[@]}" ; do
        for (( n=1024; n<8192; n+=1024 )); do
            for (( i=1; i<=10; i+=1 )); do
                ./harness $n $n $n $interleaving
            done
        done
        for (( n=8192; n<26624; n+=1024 )); do
            for (( i=1; i<=5; i+=1 )); do
                ./harness $n $n $n $interleaving
            done
        done
        for (( n=26624; n<=30720; n+=1024 )); do
            for (( i=1; i<=3; i+=1 )); do
                ./harness $n $n $n $interleaving
            done
        done
    done
}

function mem_sweep_skinny {
    a=("$@")
    for interleaving in "${a[@]}" ; do
        #exponential
        for (( n=64; n<1048576; n*=2 )); do
            ./harness 64 $n 64 $interleaving
        done

        # linear
        for (( n=1048576; n<=16777216; n+=1048576 )); do
            ./harness 64 $n 64 $interleaving
        done
    done
}

function timing_sweep_skinny {
    a=("$@")
    for interleaving in "${a[@]}" ; do
        #exponential
        for (( n=64; n<1048576; n*=2 )); do
            for (( i=1; i<=10; i+=1 )); do
                ./harness 64 $n 64 $interleaving
            done
        done

        #linear
        for (( n=1048576; n<=16777216; n+=1048576 )); do
            for (( i=1; i<=5; i+=1 )); do
                ./harness 64 $n 64 $interleaving
            done
        done
    done
}

if [ "$1" = "strassen" ]; then
    echo -e "\e[0;32mrunning STRASSEN DOUBLE PRECISION...\e[0m"
    interleavings=(BB DBB BDB BBD DBDB BDDB BDBD BBB DBBB BDBB BBDB BBBD BDBDB BBDBD BBBB BDBBB BBDBB BBBDB BDBDBB BBDBDB)

    icc $DFLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
    mem_sweep_square "${interleavings[@]}"

    icc $FLAGS -I framework -o harness algorithms/strassen/*.cpp $FRAMEWORK
    timing_sweep_square "${interleavings[@]}"

elif [ "$1" = "strassen-single" ]; then
    echo -e "\e[0;32mrunning STRASSEN SINGLE PRECISION...\e[0m"
    interleavings=(BB DBB BDB BBD DBDB BDDB BDBD BBB DBBB BDBB BBDB BBBD BDBDB BBDBD BBBB BDBBB BBDBB BBBDB BDBDBB BBDBDB)

    icc $DFLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
    mem_sweep_square "${interleavings[@]}"

    icc $FLAGS -I framework -o harness algorithms/strassen-single/*.cpp $FRAMEWORK
    timing_sweep_square "${interleavings[@]}"

elif [ "$1" = "carma" ]; then
    echo -e "\e[0;32mrunning CARMA DOUBLE...\e[0m"
    interleavings5=(BBBBB DBBBBB BDBBBB BBBDBB BBBBBD BDBDBBB BBBDBDB BBBBBDD BBBDDBB BBDBDBDB BDBDBDBB BDBDBDBDB)
    interleavings6=(BBBBBB BBBDBBB BDBBBBB BBBBBDB BBBBDBB BBBBBBD DBBBBBB BBBDDBBB BBDBBDBB BBBBBDDB BBDBDBDBB BDBDBDBDBDB)
    interleavings7=(BBBBBBB BBBDBBBB BBBBDBBB BDBBBBBB BBBBBBDB BBBBBBBD DBBBBBBB BBBDBDBBB BBDBBBDBB BDBBBBBDB BBBBBBDDB BBBDBDBDBB BDBDBDBDBDBDB)

    icc  -I framework $DFLAGS -I framework -o harness  algorithms/carma/*.cpp $FRAMEWORK
    mem_sweep_skinny "${interleavings5[@]}"
    mem_sweep_skinny "${interleavings6[@]}"
    mem_sweep_skinny "${interleavings7[@]}"

    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma/*.cpp $FRAMEWORK
    timing_sweep_skinny "${interleavings5[@]}"
    timing_sweep_skinny "${interleavings6[@]}"
    timing_sweep_skinny "${interleavings7[@]}"

elif [ "$1" = "carma-single" ]; then
    echo -e "\e[0;32mrunning CARMA SINGLE...\e[0m"
    interleavings5=(BBBBB DBBBBB BDBBBB BBBDBB BBBBBD BDBDBBB BBBDBDB BBBBBDD BBBDDBB BBDBDBDB BDBDBDBB BDBDBDBDB)
    interleavings6=(BBBBBB BBBDBBB BDBBBBB BBBBBDB BBBBDBB BBBBBBD DBBBBBB BBBDDBBB BBDBBDBB BBBBBDDB BBDBDBDBB BDBDBDBDBDB)
    interleavings7=(BBBBBBB BBBDBBBB BBBBDBBB BDBBBBBB BBBBBBDB BBBBBBBD DBBBBBBB BBBDBDBBB BBDBBBDBB BDBBBBBDB BBBBBBDDB BBBDBDBDBB BDBDBDBDBDBDB)

    icc  -I framework $DFLAGS -I framework -o harness  algorithms/carma-single/*.cpp $FRAMEWORK
    mem_sweep_skinny "${interleavings5[@]}"
    mem_sweep_skinny "${interleavings6[@]}"
    mem_sweep_skinny "${interleavings7[@]}"

    icc  -I framework $FLAGS -I framework -o harness  algorithms/carma-single/*.cpp $FRAMEWORK
    timing_sweep_skinny "${interleavings5[@]}"
    timing_sweep_skinny "${interleavings6[@]}"
    timing_sweep_skinny "${interleavings7[@]}"

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
