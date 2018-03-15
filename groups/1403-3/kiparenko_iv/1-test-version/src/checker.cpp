#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
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

using ulong = unsigned long;
int main(int argc, char** argv) {
  const int lsize = sizeof (ulong);
  if (argc < 2) {
    cout << "Wrong args\n";
    return 1;
  }
  string name(argv[1]);
  double res_time;

  FILE* solution_is = fopen((name + ".solution").c_str(), "rb");
  FILE* answer_is = fopen((name + ".answer").c_str(), "rb");

  ulong n1, n2;
  fread(&res_time, sizeof (res_time), 1, solution_is);
  fread(&n1, lsize, 1, solution_is);
  fread(&n2, lsize, 1, answer_is);

  bool result = (n1 == n2);

  if (result) {
    ulong n = n1;
    ulong data1[n], data2[n];

    fread(data1, lsize, n, solution_is);
    fread(data2, lsize, n, answer_is);

    for (ulong i = 0; i < n; i++)
      if (data1[i] != data2[i]) {
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