#ifndef SIMPLEARRAY_CPP
#define SIMPLEARRAY_CPP
#ifndef SIMPLEARRAY_H
  #include "simplearray.h"
#endif

/****************************************************************************/
template <class T>    
Boolean operator == (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (Obj1_.IsEqual(Obj2_));
}

/****************************************************************************/
template <class T>    
Boolean operator != (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (!Obj1_.IsEqual(Obj2_));
}

/****************************************************************************/
template <class T>    
Boolean operator > (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (Obj1_.IsGreater(Obj2_));
}

/****************************************************************************/
template <class T>    
Boolean operator < (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (Obj1_.IsLesser(Obj2_));
}

/****************************************************************************/
template <class T>    
Boolean operator >= (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (Obj1_.IsEqual(Obj2_) || Obj1_.IsGreater(Obj2_));
}

/****************************************************************************/
template <class T>    
Boolean operator <= (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_)
{
  return (Obj1_.IsEqual(Obj2_) || Obj1_.IsLesser(Obj2_));
}

/****************************************************************************/
template <class T>    
SimpleArray<T> Apply(const SimpleArray<T>& ia, T(*func)(T))
{
  size_t x;
  SimpleArray<T> Result_(ia);
  
  for (x = 0; x < ia._Size; x++)
    Result_._Buffer[x] = (*func)(Result_._Buffer[x]);

  return Result_;
}

