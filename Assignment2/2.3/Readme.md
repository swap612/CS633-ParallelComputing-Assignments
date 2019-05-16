Assignment 2.3
====

## Implementing MPI_Reduce using point to point communication primitives  

## Steps to run the program    
On HPC2010  
    1. make   
    2. qsub run.sh -> it will generate the plotdata files required for plotting the graphs

## Steps to Plot the graph  
**NOTE**: For automated plot generation, please install the python3 and matplotlib  
1. Run ./plotgraph.sh  - It will generate the graphs by running plot.py with the plotdata files  

## Observations from plots:    
1. My Reduce function takes more time as compared to the MPI Reduce, this is because of the implementation algorithm used, I used binomial algorithm which will be less efficient than MPIREDUCE algorithms as they varies based on size.  
2. With increase in Data from 1MB to 512MB, the time taken will be going to increase as more data need to be transfered so transfer time increases.
3. For a given D, as we increase the number of processes N from 8,16,32,64 to 128, the time should increase linearly. But for some cases it doesn't always increases linearly. This may happen due to network traffic, load on an node and the topology of the network. Therefore there are some ups and downs in the plots.