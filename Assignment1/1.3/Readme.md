Assignment1.3
===
Same as 1.1, 1.2
 
The mapping of Producer-Consumer written in 1.1 is optimal.  
I have assigned even ranks to producer and odd ranks to consumer. Mapping like 0->1, 2->3.  
So, the data is always send on the same node, no data transfer across the nodes which improve the performance. So i think this mapping is optimal.

Also, the mapping done in 1.2 is optimal.  
I have created the new communicator based on color = (myrank / 2)    
This will assign same color for consecutive ranks in pair of 2, new comm will be like (0,1),(2,3),(4,5).  
So, in 1.2 also the data is always send on the same node, no data transfer across the nodes which improve the performance. So i think this mapping is optimal.