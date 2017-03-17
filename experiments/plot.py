#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

rmem = np.arange(0, 10)
fct = np.array([13.75, 13.78, 13.01, 11.66, 11.01, 10.65, 8.8, 8.51, 8.51, 8.51])
rmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k']

plt.plot(rmem, fct)
plt.xticks(rmem, rmem_labels)
plt.margins(0.2)

plt.xlabel('rmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()