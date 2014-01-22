### FRPA: A Framework for Recursive Parallel Algorithms

### Usage
```
bash run.sh <alg>
```
ex:
```
bash run.sh carma
```
```
bash run.sh mergesort
```

### Making your own FRPA algorithm
*  Add your algorithm, which implements the proper methods (see QuicksortProblem.cpp for a simple example)
*  Add a running/testing harness for your algorithm (see quicksort_harness.cpp for a simple example)
*  Add a case in run.sh that will compile your algorithm (see run.sh for existing examples)

### Tracking memory allocation
*  Include "memory.h" and "debug.h" in the *algorithm*Problem.cpp file (see SyrkProblem.cpp for a simple example)
*  Add "-DDEBUG" to the flags in the run.sh file

### Requirements
You must have Intel Parallel Studio XE installed, which will provide Cilk and MKL.

### Authors
David Eliahu, Omer Spillinger

### Contributors
Peter Birsinger, Benjamin Lipshitz, Kevin Sheu

### Mentors/Supervisors
Professor Armando Fox, Professor James Demmel, Benjamin Lipshitz, Oded Schwartz, Shoaib Kamil
