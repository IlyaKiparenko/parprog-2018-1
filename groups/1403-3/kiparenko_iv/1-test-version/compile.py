import subprocess as cmd

cmd.call("g++ -std=c++11 src/before_code.cpp src/sol.cpp -o bin/solver.exe")
cmd.call("g++ -std=c++11 src/checker.cpp -o bin/checker.exe")
cmd.call("g++ -std=c++11 src/generator.cpp -o bin/generator.exe")