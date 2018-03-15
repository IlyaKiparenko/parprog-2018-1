#include <utility>

using namespace std;

using ulong = unsigned long;

template<typename T>
void compare(T&& a, T&& b) {
  if (a > b)
    swap(a, b);
}

void unshuffle(ulong* a, ulong l, ulong r) {
  long half = (r - l) / 2;
  ulong b[r - l];
  for (ulong i = 0; i < half; i++) {
    b[i] = a[l + 2*i];
    b[i + half] = a[l + 2*i + 1];
  }

  for (ulong i = l; i < r; i++) {
    a[i] = b[i - l];
  }
}

void shuffle(ulong* a, ulong l, ulong r) {
  long half = (r - l) / 2;
  ulong b[r - l];
  for (ulong i = 0; i < half; i++) {
    b[2*i] = a[l + i];
    b[2*i + 1] = a[l + i + half];
  }
  for (ulong i = l; i < r; i++) {
    a[i] = b[i - l];
  }
}

static void my_merge(ulong* a, ulong l, ulong r);

void my_sort(ulong* a, ulong l, ulong r) {
  if ((r - l) > 1) {
    ulong m = l + (r - l) / 2;
    my_sort(a, l, m);
    my_sort(a, m, r);
    my_merge(a, l, r);
  }
}

static void my_merge(ulong* a, ulong l, ulong r) {
  ulong m = l + (r - l) / 2;
  if ((r - l) > 2) {
    unshuffle(a, l, r);
    my_merge(a, l, m);
    my_merge(a, m, r);
    shuffle(a, l, r);

    for (ulong i = l + 1; i < r - 1; i+=2)
      compare(a[i], a[i + 1]);
  } else {
    compare(a[l], a[l+1]);
  }
}

void my_sort(ulong* a, ulong size) {
  my_sort(a, 0, size);
}