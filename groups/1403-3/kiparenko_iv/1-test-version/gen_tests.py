import subprocess as cmd

for i in range(18):
  cmd.call("bin/generator.exe {} tests/test_{:02}".format(2 ** i, i))
  
cmd.call("bin/generator.exe 0 tests/test_empty")