#ifndef PRIMESTABLE_CPP
#define PRIMESTABLE_CPP
#ifndef PRIMESTABLE_H
  #include "primetbl.h"
#endif

const Ulong PrimesTable::MAXENTRIES = 100000UL;

/****************************************************************************/
PrimesTable::TableBuffer::TableBuffer(const PrimesTable::TableBuffer& Obj_):
_Limit(Obj_._Limit),
_Distance(Obj_._Distance)
{
  memmove(_Table, Obj_._Table, sizeof(long) * TABLESIZE);
}

/****************************************************************************/
Boolean PrimesTable::TableBuffer::RetrieveBlock(Ulong Index_, long Offset_, fstream& Fin_)
{
  Ulong BlockNum_ = Index_ / PrimesTable::TABLESIZE;

  Fin_.seekg(0, ios::end);
  Fin_.clear();

  long StartPos_ = BlockNum_ * TABLESIZE * sizeof(long);
  long Fsize_ = Fin_.tellg();
  Fsize_ -= Offset_;

  if (StartPos_ < Fsize_)    
    if (Fin_.seekg(StartPos_ + Offset_).good())
    {
      Fin_.read((char*)_Table, TABLESIZE * sizeof(long));

      size_t i;
      long BufSize_ = TABLESIZE;

      for (i = TABLESIZE - 1; i != size_t(-1); --i)
        if (!_Table[i])
          --BufSize_;
        else
          break;

      if (BufSize_)
      {
        StartPos_ /= sizeof(long);
        SetLimit(StartPos_, BufSize_);
        return Fin_.good();
      }
    }

  return FALSE;
}

/****************************************************************************/
Ulong PrimesTable::TableBuffer::GetNumber(Ulong Index_)
{
  if (!InRange(Index_))
    return 0;
    
  Index_ -= LowBound();
  return _Table[Index_];
}

/****************************************************************************/
/****************************************************************************/
PrimesTable::PrimesTable():
_DataFile("primestable.dat"),
_Offset(0),
_TotalSize(0),
_TableIndex(0),
_Bitv(100001)
{
  PATHTOKENIZER(Pathtok_, "PATH");
  size_t Max_ = Pathtok_.Max();
  size_t i;

  char* AbsPath_ = new_char_array(300 + strlen(_DataFile), _DataFile);    
  _File.open(AbsPath_, ios::in | ios::binary);
  
  for (i = 0; !_File.good() && i < Max_; i++)
  {
    strcpy(AbsPath_, Pathtok_[i]);
    strcat(AbsPath_, Pathtok_.DirSeparator());
    strcat(AbsPath_, _DataFile);
    _File.clear();
    _File.open(AbsPath_, ios::in | ios::binary);
  }
  
  if (!_File.good())
  {
    _File.open(_DataFile, ios::out | ios::in | ios::binary);

    if (_File.good())
    {
      _Bitv.BinaryWrite(_File);
      _File.write((char*)(&_TotalSize), sizeof(long));
      _Offset = _File.tellp();
      GenerateTable();
    }
  }
  else
  {
    _Bitv.BinaryRead(_File);
    _File.read((char*)(&_TotalSize), sizeof(long));
    _Offset = _File.tellg();    
    _File.seekg(_Offset);
  }

  ::DeleteArray(AbsPath_);  
}

/****************************************************************************/
PrimesTable::~PrimesTable()
{
  _File.close();
}

/****************************************************************************/
void PrimesTable::StoreTable()
{
  _File.seekp(0, ios::end);
  _File.clear();

  _File.write((char*)_CurTable._Table, sizeof(long) * TABLESIZE);
  memset(_CurTable._Table, 0x00, sizeof(long) * TABLESIZE);
  _TableIndex = 0;  
}

/****************************************************************************/
Ulong PrimesTable::GetTableElement(Ulong Index_, PrimesTable::TableBuffer& TempTable_)
{
  if (!TempTable_._Limit || !TempTable_.InRange(Index_))
    TempTable_.RetrieveBlock(Index_, _Offset, _File);

  return TempTable_.GetNumber(Index_);
}

/****************************************************************************/
Ulong PrimesTable::GetTableElementOnGenerate(Ulong Index_, PrimesTable::TableBuffer& TempTable_)
{
  Ulong Num_;
  Ulong LowBound_ = (_TotalSize < TABLESIZE) ? 0:(_TotalSize - _TableIndex);
  
  if (Index_ < LowBound_)
    return GetTableElement(Index_, TempTable_);
  else
  {
    Index_ -= LowBound_;
    Num_ = _CurTable._Table[Index_];
  }

  return Num_;
}

/****************************************************************************/
void PrimesTable::GenerateTableHelper(Ulong Number_, PrimesTable::TableBuffer& TempTable_)
{
  Boolean NotPrime_;
  Ulong Index_, Max_;
  Ulong Divisor_;

  Max_ = Ulong(sqrt(Number_));
  NotPrime_ = FALSE;
  
  for (Index_ = 1; Index_ < _TotalSize && !NotPrime_; ++Index_)
  {
    Divisor_ = GetTableElementOnGenerate(Index_, TempTable_);
    
    if (Divisor_ > Max_)
      break;
    else
      NotPrime_ = Number_ % Divisor_ == 0;
  }

  if (!NotPrime_)
  {
    _Bitv.SetBit(Number_);
    _CurTable._Table[_TableIndex++] = Number_;
    ++_TotalSize;
  }
}

