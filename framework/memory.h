#ifndef MEMORY
#define MEMORY

#include <stdlib.h>
#include <pthread.h>

class Memory {
public:
    static int current;
    static int max;
    static void* malloctrack(size_t size);
    static void freetrack(void* ptr);
    static int getMem();
    static int getMax();
};

#endif
