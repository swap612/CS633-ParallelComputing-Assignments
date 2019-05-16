Assignment 1.1
====
## Producer consumer problem using point-to-point communication primitives 

## Steps to run:
**NOTE**: For automated plot generation, please install the python3 and matplotlib

1. Execute run script from terminal. It will make the program, generate the machine file, run the program and generate the plots automatically.   
     ./run.sh

## Logic for Producer Consumer Mapping:
I have assigned even ranks to producer and odd ranks to consumer. Mapping like 0->1, 2->3.   
So, the data is always send on the same node, no data transfer across the nodes which improve the performance.  

## Observations from plots:  
1. With increase in Data from 1KB to 1024KB, the time taken will be going to increase as more data need to be transfered so transfer time increases.
2. For a given D, as we increase the number of processes N from 8,16,32,64 to 128, the time should increase linearly. But for some cases it doesn't always increases linearly. This may happen due to network traffic, load on an node and the topology of the network. Therefore there are some ups and downs in the plots.