/****************************************************************************/    
void PrimesTable::InitTable()
{
  _CurTable._Table[0] = 1;
  _CurTable._Table[1] = 2;
  _CurTable._Table[2] = 3;
  _CurTable._Table[3] = 5;
  _CurTable._Table[4] = 7;
  _CurTable._Table[5] = 11;
  _CurTable._Table[6] = 13;
  _CurTable._Table[7] = 17;
  _CurTable._Table[8] = 19;
  _TotalSize = _TableIndex = 9;

  _Bitv.SetBit(1);
  _Bitv.SetBit(2);
  _Bitv.SetBit(3);
  _Bitv.SetBit(5);
  _Bitv.SetBit(7);
  _Bitv.SetBit(11);
  _Bitv.SetBit(13);
  _Bitv.SetBit(17);
  _Bitv.SetBit(19);
}

/****************************************************************************/    
void PrimesTable::GenerateTable()
{
  PrimesTable::TableBuffer TempTable_;
  Ulong Number_;

  InitTable();
  
  for (Number_ = LOOKAHEAD; Number_ <= MAXENTRIES; ++Number_)
  {
    GenerateTableHelper(Number_, TempTable_);

    if (_TableIndex && _TableIndex % TABLESIZE == 0)
      StoreTable();
  }

  StoreTable();
  _File.seekp(0);
  _Bitv.BinaryWrite(_File);
  _File.write((char*)(&_TotalSize), sizeof(long));
}

/****************************************************************************/
Boolean PrimesTable::SieveFindPrime(Ulong Number_)
{
  Boolean NotPrime_;
  Ulong Index_, Max_;
  Ulong Divisor_;

  Max_ = Ulong(sqrt(Number_));
  NotPrime_ = FALSE;
  
  for (Index_ = 1; Index_ < _TotalSize && !NotPrime_; ++Index_)
  {
    Divisor_ = GetTableElement(Index_, _CurTable);
    
    if (Divisor_ > Max_)
      break;
    else
      NotPrime_ = Number_ % Divisor_ == 0;
  }

  return !NotPrime_;
}

/****************************************************************************/
Ulong PrimesTable::MaxPrimes()
{
  return _TotalSize;
}

/****************************************************************************/    
Ulong PrimesTable::Ceiling(double Value_)
{
  Ulong Num_, i;
  Num_ = Ulong(ceil(Value_));
  
  if (Num_ <= MAXENTRIES)
  {
    for (i = Num_; i <= MAXENTRIES; ++i)
      if (_Bitv[i])
        return i;
  }
  else
    i = Num_;

  Ulong Max_ = 4294967295UL;
  for (;i < Max_; ++i)
    if (SieveFindPrime(i))
      return i;

  return 0;  
}

/****************************************************************************/    
Ulong PrimesTable::Floor(double Value_)
{
  Ulong Num_, i;
  Num_ = Ulong(floor(Value_));

  if (Num_ > MAXENTRIES)
  {
    for (i = Num_; i > MAXENTRIES; --i)
      if (SieveFindPrime(i))    
        return i;  
  }
  else
    i = Num_;

  for (;i != Ulong(-1); --i)
    if (_Bitv[i])  
      return i;

  return 0;
}

/****************************************************************************/    
Ulong PrimesTable::NthPrime(Ulong Index_, Boolean* Found_)
{
  if (Found_)
    *Found_ = TRUE;
    
  if (Index_ < _TotalSize)
    return GetTableElement(Index_, _CurTable);

  Ulong Max_ = 4294967295UL;
  Ulong Cnt_ = _TotalSize;
  Ulong i;
  
  for (i = MAXENTRIES + 1; i < Max_; ++i)
    if (SieveFindPrime(i))
      if (Cnt_ == Index_)
        return i;
      else
        ++Cnt_;

  if (Found_)
    *Found_ = FALSE;
        
  return 0;
}

/****************************************************************************/    
Ulong PrimesTable::NextPrimeFactor(Ulong Number_, Ulong& Quotient_, Ulong& Exponent_)
{
  Boolean NotPrime_;
  Ulong Index_, Max_;
  Ulong Divisor_;

  Max_ = Ulong(sqrt(Number_));
  NotPrime_ = FALSE;
  Exponent_ = 0;

  if (Max_ > MAXENTRIES)
    return 0;
    
  for (Index_ = 1; Index_ < _TotalSize && !NotPrime_; ++Index_)
  {
    Divisor_ = GetTableElement(Index_, _CurTable);
    
    if (Divisor_ > Max_)
      break;
    else
    {
      NotPrime_ = Number_ % Divisor_ == 0;
        
      if (NotPrime_)
      {
        do
        {
          Number_ /= Divisor_;
          ++Exponent_;
        }
        while (Number_ % Divisor_ == 0);
        Quotient_ = Number_;
      }
    }
  }

  return (NotPrime_ ? Divisor_:Number_);
}

