#ifndef PAIR_H
#define PAIR_H
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif

/****************************************************************************/
template <class T1, class T2>
struct Pair
{
  friend ostream& operator << (ostream& os, const Pair<T1, T2>& obj)
      { os <<obj._First <<"\t" <<obj._Second <<endl;
        return os; }
  friend istream& operator >> (istream& is, Pair<T1, T2>& obj)
      { is >>obj._First >>obj._Second;
        return is; }

  T1 _First;
  T2 _Second;

  Pair(const T1& x, const T2& y):
    _First(x), _Second(y) {}
};

/****************************************************************************/
template <class T1, class T2>
Boolean operator == (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First == y._First) &&
    (x._Second == y._Second)
  );
}

/****************************************************************************/
template <class T1, class T2>
Boolean operator < (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First < y._First) ||
    (!(y._First < x._First) && (x._Second < y._Second))
  );
}

/****************************************************************************/
template <class T1, class T2>
Boolean operator > (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First > y._First) ||
    (!(y._First > x._First) && (x._Second > y._Second))
  );
}

/****************************************************************************/
template <class T1, class T2>
Boolean operator <= (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First <= y._First) ||
    (!(y._First <= x._First) && (x._Second <= y._Second))
  );
}

/****************************************************************************/
template <class T1, class T2>
Boolean operator >= (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First >= y._First) ||
    (!(y._First >= x._First) && (x._Second >= y._Second))
  );
}

/****************************************************************************/
template <class T1, class T2>
Boolean operator != (const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return
  (
    (x._First != y._First) ||
    (x._Second != y._Second)
  );
}

/****************************************************************************/
#endif





