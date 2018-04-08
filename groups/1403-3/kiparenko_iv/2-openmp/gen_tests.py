import subprocess as cmd
import sys

args = sys.argv[1:]
args.extend([-1, 25])

l, r = map(int, args[:2])

if l == -1:
  cmd.call("bin/generator.exe 0 tests/test_empty")
  l = 0
if r == -1:
  r = l + 1
if r > 26:
  print("Test is too big!")
  r = 26;

for i in range(l, r):
  cmd.call("bin/generator.exe {} tests/test_{:02}".format(2 ** i, i))
