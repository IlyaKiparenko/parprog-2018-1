#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <cstdio>

using namespace std;

using ulong = unsigned long;
int main(int argc, char** argv) {
  const int lsize = sizeof (ulong);
  if (argc < 3) {
    cout << "Wrong args\n";
    return 1;
  }

  default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
  uniform_int_distribution <ulong> distribution(0, 1e8);

  ulong n = stoul(argv[1]);
  string name(argv[2]);

  FILE* test_os = fopen((name + ".test").c_str(), "wb");
  FILE* answer_os = fopen((name + ".answer").c_str(), "wb");
  fwrite(&n, lsize, 1, test_os);
  fwrite(&n, lsize, 1, answer_os);

  ulong data[n];

  for (ulong i = 0; i < n; i++)
    data[i] = distribution(generator);
  fwrite(data, lsize, n, test_os);

  sort(data, data + n);

  fwrite(data, lsize, n, answer_os);

  return 0;
}