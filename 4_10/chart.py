import matplotlib.pyplot as plt
import numpy as np

filename = "as_23_subgraph#_summary.txt"
file = open(filename,'r')
raw_data = [line.split() for line in file.readlines()]
data = np.array(raw_data[:])[:,-1]
temp = data.astype(np.float)

#max_val=float(data[np.argmax(data)])
#print(max_val)
#bin=np.linspace(0,max_val+1,100)
#plt.hist(temp, bins='sqrt')
#plt.show()

plt.plot(np.arange(733), temp)
plt.show()
