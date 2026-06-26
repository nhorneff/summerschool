// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <vector>
#include <mpi.h>
#include <iostream>

#define MAX_PRINT_SIZE 12

void init_buffer(std::vector<int> &buffer);
void print_buffer(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int size, rank, buf_size=12;
    std::vector<int> buf(buf_size);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Initialize message buffer */
    init_buffer(buf);

    /* Print data that will be sent */
    print_buffer(buf);

    /* Start timing */
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    /* Send everywhere */
    // TODO: Implement the broadcast of the array buf
    /* Send everywhere */
    const int tag = 0;
    // broadcast
    // if (rank == 0) {
    //     for (int dest = 1; dest < size; dest++) {
    //         MPI_Send(buf.data(), buf.size(), MPI_INT, dest, tag, MPI_COMM_WORLD);
    //     }
    // } else {
    //     MPI_Recv(buf.data(), buf.size(), MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }
    // scatter
    int bufLengthPerProcess = buf.size() / size;
    std::vector<int> localBuf(bufLengthPerProcess);
    // with mpi send/recv
    // if (rank == 0) {
    //     // Keep block 0
    //     for (int i = 0; i < bufLengthPerProcess; i++)
    //     localBuf[i] = buf[i];
    //     // Send remaining blocks
    //     for (int dest = 1; dest < size; dest++) {
    //         int localIndexStart = dest * bufLengthPerProcess;
    //         std::cout << localIndexStart << std::endl;
    //         MPI_Send(buf.data()+localIndexStart, bufLengthPerProcess, MPI_INT, dest, tag, MPI_COMM_WORLD);
    //     }
    // } else {
    //     MPI_Recv(localBuf.data(), bufLengthPerProcess, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }
    // with mpi_scatter
    MPI_Scatter(buf.data(), bufLengthPerProcess, MPI_INT,
            localBuf.data(), bufLengthPerProcess, MPI_INT,
            0, MPI_COMM_WORLD);
    // Copy localBuf back into buf only for printing
    for (int i = 0; i < buf.size(); i++) {
        buf[i] = -1;
    }

    for (int i = 0; i < bufLengthPerProcess; i++) {
        buf[i] = localBuf[i];
    }
    // broadcast
    // MPI_Bcast(buf.data(), buf.size(), MPI_INT, 0, MPI_COMM_WORLD);

    /* End timing */
    double t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(buf);
    if (rank == 0) {
        printf("Time elapsed: %6.8f s\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}


void init_buffer(std::vector<int> &buffer)
{
    int rank;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = i;
        }
    } else {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = -1;
        }
    }
}


void print_buffer(std::vector<int> &buffer)
{
    int rank, size;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> printbuffer(buffersize * size);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < size; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < MAX_PRINT_SIZE; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            if (MAX_PRINT_SIZE < buffersize) {
                printf(" ...");
            }
            printf("\n");
        }
        printf("\n");
    }
}
