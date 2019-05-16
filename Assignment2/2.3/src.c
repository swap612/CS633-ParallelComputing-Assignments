/***********************************************************************************************
 *   File: src.c                                                                               *
 *   Description: 2.3- Implementing a MPI_Reduce(MPI_PROD) using Point-Point communication     *
 *                                                                                             *
 *   Author: Swapnil Raykar                                                                    *
 *                                                                                             *
 ***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    MPI_Status status;

    //getting rank and size
    int myrank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3)
    { // Checking for the src argument datasize
        printf("\nThis program takes 2 argument 1st a data size(in KB) and 2nd the implementation type using Point2Point(0) / MPI_REDUCE(1)");
        exit(1);
    }

    int input = atoi(argv[1]);                                      // atoi used to convert argv[1](size in MB) into int
    int dataSize = 1024 * 1024 * input;                             // Data size in bytes
    int dataElements = dataSize / sizeof(double);                   // No of double elements of data
    double *data = (double *)malloc(dataElements * sizeof(double)); // Data Array
    
    double *proddata;
    // defining time variables
    double proc_time, start_time, max_time;

    // generate the data array
    srand(time(0) + myrank);
    int i;
    for (i = 0; i < dataElements; i++)
    {
        data[i] = myrank;
    }
    int multiplier = 1;
    // Take start time
    start_time = MPI_Wtime();

    if (atoi(argv[2]) == 0) // Reduce using pt-pt communication
    {
        int newrank = myrank;
        int itr;
        for ( itr = 1; itr <= (log(size) / log(2)); itr++)
        {
            if (newrank % 2 == 0)
            {
                double *recvdata = (double *)malloc(dataElements * sizeof(double));
                //recv
                MPI_Recv(recvdata, dataElements, MPI_DOUBLE, (newrank + 1) * multiplier, itr, MPI_COMM_WORLD, &status);
                // printf(" rank: %d data %lf recvdata %lf\n", newrank, data[0], recvdata[0]);

                // Perform PROD
                for (i = 0; i < dataElements; i++)
                {
                    data[i] = data[i] * recvdata[i];
                }
                // printf("itr %d rank:%d-> %d mult data %lf\n", itr, (newrank + 1) * multiplier, myrank, data[0]);

                newrank = newrank / 2;
                free(recvdata);
            }
            else if (newrank % 2 == 1)
            {
                MPI_Send(data, dataElements, MPI_DOUBLE, (newrank - 1) * multiplier, itr, MPI_COMM_WORLD);
                // printf("itr %d Producer rank: %d -> %d send data %lf\n", itr, myrank, (newrank - 1) * multiplier, data[0]);
                free(data);
                break;

            }
            multiplier *= 2;
        }
    }
    else if (atoi(argv[2]) == 1) //using MPI_Reduce
    {
        // printf("Rank: %d data %lf\n", myrank, data[0]);
        if (myrank == 0)
            proddata = (double *)malloc(dataElements * sizeof(double)); // Data Array

        // Reduce func to do PROD
        MPI_Reduce(data, proddata, dataElements, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
        // if (myrank == 0)
            // printf("Consumer rank: %d recv. data %lf\n", myrank, proddata[0]);
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