# include <stdio.h>
# include <math.h>
# include <float.h>
# include <limits.h>
# include <sys/time.h>
# include <papi.h>

#ifndef STREAM_ARRAY_SIZE
#   define STREAM_ARRAY_SIZE    6553600 //100MB
#endif

#ifndef NUM_ITERATIONS
#   define NUM_ITERATIONS    100
#endif

static double  a[STREAM_ARRAY_SIZE], b[STREAM_ARRAY_SIZE];

void handle_error (int retval) {
     printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
     exit(1);
}

int main() {
    srand48(time(NULL));

    int j, i;
    FILE *f = fopen("data_stream_misses.csv","a");

    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        exit(1);
    }

    int NUM_EVENTS = 1;
    long long counters[NUM_EVENTS];

    int PAPI_events[] = {
        PAPI_L3_TCM
    };

    for (i = 0; i < STREAM_ARRAY_SIZE; i++) {
        a[i] = 2 * drand48() - 1;
        b[i] = 2 * drand48() - 1;
    }

    for (i = 0; i < NUM_ITERATIONS; i++) {
        if (PAPI_start_counters(PAPI_events, NUM_EVENTS) != PAPI_OK) {
            fprintf(stderr, "PAPI_start_counters - FAILED\n");
            exit(1);
        }

        for (j=0; j<STREAM_ARRAY_SIZE; j++)
            b[j] = a[j];

        if (PAPI_read_counters(counters, NUM_EVENTS) != PAPI_OK) {
            fprintf(stderr, "PAPI_read_counters - FAILED\n");
            exit(1);
        }

        printf("trial %d) %lld misses (expected %d)\n", i, counters[0], 2*STREAM_ARRAY_SIZE/8);
        fprintf(f, "%d,%lld\n", STREAM_ARRAY_SIZE, counters[0]);

        if (PAPI_stop_counters(counters, NUM_EVENTS) != PAPI_OK) {
            fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
            exit(1);
        }
    }

    PAPI_shutdown();
    fclose(f);

    return 0;
}