/****************************************************************************/
Boolean PrimesTable::IsPrime(Ulong Num_, Boolean* Computable_)
{
  Ulong Max_ = Ulong(sqrt(Num_));

  if (Max_ > MAXENTRIES)
  {
    if (Computable_)
      *Computable_ = FALSE;
  }
  else if (Num_ > MAXENTRIES)
    return SieveFindPrime(Num_);
  else
    return _Bitv[Num_];

  return 0;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* PrimesTable::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/    
void PrimesTable::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* PrimesTable::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void PrimesTable::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if PRIMESTABLE_DEBUG1

Boolean VerifyPrime(Ulong x)
{
  Ulong max_ = Ulong(sqrt(x));
  Ulong i;
  Ulong notprime_ = 0;

  for (i = 2; i <= max_ && !notprime_; ++i)
    if (i > max_)
      break;
    else
      notprime_ = x % i == 0;

  return !notprime_;
}

Boolean VerifyPrimeFactors(PrimesTable& Tbl_, Ulong* Arr_)
{
  Ulong Factor_, Count_;
  Ulong Quotient_;
  Ulong Value_ = 1;
  int i, x;

  for (x = 0; x < 2; ++x)
    for (i = 0; i < 4; ++i)
      Value_ *= Arr_[i];

  for (i = 0; i < 4; ++i)
  {
    Factor_ = Tbl_.NextPrimeFactor(Value_, Quotient_, Count_);
    Value_ = Quotient_;
    
    if (Factor_ != Arr_[i] || Count_ != 2)
      return 0;
  }

  return 1;
}

int main()
{
  const Ulong TESTMAX = 10000;

  PrimesTable Tbl_;
  Ulong Value_, nthprime_, prevprime_, nextprime_;
  int i, x, y, Max_, Limit_;
  Boolean Error_, Verified_, nthok_, ceilfloorok_;
  char* msg_;

  Max_ = TESTMAX;
  Verified_ = TRUE;
  x = 0; y = 2; i = 0;
  nextprime_ = prevprime_ = 0;
  Limit_ = Max_ * 2;    
  Error_ = FALSE;

#if PRIMESTABLE_DEBUG_OUTPUT
  for (x = 2; x <= Limit_; ++x)
    if (Tbl_.IsPrime(x))
      cout <<"\t" <<x;      
      
  cout <<endl <<endl;
#endif
  x = 0;

  for (y = 2; y <= Limit_; Max_ *= 2)
  {
    for (i = y; i <= Max_; ++i)
    {
      if (Tbl_.IsPrime(i))
      {
#if PRIMESTABLE_DEBUG_OUTPUT      
        cout <<"\t" <<i;
#endif
    
        ++x;
        nthprime_ = Tbl_.NthPrime(x);
        nthok_ = nthprime_ == i;

        if (prevprime_)
          ceilfloorok_ = Tbl_.Ceiling(prevprime_ + .5) == nthprime_ &&
                         Tbl_.Floor(nthprime_ - .5) == prevprime_;
        else
          ceilfloorok_ = Tbl_.Ceiling(nthprime_ - .5) == nthprime_;

        if (nextprime_)
          ceilfloorok_ = ceilfloorok_ && nextprime_ == nthprime_;

        nextprime_ = Tbl_.Ceiling(nthprime_ + .5);        
        prevprime_ = nthprime_;

        Verified_ = VerifyPrime(i) && nthok_ && ceilfloorok_;
        msg_ = "prime";
      }
      else
      {
        Verified_ = !VerifyPrime(i);
        msg_ = "not prime";
      }

      if (!Verified_)
      {
        Error_ = TRUE;
        break;
      }
    }

    if (Error_)
      break;
      
    y = Max_;
  }

#if PRIMESTABLE_DEBUG_OUTPUT
  cout <<endl <<endl;
#endif
  Ulong pfarr_[10] = {3, 5, 7, 11};
  Boolean primefactorsok_;
  primefactorsok_ = VerifyPrimeFactors(Tbl_, pfarr_);
  Verified_ = Verified_ && primefactorsok_;
  
  if (!Verified_)
    cout <<"Error: generating primes: " <<endl
         <<"\t: " <<i <<" is " <<msg_ <<endl
         <<"\t: nthok = " <<nthok_ <<endl
         <<"\t: nthprime = " <<nthprime_ <<endl
         <<"\t: ceilfloorok = " <<ceilfloorok_ <<endl
         <<"\t: primefactorsok = " <<primefactorsok_ <<endl
         <<endl;
  else
    cout <<"Prime number generation verified" <<endl;
    
  return 0;
}
#else
#if PRIMESTABLE_DEBUG2
void main()
{
  PrimesTable Tbl_;
//  Tbl_.IsPrime(100000UL);
}
#endif
#endif

/****************************************************************************/
#endif




