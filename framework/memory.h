#ifndef MEMORY
#define MEMORY


#include <stdlib.h>
#include <pthread.h>
#include <map>


class Memory {
public:
	static void* malloctrack(size_t size);
	static void freetrack(void* ptr);
	static int getMem();

	static pthread_mutex_t m1;
	static int current;
	static int max;
	static bool initialized;
	static std::map<void*, size_t> track;
};

#endif