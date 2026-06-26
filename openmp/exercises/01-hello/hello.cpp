// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>

#ifdef _OPENMP
#include <omp.h>
#else
static int omp_get_thread_num()  { return 0; }
static int omp_get_num_threads() { return 1; }
#endif

int main()
{
    printf("Hello world!\n");

    #pragma omp parallel
    {
        printf("Hello from thread!\n");
        int local_thread = omp_get_thread_num();
        printf("thread id %i\n",local_thread);
    }
    int nthreads = omp_get_num_threads();
    printf("the total thread number is %i\n",nthreads);   

    return 0;
}
