import sys
import glob
import csv
import numpy
import os

input_filename = sys.argv[1]
base = input_filename.split(".")[0]
ext = "." + input_filename.split(".")[1]

with open(input_filename, 'r') as input_file:
  first_line = input_file.readline()

if first_line.startswith("algorithm"):
  print("ERROR: Already collated")
  sys.exit(0)

os.system("mkdir -p original/")
os.system("mkdir -p memory/")
original_filename = "original/" + base + "-original" + ext
mem_filename = "memory/" + base + "-mem" + ext
temp_filename = base + ".temp"
output_filename = input_filename

os.system("rm -f " + original_filename)
os.system("rm -f " + mem_filename)
os.system("rm -f " + temp_filename)

os.system("mv " + input_filename + " " + original_filename)
del(input_filename)

results = {}
with open(mem_filename,'wb') as mem_file:
  mem_writer = csv.writer(mem_file)
  with open(original_filename,'rb') as orig_file:
    reader = csv.reader(orig_file)
    for row in reader:
      if len(row) > 6: # memory info included
        mem_writer.writerow(row)
      else:
        key = ",".join(row[0:-1])
        val = row[-1]
        if results.has_key(key):
          results[key].append(val)
        else:
          results[key] = [val]

with open(temp_filename,'wb') as temp_file:
  for result in results.items():
    temp_file.write(result[0] + "," + ",".join(result[1])+"\n")

os.system("sort -t, -k 1,1 -k 5,5 -k 2,2n -k 3,3n -k 4,4n " + temp_filename + " -o " + temp_filename)
os.system("sort -t, -k 1,1 -k 5,5 -k 2,2n -k 3,3n -k 4,4n " + mem_filename + " -o " + mem_filename)

with open(output_filename,'wb') as output_file:
  output_file.write("algorithm,m,k,n,interleaving,max,avg,median,min,stddev,numtrials\n")
  writer = csv.writer(output_file)
  with open(temp_filename,'rb') as temp_file:
    reader = csv.reader(temp_file)
    for row in reader:
      key = row[:5]
      vals = row[5:]
      vals = map(float, vals)
      max_gflops = round(max(vals),4)
      avg_gflops = round(numpy.average(vals),4)
      median_gflops = round(numpy.median(vals),4)
      min_gflops = round(min(vals),4)
      std_dev = round(numpy.std(vals),4)
      num_trails = len(vals)
      writer.writerow(key + [max_gflops, avg_gflops, median_gflops, min_gflops, std_dev, num_trails])

os.system("rm -f " + temp_filename)
print "done"
