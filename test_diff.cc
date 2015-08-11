#include "diff.h"
#include <cstdio>

typedef diff::AD<3> ScalarT;

void print(ScalarT x)
{
  printf("%f [ ", x.val());
  for (int i=0; i < x.size(); ++i)
    printf(" %f ", x.dx(i));
  printf("]\n");
}

void compare(ScalarT x, const char* name, const char* val)
{
  printf("%s should be : %s\n", name, val);
  printf("%s is        : ", name);
  print(x);
}

ScalarT f1(ScalarT a, ScalarT b, ScalarT c)
{
  return a + b + c;
}

ScalarT f2(ScalarT a, ScalarT b, ScalarT c)
{
  return -a - b - c;
}

ScalarT f3(ScalarT b, ScalarT c)
{
  return b * exp(c);
}

ScalarT f4(ScalarT a, ScalarT c)
{
  return a/c;
}

int main()
{
  ScalarT a = 1.0;
  ScalarT b = 2.0;
  ScalarT c = 3.0;

  a.diff(0);
  b.diff(1);
  c.diff(2);

  ScalarT f = f1(a,b,c);
  compare(f, "f", "6.000000 [  1.000000  1.000000  1.000000 ]");

  ScalarT g = f2(a,b,c);
  compare(g, "g", "-6.000000 [  -1.000000  -1.000000  -1.000000 ]");

  ScalarT h = f3(b,c);
  compare(h, "h", "40.171074 [  0.000000  20.085537  40.171074 ]");

  ScalarT i = f4(a,c);
  compare(i, "i", "0.333333 [  0.333333  0.000000  -0.111111 ]");

  ScalarT j = 2.0 * (a + b - c) / 4.0;
  compare(j, "j", "0.000000 [  0.500000  0.500000  -0.500000 ]");

  ScalarT k = (f + h) * i * exp(j*g) / i;
  compare(k, "k", "you should check this with wolfram or something...");
}
