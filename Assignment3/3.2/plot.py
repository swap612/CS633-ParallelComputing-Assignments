import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#read the plot file
data=pd.read_csv('read_plot',header =None)
# change the b/w column into 9,10 dimensions
y=np.reshape(data[1].values, (9,10) )
# transpose the data
z=y.T
# select the 10th element from column 0 to display the X axis labels
col=data.iloc[::10][0]
# plotting the boxplot
fig, ax = plt.subplots()
ax.boxplot(z,labels=col)
# plot legends
plt.xlabel('nspec_process')
plt.ylabel('Read Bandwidth (MiB/s)')
plt.savefig('read_plot')

plt.clf()

#Write plot 
data=pd.read_csv('write_plot',header =None)
y=np.reshape(data[1].values, (9,10) )
z=y.T
col=data.iloc[::10][0]

fig, ax = plt.subplots()
ax.boxplot(z,labels=col)
plt.xlabel('nspec_process')
plt.ylabel('Write Bandwidth (MiB/s)')
plt.savefig('write_plot')

