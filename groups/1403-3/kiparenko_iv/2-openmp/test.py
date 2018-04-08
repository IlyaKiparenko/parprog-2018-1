import os
import shutil
import subprocess as cmd
import sys

num_threads = 1
if len(sys.argv) > 1:
  num_threads = int(sys.argv[1])

tests = sorted(set(test.split(".")[0] for test in os.listdir("tests")))

for test in tests:
  cmd.call("bin/solver.exe tests/{} {}".format(test, num_threads))
  cmd.call("bin/checker.exe tests/{}".format(test))
  shutil.move("result.txt", "results/{}.result".format(test))