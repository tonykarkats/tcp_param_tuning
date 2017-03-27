#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

# wmem
wmem_results_file = "tcp_mem.out"
fct = {}

with open(wmem_results_file, "r") as infile:
    lines = infile.readlines()
    for line in lines:
        flowsize = int(line.split()[0])
        if flowsize in fct:
            fct[flowsize].append((int(line.split()[1]), int(line.split()[2])))
        else:
            fct[flowsize] = []
		
for flowsize in fct:
    print(fct[flowsize])

#wmem = np.arange(0, 11)
#fct_w = np.array([238, 235, 178, 44.31, 32.44, 20.73, 15.91, 9.9, 8.51, 8.51, 8.51 ])
#vm_fct_w = np.array([313, 300, 216, 70, 64, 43.2, 34.5, 20.42, 12.1, 11.1, 11.0])
#azure_fct_w = np.array([52.81, 44.57, 63.24, 38.27, 38.11, 38.55, 38.31, 20.74, 15.11, 10.68, 10.85])
#wmem_labels = ['512', '1k', '2k', '4k', '8k', '16k', '32k', '64k', '128k', '256k', '512k']
#
#plt.plot(wmem, fct_w, label='Bare Metal')
#plt.plot(wmem, vm_fct_w, label='Dryad VM')
#plt.plot(wmem, azure_fct_w, label='Azure VM')
#plt.xticks(wmem, wmem_labels)
#plt.margins(0.2)
#plt.legend()
#
#plt.xlabel('wmem size (Bytes)')
#plt.ylabel('FCT (sec)')
#plt.title('Flow completion time for a single 1GB flow')
#plt.grid(True)
#plt.show()
