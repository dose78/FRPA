#ifndef MEMORY
#define MEMORY

#include <stdlib.h>
#include <pthread.h>

class Memory {
public:
    static void* malloctrack(size_t size);
    static void freetrack(void* ptr);
    static long getCurrent();
    static long getMax();
    static long getTotal();
    static void printCurrent();
    static void printMax();
    static void printTotal();
    static void printAll();

private:
    static long current;
    static long max;
    static long total;
};

#endif
