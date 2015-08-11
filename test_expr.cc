#include "diff.h"
#include "expr.h"
#include <cstdio>

int main()
{
  typedef diff::ValExpr<diff::AD<1> > Val;
  Val x;
  Val y;
  x.val() = 123.0;
  y.val() = 42.0;
  x.dx(0) = 1.0;
  Val f = x * y;
  printf("the value of (%f * %f) is %f\n", x.val(), y.val(), f.val());
  printf("the derivative of (x * y) with respect to x\n"
         "  at x = %f and y = %f is %f\n", x.val(), y.val(), f.dx(0));
}
