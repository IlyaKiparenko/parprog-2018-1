import os
import shutil
import subprocess as cmd

tests = sorted(set(test.split(".")[0] for test in os.listdir("tests")))

for test in tests:
  cmd.call("bin/solver.exe tests/{}".format(test))
  cmd.call("bin/checker.exe tests/{}".format(test))
  shutil.move("result.txt", "results/{}.result".format(test))