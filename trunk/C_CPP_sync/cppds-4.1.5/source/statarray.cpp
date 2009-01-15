#ifndef STATARRAY_CPP
#define STATARRAY_CPP
#ifndef STATARRAY_H
  #include "statarray.h"
#endif

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator == (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();

  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;  

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left == *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator != (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left != *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator < (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left < *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator <= (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left <= *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator > (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left > *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
StatArray<Boolean> StatArray<T>::operator >= (const StatArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  StatArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left >= *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
StatArray<T>::StatArray():
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
StatArray<T>::StatArray(size_t n, capacity c):
DynArray<T>(n, c),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
StatArray<T>::StatArray(const DynArray<T>& x):
DynArray<T>(x),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
StatArray<T>::StatArray(const StatArray<T>& x):
DynArray<T>(x),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
StatArray<T>::StatArray(const T& x, size_t n):
DynArray<T>(x, n),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
StatArray<T>::StatArray(const T* s, size_t n):
DynArray<T>(s, n),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
Object& StatArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  StatArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromStatArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& StatArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  StatArrayAcceptor<T> TrgComp_(this);
  return *((Object*) ObjComp_->AssignFromStatArray(TrgComp_));
}

/****************************************************************************/
template <class T>
Object* StatArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new StatArray<T>(*this));

  StatArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromStatArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* StatArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
Object* StatArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
DynArray<T>& StatArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static StatArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& StatArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StatArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& StatArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StatArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int StatArray<T>::ClassID() const
{
  return TypeInfo::STATARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* StatArray<T>::GiveObjectAcceptor() const
{
  return (new StatArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
istream& StatArray<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& StatArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;
  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
    Os_ <<((*this)[i]) <<" ";

  return Os_;
}

/****************************************************************************/
template <class T>
T StatArray<T>::Sum(ifstream& fin, Boolean& Error_)
{
  T sum;
  T temp;

  Error_ = NO_ERROR;
  fin.seekg(0);
  fin.clear();

  fin >>temp;
  if (!fin.eof())
    sum = temp;
  else
  {
    Error_ = EMPTY_ARRAY;
    return T(0);
  }

  while (!fin.eof())
  {  
    fin >>temp;
    if (!fin.eof())
      sum += temp;
  }

  return sum;
}

/****************************************************************************/
template <class T>
T StatArray<T>::Product(ifstream& fin, Boolean& Error_)
{
  T product;
  T temp;

  Error_ = NO_ERROR;
  fin.seekg(0);
  fin.clear();

  fin >>temp;
  if (!fin.eof())
    product = temp;
  else
  {
    Error_ = EMPTY_ARRAY;
    return T(0);
  }

  while (!fin.eof())
  {  
    fin >>temp;
    if (!fin.eof())
      product *= temp;
  }

  return product;
}

/****************************************************************************/
template <class T>
T StatArray<T>::AvgMean(ifstream& fin, Boolean& Error_)
{
  const int MAXBUFFER = 10;

  T* array[10];  
  int index, max;

  for (index = 0; index < MAXBUFFER; index++)
    array[index] = new T();

  Error_ = NO_ERROR;
  fin.seekg(0);
  fin.clear();
  fin.seekg(0);

  for (max = index = 0; !fin.eof(); index++)
  {
    for (;!fin.eof() && index < MAXBUFFER; index++, max++)
      fin >>(*array[index]);

    for (index -= fin.eof() ? 2:1; index > 0; index--)
      (*array[0]) += (*array[index]);
  }

  T resval_ = T(0);
  if (max > 1)
    resval_ = ((*array[0]) / --max);
  else
    Error_ = EMPTY_ARRAY;  
  
  for (index = 0; index < MAXBUFFER; index++)
    delete array[index];

  return resval_;
}

/****************************************************************************/
template <class T>
T StatArray<T>::StdDeviation(ifstream& fin, Boolean& Error_)
{
  Error_ = NO_ERROR;
  T resval_ = Variance(fin, Error_);
  
  if (Error_)
    Error_ = EMPTY_ARRAY;        
  else
    resval_ = sqrt(resval_);
  
  return resval_;  
}

/****************************************************************************/
template <class T>
T StatArray<T>::AvgDeviation(ifstream& fin, Boolean& Error_)
{
  const int MAXBUFFER = 10;
  Error_ = NO_ERROR;
  
  T* array[10];
  T mean;
  int index, max;

  mean = AvgMean(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  for (index = 0; index < MAXBUFFER; index++)
    array[index] = new T();  

  fin.seekg(0);
  fin.clear();
  fin.seekg(0);

  for (max = index = 0; !fin.eof(); index++)
  {
    for (;!fin.eof() && index < MAXBUFFER; index++, max++)
    {
      fin >>(*array[index]);

      if (!fin.eof())
        (*array[index]) = ::fabs((*array[index]) - mean);
    }

    for (index -= fin.eof() ? 2:1; index > 0; index--)
      (*array[0]) += (*array[index]);
  }

  T resval_ = T(0);
  if (max > 1)
    resval_ = ((*array[0]) / --max);
  else
    Error_ = EMPTY_ARRAY;    

  for (index = 0; index < MAXBUFFER; index++)
    delete array[index];
  
  return resval_;  
}

/****************************************************************************/
template <class T>
T StatArray<T>::Variance(ifstream& fin, Boolean& Error_)
{
  const int MAXBUFFER = 10;
  Error_ = NO_ERROR;

  T* array[10];
  T mean;
  int index, max;

  mean = AvgMean(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  for (index = 0; index < MAXBUFFER; index++)
    array[index] = new T();  

  fin.seekg(0);
  fin.clear();
  fin.seekg(0);

  for (max = index = 0; !fin.eof(); index++)
  {
    for (;!fin.eof() && index < MAXBUFFER; index++, max++)
    {
      fin >>(*array[index]);

      if (!fin.eof())
      {
        (*array[index]) -= mean;
        (*array[index]) *= (*array[index]);
      }
    }

    for (index -= fin.eof() ? 2:1; index > 0; index--)
      (*array[0]) += (*array[index]);
  }

  T resval_ = T(0);
  if (max > 1)
    resval_ = ((*array[0]) / --max);    
  else
    Error_ = EMPTY_ARRAY;

  for (index = 0; index < MAXBUFFER; index++)
    delete array[index];
  
  return resval_;
}

/****************************************************************************/
template <class T>
T StatArray<T>::Skew(ifstream& fin, Boolean& Error_)
{
  const int MAXBUFFER = 10;
  Error_ = NO_ERROR;
  
  T* array[10];
  T mean, stddev;
  int index, max;

  mean = AvgMean(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  stddev = StdDeviation(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  for (index = 0; index < MAXBUFFER; index++)
    array[index] = new T();  

  fin.seekg(0);
  fin.clear();
  fin.seekg(0);

  for (max = index = 0; !fin.eof(); index++)
  {
    for (;!fin.eof() && index < MAXBUFFER; index++, max++)
    {
      fin >>(*array[index]);

      if (!fin.eof())
      {
        (*array[index]) = ((*array[index]) - mean) / stddev;
        (*array[index]) = ((*array[index]) * (*array[index]) * (*array[index]));
      }
    }

    for (index -= fin.eof() ? 2:1; index > 0; index--)
      (*array[0]) += (*array[index]);
  }

  T resval_ = T(0);
  if (max > 2)
  {
    max -= 2;
    resval_ = (*array[0]) / max;
  }
  else
    Error_ = EMPTY_ARRAY;

  for (index = 0; index < MAXBUFFER; index++)
    delete array[index];
  
  return resval_;
}

/****************************************************************************/
template <class T>
T StatArray<T>::Kurt(ifstream& fin, Boolean& Error_)
{
  const int MAXBUFFER = 10;
  Error_ = NO_ERROR;
  
  T* array[10];
  T mean, stddev;
  int index, max;

  mean = AvgMean(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  stddev = StdDeviation(fin, Error_);
  if (Error_)
  {
    Error_ = EMPTY_ARRAY;        
    return T(0);
  }
  
  for (index = 0; index < MAXBUFFER; index++)
    array[index] = new T();  

  fin.seekg(0);
  fin.clear();
  fin.seekg(0);

  for (max = index = 0; !fin.eof(); index++)
  {
    for (;!fin.eof() && index < MAXBUFFER; index++, max++)
    {
      fin >>(*array[index]);

      if (!fin.eof())
      {
        (*array[index]) = ((*array[index]) - mean) / stddev;
        (*array[index]) *= (*array[index]);
        (*array[index]) *= (*array[index]);        
      }
    }

    for (index -= fin.eof() ? 2:1; index > 0; index--)
      (*array[0]) += (*array[index]);
  }

  T resval_ = T(0);
  if (max > 2)
  {
    max -= 2;
    resval_ = (*array[0]) / max;
    resval_ -= T(3.0);
  }
  else
    Error_ = EMPTY_ARRAY;

  for (index = 0; index < MAXBUFFER; index++)
    delete array[index];
  
  return resval_;
}

/****************************************************************************/
template <class T>
StatArray<T>& StatArray<T>::operator = (const StatArray<T>& Arr_)
{
  DynArray<T>::operator = (Arr_);
  return *this;
}

/****************************************************************************/
template <class T>
StatArray<T>& StatArray<T>::operator = (const DynArray<T>& Arr_)
{
  DynArray<T>::operator = (Arr_);
  return *this;
}

/****************************************************************************/
template <class T>
StatArray<T>& StatArray<T>::operator += (const DynArray<T>& Arr_)
{
  DynArray<T>::operator += (Arr_);
  return *this;
}

/****************************************************************************/
template <class T>
StatArray<T>& StatArray<T>::operator += (const StatArray<T>& Arr_)
{
  DynArray<T>::operator += (Arr_);
  return *this;
}

/****************************************************************************/
template <class T>
T StatArray<T>::Min() const
{
  if (RunLength())
  {
    size_t i, Max_;
    T Result_ = (*this)[0];

    for (i = 1, Max_ = RunLength(); i < Max_; i++)
      if ((*this)[i] < Result_)
        Result_ = (*this)[i];

    return Result_;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::Max() const
{
  if (RunLength())
  {
    size_t i, Max_;
    T Result_ = (*this)[0];

    for (i = 1, Max_ = RunLength(); i < Max_; i++)
      if ((*this)[i] > Result_)
        Result_ = (*this)[i];

    return Result_;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
void StatArray<T>::MinMax(T& minimum, T& maximum) const
{
  if (RunLength())
  {
    size_t x, Max_;
    T MinResult_ = (*this)[0];
    T MaxResult_ = (*this)[0];

    for (x = 1, Max_ = RunLength(); x < Max_; x++)
    {
      if ((*this)[x] < MinResult_)
        MinResult_ = (*this)[x];

      if ((*this)[x] > MaxResult_)
        MaxResult_ = (*this)[x];
    }

    minimum = MinResult_;
    maximum = MaxResult_;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)
}

/****************************************************************************/
template <class T>
T StatArray<T>::RangeOf() const
{
  T minimum, maximum;
  MinMax(minimum, maximum);
  return T(::fabs(((double)maximum) - ((double)minimum)));
}

/****************************************************************************/
template <class T>
T StatArray<T>::Sum() const
{
  if (RunLength())
  {
    size_t i, Max_;
    T Result_ = (*this)[0];

    for (i = 1, Max_ = RunLength(); i < Max_; i++)
      Result_ += (*this)[i];

    return Result_;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T(0);
}

/****************************************************************************/
template <class T>
T StatArray<T>::Product() const
{
  if (RunLength())
  {
    size_t i, Max_;
    T Result_ = (*this)[0];

    for (i = 1, Max_ = RunLength(); i < Max_; i++)
      Result_ *= (*this)[i];

    return Result_;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T(0);
}

/****************************************************************************/
template <class T>
T StatArray<T>::Mean() const
{
  return T(double(Sum()) / RunLength());
}

/****************************************************************************/
template <class T>
T StatArray<T>::Median() const
{
  if (RunLength())
  {
    size_t x, y, Max_;
    StatArray<T> Temp_(*this);
    ItemSorter<T, T> Sorter_;
    
    Sorter_.SetSortHelper(ObjectSortHelper<T, T>::Make());
    Sorter_.QuickSort(Temp_.Base(), 0, Temp_.RunLength() - 1);

    for (x = y = 0, Max_ = Temp_.RunLength(); y < Max_; y++)
      if (Temp_[x] != Temp_[y])
        Temp_[++x] = Temp_[y];

    ++x;
    Temp_.Resize(x, T());
    return Temp_[x / 2];
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T** StatArray<T>::Mode() const
{
  if (RunLength())
  {
    size_t x, y, Max_;
    StatArray<T> Temp_(*this);
    ItemSorter<T, T> Sorter_;
    int* Indexes_ = (int*)MemMatrix::Matrix().Allocate(Temp_.RunLength() * sizeof(int));
    
    Sorter_.SetSortHelper(ObjectSortHelper<T, T>::Make());
    Sorter_.QuickSort(Temp_.Base(), 0, Temp_.RunLength() - 1);
    
    Max_ = Temp_.RunLength();
    for (x = 0; x < Max_; x++)
      Indexes_[x] = 0;

    for (x = y = 0; y < Max_; y++)
      if (Temp_[x] == Temp_[y])
        Indexes_[x]++;
      else
      {
        while (x < y)
          Indexes_[++x] = 0;
        Indexes_[x] = 1;
      }

    size_t highx = 0;
    size_t times = 0;
    
    for (x = 0; x < Temp_.RunLength(); x++)
      if (Indexes_[highx] < Indexes_[x])
        highx = x;

    for (x = 0; x < Temp_.RunLength(); x++)
      if (Indexes_[x] == Indexes_[highx])
      {
        ++times;
        if (x != highx)
          Indexes_[x] = -1;
      }

    Indexes_[highx] = -1;
    T** Array_ = new T*[times+1];
    
    for (x = y = 0; x < Temp_.RunLength(); x++)
      if (Indexes_[x] < 0)
        Array_[y++] = new T(Temp_[x]);

    MemMatrix::Matrix().Deallocate(Indexes_);
    Array_[y] = NULL;
    
    return Array_;
  }  
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return ((T**)NULL);
}

/****************************************************************************/
template <class T>
T StatArray<T>::MedianSort()
{
  if (RunLength())
  {
    size_t x, y, Max_;
    ItemSorter<T, T> Sorter_;
    
    Sorter_.SetSortHelper(ObjectSortHelper<T, T>::Make());
    Sorter_.QuickSort(Base(), 0, RunLength() - 1);
    StatArray<T> Temp_(*this);

    for (x = y = 0, Max_ = Temp_.RunLength(); y < Max_; y++)
      if (Temp_[x] != Temp_[y])
        Temp_[++x] = Temp_[y];

    ++x;
    Temp_.Resize(x, T());
    return Temp_[x / 2];    
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)  

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::Variance() const
{
  if (RunLength())
  {
    T temp, result = T(0.0);
    T m = Mean();
    const T* Ptr_ = Base();
    size_t i, Max_;

    for (i = 0, Max_ = RunLength(); i < Max_; i++)
    {
      temp = *Ptr_ - m;
      result += temp * temp;
      ++Ptr_;
    }

    result /= ((T)Max_);
    return result;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::StdDeviation() const
{
  return sqrt(Variance());
}

/****************************************************************************/
template <class T>
T StatArray<T>::AvgDeviation() const
{
  if (RunLength())
  {
    T result = T(0.0);
    T m = Mean();
    const T* Ptr_ = Base();
    size_t i, Max_;
  
    for (i = 0, Max_ = RunLength(); i < Max_; i++)
    {
      result += ::fabs(*Ptr_ - m);
      ++Ptr_;
    }

    result /= ((T)Max_);
    return result;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::Skew() const
{
  if (RunLength())
  {
    T result = T(0.0);
    T sd = StdDeviation();
    
    if (sd != T(0.0))
    {
      T temp;
      T m = Mean();
      const T* Ptr_ = Base();
      size_t i, Max_;

      for (i = 0, Max_ = RunLength(); i < Max_; i++)
      {
        temp = (*Ptr_ - m) / sd;
        result += (temp * temp * temp);
        ++Ptr_;
      }

      result /= ((T)(Max_ - 1));
    }

    return result;    
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::Kurt() const
{
  if (RunLength())
  {
    T result = T(0.0);
    T sd = StdDeviation();

    if (sd != T(0.0))
    {
      T temp;
      T m = Mean();
      const T* Ptr_ = Base();
      size_t i, Max_;

      for (i = 0, Max_ = RunLength(); i < Max_; i++)
      {
        temp = (*Ptr_ - m) / sd;
        result += (temp * temp * temp * temp);
        ++Ptr_;
      }

      result /= ((T)(Max_ - 1));
      result -= T(3.0);
    }
    
    return result;    
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
void StatArray<T>::Moment(T& Mean_, T& AvgDev_, T& StdDev_, T& Var_, T& Skew_, T& Kurt_) const
{
  if (RunLength())
  {
    T temp, tempsqr, cnt;
    size_t i, Max_;

    cnt    = (T)RunLength();
    Mean_   = Mean();
    Var_    = T(0.0);
    AvgDev_ = T(0.0);
    Max_ = RunLength();
    
    const T * ptr = Base();

    for (i = 0; i < Max_; ++i)
    {
      temp    = *ptr - Mean_;
      Var_    += temp * temp;
      AvgDev_ += fabs(temp);
      ++ptr;
    }

    Var_    /= cnt;
    AvgDev_ /= cnt;
    StdDev_  = sqrt(Var_);

    if (StdDev_ == T(0.0))
    {
      Skew_ = T(0.0);
      Kurt_ = T(0.0);
    }
    else
    {
      ptr  = Base();
      Skew_ = T(0.0);
      Kurt_ = T(0.0);

      for (i = 0; i < Max_; ++i)
      {
        temp = (*ptr - Mean_) / StdDev_;
        tempsqr = temp * temp;
        Kurt_ += (tempsqr * tempsqr);
        Skew_ += (tempsqr * temp);
        ++ptr;
      }

      Skew_ /= ((T)(cnt - 1));
      Kurt_ /= ((T)(cnt - 1));
      Kurt_ -= T(3.0);
    }
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)
}

/****************************************************************************/
template <class T>
T StatArray<T>::ZScore(const Index& i) const
{
  T result = T(0.0);
  T sd = StdDeviation();

  if (sd == T(0.0))
    result = T(0.0);
  else if (size_t(i) < RunLength())
    result = ((*this)[size_t(i)] - Mean()) / sd;

  return result;
}

/****************************************************************************/
template <class T>
T StatArray<T>::ZScore(int i) const
{
  if (RunLength())
  {
    T result = T(0.0);
    T sd = StdDeviation();

    if (sd == T(0.0))
      result = T(0.0);
    else if (i < RunLength())
      result = ((*this)[i] - Mean()) / sd;

    return result;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
StatArray<T> StatArray<T>::ZScore() const
{
  if (RunLength())
  {
    StatArray<T> result(RunLength(), default_size);
    T sd = StdDeviation();

    if (sd == T(0.0))
      result.AssignArg(T(0.0), RunLength());
    else
    {
      const T* src = Base();
      T* dest = result.Base();
      T m = Mean();
      size_t i, Max_;

      for (i = 0, Max_ = RunLength(); i < Max_; i++)
      {
        *dest = (*src - m) / sd;
        ++src;
        ++dest;
      }
    }

    return result;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();
}

/****************************************************************************/
template <class T>
T StatArray<T>::Correlation(const StatArray<T>& dsa)
{
  if (RunLength())
  {
    if (RunLength() != dsa.RunLength())
      XInCompat();

    const T* Ptr1_ = Base();
    const T* Ptr2_ = dsa.Base();
    T result = T(0.0);
    T m1 = Mean();
    T m2 = dsa.Mean();
    T sd1 = StdDeviation();
    T sd2 = dsa.StdDeviation();

    if ((sd1 == T(0.0)) || (sd2 == T(0.0)))
    {
      if (sd1 == sd2)
        result = T(1.0);
      else
        result = T(0.0);
    }
    else
    {
      size_t i, Max_;
    
      for (i = 0, Max_ = RunLength(); i < Max_; i++)
      {
        result += ((*Ptr1_ - m1) / sd1) * ((*Ptr2_ - m2) / sd2);
        ++Ptr1_;
        ++Ptr2_;
      }

      result /= Max_;
    }

    return result;
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)

  return T();  
}

/****************************************************************************/
template <class T>
void StatArray<T>::FillRandom(T low, T high)
{
  if (RunLength())
  {
    SetSeed(time(NULL));
  
    if (high <= low)
      low = high;

    T range = high - low;
    T* ptr = Base();
    size_t i, Max_;

    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
    {
      *ptr = NextRand() * range + low;
      ++ptr;
    }
  }
  else
    ASSIGN_CONST_MEMBER(StatArray<T>, _ErrorStat, EMPTY_ARRAY)
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* StatArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void StatArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* StatArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void StatArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
StatArrayAcceptor<T>::StatArrayAcceptor(const StatArray<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void StatArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const StatArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* StatArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean StatArrayAcceptor<T>::IsStatArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
Boolean StatArrayAcceptor<T>::IsDynamicArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
void* StatArrayAcceptor<T>::CreateFromStatArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new StatArray<T>(*((const StatArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StatArrayAcceptor<T>::CreateFromDynamicArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new StatArray<T>(*((const DynArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* StatArrayAcceptor<T>::AssignFromStatArray(const ObjectAcceptor& Obj_)
{
  const StatArray<T>* TrgPtr_ = (const StatArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StatArrayAcceptor<T>::AssignFromDynamicArray(const ObjectAcceptor& Obj_)
{
  const DynArray<T>* TrgPtr_ = (const DynArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* StatArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void StatArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
#endif




