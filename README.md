### Usage
```
bash runner.sh <alg>
```
ex:
```
bash runner.sh carma.c
```
```
bash runner.sh mergesort.c
```

### Making your own CARDIO algorithm
*  Add your algorithm, which implements the proper methods (see quicksort.c for a simple example)
*  Add a running/testing harness for your algorithm (see sort_harness.c for a simple example)
*  Add a case in runner.sh that will compile your algorithm (see runner.sh for exiting examples)

### Requirements
You must have Intel Parallel Studio XE installed, which will provide Cilk and MKL.

### Authors
David Eliahu, Omer Spillinger

### Mentors/Supervisors
Professor Armando Fox, Professor James Demmel, Benjamin Lipshitz, Oded Schwartz, Shoaib Kamil
