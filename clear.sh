#!/bin/bash

rm -rf *.csv
rm -rf timing.txt
rm -rf nohup.out
rm -rf harness

if [ ${1} ] && [ ${1} = "opentuner" ]; then
    rm -rf opentuner.db opentuner.log
fi
