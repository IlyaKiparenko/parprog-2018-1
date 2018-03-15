#include <string>
#include <cstdio>
#include <iostream>
#include <chrono>

using namespace std;

using ulong = unsigned long;

void my_sort(ulong* a, ulong size);

int main(int argc, char** argv) {
  const int lsize = sizeof (ulong);

  if (argc < 2) {
    cout << "Wrong args\n";
    return 1;
  }
  string name(argv[1]);

  FILE* test_is = fopen((name + ".test").c_str(), "rb");
  FILE* solution_os = fopen((name + ".solution").c_str(), "wb");

  ulong n;
  fread(&n, lsize, 1, test_is);

  ulong v[n];

  fread(v, lsize, n, test_is);

  auto start = chrono::high_resolution_clock::now();
  my_sort(v, n);
  auto end  = chrono::high_resolution_clock::now();

  double time = chrono::duration_cast<chrono::duration<double>>(end - start).count();

  fwrite(&time, sizeof (time), 1, solution_os);
  fwrite(&n, lsize, 1, solution_os);
  fwrite(v, lsize, n, solution_os);

  return 0;
}