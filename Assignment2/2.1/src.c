/***********************************************************************************************
 *   File: src.c                                                                               *
 *   Description: Assignment 2.1                                                               *
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

    // defining new communicator
    MPI_Comm newcomm;
    int newrank, newsize;

    if (size % 4 != 0) //For 3-1 mapping of producer-consumer the size should be multiple of 4
    {                  // Checking for the mpiexec argument -n (no of processes)
        printf("\n please enter n value as multiple of 4");
        exit(1);
    }

    if (argc != 3)
    { // Checking for the src argument datasize
        printf("\nThis program takes 2 argument 1st a data size(in KB) and 2nd the implementation type P2P(0)/Collective(1)");
        exit(1);
    }

    const int PROD_COUNT = 3;     // Producer count per consumer
    const int STAGE_COUNT = 1000; // Max Iteration count

    int input = atoi(argv[1]);                    // atoi used to convert argv[1](size in KB) into int
    int dataSize = 1024 * input;                  // Data size in bytes
    int dataElements = dataSize / sizeof(double); // No of double elements of data
    double data[dataElements];                    // Data Array
    double consumerData[PROD_COUNT][dataElements];
    // defining time variables
    double proc_time, start_time, max_time;

    // Take start time
    start_time = MPI_Wtime();

    if (atoi(argv[2]) == 0) //point-to-point comm.
    {
        // Divide the ranks as producer and consumers Producer-Ranks which are not multiple of 4
        // for eg. 0-3, 0 will be consumer and 1-3 rank will be producer
        if (myrank % 4 != 0)
        {
            // generate the data array
            //Set the seed for rand() based on time, to generate different random numbers every time
            srand(time(0) + myrank);
            int i;
            for (i = 0; i < dataElements; i++)
            {
                data[i] = rand();
            }

            // Manage 1000 iteration
            int stage;
            for (stage = 1; stage <= STAGE_COUNT; stage++)
            {
                //sending the data to corresponding consumer (3-1 mapping)
                MPI_Send(data, dataElements, MPI_DOUBLE, myrank - (myrank % 4), stage, MPI_COMM_WORLD);
                // printf("Producer rank: %d stage %d send data %lf\n", myrank, stage, data[0]);

                // Update the data array

                for (i = 0; i < dataElements; i++)
                {
                    data[i] = data[i] / 2;
                }
            }
        }
        else if (myrank % 4 == 0) //Consumer
        {
            int stage;
            for (stage = 1; stage <= STAGE_COUNT; stage++)
            {

                //consumer receives data from 3 producers
                int i;
                for (i = 0; i < PROD_COUNT; i++)
                {
                    MPI_Recv(consumerData[i], dataElements, MPI_DOUBLE, MPI_ANY_SOURCE, stage, MPI_COMM_WORLD, &status);
                }
                // printf("Consumer rank: %d stage %d recv. data %lf  %lf  %lf\n", myrank, stage, consumerData[0][0],consumerData[1][0],consumerData[2][0]);
            }
        }
    }
    else if (atoi(argv[2]) == 1) // using collective
    {
        // Splitting by assigning same color for consecutive 4 ranks(rank 0-3),
        // so new rank 0 will consume data produced by rank 1-3
        int colour = myrank / 4;
        MPI_Comm_split(MPI_COMM_WORLD, colour, myrank, &newcomm);

        // get new rank and size
        MPI_Comm_rank(newcomm, &newrank);
        MPI_Comm_size(newcomm, &newsize);
        // printf("My Rank: %d New Rank: %d New Size: %d \n", myrank, newrank, newsize);

        if (newrank != 0)
        {
            // generate the data array
            srand(time(0) + myrank);
            int i;
            for (i = 0; i < dataElements; i++)
            {
                data[i] = rand();
            }
        }
        else
        {
            // set data as 0 for consumer
            int i;
            for (i = 0; i < dataElements; i++)
            {
                data[i] = 0;
            }
        }

        // 1000 iteration
        int stage;
        for (stage = 1; stage <= STAGE_COUNT; stage++)
        {
            // consumer rank 0 gathers data from all producers
            MPI_Gather(data, dataElements, MPI_DOUBLE, consumerData, dataElements, MPI_DOUBLE, 0, newcomm);
            // printf("Producer rank: %d stage %d send data %lf\n", myrank, stage, data[0]);

            if (newrank != 0) // Producers update the data array
            {
                int i;
                for (i = 0; i < dataElements; i++)
                {
                    data[i] = data[i] / 2; // update the producer data as x'=x/2
                }
            }
        }
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
