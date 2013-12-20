#!/bin/bash
source /opt/intel/bin/iccvars.sh intel64

rm -rf *.csv
rm -rf nohup.out

# Run from 64KB to 100MB total memory
for (( array_size=4096; array_size<6553600; array_size+=4096 )); do
    rm -f stream *.o
    icc -O2 -opt-streaming-stores never -openmp stream.c -DSTREAM_ARRAY_SIZE=$array_size /usr/local/lib/libpapi.a -o stream
    ./stream
done

rm -f stream *.o
