#!/usr/bin/python

import os
import sys

folder = sys.argv[1]
os.mkdir("run_avg")

for file in os.listdir(folder):
	print("Opening {}".format(file))
	with open("{}/{}".format(folder,file), "r") as infile:
		outfile = open("run_avg/{}".format(file), "w+")
		lines = infile.readlines()
		for line in lines[6:]:
			if line.strip() == '':
				outfile.write("\n")
			else:
				avg = (int(line.split()[2]) + int(line.split()[5]) + int(line.split()[8]) + int(line.split()[11]) + int(line.split()[14]))/5
				print(avg)
				outfile.write("{} {} {}\n".format(line.split()[0], line.split()[1], avg))

outfile.close()
			
			
