// SPDX-FileCopyrightText: 2019 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <mpi.h>
constexpr int n = 840;

double for_loop(int istart, int istop, int n)
{
    double sum = 0.0;

    for (int i = istart; i <= istop; i++) {
        double x = (i - 0.5) / static_cast<double>(n);
        sum += 1.0 / (1.0 + x * x);
    }

    return sum;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, ntasks;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Computing approximation to pi with N=%d\n", n);
    }

    std::cout << "rank" << rank << std::endl;

    int istart = rank * n / ntasks + 1;
    int istop  = (rank + 1) * n / ntasks;

    double local_sum = for_loop(istart, istop, n);

    int message_tag = 1;

    if (rank == 0) {
        double global_sum = local_sum;

        for (int sender = 1; sender < ntasks; sender++) {
            double received_sum = 0.0;

            MPI_Recv(&received_sum, 1, MPI_DOUBLE,
                     sender, message_tag, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            global_sum += received_sum;
        }

        double pi = global_sum * 4.0 / n;
        printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
    }
    else {
        MPI_Send(&local_sum, 1, MPI_DOUBLE,
                 0, message_tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}