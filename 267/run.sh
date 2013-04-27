rm a.out
icc -mkl TrsmProblem.cpp MultProblem.cpp test.cpp Framework.cpp
./a.out
