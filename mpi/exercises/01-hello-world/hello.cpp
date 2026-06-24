// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, numberOfRanks;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfRanks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0){
        std::cout << rank << std::endl;
    } else if (rank==numberOfRanks-1){
        std::cout << "I'm the last but not least" << std::endl;
    } else if (rank==42){
        std::cout << "I'm the Answer to the Ultimate Question of Life, the Universe, and Everything!" << std::endl;
    }
    MPI_Finalize();
}
