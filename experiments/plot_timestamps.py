#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

# rmem
ind = [1, 2]
fct = np.array([8.41, 8.48])
labels = ['w/o timestamps', 'w/ timestamps']

plt.bar(ind, fct)
plt.xticks(ind, labels)
plt.margins(0.2)

plt.xlabel('rmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()
