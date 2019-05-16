/***********************************************************************************************
 *   File: src.c                                                                               *
 *   Description: Producer-Consumer Problem using MPI Collectives                              *
 *                                                                                             *
 *   Author: Swapnil Raykar                                                                    *
 *                                                                                             *
 ***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);

    //getting rank and size
    int myrank, size, newrank, newsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size % 2 == 1)      //For 1-1 mapping of producer-consumer the size should be even      
    { // Checking for the mpiexec argument -n (no of processes)
        printf("\n please enter even(2,4,..) value for n");
        exit(1);
    }

    if (argc != 2)
    { // Checking for the src argument datasize
        printf("\nThis program takes 1 argument as a data size(in KB)");
        exit(1);
    }

    int input = atoi(argv[1]);                    // atoi used to convert argv[1](size in KB) into int
    int dataSize = 1024 * input;                  // Data size in bytes
    int dataElements = dataSize / sizeof(double); // No of double elements of data
    double data[dataElements];                    // Data Array

    // defining time variables
    double proc_time, start_time, max_time;
    // defining new communicator
    MPI_Comm newcomm;

    // Take start time
    start_time = MPI_Wtime();

    // splitting
    int colour = myrank / 2;        // Assigning same color for consecutive ranks in pair of 2, so data transfer will be on same node
    MPI_Comm_split(MPI_COMM_WORLD, colour, myrank, &newcomm);
   
    // get new rank and size
    MPI_Comm_rank(newcomm, &newrank);
    MPI_Comm_size(newcomm, &newsize);
    // printf("My Rank: %d New Rank: %d New Size: %d \n", myrank, newrank, newsize);

    if (newrank == 0)       // Producer as rank 0 of each newcomm prepares the data to broadcast
    {
        // generate the data array
        //Set the seed for rand() based on time, to generate different random numbers every time
        srand(time(0));
        for (int i = 0; i < dataElements; i++)
        {
            data[i] = rand();
        }
    }

    // Broadcasting the data to newcomm containing 1 producer and 1 consumer(rank 0 will bcast to rank 1)
    MPI_Bcast(data, dataElements, MPI_DOUBLE, 0, newcomm);

    // Calculating Processing time
    proc_time = MPI_Wtime() - start_time;

    // Calculating Max_time at rank 0
    MPI_Reduce(&proc_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Printing the max time reduced at rank 0
    if (myrank == 0)
        printf("%d, %lf\n", size, max_time);

    MPI_Comm_free(&newcomm);
    MPI_Finalize();
    return 0;
}