#include "memory.h"

bool Memory::initialized = false;
int Memory::current;
int Memory::max;
pthread_mutex_t Memory::m1;
std::map<void*, size_t> Memory::track;

void* Memory::malloctrack(size_t size) {
	int n = 0;
	if (!initialized) {
		current = 0;
		max = 0;
		pthread_mutex_init(&m1, 0);
		initialized = true;
	}

	__sync_fetch_and_add(&current, size);

	pthread_mutex_lock(&m1);
	//current += size;
	if (current > max)
		max = current;
	pthread_mutex_unlock(&m1);
	
	void* ptr = malloc(size);
	track[ptr] = size;

	return ptr;
}

void Memory::freetrack(void* ptr) {
	if (ptr == 0)
		return;

	size_t size = track[ptr];
	size = -size;

	__sync_fetch_and_add(&current, size);

	//pthread_mutex_lock(&m1);
	//current -= size;
	//pthread_mutex_unlock(&m1);
	
	free(ptr);
}

int Memory::getMem() {
	return current;
}
