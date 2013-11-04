#include <cstdio>
#include "memory.h"

long Memory::current = 0;
long Memory::max = 0;
long Memory::total = 0;

void* Memory::malloctrack(size_t size) {
    __sync_fetch_and_add(&total, size);
    int new_usage = __sync_add_and_fetch(&current, size);
    while(new_usage > max) {
        int local_max = max;
        if (new_usage > local_max) {
            __sync_bool_compare_and_swap(&max, local_max, new_usage);
        }
    }

    void* ptr = malloc(sizeof(size_t) + size);
    size_t* ptr2 = (size_t*)ptr;
    *ptr2 = size;
    return (void*)(ptr2 + 1);
}

void Memory::freetrack(void* ptr) {
    size_t* ptr2 = (size_t*)ptr - 1;
    size_t size = *ptr2;
    __sync_sub_and_fetch(&current, size);
    free((void*)ptr2);
}

void Memory::reset() {
    current = 0;
    max = 0;
    total = 0;
}

long Memory::getCurrent() {
    return current;
}

long Memory::getMax() {
    return max;
}

long Memory::getTotal() {
    return total;
}

void Memory::printCurrent() {
    printf("memory current: %ld bytes\n", getCurrent());
}

void Memory::printMax() {
    printf("memory max: %ld bytes\n", getMax());
}

void Memory::printTotal() {
    printf("memory total: %ld bytes\n", getTotal());
}

void Memory::printAll() {
    printf("memory current: %ld, max: %ld, total: %ld (bytes)\n", getCurrent(), getMax(), getTotal());
}