/****************************************************************************/
template <class T>
SimpleArray<Boolean> SeriesArrayCompare(const SimpleArray<T>& ia1, const SimpleArray<T>& ia2)
{
  if (_CompFunc && (ia1._Size == ia2._Size))
  {
    SimpleArray<Boolean> Result_(ia1._Size);
    size_t x;
    
    for (x = 0; x < _Size; x++)
      Result_[x] = (*_CompFunc)(_Buffer[x], ia2[x]);

    return Result_;
  }

  SimpleArray<Boolean> Temp_;
  Temp_.SetError();
  return Temp_;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray():
_Error(FALSE),
_Size(0),
_Buffer(NULL)
{}

/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray(size_t n):
_Error(FALSE),
_Size(n),
_Buffer(New<T>(n))
{}

/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray(const T* a, size_t n):
_Error(FALSE),
_Size(n),
_Buffer(New<T>(n))
{
  size_t x;
  for (x = 0; x < n; x++)
    _Buffer[x] = a[x];
}

/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray(const SimpleArray<T>& Arr_, size_t start_, size_t end_):
_Error(FALSE),
_Size((end_ > start_) ? (end_ - start_ + 1):Arr_._Size),
_Buffer(New<T>((end_ > start_) ? (end_ - start_ + 1):Arr_._Size))
{
  if (end_ > start_ && end_ < Arr_._Size)
  {
    size_t x, y;
    for (x = 0, y = start_; x <= end_ - start_; x++, y++)
      _Buffer[x] = Arr_._Buffer[y];
  }
}

/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray(const SimpleArray<T>& Arr_):
_Error(FALSE),
_Size(Arr_._Size),
_Buffer(New<T>(Arr_._Size))
{
  size_t x;
  for (x = 0; x < Arr_._Size; x++)
    _Buffer[x] = Arr_._Buffer[x];
}
    
/****************************************************************************/
template <class T>
SimpleArray<T>::SimpleArray(const SimpleArray<T> & a1, const SimpleArray<T> & a2):
_Error(FALSE),
_Size(a1._Size + a2._Size),
_Buffer(New<T>(a1._Size + a2._Size))
{
  size_t x, y;
  x = 0;
  
  for (y = 0; y < a1._Size; x++, y++)
    _Buffer[x] = a1._Buffer[y];

  for (y = 0; y < a2._Size; x++, y++)
    _Buffer[x] = a1._Buffer[y];    
}
    
/****************************************************************************/
template <class T>
SimpleArray<T>::~SimpleArray()
{
  ::Delete(_Buffer);
  _Buffer = NULL;
}

/****************************************************************************/
template <class T>
void SimpleArray<T>::SetComparisonFunction(int(*CompFunc_)(const T&, const T&))
{
  SimpleArray<T>::_CompFunc = CompFunc_;
}

/****************************************************************************/
template <class T>
SimpleArray<T>& SimpleArray<T>::operator = (const SimpleArray<T>& a)
{
  if (this != &a)
  {
    ::Delete(_Buffer);

    _Error = Arr_._Error;
    _Size = Arr_._Size;
    _Buffer = New<T>(Arr_._Size);

    size_t x;
    for (x = 0; x < n; x++)
      _Buffer[x] = Arr_._Buffer[x];    
  }

  return *this;
}

/****************************************************************************/
template <class T>
SimpleArray<T>::operator const T* () const
{
  return _Buffer;
}

/****************************************************************************/
template <class T>
size_t SimpleArray<T>::GetCount() const
{
  return _Size;
}

/****************************************************************************/
template <class T>
T& SimpleArray<T>::operator [] (const Index & i)
{
  static T Dummy_;

  if (0 <= i && i < _Size)
    return _Buffer[i];

  return Dummy_;
}

/****************************************************************************/
template <class T>
T& SimpleArray<T>::operator [] (int i)
{
  static T Dummy_;

  if (0 <= i && i < _Size)
    return _Buffer[i];

  return Dummy_;
}

/****************************************************************************/
template <class T>    
const T& SimpleArray<T>::operator [] (const Index & i) const
{
  static T Dummy_;

  if (0 <= i && i < _Size)
    return _Buffer[i];

  return Dummy_;
}

/****************************************************************************/
template <class T>    
const T& SimpleArray<T>::operator [] (int i) const
{
  static T Dummy_;

  if (0 <= i && i < _Size)
    return _Buffer[i];

  return Dummy_;  
}

/****************************************************************************/
template <class T>
const T SimpleArray<T>::Read(const Index & i) const
{
  static T Dummy_;
  size_t Num_ = size_t(i);

  if (0 <= Num_ && Num_ < _Size)
    return _Buffer[Num_];

  return Dummy_;
}

/****************************************************************************/
template <class T>    
const T SimpleArray<T>::Read(int i) const
{
  static T Dummy_;

  if (0 <= i && i < _Size)
    return _Buffer[i];

  return Dummy_;
}

/****************************************************************************/
template <class T>
Boolean SimpleArray<T>::IsEqual(const SimpleArray<T>& ia) const
{
  if (_CompFunc)
  {
    size_t x;
    for (x = 0; x < _Size; x++)
      if ((*_CompFunc)(_Buffer[x], ia[x]) != 0)
        return FALSE;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SimpleArray<T>::IsLesser(const SimpleArray<T>& ia) const
{
  if (_CompFunc)
  {
    size_t x;
    for (x = 0; x < _Size; x++)
      if ((*_CompFunc)(_Buffer[x], ia[x]) >= 0)
        return FALSE;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SimpleArray<T>::IsGreater(const SimpleArray<T>& ia) const
{
  if (_CompFunc)
  {
    size_t x;
    for (x = 0; x < _Size; x++)
      if ((*_CompFunc)(_Buffer[x], ia[x]) <= 0)
        return FALSE;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
SimpleArray<Boolean> SeriesArrayCompare(const SimpleArray<T>& ia)
{
  if (_CompFunc && (_Size == ia._Size))
  {
    SimpleArray<Boolean> Result_(_Size);
    size_t x;
    
    for (x = 0; x < _Size; x++)
      Result_[x] = (*_CompFunc)(_Buffer[x], ia[x]);

    return Result_;
  }

  SimpleArray<Boolean> Temp_;
  Temp_.SetError();
  return Temp_;
}

/****************************************************************************/
template <class T>    
SimpleArray<T>& SimpleArray<T>::Apply(T(*func)(T))
{
  size_t x;
  for (x = 0; x < _Size; x++)
    _Buffer[x] = (*func)(_Buffer[x]);

  return *this;
}

/****************************************************************************/
template <class T>
void SimpleArray<T>::Fill(T i)
{
  size_t x;
  for (x = 0; x < _Size; x++)
    _Buffer[x] = i;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleArrayIter<T>::SimpleArrayIter(SimpleArray<T>& a):
_Target(a),
_ElemFirst(a.Base()),
_ElemLast(a.Base() + a.GetCount() - 1),
_ElemPtr(a.Base())
{}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>::SimpleArrayIter(const SimpleArrayIter<T>& aptr):
_Target(aptr._Target),
_ElemFirst(aptr._ElemFirst),
_ElemLast(aptr._ElemLast),
_ElemPtr(aptr._ElemPtr)
{}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>& SimpleArrayIter<T>::operator = (const SimpleArrayIter<T> & aptr)
{
  if (this != &aptr)
  {
    _Target = aptr._Target;
    _ElemFirst = aptr._ElemFirst;
    _ElemLast = aptr._ElemLast;
    _ElemPtr = aptr._ElemPtr;  
  }

  return *this;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>& SimpleArrayIter<T>::operator = (const Index& i)
{
  size_t Num_ = size_t(i);

  if (0 <= Num_ && Num_ < _Target.RunLength())
    _ElemPtr = _ElemFirst + Num_;

  return *this;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>& SimpleArrayIter<T>::operator = (size_t i)
{
  if (0 <= i && i < _Target.RunLength())
    _ElemPtr = _ElemFirst + Num_;

  return *this;
}

/****************************************************************************/
template <class T>
T& SimpleArrayIter<T>::operator * ()
{
  return *_ElemPtr;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>& SimpleArrayIter<T>::operator ++ ()
{
  if (_ElemPtr != _ElemLast)
    _ElemPtr++;

  return *this;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T> SimpleArrayIter<T>::operator ++ (int dummy)
{
  SimpleArrayIter result(*this);

  if (_ElemPtr != _ElemLast)
    _ElemPtr++;

  return result;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T>& SimpleArrayIter<T>::operator -- ()
{
  if (_ElemPtr != _ElemFirst)
    _ElemPtr--;

  return *this;
}

/****************************************************************************/
template <class T>
SimpleArrayIter<T> SimpleArrayIter<T>::operator -- (int dummy)
{
  SimpleArrayIter result(*this);

  if (_ElemPtr != _ElemFirst)
    _ElemPtr--;

  return result;
}

/****************************************************************************/
template <class T>
void SimpleArrayIter<T>::SetFirst()
{
  _ElemPtr = _ElemFirst;
}

/****************************************************************************/
template <class T>
void SimpleArrayIter<T>::SetLast()
{
  _ElemPtr = _ElemLast;
}

/****************************************************************************/
template <class T>
int SimpleArrayIter<T>::IsFirst()
{
  return (_ElemPtr == _ElemFirst);
}

/****************************************************************************/
template <class T>    
int SimpleArrayIter<T>::IsLast()
{
  return (_ElemPtr == _ElemLast);
}

/****************************************************************************/
template <class T>    
int SimpleArrayIter<T>::operator == (const SimpleArrayIter<T>& aptr)
{
  return (ElemPtr == aptr.ElemPtr);
}

/****************************************************************************/
template <class T>    
int SimpleArrayIter<T>::operator != (const SimpleArrayIter<T>& aptr)
{
  return (ElemPtr != aptr.ElemPtr);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleArrayIter<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleArrayIter<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleArrayIter<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleArrayIter<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if SIMPLEARRAY_DEBUG
// template function to display array
template <class T>
ostream & operator << (ostream & buffer, SimpleArray<T> & a)
{
  buffer << "[" <<0 << "," <<a.GetCount() << "] ";

  for (int i = 0; i < a.GetCount(); ++i)
    buffer << a[i] << " ";

  return buffer;
}

void ArrayTest1(ostream & buffer)
{
    // banner
    buffer << "\r\nArrays: Basic Tests"
              "\r\n-------------------\r\n";

    // locals
    SimpleArray<int> ia(21);
    int n;

    // fill array with count
    for (n = 1; n <= 20; ++n)
	ia[n] = n;

    // display array
    buffer <<"         Array A = " << ia << "\r\n";

    // duplicate array
    SimpleArray<int> ib(ia);
    buffer <<"<copy>   Array B = " << ib << "\r\n";

    // create a subset
    SimpleArray<int> ic(ia,3,16);
    buffer <<"<subset> Array C = " << ic << "\r\n";
}

/****************************************************************************/
void ArrayTest2(ostream & buffer)
{
    // banner
    buffer << "\r\nArrays: Sorting"
              "\r\n---------------\r\n";

    // constants
    int MaxVal = 1701;

    // locals
    int n;
    SimpleArray<int> a(102);

    // fill array with random values and display it
    for (n = 0; n <= 101; ++n)
	a[n]  = RANDOM(MaxVal);

    // duplicate array for second sort
    SimpleArray<int> a2(a);

    // now sort with new manipulator
    buffer << "With QuickSort:\r\n";
    ItemSorter<int, int> Sorter_;
    Sorter_.SetSortHelper(ObjectSortHelper<int, int>::Make());

    buffer << "   Unsorted Array = " << a << "\r\n";
    Sorter_.QuickSort(a.Base(), 0, 101);
    buffer << "     Sorted Array = " << a << "\r\n";
    Sorter_.SetOrder(SortOrder::DECREASING);
    Sorter_.QuickSort(a.Base(), 0, 101);
    buffer << "     Reverse Sort = " << a << "\r\n";
}

/****************************************************************************/
void ArrayTest8(ostream & buffer)
{
    buffer << "\r\nArrays: String Object Example"
              "\r\n-----------------------------\r\n";

    SimpleArray<SimpleStr> sa(10);

    sa[0] = SimpleStr("Zero");
    sa[1] = "One";
    sa[2] = "Two";
    sa[3] = "Three";
    sa[4] = "Four";
    sa[5] = "Five";
    sa[6] = "Six";
    sa[7] = "Seven";
    sa[8] = "Eight";
    sa[9] = "Nine";

    buffer << "String array test = " << sa << "\r\n";
}

/****************************************************************************/
void ArrayTest12(ostream & buffer)
{
    buffer << "\r\nArrays: Iterator Tests"
              "\r\n----------------------\r\n";

    SimpleArray<double> da(11);
    size_t i;
    
    for (i = 0; i < 11; i++)
      da[i] = i;    

    SimpleArrayIter<double> dptr(da);

    buffer << " forward: ";
    while (1)
    {
        buffer << (*dptr) << " ";
        
        if (dptr.IsLast())
            break;

        ++dptr;
    }

    buffer << "\r\nbackward: ";
    while (1)
    {
        buffer << (*dptr) << " ";
        
        if (dptr.IsFirst())
            break;

        --dptr;
    }

    buffer << "\r\n";
}

/****************************************************************************/
int main()
{
  RANDOMIZE();
  ArrayTest1(cout);
  ArrayTest2(cout);
  ArrayTest8(cout);
  ArrayTest12(cout);
  return 0;
}
#endif
/****************************************************************************/
#endif





