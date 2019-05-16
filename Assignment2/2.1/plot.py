# importing required modules 
import matplotlib.pyplot as plt
import numpy as np 
import sys

# Read data from file passed as an argument
x, y = np.loadtxt(sys.argv[1], delimiter=',', unpack=True)      # store the values from file into x and y
plt.plot(x[0:5],y[0:5], marker='s')

Data = sys.argv[1][9:]      #take datasize from argument- filename(eg. from filename plotdata_512 it takes 512 )
plt.plot(x[5:10],y[5:10], marker='s')
# Set the labels and title to plot 
plt.xlabel('N (No. of Processes)')
plt.ylabel('Time (seconds)')
plt.title("Data = "+Data+" KB")
plt.legend(['P2P', 'Collective'], loc='lower right')

# Output the plot to file perf-D
plt.savefig("perf-"+Data)
