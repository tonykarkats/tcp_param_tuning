#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

# rmem
rmem = np.arange(0, 10)
fct_r = np.array([13.75, 13.78, 13.01, 11.66, 11.01, 10.65, 8.8, 8.51, 8.51, 8.51])
vm_fct_r = np.array([18.54, 16.44, 15.43, 14.3, 12.2, 11.7, 10.23, 10.23, 10.23])
rmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k']

plt.plot(rmem, fct_r)
plt.plot(rmem, vm_fct_r)
plt.xticks(rmem, rmem_labels)
plt.margins(0.2)

plt.xlabel('rmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()


# wmem
wmem = np.arange(0, 10)
fct_w = np.array([238, 235, 178, 44.31, 32.44, 20.73, 15.91, 9.9, 8.51, 8.51 ])
wmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k']

plt.plot(wmem, fct_w)
plt.xticks(wmem, wmem_labels)
plt.margins(0.2)

plt.xlabel('wmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()
