#include <iostream>
#include <vector>
#include <algorithm>
#include "omp.h"

using namespace std;

struct comparator {
  int a1;
  int a2;
  omp_lock_t left, right, sync;
  comparator(int a, int b) {
    a1 = a;
    a2 = b;
  }
};

void M(vector<comparator>& a, int l, int r, int rs, int real_size) {
  int step = rs*2;
  if ((r - l) > step) {
    M(a, l, r, step, real_size);
    M(a, l + rs, r, step, real_size);
    for (int i = l + rs; i + rs < min(r, real_size); i += step)
      a.push_back({i, i + rs});
  } else {
    if (l + rs < real_size)
      a.push_back({l, l + rs});
  }
}

void S(vector<comparator>& a, int l, int r, int real_size) {
  if ((r - l) > 1) {
    int m = l + (r - l) / 2;
    S(a, l, m, real_size);
    S(a, m, r, real_size);
    M(a, l, r, 1, real_size);
  }
}
vector<comparator> gen_comparators(int real_size) {
  vector<comparator> ans;
  int ln = 0;
  int temp = real_size;
  while (temp > 1) {
    temp /= 2;
    ln++;
  }

  int size = (real_size == (1 << ln)) ? real_size : (2 << ln);
  ans.reserve(ln * (ln - 1) * (size / 4) + size - 1);
  S(ans, 0, size, real_size);
  return ans;
}

template<typename T>
void sort(vector<T>& a, int num_threads) {
  if (num_threads == 1 || a.size() < 2 * num_threads) {
    sort(a.begin(), a.end());
    return;
  }
  
  size_t div = a.size() / num_threads;
  size_t rem = a.size() % num_threads;

  if (rem > 0) {
    div++;
    rem = num_threads - rem;
  }

  T* data = a.data();
  vector<comparator> comps = gen_comparators(num_threads);

  #pragma omp parallel num_threads(num_threads) shared(data, num_threads, comps)
  {
    size_t id = omp_get_thread_num();

    for (auto& comp : comps) {
      if (id == comp.a1) {
        omp_init_lock(&comp.left);
        omp_init_lock(&comp.sync);
        omp_set_lock(&comp.left);
        omp_set_lock(&comp.sync);
      } else if (id == comp.a2) {
        omp_init_lock(&comp.right);
        omp_set_lock(&comp.right);
      }
    }

    #pragma omp barrier

    size_t l = id * div;
    size_t r = (id + 1) * div;
    if (id == (num_threads - 1))
      r -= rem;
    
    vector<T> local_mem2(r - l);
    sort(data + l, data + r);

    size_t len = 0;
    for (auto& comp : comps) {
      size_t left  = comp.a1;
      size_t right = comp.a2;
      if (id == left) {
        omp_unset_lock(&comp.left);
        omp_set_lock(&comp.right);
        omp_unset_lock(&comp.sync);

        len = (right == (num_threads - 1)) ? (div - rem) : div;

          size_t res_index = l, cur_index = div*right, tmp_index = 0;
          for (; (tmp_index < r - l) && (cur_index < (len + div*right)); tmp_index++) {
            T result = data[res_index];
            T current = data[cur_index];
            if (result < current) {
              local_mem2[tmp_index] = result;
              res_index++;
            } else {
              local_mem2[tmp_index] = current;
              cur_index++;
            }
          }

          for (; tmp_index < r - l; tmp_index++, res_index++)
            local_mem2[tmp_index] = data[res_index];

          omp_set_lock(&comp.left);
          omp_unset_lock(&comp.right);
          
          for (size_t i = 0; i < r - l; i++)
            data[l + i] = local_mem2[i];
        
      } else if (id == right) {
        omp_set_lock(&comp.left);
        omp_unset_lock(&comp.right);
        omp_set_lock(&comp.sync);

        len = div;

        size_t res_index = r - 1, cur_index = div*left + len, tmp_index = r - l;
        for (; (tmp_index >= 1); tmp_index--) {
          T result = data[res_index];
          T current = data[cur_index - 1];
          if (result > current) {
            local_mem2[tmp_index-1] = result;
            res_index--;
          } else {
            local_mem2[tmp_index-1] = current;
            cur_index--;
          }
        }

        omp_unset_lock(&comp.left);
        omp_set_lock(&comp.right);
        
        for (size_t i = 0; i < r - l; i++)
          data[l + i] = local_mem2[i];
      }
    }
  }

  for (auto& c: comps) {
    omp_destroy_lock(&c.left);
    omp_destroy_lock(&c.right);
    omp_destroy_lock(&c.sync);
  }
}

void my_sort(vector<double>& a, int num_threads) {
  sort(a, num_threads);
}
