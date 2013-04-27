### Usage
```
bash runner.sh <alg>
```
ex:
```
bash runner.sh carma
```
```
bash runner.sh mergesort
```

### Making your own CARDIO algorithm
*  Add your algorithm, which implements the proper methods (see QuicksortProblem.cpp for a simple example)
*  Add a running/testing harness for your algorithm (see quicksort_harness.cpp for a simple example)
*  Add a case in run.sh that will compile your algorithm (see run.sh for exiting examples)

### Requirements
You must have Intel Parallel Studio XE installed, which will provide Cilk and MKL.

### Authors
David Eliahu, Omer Spillinger

### Mentors/Supervisors
Professor Armando Fox, Professor James Demmel, Benjamin Lipshitz, Oded Schwartz, Shoaib Kamil
