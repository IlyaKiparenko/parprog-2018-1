#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

// Используется для взаимодействия с тестирующей системой
////////////////////////////////////////////////////////////////////////////////////////////
/*
// Checker может устанавливать вот эти три вердикта:
AC = Accepted = Решение выдаёт корректный результат на данном тесте
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
PE = Presentation Error = Ошибка формата выходных данных
// Остальные вердикты checker не может устанавливать
NO = No verdict = Вердикт отсутствует
CE = Compilation Error = Ошибка компиляции
ML = Memory Limit Exceeded = Превышено ограничение по памяти
TL = Time Limit Exceeded = Превышено ограничение по времени работы
RE = Runtime Error = Ошибка времени исполнения программы
IL = Idle Limit Exceeded = Превышено время простоя (бездействия) программы
DE = Deadly Error = Ошибка тестирующей системы
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result {
 private:
  FILE * bur;
 public:
  enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
  result (bool read = false) {
    if (read) bur = fopen("result.txt", "r");
    else bur = fopen("result.txt", "w");
  }
  ~result() {
    fclose (bur);
  }
  void write_type(ext_cls t) {
    fwrite(&t, sizeof (t), 1, bur);
  }
// Сообщить тестирующей системе, что решение получило один из вердиктов verdict
  void write_verdict(verdict v) {
    write_type(ext_cls::VERDICT);
    fwrite(&v, sizeof (v), 1, bur);
  }
// Написать сообщение от checker'a пользователю.
// Например, что решение верное, или неверное.
// Использовать только латинские буквы и знаки препинания
  void write_message(string str) {
    write_type(ext_cls::MESSAGE);
    int l = str.size ();
    fwrite(&l, sizeof (l), 1, bur);
    fwrite (&str[0], sizeof (str[0]), l, bur);
  }
// Сообщить тестирующей системе время работы программы участника,
// вычисленное с помощью before_code
// x имеет размерность 100 нс = 10 ^ (-7) сек
  void write_time(long long x) {
    write_type(ext_cls::TIME);
    fwrite(&x, sizeof (x), 1, bur);
  }
// Сообщить тестирующей системе, память затребованную программой участника
  void write_memory(unsigned long long x) {
    write_type(ext_cls::MEMORY);
    fwrite(&x, sizeof (x), 1, bur);
  }
} checker_result;
////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

int main(int argc, char** argv) {
  using T = double;
  const int tsize = sizeof (T);
  const int isize = sizeof (size_t);
  if (argc < 2) {
    cout << "Wrong args\n";
    return 1;
  }
  string name(argv[1]);
  double res_time;

  FILE* solution_is = fopen((name + ".solution").c_str(), "rb");
  FILE* answer_is = fopen((name + ".answer").c_str(), "rb");
  
  if (answer_is == 0 || solution_is == 0) {
    cout << "Wrong files\n";
    return 1;
  }

  size_t n1, n2;
  fread(&res_time, sizeof (res_time), 1, solution_is);
  fread(&n1, isize, 1, solution_is);
  fread(&n2, isize, 1, answer_is);

  bool result = (n1 == n2);

  if (result) {
    size_t n = n1;
    vector<T> v1(n), v2(n);

    fread(v1.data(), tsize, n, solution_is);
    fread(v2.data(), tsize, n, answer_is);
    
    fclose(solution_is);
    fclose(answer_is);

    for (size_t i = 0; i < n; i++)
      if (v1[i] != v2[i]) {
        result = false;
        break;
      }
  }

  if (result) {
    cout << "Test [" << name << "] OK\n";
    checker_result.write_message ("AC. Output is correct.");
    checker_result.write_verdict (verdict::AC);
  } else {
    cout << "Test [" << name << "] FAILED\n";
    checker_result.write_message ("WA. Output is incorrect.");
    checker_result.write_verdict (verdict::WA);
  }

  checker_result.write_time (res_time * 1e7);

  return 0;
}