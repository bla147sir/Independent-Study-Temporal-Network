import matplotlib.pyplot as plt
import numpy as np
import sys

filename = sys.argv[1]
#filename = "a2q_12_leaves#_summary.txt"
file = open(filename,'r')
raw_data = [line.split() for line in file.readlines()]
data = np.array(raw_data[:])[:,-1]
temp = data.astype(np.float)
fig = plt.figure()
#max_val=float(data[np.argmax(data)])
#print(max_val)
#bin=np.linspace(0,max_val+1,100)
#plt.hist(temp, bins='sqrt')
#plt.show()

plt.plot(np.arange(len(raw_data)), temp)
fig.savefig(filename + '.png')
