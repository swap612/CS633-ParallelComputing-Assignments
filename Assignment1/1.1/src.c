/***********************************************************************************************
 *   File: src.c                                                                               *
 *   Description: Producer-Consumer Problem using MPI Point-to-Point primitives                *
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
    MPI_Status status;

    //getting rank and size
    int myrank, size;
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

    // Take start time
    start_time = MPI_Wtime();

    // Divide the even ranks as producer and odd rank as consumers
    if (myrank % 2 == 0) // Producer
    {
        // generate the data array
        //Set the seed for rand() based on time, to generate different random numbers every time
        srand(time(0));     
        for (int i = 0; i < dataElements; i++)
        {
            data[i] = rand();
        }

        //sending the data to corresponding consumer (one-one mapping every producer(r) sends to next consumer(r+1))
        MPI_Send(data, dataElements, MPI_DOUBLE, myrank + 1, 1, MPI_COMM_WORLD);
        // printf("Prodeucer rank: %d send data %lf\n", myrank,data[0]);
    }
    else if (myrank % 2 == 1) //Consumer
    {
        //consumer(r) receives data from producer(r-1)
        MPI_Recv(data, dataElements, MPI_DOUBLE, myrank - 1, 1, MPI_COMM_WORLD, &status);
        // printf("Consumer rank: %d recv. data %lf\n", myrank, data[0]);
    }
    
    // Calculating Processing time
    proc_time = MPI_Wtime() - start_time;

    // Calculating Max_time at rank 0 
    MPI_Reduce(&proc_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Printing the max time reduced at rank 0
    if (myrank == 0)
        printf("%d, %lf\n", size, max_time);

    MPI_Finalize();
    return 0;
}