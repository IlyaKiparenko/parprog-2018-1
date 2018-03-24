#include <utility>

using namespace std;

template<typename T>
void compare(T&& a, T&& b) {
  if (a > b)
    swap(a, b);
}

template<typename T>
class Sorter {
private:
  T* data;
  T* memory;
  
  void unshuffle(size_t l, size_t r) {
    size_t half = (r - l) / 2;
    for (size_t i = 0; i < half; i++) {
      memory[i] = data[l + 2*i];
      memory[i + half] = data[l + 2*i + 1];
    }

    for (size_t i = l; i < r; i++) {
      data[i] = memory[i - l];
    }
  }

  void shuffle(size_t l, size_t r) {
    size_t half = (r - l) / 2;
    for (size_t i = 0; i < half; i++) {
      memory[2*i] = data[l + i];
      memory[2*i + 1] = data[l + i + half];
    }
    for (size_t i = l; i < r; i++) {
      data[i] = memory[i - l];
    }
  }

  void my_merge(size_t l, size_t r) {
    if ((r - l) > 2) {
      size_t m = l + (r - l) / 2;
      unshuffle(l, r);
      my_merge(l, m);
      my_merge(m, r);
      shuffle(l, r);

      for (size_t i = l + 1; i < r - 1; i += 2)
        compare(data[i], data[i + 1]);
    } else {
      compare(data[l], data[l + 1]);
    }
  }

public:
  Sorter(T* a, size_t size) {
    data = a;
    memory = new T[size];
  }

  void my_sort(size_t l, size_t r) {
    if ((r - l) > 1) {
      size_t m = l + (r - l) / 2;
      my_sort(l, m);
      my_sort(m, r);
      my_merge(l, r);
    }
  }

  ~Sorter() {
    delete [] memory;
  }
};

template <typename T>
void my_sort(T* a, size_t size) {
  Sorter<T> sorter(a, size);
  sorter.my_sort(0, size);
}