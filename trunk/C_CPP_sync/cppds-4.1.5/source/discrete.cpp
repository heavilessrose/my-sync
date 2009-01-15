#ifndef DISCRETE_CPP
#define DISCRETE_CPP
#ifndef DISCRETE_H
  #include "discrete.h"
#endif

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Factorial(const TYPE& Val_)
{
  if (Val_ < TYPE(0))
    return 0;

  if (Val_ == TYPE(0))
    return 1;

  TYPE Total_ = 1;
  TYPE Index_ = 1;
  for (; Index_ <= Val_; Index_++)
    Total_ *= Index_;

  return Total_;
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Factorial2(const TYPE& Low_, const TYPE& High_)
{
  if (High_ <= TYPE(0) || Low_ <= TYPE(0))
    return 0;

  if (Low_ > High_)
    return 1;

  TYPE Total_ = Low_;
  TYPE Index_ = Low_;
  for (++Index_; Index_ <= High_; Index_++)
    Total_ *= Index_;

  return Total_;
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Permutation(const TYPE& NumObjs_, const TYPE& PermSize_)
{
  return ((PermSize_ != TYPE(0)) ? Factorial2(NumObjs_ - PermSize_ + TYPE(1), NumObjs_):TYPE(1));
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Permutation2(const TYPE& SetSize_, const TYPE* IndishObjects_, size_t Len_)
{
  TYPE Index_, Denom_;

  Denom_ = 1;
  for (Index_ = 0; Index_ < Len_; Index_++)
    Denom_ *= Factorial(IndishObjects_[Index_]);

  return (Factorial(SetSize_) / Denom_);
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Arrangement(const TYPE& NumObjs_, const TYPE& PermSize_)
{
  return TYPE(pow(NumObjs_, PermSize_));
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::Combination(const TYPE& NumObjs_, const TYPE& CombSize_)
{
  TYPE Half_ = NumObjs_ / TYPE(2);
  return
  (
    (NumObjs_ <= Half_) ?
        (Permutation(NumObjs_, CombSize_) / Factorial(CombSize_)):
        (Factorial2(CombSize_ + TYPE(1), NumObjs_) / Factorial(NumObjs_ - CombSize_))
  );
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::CombinationWithRep(const TYPE& NumObjs_, const TYPE& CombSize_)
{
  return Combination(NumObjs_ + CombSize_ - TYPE(1), CombSize_);
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::PermutationWithRep(const TYPE& NumObjs_, const TYPE& PermSize_)
{
  if (PermSize_ != TYPE(0))
  {
    TYPE x;
    TYPE temp = NumObjs_;
  
    for (x = 1; x < PermSize_; x++)
      temp *= NumObjs_;

    return temp;
  }

  return TYPE(1);  
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::GCD(const TYPE& Num1_, const TYPE& Num2_)
{
    TYPE temp;
    TYPE x, y;
    
    x = Num1_;
    y = Num2_;
    
    while (y != TYPE(0))
    {
        temp = x;
        temp %= y;
        x = y;
        y = temp;
    }

    return x;
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::LCM(const TYPE& Num1_, const TYPE& Num2_)
{
    TYPE s, l;

    if (Num1_ == Num2_)
        return Num1_;

    if (Num1_ < Num2_)
    {
        s = Num1_;
        l = Num2_;
    }
    else
    {
        l = Num1_;
        s = Num2_;
    }

    return ((s * l) / GCD(s,l));
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::NumberOfRelations(const TYPE& SizeSetA_, const TYPE& SizeSetB_)
{
  return TYPE(pow(2, SizeSetA_ * SizeSetB_));
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::NumberOfFunctions(const TYPE& SizeSetA_, const TYPE& SizeSetB_)
{
  return TYPE(pow(SizeSetB_, SizeSetA_));
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::NumberOf1To1Functions(const TYPE& SizeSetA_, const TYPE& SizeSetB_)
{
  return Permutation(SizeSetB_, SizeSetA_);
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::NumberOfOntoFunctions(const TYPE& SizeSetA_, const TYPE& SizeSetB_)
{
  TYPE Index_, Total_;  
  for (Total_ = Index_ = 0; Index_ <= SizeSetB_; Index_++)
    Total_ += TYPE(pow(-1, Index_)) *
    Combination(SizeSetB_, SizeSetB_ - Index_) *
    TYPE(pow(SizeSetB_ - Index_, SizeSetA_));

  return Total_;
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::StirlingNumber2(const TYPE& DistinctObjs_, const TYPE& IdentConts_)
{
  TYPE Index_, Total_, Denom_;
  for (Total_ = Index_ = 0; Index_ <= IdentConts_; Index_++)
    Total_ += TYPE(pow(-1, Index_)) *
    Combination(IdentConts_, IdentConts_ - Index_) *
    TYPE(pow(IdentConts_ - Index_, DistinctObjs_));

  Denom_ = Factorial(IdentConts_);
  Total_ /= Denom_;
  
  return Total_;
}

/****************************************************************************/
template <class TYPE>
TYPE Discrete<TYPE>::NumberOfDistributions(const TYPE& NumObjects_, const TYPE& NumConts_, int Properties_)
{
  if (Properties_ == (OBJECTS_DISTINCT | CONTAINERS_DISTINCT | EMPTY_CONTAINERS_ALLOWED))
    return TYPE(pow(NumConts_, NumObjects_));
  else if (Properties_ == (OBJECTS_DISTINCT | CONTAINERS_DISTINCT))
    return (Factorial(NumConts_) * StirlingNumber2(NumObjects_, NumConts_));
  else if (Properties_ == (OBJECTS_DISTINCT | EMPTY_CONTAINERS_ALLOWED))
  {
    TYPE Total_, Index_;
    
    Total_ = 0;    
    for (Index_ = 1; Index_ <= NumConts_; Index_++)
      Total_ += StirlingNumber2(NumObjects_, Index_);

    return Total_;
  }
  else if (Properties_ == OBJECTS_DISTINCT)
    return StirlingNumber2(NumObjects_, NumConts_);
  else if (Properties_ == (CONTAINERS_DISTINCT | EMPTY_CONTAINERS_ALLOWED))
    return CombinationWithRep(NumConts_, NumObjects_);
  else if (Properties_ == CONTAINERS_DISTINCT)
    return Combination(NumObjects_ - 1, NumConts_ - 1);

  return 0;
}

/****************************************************************************/
/****************************************************************************/
template <class TYPE>
DiscreteSet<TYPE>::DiscreteSet(int ZeroTerminated_):
_Data(NULL),
_SelData(NULL),
_Indices(NULL),
_Size(0),
_Sel(0),
_Done(0),
_ZeroTerm(ZeroTerminated_),
_Stack(NULL),
_Loops(NULL),
_CascDisc(NULL),
_CascData(NULL)
{}

/****************************************************************************/
template <class TYPE>
DiscreteSet<TYPE>::~DiscreteSet()
{
  Sdata* Node_;

  while (_Stack)
  {
    Node_ = Pop();
    delete Node_;
  }

  delete[] _Indices;
  delete[] _SelData;
  delete[] _CascData;

  delete _CascDisc;
  ClearLoops();
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ClearLoops()
{
  if (_Loops)
  {
    size_t x;
    for (x = 0; x < _Sel; x++)
      delete _Loops[x];
    delete[] _Loops;
    _Loops = NULL;
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Push(size_t s, size_t x, size_t r, int f)
{
  Sdata* Node_ = new Sdata;
  Node_->_Source = s;
  Node_->_Target = x;
  Node_->_Reset = r;
  Node_->_FncNum = f;
  Node_->_Next = _Stack;
  _Stack = Node_;
}

/****************************************************************************/
template <class TYPE>
TYPENAME DiscreteSet<TYPE>::Sdata* DiscreteSet<TYPE>::Pop()
{
  if (_Stack)
  {
    Sdata* Node_ = _Stack;
    _Stack = _Stack->_Next;
    Node_->_Next = NULL;
    return Node_;
  }

  return NULL;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::RetrieveCrSel()
{
  size_t cnt = 0;
  size_t x = _Size;

  do
  {
    --x;
    if (_Indices[x])
      _SelData[cnt++] = _Data[x];
  }
  while (x);

  if (_ZeroTerm)
    _SelData[cnt] = 0;
    
  ReverseData(_SelData, _Sel);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::RetrievePrWithRepSel()
{
  size_t len = _Sel - 1;
  size_t x = _Sel - 1;

  for (;;)
  {
    _SelData[len-x] = _Data[_Indices[x]];
    
    if (x)
      --x;
    else
      break;      
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Swap()
{
  TYPE Temp_ = _Data[0];
  _Data[0] = _Data[1];
  _Data[1] = Temp_;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ShiftToTop(TYPE* Ptr_, size_t Dist_, TYPE Item_)
{
  TYPE* Start_ = Ptr_;
  Ptr_ -= Dist_;
  
  while (Ptr_ != Start_)
  {
    ++Ptr_;
    Ptr_[-1] = Ptr_[0];
  }

  *Ptr_ = Item_;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ShiftBack(TYPE* Ptr_, size_t Dist_, TYPE Item_)
{
  TYPE* End_ = Ptr_ - Dist_ + 1;
  ++Ptr_;

  do
  {
    Ptr_[0] = Ptr_[-1];
    --Ptr_;    
  }
  while (Ptr_ != End_);

  *Ptr_ = Item_;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ShiftUp(size_t Index_)
{
  _Indices[Index_ + 1] = 1;
  _Indices[Index_] = 0;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ResetCr(size_t x)
{
  if (x && _Indices[x - 1])
    ResetCr(x - 1);

  if (x)
  {
    size_t pos;
    for (pos = x - 1; !_Indices[pos] && pos; pos--);
    if (_Indices[pos])
      ++pos;

    if (pos < x)
    {
      _Indices[x] = 0;
      _Indices[pos] = 1;
    }
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Reset(size_t s)
{
  // resetting
  ShiftBack(&_Data[s-1], s - _Indices[s], _Data[s]);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::MoveToTop(size_t s, size_t x, size_t r)
{
  if (s)
  {
    Push(s, x, r, SECTION1);
    
    if (s > 1)
      MoveToTop(s - 1, s - 2, 0);
    else
      Push(0, 0, 0, RETURN_DATA);
        
    return;
  }

  Section1(s, x, r);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::CrMoveToTop(size_t Index_)
{
  if (Index_ < _Size - 1)
  {
    if (_Indices[Index_ + 1] == 0)
    {
      ShiftUp(Index_);

      if (Index_ > 0 && _Indices[Index_ - 1])
      {
        ResetCr(Index_ - 1);
        Push(0, 0, 0, RETRIEVE_SELECTION);
        return;
      }
      else
      {
        Push(Index_ + 1, 0, 0, COMB_MOVE_TO_TOP);
        Push(0, 0, 0, RETRIEVE_SELECTION);
        return;        
      }
    }    
    else
    {
      size_t last = !Index_ || !_Indices[Index_ - 1];
      Push(last, 0, 0, MOVE_UP_FROM_ZERO);
      CrMoveToTop(Index_ + 1);
      return;      
    }
  }
  else
    _Done = 1;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::CrWithRepMoveToTop(size_t Index_)
{
  if (Index_ < _Sel - 1 && _Indices[Index_ + 1] == _Indices[Index_] && _Indices[Index_] < _Size - 1)
  {
    _Indices[Index_] = 0;
    CrWithRepMoveToTop(Index_ + 1);
  }
  else if (_Indices[Index_] < _Size - 1)
  {
    _Indices[Index_]++;
    Push(1, 0, 0, MOVE_UP_FROM_ZERO);
    Push(0, 0, 0, RETRIEVE_SELECTION);
    return;
  }
  else
    _Done = 1;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::MoveUpFromZeroWithRep(size_t Last_)
{
  if (Last_ && !_Done)
    CrWithRepMoveToTop(0);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::MoveUpFromZero(size_t Last_)
{
  if (Last_ && !_Done)
    CrMoveToTop(0);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Section1(size_t s, size_t x, size_t r)
{
  if (r)
    Reset(s);
  
  _Indices[s] = x;

  // moving to top
  ShiftToTop(&_Data[s], s - x, _Data[_Indices[s]]);
  
  if (_Indices[s] == 0)
  {
    if (s == 1)
    {
      Push(s, x, r, SECTION2);
      Push(0, 0, 0, RETURN_DATA);
      return;
    }
    else
    {
      Push(s, x, r, SECTION2);
      MoveToTop(s - 1, s - 2, 0);
      return;
    }
      
    Section2(s);
  }
  else
    MoveToTop(s, x - 1, 1);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Section2(size_t s)
{
  Reset(s);
  if (s >= _Size - 1)
    _Done = 1;
}

/****************************************************************************/
template <class TYPE>
int DiscreteSet<TYPE>::HasMore() const
{
  return (!_Done || _Stack);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::ReverseData(TYPE* Data_, size_t Size_)
{
  size_t x = 0;
  size_t y = Size_ - 1;

  while (x < y)
  {
    TYPE Temp_ = Data_[x];
    Data_[x] = Data_[y];
    Data_[y] = Temp_;

    ++x;
    --y;
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Accept(ForLoop<size_t>* LoopPtr_)
{
  size_t x = LoopPtr_->GetLoopNumber();
  size_t v = *LoopPtr_->GetIndex();
  _Indices[x] = v;
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Permute(TYPE* Set_, size_t Size_, size_t Sel_)
{
  if (Sel_ > Size_ || !Sel_)
    return;
  else if (Sel_ == Size_)
    Permute(Set_, Size_);
  else
  {
    _CascDisc = new DiscreteSet<TYPE>(_ZeroTerm);

    if (_ZeroTerm)
    {
      _CascData = new TYPE[Size_ + 1];
      _CascData[Size_] = TYPE(0);
    }
    else
      _CascData = new TYPE[Size_];

    size_t x;
    for (x = 0; x < Size_; x++)
      _CascData[x] = Set_[x];
  
    _CascDisc->Combine(_CascData, Size_, Sel_);
  
    TYPE* RetData_ = _CascDisc->NextCombination();
    if (RetData_)
      Permute(RetData_, Sel_);
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Permute(TYPE* Set_, size_t Size_)
{
  _Data = Set_;
  _Size = Size_;
  _Done = 0;

  if (_Indices)
    delete[] _Indices;
    
  _Indices = new size_t[_Size];

  size_t x;
  for (x = 0; x < _Size; x++)
    _Indices[x] = x;

  if (_Size == 1)
  {
    Push(0, 0, 0, RETURN_DATA);
    _Done = 1;
    return;
  }
  else if (Size_ == 2)
  {
    Push(0, 0, 0, RETURN_DATA);
    Push(0, 0, 0, SWAP);
    Push(0, 0, 0, RETURN_DATA);
    _Done = 1;
    return;    
  }
  else
    MoveToTop(_Size - 1, _Size - 2, 0);
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::Combine(TYPE* Set_, size_t Size_, size_t Sel_)
{
  if (Sel_ > Size_ || !Sel_)
    return;

  _Data = Set_;
  _Size = Size_;
  _Sel = Sel_;
  _Done = 0;    

  if (_Indices)
    delete[] _Indices;

  if (_SelData)
    delete[] _SelData;
    
  _Indices = new size_t[_Size];
  
  if (_ZeroTerm)
  {
    _SelData = new TYPE[_Sel + 1];
    _SelData[_Sel] = TYPE(0);
  }
  else
    _SelData = new TYPE[_Sel];

  size_t x;
  for (x = 0; x < _Sel; x++)
    _Indices[x] = 1;

  for (;x < _Size; x++)
    _Indices[x] = 0;

  if (_Sel == _Size)
  {
    Push(0, 0, 0, RETRIEVE_SELECTION);
    _Done = 1;    
  }
  else
  {
    Push(0, 0, 0, COMB_MOVE_TO_TOP);
    Push(0, 0, 0, RETRIEVE_SELECTION);
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::CombineWithRep(TYPE* Set_, size_t Size_, size_t Sel_)
{
  if (!Size_ || !Sel_)
    return;

  _Data = Set_;
  _Size = Size_;
  _Sel = Sel_;
  _Done = 0;    

  if (_Indices)
    delete[] _Indices;

  if (_SelData)
    delete[] _SelData;
    
  _Indices = new size_t[_Sel];
  
  if (_ZeroTerm)
  {
    _SelData = new TYPE[_Sel + 1];
    _SelData[_Sel] = TYPE(0);
  }
  else
    _SelData = new TYPE[_Sel];

  size_t x;
  for (x = 0; x < _Sel; x++)
    _Indices[x] = 0;

  if (_Size == 1)
  {
    Push(0, 0, 0, RETRIEVE_SELECTION);
    _Done = 1;    
  }
  else
  {
    Push(0, 0, 0, COMB_MOVE_TO_TOP);
    Push(0, 0, 0, RETRIEVE_SELECTION);  
  }
}

/****************************************************************************/
template <class TYPE>
void DiscreteSet<TYPE>::PermuteWithRep(TYPE* Set_, size_t Size_, size_t Sel_)
{
  if (!Size_ || !Sel_)
    return;

  _Data = Set_;
  _Size = Size_;
  _Sel = Sel_;
  _Done = 0;

  if (_Indices)
    delete[] _Indices;

  if (_SelData)
    delete[] _SelData;
    
  _Indices = new size_t[_Sel];
  
  if (_ZeroTerm)
  {
    _SelData = new TYPE[_Sel + 1];
    _SelData[_Sel] = TYPE(0);
  }
  else
    _SelData = new TYPE[_Sel];

  size_t x;
  for (x = 0; x < _Sel; x++)
    _Indices[x] = 0;
    
  _Loops = new ForLoop<size_t>*[Sel_];

  for (x = 0; x < Sel_; x++)
  {
    _Loops[x] = new ForLoop<size_t>();
    (*_Loops[x])
      .SetStartValue(0)
      .SetStepValue(1)
      .SetIndexLimitPtr(&_Size)
      .AddLoopBody(Transferring, this, ForLoopEnums::PRE_LOOP);

    if (x)
      _Loops[x-1]->AddLoop(_Loops[x]);
  }
}

/****************************************************************************/
template <class TYPE>
TYPE* DiscreteSet<TYPE>::NextPermutation()
{
  int Found_ = 0;
  Sdata* Node_;

  while (HasMore() && !Found_)
    if (_Stack)
    {
      Node_ = Pop();
        
      if (Node_->_FncNum == SECTION1)
        Section1(Node_->_Source, Node_->_Target, Node_->_Reset);
      else if (Node_->_FncNum == RETURN_DATA)
        Found_ = 1;              
      else if (Node_->_FncNum == SECTION2)
        Section2(Node_->_Source);
      else if (Node_->_FncNum == SWAP)
        Swap();

      delete Node_;
    }

  if (Found_)
    return _Data;

  if (_CascDisc && _CascData)
  {
    TYPE* RetData_ = _CascDisc->NextCombination();
    
    if (RetData_)
    {
      Permute(RetData_, _Size);
      return NextPermutation();
    }
  }  
  
  return NULL;
}

/****************************************************************************/
template <class TYPE>
TYPE* DiscreteSet<TYPE>::NextCombination()
{
  if (_Loops)
    return NextPermutationWithRep();

  int Found_ = 0;
  Sdata* Node_;

  while (HasMore() && !Found_)
    if (_Stack)
    {
      Node_ = Pop();
        
      if (Node_->_FncNum == MOVE_UP_FROM_ZERO)
        MoveUpFromZero(Node_->_Source);
      else if (Node_->_FncNum == COMB_MOVE_TO_TOP)
        CrMoveToTop(Node_->_Source);
      else if (Node_->_FncNum == RETRIEVE_SELECTION)
      {
        RetrieveCrSel();      
        Found_ = 1;        
      }
      
      delete Node_;
    }

  if (Found_)
    return _SelData;
  
  return NULL;
}

/****************************************************************************/
template <class TYPE>
TYPE* DiscreteSet<TYPE>::NextCombinationWithRep()
{
  int Found_ = 0;
  Sdata* Node_;

  while (HasMore() && !Found_)
    if (_Stack)
    {
      Node_ = Pop();
        
      if (Node_->_FncNum == MOVE_UP_FROM_ZERO)
        MoveUpFromZeroWithRep(Node_->_Source);
      else if (Node_->_FncNum == COMB_MOVE_TO_TOP)
        CrWithRepMoveToTop(Node_->_Source);
      else if (Node_->_FncNum == RETRIEVE_SELECTION)
      {
        RetrievePrWithRepSel();
        Found_ = 1;        
      }
      
      delete Node_;
    }

  if (Found_)
    return _SelData;
  
  return NULL;
}

/****************************************************************************/
template <class TYPE>
TYPE* DiscreteSet<TYPE>::NextPermutationWithRep()
{
  if (!_Done)
    if (_Loops && _Loops[0] && _Loops[0]->Run())
    {
      RetrievePrWithRepSel();
      return _SelData;
    }
    else
    {
      ClearLoops();
      _Done = 1;
    }

  return NULL;
}

/****************************************************************************/
/****************************************************************************/
#if DISCRETE_DEBUG
int main()
{
  assert(Discrete<Ulong>::Factorial(6) == 720);
  assert(Discrete<Ulong>::Permutation(10, 5) == 30240);
  assert(Discrete<Ulong>::Permutation(10, 3) == 720);
  assert(Discrete<Ulong>::Permutation(10, 7) == 604800);
  assert(Discrete<Ulong>::Permutation(8, 0) == 1);
  assert(Discrete<Ulong>::Permutation(8, 8) == 40320);
  
  Ulong Arr[2] = {4, 3};
  assert(Discrete<Ulong>::Permutation2(10, Arr, 2) == 25200);
  assert(Discrete<Ulong>::Combination(10, 5) == 252);
  assert(Discrete<Ulong>::Combination(10, 7) == 120);
  assert(Discrete<Ulong>::Combination(10, 3) == 120);
  assert(Discrete<Ulong>::Combination(8, 0) == 1);
  assert(Discrete<Ulong>::Combination(8, 8) == 1);
  
  assert(Discrete<Ulong>::PermutationWithRep(4, 6) == 4096);
  assert(Discrete<Ulong>::CombinationWithRep(4, 6) == 84);  
  assert(Discrete<Ulong>::GCD(456, 624) == 24);
  assert(Discrete<Ulong>::GCD(116, 641) == 1);
  assert(Discrete<Ulong>::LCM(456, 624) == 11856);
  assert(Discrete<Ulong>::LCM(456, 624) *
         Discrete<Ulong>::GCD(456, 624) == (456 * 624));
  assert(Discrete<Ulong>::StirlingNumber2(8, 3) == 966);

  size_t x;
  size_t Size = 5;
  size_t Sel = 3;

//  system("clear");
  DiscreteSet<char> Disc(1);
  char* Data = new char[Size + 1];
  char* RetData;
  
  for (x = 0; x < Size; x++)
    Data[x] = 'A' + x;
  Data[Size] = 0;

  cout <<"Testing Permutation" <<endl;
  Disc.Permute(Data, Size, Size);
  for (;;)
  {
    RetData = Disc.NextPermutation();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Permutation With Selection" <<endl;
  Disc.Permute(Data, Size, Sel);
  for (;;)
  {
    RetData = Disc.NextPermutation();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Combination" <<endl;
  Disc.Combine(Data, Size, Size);
  for (;;)
  {
    RetData = Disc.NextCombination();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }  

  cout <<"Testing Combination With Selection" <<endl;
  Disc.Combine(Data, Size, Sel);
  for (;;)
  {
    RetData = Disc.NextCombination();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Permutation With Repetition" <<endl;
  Disc.PermuteWithRep(Data, Size, Size);
  for (;;)
  {
    RetData = Disc.NextCombination();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Permutation With Repetition With Selection" <<endl;
  Disc.PermuteWithRep(Data, Size, Sel);
  for (;;)
  {
    RetData = Disc.NextCombination();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Permutation With Repetition With Selection" <<endl;
  Disc.PermuteWithRep(Data, Sel, Size);
  for (;;)
  {
    RetData = Disc.NextCombination();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }    

  cout <<"Testing Combination With Repetition" <<endl;
  Disc.CombineWithRep(Data, Size, Size);
  for (;;)
  {
    RetData = Disc.NextCombinationWithRep();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Combination With Repetition With Selection" <<endl;
  Disc.CombineWithRep(Data, Size, Sel);
  for (;;)
  {
    RetData = Disc.NextCombinationWithRep();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }

  cout <<"Testing Combination With Repetition With Selection" <<endl;
  Disc.CombineWithRep(Data, Sel, Size);
  for (;;)
  {
    RetData = Disc.NextCombinationWithRep();
    if (RetData)
      cout <<RetData <<endl;
    else
      break;
  }  

  cout <<"SUCCESS Testing Discrete" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif




