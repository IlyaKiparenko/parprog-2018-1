#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  using T = double;
  const int tsize = sizeof (T);
  const int isize = sizeof (size_t);
  if (argc < 3) {
    cout << "Wrong args\n";
    return 1;
  }

  default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
  uniform_real_distribution <T> distribution(0, 1e8);

  size_t n = stoul(argv[1]);
  string name(argv[2]);

  FILE* test_os = fopen((name + ".test").c_str(), "wb");
  FILE* answer_os = fopen((name + ".answer").c_str(), "wb");
  
  if (test_os == 0 || answer_os == 0) {
    cout << "Wrong files\n";
    return 1;
  }
  
  fwrite(&n, isize, 1, test_os);
  fwrite(&n, isize, 1, answer_os);
  
  vector<T> v(n);

  for (size_t i = 0; i < n; i++)
    v[i] = distribution(generator);

  fwrite(v.data(), tsize, n, test_os);

  sort(v.begin(), v.end());

  fwrite(v.data(), tsize, n, answer_os);
  
  fclose(test_os);
  fclose(answer_os);

  return 0;
}