import sys
import glob
import csv
import numpy
import os

input_filename = sys.argv[1]
mem_output_filename = input_filename.split(".")[0] + "-mem." + input_filename.split(".")[1]
timing_output_filename = input_filename.split(".")[0] + "-timing." + input_filename.split(".")[1]
os.system("rm -f " + mem_output_filename)
os.system("rm -f " + timing_output_filename)

results = {}
with open(mem_output_filename,'wb') as mem_output_file:
  mem_writer = csv.writer(mem_output_file)
  with open(timing_output_filename,'wb') as timing_output_file:
    timing_writer = csv.writer(timing_output_file)
    with open(input_filename,'rb') as input_file:
      reader = csv.reader(input_file)
      for row in reader:
        if len(row) > 6: # memory info included
          mem_writer.writerow(row)
        else:
          timing_writer.writerow(row)

os.system("rm -f " + input_filename)
os.system("mv " + timing_output_filename + " " + input_filename)

print "done"
