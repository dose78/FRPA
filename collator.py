import sys
import glob
import csv
import numpy
import os

data_name = sys.argv[1]
output_name = data_name.split(".")[0] + "-out." + data_name.split(".")[1]
vals = []

def makeline(key, vals):
  vals = map(float, vals)
  line = key + ","
  line += str(round(max(vals),3)) + ","
  line += str(round(numpy.average(vals),3)) + ","
  line += str(round(numpy.median(vals),3)) + ","
  line += str(round(min(vals),3)) + ","
  line += str(round(numpy.std(vals),3)) + ","
  line += str(len(vals)) + "\n"
  return line

with open(output_name,'wb') as output:
  output.write("algorithm,m,k,n,interleaving,max,avg,median,min,stddev,numtrials\n")
  with open(data_name,'rb') as data:
    reader = csv.reader(data)
    first_line = reader.next()
    prevkey = ",".join(first_line[0:-1])
    vals.append(first_line[-1])
    for row in reader:
      key = ",".join(row[0:-1])
      val = row[-1]
      if prevkey == key:
        vals.append(val)
      else:
        output.write(makeline(prevkey, vals))
        vals = [val]
        prevkey = key

  output.write(makeline(key, vals))

print "done"
