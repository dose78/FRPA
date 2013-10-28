#ifndef MEMORY
#define MEMORY

#include <stdlib.h>
#include <pthread.h>

class Memory {
public:
    static void* malloctrack(size_t size);
    static void freetrack(void* ptr);
    static int getCurrent();
    static int getMax();
    static int getTotal();
    static void printCurrent();
    static void printMax();
    static void printTotal();
    static void printAll();

private:
    static int current;
    static int max;
    static int total;
};

#endif
