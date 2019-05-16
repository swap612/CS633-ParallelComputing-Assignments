/***********************************************************************************************
 *   File: src.c                                                                               *
 *   Description: Assignment 2.2- Ping Pong Benachmark to measure bandwidth                    *
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

    if (argc != 2)
    { // Checking for the src argument datasize
        printf("\nThis program takes 1 argument a data size(in MB)");
        exit(1);
    }

    int input = atoi(argv[1]);                    // atoi used to convert argv[1](size in MB) into int
    int dataSize = 1024 * 1024 * input;           // Data size in bytes
    int dataElements = dataSize / sizeof(double); // No of double elements of data

    // defining time variables
    double proc_time, start_time, total_time, sum_time, itr_time;

    // generate the data array
    double *data = (double *)malloc(dataElements * sizeof(double)); // Data Array

    //Set the seed for rand() based on time, to generate different random numbers every time
    srand(time(0) + myrank);
    int i;
    for (i = 0; i < dataElements; i++)
    {
        data[i] = rand();
    }

    double avg_bw = 0;
    int itr;
    for (itr = 1; itr < 11; itr++)
    {
        sum_time = 0; // Reseting the time
        itr_time = 0;
        // sending data
        int receiver;
        for (receiver = myrank + 1; receiver < size; receiver++) // send to all nodes greater than myrank
        {
            start_time = MPI_Wtime();

            //sending the data to corresponding reciever
            MPI_Send(data, dataElements, MPI_DOUBLE, receiver, 1, MPI_COMM_WORLD);
            // printf("Producer rank: %d send to %d  Data: %lf  %lf  %lf\n", myrank, receiver, data[0], data[1], data[2]);

            proc_time = MPI_Wtime() - start_time;
            
            sum_time += proc_time;  // adding the time requires to send all the processes

        }

        double *recvdata;
        
        // Receiving data
        int sender;
        for (sender = 0; sender < myrank; sender++)     // Recieve from all the nodes less than myrank
        {
            recvdata = (double *)malloc(dataElements * sizeof(double));
            // printf("%d recieving from %d\n", myrank, sender);
            MPI_Recv(recvdata, dataElements, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            // printf("Consumer rank: %d recv from %d Data: %lf  %lf  %lf\n", myrank, status.MPI_SOURCE, recvdata[0], recvdata[1], recvdata[2]);
            free(recvdata);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // Calculating Max_time at rank 0
        MPI_Reduce(&sum_time, &itr_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        // calculating iteration bandwidth
        if (myrank == 0)
        {
            int total_sends = (size * (size - 1)) / 2;      // nC2 sends
            avg_bw = (input / (itr_time / total_sends));    //calculating BW
            printf("%d, %lf\n", input, avg_bw);
            // printf("Avg BW: %lf MB/s Total Time:%lf Send Count %d Data: %d \n", avg_bw, itr_time, total_sends, dataSize);
        }
    }
    free(data); // Free memory

    MPI_Finalize();
    return 0;
}
