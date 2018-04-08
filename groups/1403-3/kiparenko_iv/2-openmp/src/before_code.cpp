#include <string>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <utility>
#include <vector>

using namespace std;

void my_sort(vector<double>& a, int num_threads);

int main(int argc, char** argv) {
  using T = double;
  const int tsize = sizeof (T);
  const int isize = sizeof (size_t);

  if (argc < 3) {
    cout << "Wrong args\n";
    return 1;
  }

  string name(argv[1]);
  int num_threads = stoi(argv[2]);

  FILE* test_is = fopen((name + ".test").c_str(), "rb");
  FILE* solution_os = fopen((name + ".solution").c_str(), "wb");
  
  if (test_is == 0 || solution_os == 0) {
    cout << "Wrong files\n";
    return 1;
  }
  
  size_t n;
  
  fread(&n, isize, 1, test_is);
  
  vector<T> v(n);
  
  fread(v.data(), tsize, n, test_is);
  fclose(test_is);

  auto start = chrono::high_resolution_clock::now();
  my_sort(v, num_threads);
  auto end  = chrono::high_resolution_clock::now();

  double time = chrono::duration_cast<chrono::duration<double>>(end - start).count();

  fwrite(&time, sizeof (time), 1, solution_os);
  fwrite(&n, isize, 1, solution_os);
  fwrite(v.data(), tsize, n, solution_os);

  fclose(solution_os);

  return 0;
}