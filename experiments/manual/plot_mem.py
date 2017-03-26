#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

# rmem
rmem = np.arange(0, 10)
fct_r = np.array([13.75, 13.78, 13.01, 11.66, 11.01, 10.65, 8.8, 8.51, 8.51, 8.51])
vm_fct_r = np.array([18.54, 16.44, 15.43, 14.3, 12.2, 11.7, 10.23, 10.11, 10.11, 10.11])
rmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k']

plt.plot(rmem, fct_r, label='Bare Metal')
plt.plot(rmem, vm_fct_r, label='VM')
plt.xticks(rmem, rmem_labels)
plt.legend()
plt.margins(0.2)

plt.xlabel('rmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()


# wmem
wmem = np.arange(0, 11)
fct_w = np.array([238, 235, 178, 44.31, 32.44, 20.73, 15.91, 9.9, 8.51, 8.51, 8.51 ])
vm_fct_w = np.array([313, 300, 216, 70, 64, 43.2, 34.5, 20.42, 12.1, 11.1, 11.0])
azure_fct_w = np.array([52.81, 44.57, 63.24, 38.27, 38.11, 38.55, 38.31, 20.74, 15.11, 10.68, 10.85])
wmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k', '512k']

plt.plot(wmem, fct_w, label='Bare Metal')
plt.plot(wmem, vm_fct_w, label='Dryad VM')
plt.plot(wmem, azure_fct_w, label='Azure VM')
plt.xticks(wmem, wmem_labels)
plt.margins(0.2)
plt.legend()

plt.xlabel('wmem size (Bytes)')
plt.ylabel('FCT (sec)')
plt.title('Flow completion time for a single 1GB flow')
plt.grid(True)
plt.show()
