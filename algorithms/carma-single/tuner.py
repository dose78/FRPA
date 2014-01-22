#!/usr/bin/env python

# run with otpython tune.py --test-limit 10 --max_depth 8
# program must be complied into ./harness

import argparse
import opentuner
from opentuner.search.manipulator import *
from opentuner.measurement import MeasurementInterface

parser = argparse.ArgumentParser(parents=opentuner.argparsers())
parser.add_argument('--max_depth', type=int, default=10, help='max depth of decision tree')
parser.add_argument('--m', type=int, default=1024, help='m dimension of matrix')
parser.add_argument('--k', type=int, default=1024, help='k dimension of matrix')
parser.add_argument('--n', type=int, default=1024, help='n dimension of matrix')
parser.add_argument('--out_file', type=str, default='opentuner.csv', help='output file')

class FRPATuner(MeasurementInterface):
    def __init__(self, args):
        super(FRPATuner, self).__init__(args)
        self.MAX_DEPTH = args.max_depth
        self.m = args.m
        self.k = args.k
        self.n = args.n
        self.outfile = args.out_file
        print "size: " + str(self.m) + "," + str(self.k) + "," + str(self.n)

    def run(self, desired_result, input, limit):
        interleaving = ""
        cfg = desired_result.configuration.data
        for i in range(0, cfg['depth']):
            if cfg['schedule'][i]:
                interleaving += 'B'
            else:
                interleaving += 'D'

        run_command = " ".join(['./harness', str(self.m), str(self.k), str(self.n), interleaving])
        run_command = run_command + "; " + run_command + "; " + run_command
        result = self.call_program(run_command)
        stdout = result['stdout']
        lines = stdout.split('\n')
        gflops = 0
        for line in lines:
            if len(line) > 1:
                gflops += float(line.split(',')[-1])
        gflops = gflops/3
        print interleaving + ": " + str(gflops) + " gflops"
        with open(self.outfile, 'a') as file:
            file.write(str(gflops) + ",")
        return opentuner.resultsdb.models.Result(time=(-1*gflops))

    def manipulator(self):
        manipulator = ConfigurationManipulator()

        params = [
            IntegerParameter("depth", 1, self.MAX_DEPTH),
            BooleanArrayParameter("schedule", self.MAX_DEPTH),
        ]

        for param in params:
            manipulator.add_parameter(param)

        return manipulator

if __name__ == '__main__':
    args = parser.parse_args()
    FRPATuner.main(args)
