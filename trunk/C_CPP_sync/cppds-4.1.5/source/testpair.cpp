#include <assert.h>
#include "pair.h"

void main()
{
  Pair<int, int> p1(12, 24);
  Pair<int, int> p2(12, 24);
  Pair<int, int> p3(2, 26);
  Pair<int, int> p4(12, 20);

  cout <<p1;
  cout <<p3;
  assert(p1 == p2);
  assert(p3 < p1);
  assert(p4 < p1);
  assert(p1 > p3);
  assert(p1 > p4);
}
