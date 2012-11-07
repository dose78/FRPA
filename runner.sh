#!/bin/bash

export CILK_NWORKERS=32

echo -e "\e[01;34mcompiling...\e[0m"

if [ "$1" = "quicksort" ] || [ "$1" = "quicksort.c" ]; then
  icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer sort_harness.c quicksort.c
  echo -e "\e[0;32mrunning quicksort...\e[0m"

elif [ "$1" = "mergesort" ] || [ "$1" = "mergesort.c" ]; then
  icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer sort_harness.c mergesort.c
  echo -e "\e[0;32mrunning mergesort...\e[0m"

elif [ "$1" = "carma" ] || [ "$1" = "carma.c" ]; then
  icc -mkl -o harness -O3 -ipo -xHOST -no-prec-div -fno-strict-aliasing -fno-omit-frame-pointer carma_harness.c carma.c
  echo -e "\e[0;32mrunning CARMA...\e[0m"

else
  echo -e "\e[0;31mERROR: Algorithm not found\e[0m"
  exit
fi

./harness
rm -rf harness
