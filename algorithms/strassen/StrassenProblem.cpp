#include "StrassenProblem.h"

StrassenProblem::StrassenProblem(int m, int k, int n, double *A, double *B, double *C) {
    this->m = m;
    this->n = n;
    this->k = k;
    this->A = A;
    this->B = B;
    this->C = C;
}

bool StrassenProblem::shouldRunBaseCase(int depth) {
    return (depth >= 2);
/*
    if (m <= 32 || k <= 32 || n <= 32){
        return true;
    }
    else {
        return false;
    }
*/
}

void StrassenProblem::runBaseCase() {
    // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T, n, X, n);
    cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m, n,k, 1, A,m, B,k, 0, C,m);
    //printf("Strassen %d Base Case Ran\n",n);
}

std::vector<Task*> StrassenProblem::split() {
    int T_m = m/2;
    int T_k = k/2;
    int S_k = T_k;
    int S_n = n/2;
    int i;
    //compute T0-6, S0-6
    double *T0 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T1 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T2 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T3 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T4 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T5 = (double*) malloc(T_m * T_k * sizeof(double));
    double *T6 = (double*) malloc(T_m * T_k * sizeof(double));

    double *S0 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S1 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S2 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S3 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S4 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S5 = (double*) malloc(S_k * S_n * sizeof(double));
    double *S6 = (double*) malloc(S_k * S_n * sizeof(double));

    for (i = 0; i < T_k; i++){
        memcpy(T0 +i * T_m, A + i*m, T_m * sizeof(double));
        memcpy(T1 +i * T_m, A +(i+T_k) *m, T_m * sizeof(double));

        memcpy(T2 +i * T_m, A +T_m+ i*m, T_m * sizeof(double));
        memcpy(T4 +i * T_m, A + i*m, T_m * sizeof(double));

        memcpy(T5 +i * T_m, A + (i+T_k)*m, T_m * sizeof(double));
        memcpy(T6 +i * T_m, A +T_m+ (i+T_k)*m, T_m * sizeof(double));
    }

    for (i = 0; i < S_n; i++){
        memcpy(S0 +i * S_k, B + i*k, S_k * sizeof(double));

        memcpy(S1 +i * S_k, B + S_k + i*k, S_k * sizeof(double));
        memcpy(S2 +i * S_k, B + (i+S_n)*k, S_k * sizeof(double));
        memcpy(S3 +i * S_k, B + S_k+(i+S_n)*k, S_k * sizeof(double));
        memcpy(S4 +i * S_k, B + S_k+ (i+S_n)*k, S_k * sizeof(double));
        memcpy(S5 +i * S_k, B + S_k +(i+S_n)*k, S_k * sizeof(double));
    }

    matrix_subtract(T_m *T_k, T0, T2, T4);
    matrix_add(T_m *T_k, T2, T6, T2);
    matrix_subtract(T_m *T_k, T2, T0, T3);
    matrix_subtract(T_m *T_k, T1, T3, T5);

    matrix_subtract(S_k*S_n, S4, S2, S4);
    matrix_subtract(S_k *S_n, S2, S0, S2);
    matrix_subtract(S_k *S_n, S3, S2, S3);
    matrix_subtract(S_k *S_n, S3, S1, S6);

    // compute Q0-6
    double *Q0 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q1 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q2 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q3 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q4 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q5 = (double*) malloc(T_m * S_n * sizeof(double));
    double *Q6 = (double*) malloc(T_m * S_n * sizeof(double));

    Task* task1 = new Task(new StrassenProblem(T_m, T_k, S_n, T0, S0, Q0));
    Task* task2 = new Task(new StrassenProblem(T_m, T_k, S_n, T1, S1, Q1));
    Task* task3 = new Task(new StrassenProblem(T_m, T_k, S_n, T2, S2, Q2));
    Task* task4 = new Task(new StrassenProblem(T_m, T_k, S_n, T3, S3, Q3));
    Task* task5 = new Task(new StrassenProblem(T_m, T_k, S_n, T4, S4, Q4));
    Task* task6 = new Task(new StrassenProblem(T_m, T_k, S_n, T5, S5, Q5));
    Task* task7 = new Task(new StrassenProblem(T_m, T_k, S_n, T6, S6, Q6));

    std::vector<Task*> tasks (7);
    tasks[0] = task1;
    tasks[1] = task2;
    tasks[2] = task3;
    tasks[3] = task4;
    tasks[4] = task5;
    tasks[5] = task6;
    tasks[6] = task7;

    return tasks;
}

void StrassenProblem::merge(std::vector<Problem*> problems) {
    //printf("Strassen Merge\n");
    int T_m = m/2;
    int T_k = k/2;
    int S_k = T_k;
    int S_n = n/2;

    double **Q =  (double**) malloc(7 * sizeof(double*));

    int i = 0;
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenProblem *problem = (StrassenProblem*)*problemIterator;
       	Q[i++] = (double*)problem->C;
    }

    double *U1 = (double*) malloc(T_m * S_n * sizeof(double));
    double *U2 = (double*) malloc(T_m * S_n * sizeof(double));
    double *U3 = (double*) malloc(T_m * S_n * sizeof(double));

    matrix_add(T_m *S_n, Q[0], Q[3], U1);
    matrix_add(T_m *S_n, U1, Q[4], U2);
    matrix_add(T_m *S_n, U1, Q[2], U3);

    //compute C
    double *C11 = (double*) malloc(T_m * S_n * sizeof(double));
    double *C12 = (double*) malloc(T_m * S_n * sizeof(double));
    double *C21 = (double*) malloc(T_m * S_n * sizeof(double));
    double *C22 = (double*) malloc(T_m * S_n * sizeof(double));

    matrix_add(T_m *S_n, Q[0], Q[1], C11);
    matrix_add(T_m *S_n, U3, Q[5], C12);
    matrix_subtract(T_m *S_n, U2, Q[6], C21);
    matrix_add(T_m*S_n, U2, Q[2], C22);

    for (i = 0; i < S_n; i++){
        memcpy(C +i * m, C11 + i*T_m, T_m * sizeof(double));
        memcpy(C + (i+S_n) * m, C12 + i*T_m, T_m * sizeof(double));
        memcpy(C +T_m+i * m, C21 + i*T_m, T_m * sizeof(double));
        memcpy(C +T_m+(i+S_n) * m, C22 + i*T_m, T_m * sizeof(double));
    }
}

void StrassenProblem::matrix_add(int N, double *A, double *B, double *C) {
    int i,j;
    for (i =0 ; i < N; i++){
        C[i] = A[i] + B[i];
    }
}

void StrassenProblem::matrix_subtract(int N, double *A, double *B, double *C) {
    int i,j;
    for (i =0 ; i < N; i++){
        C[i] = A[i] - B[i];
    }
}
