#ifndef LONGNUM_CPP
#define LONGNUM_CPP
#ifndef LONGNUM_H
  #include "longnum.h"
#endif

const LongNumber* LongNumber::p_E = NULL;
const LongNumber* LongNumber::p_PI = NULL;
const LongNumber* LongNumber::p_ROOT2 = NULL;
const LongNumber* LongNumber::p_ROOT3 = NULL;
const LongNumber* LongNumber::p_NAN = NULL;
const LongNumber* LongNumber::p_INFINITY = NULL;
long LongNumber::_Instances = 0;
int LongNumber::_OmitConsts = 0;
int LongNumber::_UseSeries = 0;
long LongNumber::_DefaultPrecision = LongNumber::DIV_PRECISION;
const char* LongNumber::_DataFile = "longnumconsts.dat";

/****************************************************************************/
/****************************************************************************/
DecimalPoint::DecimalPoint():
_RightShift(0),
_DecExponent(0),
_IncExponent(0)
{}

/****************************************************************************/
DecimalPoint::DecimalPoint(const char* NumStr_):
_RightShift(0),
_DecExponent(0),
_IncExponent(0)
{
  // Non-decimal bases are only valid for integers
  int Radix_ = FindRadix(NumStr_);
  
  if (Radix_ == 10)
  {
    Ulong i, Max_;
    ptrdiff_t Epos_;
  
    char* NewStr_ = RemoveSciNoteSymbol(NumStr_, Epos_);
    const char* Wptr_ = NewStr_ ? NewStr_:NumStr_;
    Max_ = strlen(Wptr_);

    // Find decimal point and shift it to the right to make
    // number an integer.
    for (i = 0; i < Max_; ++i)
     if (Wptr_[i] == '.')
      {
        ++i;
        _RightShift = Max_ - i;
        break;
      }

    // Restore sci. note. symbol and correct decimal shift for it
    if (NewStr_)
    {
      NewStr_[Epos_] = 'e';
      SciNoteDecShift(Wptr_);
      ::DeleteArray(NewStr_);
    }
  }
}

/****************************************************************************/
DecimalPoint::DecimalPoint(const DecimalPoint& Obj_):
_RightShift(Obj_._RightShift),
_DecExponent(Obj_._DecExponent),
_IncExponent(Obj_._IncExponent)
{}

/*******.********************************************************************/
DecimalPoint& DecimalPoint::operator = (const DecimalPoint& Obj_)
{
  if (this != &Obj_)
  {
    _RightShift = Obj_._RightShift;
    _DecExponent = Obj_._DecExponent;
    _IncExponent = Obj_._IncExponent;    
  }
  
  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::operator = (const char* NumStr_)
{
  // Initialize data members to zero
  _RightShift = 0;
  _DecExponent = 0;
  _IncExponent = 0;

  // Non-decimal bases are only valid for integers
  int Radix_ = FindRadix(NumStr_);

  if (Radix_ == 10)
  {
    Ulong i, Max_;
    ptrdiff_t Epos_;

    // Find decimal point and shift it to the right to make
    // number an integer.
    char* NewStr_ = RemoveSciNoteSymbol(NumStr_, Epos_);
    const char* Wptr_ = NewStr_ ? NewStr_:NumStr_;
    Max_ = strlen(Wptr_);

    // Find decimal point and shift it to the right to make
    // number an integer.
    for (i = 0; i < Max_; ++i)
      if (Wptr_[i] == '.')
      {
        ++i;
        _RightShift = Max_ - i;
        break;
      }

    // Restore sci. note. symbol and correct decimal shift for it
    if (NewStr_)
    {
      NewStr_[Epos_] = 'e';
      SciNoteDecShift(Wptr_);
      ::DeleteArray(NewStr_);
    }
  }
  
  return *this;
}

/****************************************************************************/
char* DecimalPoint::RemoveSciNoteSymbol(const char* ptr_, ptrdiff_t& Epos_)
{
  // Remove scientific notation symbols if base is 10  
  char* newp_ = NULL;  
  Boolean Found_ = (newp_ = strchr(ptr_, 'e')) || (newp_ = strchr(ptr_, 'E'));
  
  if (Found_)
  {
    Epos_ = (newp_ - ptr_);
    newp_ = new_char_string(ptr_);
    newp_[Epos_] = 0;
  }
  else
    Epos_ = 0;

  return newp_;
}

/****************************************************************************/
void DecimalPoint::SciNoteDecShift(const char* NumStr_)
{
  char* ptr_;

  if ((ptr_ = strchr(NumStr_, 'e')) ||
      (ptr_ = strchr(NumStr_, 'E')))
  {
    ++ptr_;
    
    if (*ptr_)
    {
      int Sign_ = 0;
      
      if (*ptr_ == '-')
      {
        Sign_ = -1;
        ++ptr_;
      }
      else if (*ptr_ == '+')
      {
        Sign_ = 1;
        ++ptr_;      
      }

      if (*ptr_ && isdigit(*ptr_))
      {
        int Shift_ = atoi(ptr_);
        
        if (!Sign_)
          Sign_ = 1;

        if (Sign_ > 0)        
        {        
          if (Shift_ <= _RightShift)
            _RightShift -= Shift_;
          else
          {
            Shift_ -= _RightShift;
            _RightShift = 0;
            IncreaseExponent(Shift_);
          }          
        }
        else
          DecreaseExponent(Shift_);
      }
    }
  }
}

/****************************************************************************/
DecimalPoint& DecimalPoint::DecreaseExponent(Ulong Shift_, Boolean Back_)
{
  if (Back_)
    _DecExponent -= (Shift_ > _DecExponent) ? _DecExponent:Shift_;
  else
    _DecExponent += Shift_;

  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::IncreaseExponent(Ulong Shift_, Boolean Back_)
{
  if (Back_)
    _IncExponent -= (Shift_ > _IncExponent) ? _IncExponent:Shift_;
  else
    _IncExponent += Shift_;

  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::ShiftRight(Ulong Shift_, Boolean Back_)
{
  if (Back_)
    _RightShift -= (Shift_ > _RightShift) ? _RightShift:Shift_;
  else
    _RightShift += Shift_;
    
  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::SetDecExponent(Ulong Shift_)
{
  _DecExponent = Shift_;
  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::SetIncExponent(Ulong Shift_)
{
  _IncExponent = Shift_;
  return *this;
}

/****************************************************************************/
DecimalPoint& DecimalPoint::SetRightShift(Ulong Shift_)
{
  _RightShift = Shift_;
  return *this;
}

/****************************************************************************/
Boolean DecimalPoint::operator == (const DecimalPoint& Obj_) const
{
  return (_RightShift == Obj_._RightShift);
}

/****************************************************************************/
Boolean DecimalPoint::operator != (const DecimalPoint& Obj_) const
{
  return (_RightShift != Obj_._RightShift);
}

/****************************************************************************/
Boolean DecimalPoint::operator < (const DecimalPoint& Obj_) const
{
  return (_RightShift < Obj_._RightShift);
}

/****************************************************************************/
Boolean DecimalPoint::operator > (const DecimalPoint& Obj_) const
{
  return (_RightShift > Obj_._RightShift);
}

/****************************************************************************/
Boolean DecimalPoint::operator <= (const DecimalPoint& Obj_) const
{
  return (_RightShift <= Obj_._RightShift);
}

/****************************************************************************/
Boolean DecimalPoint::operator >= (const DecimalPoint& Obj_) const
{
  return (_RightShift >= Obj_._RightShift);
}

/****************************************************************************/
long DecimalPoint::DataSize () const
{
  return (3 * sizeof(long));
}

/****************************************************************************/    
Boolean DecimalPoint::BinaryRead(istream& Is_, Boolean* Ok_)
{
  if (*Ok_)
  {
    *Ok_ =
    (Is_.read((char*)(&_RightShift), sizeof(long)).good() &&
     Is_.read((char*)(&_DecExponent), sizeof(long)).good() &&
     Is_.read((char*)(&_IncExponent), sizeof(long)).good());
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean DecimalPoint::BinaryWrite(const LongNumber* Refer_, ostream& Os_, Boolean* Ok_) const
{
  if (Refer_->OstreamCheck(Os_, Ok_))
  {
    Os_.write((char*)(&_RightShift), sizeof(long));
    if (Refer_->OstreamCheck(Os_, Ok_))
    {
      Os_.write((char*)(&_DecExponent), sizeof(long));
      if (Refer_->OstreamCheck(Os_, Ok_))
        Os_.write((char*)(&_IncExponent), sizeof(long));
    }
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean DecimalPoint::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  if (*Ok_)
  {
    *Ok_ =
    (Is_.iread((char*)(&_RightShift), sizeof(long)).good() &&
     Is_.iread((char*)(&_DecExponent), sizeof(long)).good() &&
     Is_.iread((char*)(&_IncExponent), sizeof(long)).good());
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean DecimalPoint::BinaryWrite(const LongNumber* Refer_, ByteStream& Os_, Boolean* Ok_) const
{
  if (Refer_->OstreamCheck(((ostream&)Os_), Ok_))
  {  
    Os_.owrite((char*)(&_RightShift), sizeof(long));
    if (Refer_->OstreamCheck(((ostream&)Os_), Ok_))
    {
      Os_.owrite((char*)(&_DecExponent), sizeof(long));
      if (Refer_->OstreamCheck(((ostream&)Os_), Ok_))
        Os_.owrite((char*)(&_IncExponent), sizeof(long));
    }
  }

  return *Ok_;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* DecimalPoint::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void DecimalPoint::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
LongNode::LongNode():
_BcdStr(0),
_Prev(NULL),
_Next(NULL)
{}

/****************************************************************************/
LongNode::LongNode(LongNode* CurLink_, LongNode* PrevLink_):
_BcdStr(CurLink_ ? CurLink_->_BcdStr:0),
_Prev(PrevLink_),
_Next((CurLink_ && CurLink_->_Next) ? (new LongNode(CurLink_->_Next, this)):NULL)
{}

/****************************************************************************/
LongNode::LongNode(Ulong NumVal_):
_BcdStr(NumVal_),
_Prev(NULL),
_Next(NULL)
{}

/****************************************************************************/
LongNode::LongNode(const char* NumStr_, Ushort Radix_):
_BcdStr(LongNode::FindBcdStrFromChrStr(NumStr_, Radix_)),
_Prev(NULL),
_Next(NULL)
{}

/****************************************************************************/
LongNode::~LongNode()
{
  _Prev = NULL;
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
Ulong LongNode::FindBcdStrFromChrStr(const char* NumStr_, Ushort Radix_)
{
  Ulong Value_;
  Ulong i, Max_;

  // Maximum number of bcd digits contained in 4 byte long is 8
  // in a 8 byte long it is 16
  Max_ = strlen(NumStr_);
  if (Max_ > BcdDigit::DigitsPerLong())
    Max_ = BcdDigit::DigitsPerLong();
    
  // Fill long data member with digits from string converted into BCD
  // If a non-digit character is found in the string then return 0
  Value_ = 0;  
  for (i = 0; i < Max_; ++i)
    if (!IsValidDigit(NumStr_[i], Radix_))
      return 0;
    else
      Value_ |= LongNumber::CharToBcd(NumStr_[i], Max_ - i - 1);

  return Value_;
}

/****************************************************************************/
Ulong LongNode::TotalDigits() const
{
  Ulong Zeros_ = 0;
  Ulong NumDigits_ = 0;
  Ulong Mask_ = 0x0F;
  Ulong i, Max_;

  // Count up to the total number of digits contained within this node
  // Intervening zeros are also counted, but not leading zeros which are
  // ignored. We use a mask for a bcd digit and left shift to every position
  // within the long which can hold a digit.
  Max_ = BcdDigit::DigitsPerLong();
  for (i = 0; i < Max_; ++i, Mask_ <<= 4)
    if (_BcdStr & Mask_)
    {
      NumDigits_ += 1 + Zeros_;
      Zeros_ = 0;
    }
    else
      ++Zeros_;

  return NumDigits_;
}

/****************************************************************************/
LongNode* LongNode::AddNodeWithValue(Ulong NumVal_)
{
  // When a new node is added, it is assumed that the new number string
  // passed to the method is a continuation of the current string starting at
  // the base address of a char* and ending at the null terminator. In which
  // case the fragments of the number are given in reverse order from
  // largest to smallest and therefore the chain to the new LongNode object
  // must also follow in the same order with the new object referred to as
  // the predecessor of the current object.

  _Prev = new LongNode(NumVal_);
  _Prev->_Next = this;
  return _Prev;
}

/****************************************************************************/
LongNode* LongNode::AddNodeWithStr(const char* NumStr_, Ushort Radix_)
{
  // When a new node is added, it is assumed that the new number string
  // passed to the method is a continuation of the current string starting at
  // the base address of a char* and ending at the null terminator. In which
  // case the fragments of the number are given in reverse order from
  // largest to smallest and therefore the chain to the new LongNode object
  // must also follow in the same order with the new object referred to as
  // the predecessor of the current object.
  
  _Prev = new LongNode(NumStr_, Radix_);
  _Prev->_Next = this;
  return _Prev;
}

/****************************************************************************/
LongNode* LongNode::AppendNodeWithValue(Ulong NumVal_)
{
  _Next = new LongNode(NumVal_);
  _Next->_Prev = this;
  return _Next;
}

/****************************************************************************/
LongNode* LongNode::AppendNodeWithStr(const char* NumStr_, Ushort Radix_)
{
  _Next = new LongNode(NumStr_, Radix_);
  _Next->_Prev = this;
  return _Next;
}

/****************************************************************************/
BcdDigit LongNode::GetDigit(Ulong Exp_) const
{
  // Return the bcd digit at the given exponent. For a LongNode object
  // the exponent range is 0 to 7 since a 4 byte long can only store a
  // maximum of 8 bcd digits. Higher exponents are wrapped around. For
  // example 8 would become 0 and 10 would become 2. Similarly for a 8 byte
  // long, but with the range set at 0 to 15
  
  BcdDigit Digit_(_BcdStr, Exp_);
  return Digit_;
}

/****************************************************************************/
void LongNode::SetDigit(const BcdDigit& Digit_, Ulong Exp_)
{
  // only interested in first 4 bits of char
  char BcdVal_ = Digit_.Digit() & 0x0F;

  // 4 byte long can only hold 8 bcd digits so exponents greater than
  // 7 will wrap-around. (exponents greater than 15 for a 8 byte long).
  Exp_ %= BcdDigit::DigitsPerLong();

  // Clear former digit at specified exponent position
  _BcdStr &= ~(0x0F << (Exp_ * 4));

  // Set digit into bcd string
  _BcdStr |= BcdVal_ << (Exp_ * 4);
}

/****************************************************************************/
void LongNode::SetCarry(const BcdDigit& Digit_, Ulong Exp_)
{
  // only interested in first 4 bits of char
  char Carry_ = Digit_.Carry() & 0x0F;

  // 4 byte long can only hold 8 bcd digits so exponents greater than
  // 7 will wrap-around. (exponents greater than 15 for a 8 byte long).
  Exp_ %= BcdDigit::DigitsPerLong();

  // Clear former digit at specified exponent position
  _BcdStr &= ~(0x0F << (Exp_ * 4));

  // Set digit into bcd string
  _BcdStr |= Carry_ << (Exp_ * 4);
}

/****************************************************************************/
long LongNode::DataSize () const
{
  return
  (
    sizeof(long) +
    (_Next ? _Next->DataSize():0)
  );
}

/****************************************************************************/    
Boolean LongNode::BinaryRead(istream& Is_, Boolean* Ok_)
{
  if (*Ok_)
  {
    *Ok_ = Is_.read((char*)(&_BcdStr), sizeof(long)).good();    
    if (_Next && *Ok_)
      *Ok_ = _Next->BinaryRead(Is_, Ok_);
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean LongNode::BinaryWrite(const LongNumber* Refer_, ostream& Os_, Boolean* Ok_) const
{
  if (Refer_->OstreamCheck(Os_, Ok_))
  {
    Os_.write((char*)(&_BcdStr), sizeof(long));
    if (Refer_->OstreamCheck(Os_, Ok_) && _Next)
      *Ok_ = _Next->BinaryWrite(Refer_, Os_, Ok_);
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean LongNode::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  if (*Ok_)
  {
    *Ok_ = Is_.iread((char*)(&_BcdStr), sizeof(long)).good();
    if (_Next && *Ok_)
      *Ok_ = _Next->BinaryRead(Is_, Ok_);
  }

  return *Ok_;
}

/****************************************************************************/    
Boolean LongNode::BinaryWrite(const LongNumber* Refer_, ByteStream& Os_, Boolean* Ok_) const
{
  if (Refer_->OstreamCheck(((ostream&)Os_), Ok_))
  {
    Os_.owrite((char*)(&_BcdStr), sizeof(long));
    if (Refer_->OstreamCheck(((ostream&)Os_), Ok_) && _Next)
      *Ok_ = _Next->BinaryWrite(Refer_, Os_, Ok_);
  }

  return *Ok_;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* LongNode::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void LongNode::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
LongNumber::LongNumber():
MixedNumberType<LongNumber, ChrString>(0),
_Sign(NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif

  FeedNumberValue(0);
}

/****************************************************************************/
LongNumber::LongNumber(long Value_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((Value_ < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;  
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
      
  Value_ = ::labs(Value_);
  FeedNumberValue(Value_);
}

/****************************************************************************/
LongNumber::LongNumber(Ulong Value_, int Sign_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((Sign_ < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;      
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  FeedNumberValue(Value_);
}

/****************************************************************************/
LongNumber::LongNumber(const LongNumber& Obj_):
MixedNumberType<LongNumber, ChrString>(Obj_),
_Sign(Obj_._Sign),
_NumNodes(Obj_._NumNodes),
_DivPrecision(Obj_.GetPrecision()),
_Base(Obj_._Base),
_SciNote(Obj_._SciNote),
_Head(Obj_._Head ? (new LongNode(Obj_._Head, Obj_._Head ? Obj_._Head->GetPrevious():NULL)):NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint(*Obj_._DecimalPt)),
_StringRep(NULL),
_NanVal(Obj_._NanVal),
_InfVal(Obj_._InfVal)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;      
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif

  _Tail = FindTail();
}

/****************************************************************************/
LongNumber::LongNumber(const ChrString& NumStr_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((*NumStr_ == '-') ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision((NumStr_.strlen() > LongNumber::_DefaultPrecision) ?
              NumStr_.strlen():LongNumber::_DefaultPrecision),
_Base(FindRadix(NumStr_.c_str())),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint(NumStr_.c_str())),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;
  
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif

  ChrString Temp_ = NumStr_;
  Temp_.ToLower();
  bool Neg_ = false;

  if (Temp_ == "nan")
    *this = vNAN();
  else if ((Temp_ == "inf" || Temp_ == "+inf") || (Neg_ = Temp_ == "-inf"))
  {
    LongNumber Lnum_ = Neg_ ? -vINFINITY():vINFINITY();
    *this = Lnum_;
  }
  else
  {
    // Check if valid base
    // Convert character string into bcd string linked list.
    // Default to 0 in base 10 on error (invalid input).
    // Non-decimal bases are only valid for integers
    if (!_Base || _Base != 10 && SafeStrChr(NumStr_.c_str(), '.'))
    {
      _Base = 10;
      FeedNumberValue(0);
    }
    else
      FeedNumberStr(NumStr_.c_str());
  }
}

/****************************************************************************/
LongNumber::LongNumber(const LongDouble& FltObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(NULL),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION);
    _DivPrecision = (TempStr_.strlen() > LongNumber::_DefaultPrecision) ?
                       TempStr_.strlen():
                       LongNumber::_DefaultPrecision;
    _DecimalPt = new DecimalPoint(TempStr_.c_str());

    LongNumber::_Instances++;
    _OriginalBase = _Base;
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif
  
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    LongNumber::_Instances++;
    _OriginalBase = _Base;
    
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif

    if (FltObj_.IsNan())
      *this = vNAN();
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      *this = Lnum_;
    }      
  }
}

/****************************************************************************/
LongNumber::LongNumber(const Double& FltObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(NULL),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(Double::TYPE_DOUBLE_PRECISION);
    _DivPrecision = (TempStr_.strlen() > LongNumber::_DefaultPrecision) ?
                       TempStr_.strlen():
                       LongNumber::_DefaultPrecision;
    _DecimalPt = new DecimalPoint(TempStr_.c_str());

    LongNumber::_Instances++;
    _OriginalBase = _Base;
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif
  
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    LongNumber::_Instances++;
    _OriginalBase = _Base;
    
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif

    if (FltObj_.IsNan())
      *this = vNAN();
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      *this = Lnum_;
    }      
  }  
}

/****************************************************************************/
LongNumber::LongNumber(const Float& FltObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(NULL),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(Float::TYPE_FLOAT_PRECISION);
    _DivPrecision = (TempStr_.strlen() > LongNumber::_DefaultPrecision) ?
                       TempStr_.strlen():
                       LongNumber::_DefaultPrecision;
    _DecimalPt = new DecimalPoint(TempStr_.c_str());

    LongNumber::_Instances++;
    _OriginalBase = _Base;
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif
  
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    LongNumber::_Instances++;
    _OriginalBase = _Base;
    
    if (LongNumber::_Instances == 1)
      #if READ_PRESETS
        ReadPresets();
      #else
        StoreConstants();
      #endif

    if (FltObj_.IsNan())
      *this = vNAN();
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      *this = Lnum_;
    }
  }
}

/****************************************************************************/
LongNumber::LongNumber(const ShortInt& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((IntObj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;  
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  if (_Sign == NumberSign::MINUS && IntObj_.AtMinValue())
  {
    ChrString TempStr_ = IntObj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -IntObj_.IntValue():IntObj_.IntValue();
    FeedNumberValue(Value_);
  }  
}

/****************************************************************************/
LongNumber::LongNumber(const LongInt& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((IntObj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;  
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  if (_Sign == NumberSign::MINUS && IntObj_.AtMinValue())
  {
    ChrString TempStr_ = IntObj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -IntObj_.IntValue():IntObj_.IntValue();
    FeedNumberValue(Value_);
  }
}

/****************************************************************************/
LongNumber::LongNumber(const Integer& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((IntObj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;  
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  if (_Sign == NumberSign::MINUS && IntObj_.AtMinValue())
  {
    ChrString TempStr_ = IntObj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -IntObj_.IntValue():IntObj_.IntValue();
    FeedNumberValue(Value_);
  }
}

/****************************************************************************/
LongNumber::LongNumber(const UShortInt& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign(NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;      
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  Ulong Value_ = IntObj_.UnsignedValue();
  FeedNumberValue(Value_);
}

/****************************************************************************/
LongNumber::LongNumber(const ULongInt& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign(NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;      
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  Ulong Value_ = IntObj_.UnsignedValue();
  FeedNumberValue(Value_);
}

/****************************************************************************/
LongNumber::LongNumber(const UInteger& IntObj_):
MixedNumberType<LongNumber, ChrString>(0),
_Sign(NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(new DecimalPoint),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  LongNumber::_Instances++;
  _OriginalBase = _Base;      
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  Ulong Value_ = IntObj_.UnsignedValue();
  FeedNumberValue(Value_);
}

/****************************************************************************/
LongNumber::LongNumber(const Double& FltObj_, int, int, int):
MixedNumberType<LongNumber, ChrString>(0),
_Sign((FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS),
_NumNodes(0),
_DivPrecision(LongNumber::_DefaultPrecision),
_Base(10),
_SciNote(FALSE),
_Head(NULL),
_Tail(NULL),
_DecimalPt(NULL),
_StringRep(NULL),
_NanVal(false),
_InfVal(false)
{
  ChrString TempStr_ = FltObj_.ToString(Double::TYPE_DOUBLE_PRECISION);
  _DivPrecision = (TempStr_.strlen() > LongNumber::_DefaultPrecision) ?
                       TempStr_.strlen():
                       LongNumber::_DefaultPrecision;
  _DecimalPt = new DecimalPoint(TempStr_.c_str());

  LongNumber::_Instances++;
  _OriginalBase = _Base;
  if (LongNumber::_Instances == 1)
    #if READ_PRESETS
      ReadPresets();
    #else
      StoreConstants();
    #endif
  
  FeedNumberStr(TempStr_.c_str());
}

/****************************************************************************/
LongNumber::~LongNumber()
{
  Clear();
  delete _StringRep;  
  LongNumber::_Instances--;

  if (!LongNumber::_Instances)
  {
    delete LongNumber::p_E;
    delete LongNumber::p_PI;
    delete LongNumber::p_ROOT2;
    delete LongNumber::p_ROOT3;
    delete LongNumber::p_NAN;
    delete LongNumber::p_INFINITY;

    LongNumber::p_E =
    LongNumber::p_PI =
    LongNumber::p_ROOT2 =
    LongNumber::p_ROOT3 =
    LongNumber::p_NAN =
    LongNumber::p_INFINITY = NULL;
  }
}

/****************************************************************************/
void LongNumber::SetNan(bool Flag_)
{
  _NanVal = Flag_;
  if (Flag_)
    _InfVal = false;
}

/****************************************************************************/
void LongNumber::SetInfinite(bool Flag_)
{
  _InfVal = Flag_;
  if (Flag_)
    _NanVal = false;    
}

/****************************************************************************/
Boolean LongNumber::IsNan(const LongNumber& Val_)
{
  return Val_._NanVal;
}

/****************************************************************************/
Boolean LongNumber::IsInfinite(const LongNumber& Val_)
{
  return Val_._InfVal;
}

/****************************************************************************/
Boolean LongNumber::IsNan() const
{
  return _NanVal;
}

/****************************************************************************/
Boolean LongNumber::IsInfinite() const
{
  return _InfVal;
}

/****************************************************************************/
const LongNumber& LongNumber::vE()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_E)
  {
    LongNumber::p_E = &LongNumber::GiveNullObject();
    Temp_ = new LongNumber(E_Series(LongNumber(1UL, NumberSign::PLUS)));
    LongNumber::p_E = Temp_;
  }

  if (LongNumber::p_E)
    return *LongNumber::p_E;

  return GiveNullObject();
}

/****************************************************************************/
const LongNumber& LongNumber::vPI()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_PI)
  {
    LongNumber::p_PI = &LongNumber::GiveNullObject();
    Temp_ = new LongNumber(Pi_Series());
    LongNumber::p_PI = Temp_;
  }

  if (LongNumber::p_PI)
    return *LongNumber::p_PI;

  return GiveNullObject();
}

/****************************************************************************/
const LongNumber& LongNumber::vROOT2()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_ROOT2)
  {
    LongNumber::p_ROOT2 = &LongNumber::GiveNullObject();
    Temp_ = new LongNumber(LongNumber::IntRoot(LongNumber(2UL, NumberSign::PLUS), 2));
    LongNumber::p_ROOT2 = Temp_;
  }

  if (LongNumber::p_ROOT2)
    return *LongNumber::p_ROOT2;

  return GiveNullObject();
}

/****************************************************************************/
const LongNumber& LongNumber::vROOT3()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_ROOT3)
  {
    LongNumber::p_ROOT3 = &LongNumber::GiveNullObject();
    Temp_ = new LongNumber(LongNumber::IntRoot(LongNumber(3UL, NumberSign::PLUS), 2));
    LongNumber::p_ROOT3 = Temp_;
  }

  if (LongNumber::p_ROOT3)
    return *LongNumber::p_ROOT3;

  return GiveNullObject();
}

/****************************************************************************/
const LongNumber& LongNumber::vNAN()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_NAN)
  {
    LongNumber::p_NAN = &LongNumber::GiveNullObject();
    Double NanVal_ = Double::vNAN;
    Temp_ = new LongNumber(NanVal_, 0, 0, 0);
    Temp_->SetNan(true);
    LongNumber::p_NAN = Temp_;
  }

  if (LongNumber::p_NAN)
    return *LongNumber::p_NAN;

  return GiveNullObject();
}

/****************************************************************************/
const LongNumber& LongNumber::vINFINITY()
{
  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_INFINITY)
  {
    LongNumber::p_INFINITY = &LongNumber::GiveNullObject();
    Double InfVal_ = Double::vINFINITY;
    Temp_ = new LongNumber(InfVal_, 0, 0, 0);
    Temp_->SetInfinite(true);
    LongNumber::p_INFINITY = Temp_;
  }

  if (LongNumber::p_INFINITY)
    return *LongNumber::p_INFINITY;

  return GiveNullObject();
}

/****************************************************************************/
// PURPOSE:
//   Method to strip leading zeros from the numeric string
//
char* LongNumber::StripLeadingZeros(char* str)
{
  size_t x, l;

  for (l = 0; str[l] && str[l] == '0'; l++);
  for (x = 0; (str[x] = str[l]); x++, l++);

  return str;
}

/****************************************************************************/
char* LongNumber::StripPaddingZeros(char* str, Ushort Radix_)
{
  char* NonZero_ = NULL;
  char* Point_;
  char* Ptr_ = NULL;
  
  for (Point_ = str; *Point_ && *Point_ != '.'; ++Point_)
    if (*Point_ != '0' && IsValidDigit(*Point_, Radix_) && !NonZero_)
    {
      NonZero_ = Point_;
      break;
    }

  if (NonZero_)
    Ptr_ = NonZero_;
  else if (*Point_ && str < Point_)
    Ptr_ = Point_ - 1;

  if (Ptr_ && str < Ptr_)
  {
    size_t Len_ = strlen(Ptr_) + 1;
    memmove(str, Ptr_, Len_);
  }
  else if (*Point_ == 0)
    strcpy(str, "0");

  return str;
}

/****************************************************************************/
// PURPOSE:
//   Method to strip trailing zeros from the numeric string
//
char* LongNumber::StripTrailingZeros(char* NumStr_)
{
  Boolean Found_ = FALSE;
  char* newp_ = NULL;
  char* Point_ = NULL;
  size_t l;
  
  for (;*NumStr_ && *NumStr_ != '.'; ++NumStr_);
  
  if (*NumStr_)
  {
    Point_ = NumStr_;
    ++NumStr_;
  }

  Found_ = (newp_ = strchr(NumStr_, 'e')) || (newp_ = strchr(NumStr_, 'E'));
  ptrdiff_t Epos_ = (newp_ - NumStr_);
  l = Found_ ? Epos_:strlen(NumStr_);

  while (l && (&NumStr_[l] != Point_))
    if (NumStr_[--l] == '0')
      NumStr_[l] = 0;
    else
      break;

  if (Point_ && &Point_[1] >= &NumStr_[l] && !NumStr_[l])
    *Point_ = 0;

  if (Found_ && (&NumStr_[strlen(NumStr_)] < newp_))
    strcat(NumStr_, newp_);

  return NumStr_;
}

/****************************************************************************/
char* LongNumber::Substring(const char* Src_, char* Buf_, size_t Start_, size_t End_)
{
  // return the "chopped" segment of the given string starting at
  // Start_ and ending at End_
  if (strlen(Src_) > Start_ && End_ > Start_)
    return strncpy(Buf_, &Src_[Start_], End_ - Start_);

  *Buf_ = 0;
  return Buf_;
}

/****************************************************************************/
LongNumber LongNumber::modf(LongNumber& ip) const
{
  LongNumber Obj_(*this);
  LongNumber One_(1UL, NumberSign::PLUS);
  LongNumber Rem_;
  
  ip = Obj_.DivProc(One_, &Rem_, TRUE);
  return Rem_;
}

/****************************************************************************/
LongNumber LongNumber::fmod(const LongNumber& y) const
{
  LongNumber Obj_(*this);
  LongNumber Rem_;

  Obj_.DivProc(y, &Rem_, FALSE);
  Obj_.Floor();
  
  Rem_ = *this;
  Rem_ -= (Obj_ * y);
  
  return Rem_;
}

/****************************************************************************/
LongNumber LongNumber::frexp(long* Exp_)
{
  if (!Exp_)
    return LongNumber();

  LongNumber LgAval_;
  LongNumber TwoPow_;
  LongNumber RetVal_;
  LongNumber Two_(2UL, NumberSign::PLUS);
  LongNumber Aval_(*this);

  LgAval_.SetPrecision(GetPrecision());
  TwoPow_.SetPrecision(GetPrecision());
  RetVal_.SetPrecision(GetPrecision());
  Two_.SetPrecision(GetPrecision());
  
  int Sign_ = _Sign;
  long i;

  Aval_.AbsoluteValue();  

  if (Aval_.AtZero())
  {
    *Exp_ = 0;
    return LongNumber();
  }
  else if (Aval_ < LongNumber("0.5"))
  {
    TwoPow_.SetValue(1UL, NumberSign::PLUS);
    
    for (i = -1; i > LONG_MIN && TwoPow_ > Aval_; i--)
    {
      TwoPow_ = IntPow(Two_, i);
      if (TwoPow_ < Aval_)
        break;
    }

    i++;
  }
  else
  {
    TwoPow_.SetValue(1UL, NumberSign::PLUS);
    
    for (i = 0; i < LONG_MAX && TwoPow_ < Aval_; i++)
    {
      TwoPow_ = IntPow(Two_, i);
      if (TwoPow_ > Aval_)
        break;
    }
  }

  TwoPow_ = IntPow(Two_, i);
  LgAval_ = Aval_.log();
  TwoPow_ = TwoPow_.log();
  LgAval_ -= TwoPow_;
  RetVal_ = LgAval_.exp();
  RetVal_.SetSign(Sign_);
  *Exp_ = i;
  RetVal_.IncExponent(2);
  RetVal_.Round();
  RetVal_.IncExponent(2, TRUE);

  return RetVal_;
}

/****************************************************************************/
LongNumber LongNumber::ldexp(long Exp_)
{
  LongNumber Result_(*this);
  LongNumber Mult_(2UL, NumberSign::PLUS);
  Mult_.SetPrecision(GetPrecision());
  
  Mult_ = IntPow(Mult_, Exp_);
  Result_ *= Mult_;
  return Result_;
}

/****************************************************************************/
LongNumber LongNumber::tan() const
{
  LongNumber Numer_ = Sin_Series(*this);
  Numer_ /= Cos_Series(*this);
  return Numer_;
}

/****************************************************************************/
LongNumber LongNumber::sin() const
{
  return Sin_Series(*this);
}

/****************************************************************************/
LongNumber LongNumber::cos() const
{
  return Cos_Series(*this);
}

/****************************************************************************/
LongNumber LongNumber::arcsin() const
{
  return Asin_Series(*this);
}

/****************************************************************************/
LongNumber LongNumber::arccos() const
{
  if (_UseSeries & ASIN_SERIES)
  {
    LongNumber Result_ = Asin_Series(*this);
    LongNumber Temp_(2UL, NumberSign::PLUS);
    Temp_ = vPI() / Temp_;
    Result_ = Temp_ - Result_;
    return Result_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::acosl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(ToString());
    double dval_ = (double)Dval_;
    dval_ = ::acos(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::arctan() const
{
  return Atan_Series(*this);
}

/****************************************************************************/
LongNumber LongNumber::arctan2(const LongNumber& y)
{
  if (_UseSeries & ATAN_SERIES)
  {
    LongNumber Result_ = *this;
    Result_ /= y;
    return Atan_Series(Result_);
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval2_(y.ToString());
    LongDouble Dval1_(ToString());
    Ldouble dval1_ = (Ldouble)Dval1_;
    Ldouble dval2_ = (Ldouble)Dval2_;  
    dval1_ = ::atan2l(dval1_, dval2_);
    Dval1_ = dval1_;
    LongNumber Result_(Dval1_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval2_(y.ToString());
    Double Dval1_(ToString());
    double dval1_ = (double)Dval1_;
    double dval2_ = (double)Dval2_;  
    dval1_ = ::atan2(dval1_, dval2_);
    Dval1_ = dval1_;
    LongNumber Result_(Dval1_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::sinh() const
{
  LongNumber Negv_ = -(*this);
  LongNumber Result_ = E_Series(*this);
  Result_ -= E_Series(Negv_);
  Result_ /= 2L;
  return Result_;
}

/****************************************************************************/
LongNumber LongNumber::cosh() const
{
  LongNumber Negv_ = -(*this);
  LongNumber Result_ = E_Series(*this);
  Result_ += E_Series(Negv_);
  Result_ /= 2L;
  return Result_;
}

/****************************************************************************/
LongNumber LongNumber::tanh() const
{
  LongNumber Result_ = sinh();
  Result_ /= cosh();
  return Result_;
}

/****************************************************************************/
LongNumber LongNumber::fabs() const
{
  LongNumber Obj_(*this);
  Obj_.AbsoluteValue();
  return Obj_;
}

/****************************************************************************/
LongNumber LongNumber::sqrt() const
{
  LongNumber Result_ = IntRoot(*this, 2, GetPrecision());
  return Result_;
}

/****************************************************************************/
LongNumber LongNumber::exp() const
{
  if (_UseSeries & E_SERIES)
    return E_Series(*this);

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::expl(dval_);
    LongDouble Rdval_(dval_);
    return LongNumber(Rdval_);  
  #else
    Double Dval_(ToString());
    double dval_ = (double)Dval_;
    dval_ = ::exp(dval_);
    Double Rdval_(dval_);
    return LongNumber(Rdval_);
  #endif
}

/****************************************************************************/
LongNumber LongNumber::log() const
{
  if (_UseSeries & LN_SERIES)
    return Ln_Series(*this);

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::logl(dval_);
    LongDouble Rdval_(dval_);
    return LongNumber(Rdval_);
  #else
    Double Dval_(ToString());
    double dval_ = (double)Dval_;
    dval_ = ::log(dval_);
    Double Rdval_(dval_);
    return LongNumber(Rdval_);
  #endif
}

/****************************************************************************/
LongNumber LongNumber::log10() const
{
  if (_UseSeries & LN_SERIES)
  {
    LongNumber Denom_(10UL, NumberSign::PLUS);
    Denom_ = Ln_Series(Denom_);
    LongNumber Result_ = Ln_Series(*this);
    Result_ /= Denom_;
    return Result_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::log10l(dval_);
    LongDouble Rdval_(dval_);
    return LongNumber(Rdval_);  
  #else
    Double Dval_(ToString());
    double dval_ = (double)Dval_;
    dval_ = ::log10(dval_);
    Double Rdval_(dval_);
    return LongNumber(Rdval_);
  #endif
}

/****************************************************************************/
LongNumber LongNumber::pow(const LongNumber& y) const
{
  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble xVal_(ToString());
    LongDouble yVal_(y.ToString());
    Ldouble xdval_ = (Ldouble)xVal_;
    Ldouble ydval_ = (Ldouble)yVal_;
    Ldouble rdval_ = ::powl(xdval_, ydval_);
    LongDouble Rdval_(rdval_);
    return LongNumber(Rdval_);  
  #else
    Double xVal_(ToString());
    Double yVal_(y.ToString());
    double xdval_ = (double)xVal_;
    double ydval_ = (double)yVal_;
    double rdval_ = ::pow(xdval_, ydval_);
    Double Rdval_(rdval_);
    return LongNumber(Rdval_);
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Factorial(Ulong Max_, int Prec_)
{
  if (Max_ <= 1)
    return LongNumber(1UL, NumberSign::PLUS);

  if (Max_ == 2)
    return LongNumber(Max_, NumberSign::PLUS);

  if (!Prec_)
    Prec_ = LongNumber::_DefaultPrecision;

  LongNumber Answer_(Max_, NumberSign::PLUS);

  while (Max_ - 2)
  {
    --Max_;  
    Answer_ *= Max_;
  }

  return Answer_;
}

/****************************************************************************/
LongNumber LongNumber::ExpPow(const LongNumber& Pow_)
{
  return LongNumber::E_Series(Pow_);
}

/****************************************************************************/
LongNumber LongNumber::IntPow(const LongNumber& Value_, long Exp_)
{
  if (!Exp_)
    return LongNumber(1UL, Value_.GetSign());

  if (Exp_ == 1L)
    return LongNumber(Value_);

  LongNumber Answer_(Value_);

  if (Value_.GetPrecision() > Answer_.GetPrecision())
    Answer_.SetPrecision(Value_.GetPrecision());
  
  long Aexp_ = labs(Exp_);
  long i;

  for (i = 1; i < Aexp_; i++)
    Answer_ *= Value_;

  if (Exp_ < 0)  
    Answer_ = 1L / Answer_;

  return Answer_;
}

/****************************************************************************/
void LongNumber::FindIntRoot(const LongNumber& x, LongNumber* Fval_, LongNumber* Dval_,
                             const LongNumber& Intercept_, long Root_)
{
  *Fval_ = IntPow(x, Root_) - Intercept_;
  *Dval_ = IntPow(x, Root_ - 1) * Root_;
}

/****************************************************************************/
LongNumber LongNumber::IntRoot(const LongNumber& Value_, long Root_, long Prec_)
{
  Boolean NegRoot_ = Root_ < 0L;
  Root_ = ::labs(Root_);
  LongNumber ErrObj_;

  if (Value_ < 0L)
  {
    ErrObj_.SetNoComplexRoots();
    return ErrObj_;    
  }

  if (Root_ == 0L)
  {
    ErrObj_.SetDivideByZero();  
    return ErrObj_;
  }

  if (Root_ == 1L || Value_ == 1L || Value_ == 0L)
    return LongNumber(Value_);

  LongNumber HighBound_(1UL, NumberSign::PLUS);
  LongNumber LowBound_(1UL, NumberSign::PLUS);
  LongNumber Intercept_(Value_);

  if (!Prec_)
    Prec_ = Intercept_.GetPrecision();
  else
  {
    Intercept_.SetPrecision(Prec_);
    HighBound_.SetPrecision(Prec_);
    LowBound_.SetPrecision(Prec_);
  }

  ChrString Epstr_("0.");
  Epstr_ += ChrString('0', Prec_);
  Epstr_ += "5";
  LongNumber Epsilon_(Epstr_);

  if (Value_ < 1L && Root_ > 1L)
    LowBound_ = Value_;
  else if (Value_ < 1L && Root_ < 1L)
    HighBound_ /= Value_;
  else if (Value_ > 1L && Root_ > 1L)
    HighBound_ = Value_;
  else if (Value_ > 1L && Root_ < 1L)
    LowBound_ /= Value_;
  
  if (NegRoot_)
    if (_UseSeries & SQRT_SERIES)  
      return (1L / RtSafe(LowBound_, HighBound_, Epsilon_, Intercept_, Root_));
    else
    {
      #if HAS_LONGDOUBLE_STDFNCS
        LongDouble Dval_(Value_.ToString());
        Ldouble dval_ = (Ldouble)Dval_;
        dval_ = ::sqrtl(dval_);
        Dval_ = dval_;
        LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
        return Result_;      
      #else
        Double Dval_(Value_.ToString());
        double dval_ = (double)Dval_;
        dval_ = ::sqrt(dval_);
        Dval_ = dval_;
        LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
        return Result_;
      #endif
    }
    
  if (_UseSeries & SQRT_SERIES)
    return RtSafe(LowBound_, HighBound_, Epsilon_, Intercept_, Root_);
  else
  {
    #if HAS_LONGDOUBLE_STDFNCS
      LongDouble Dval_(Value_.ToString());
      Ldouble dval_ = (Ldouble)Dval_;
      dval_ = ::sqrtl(dval_);
      Dval_ = dval_;
      LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
      return Result_;
    #else  
      Double Dval_(Value_.ToString());
      double dval_ = (double)Dval_;
      dval_ = ::sqrt(dval_);
      Dval_ = dval_;
      LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
      return Result_;
    #endif
  }  
}

/****************************************************************************/
LongNumber LongNumber::RtSafe(const LongNumber& x1, const LongNumber& x2, const LongNumber& xacc,
                              const LongNumber& Intercept_, long Root_)
{
  Ulong j;
  LongNumber ErrObj_;  
  LongNumber df, dx, dxold, f, fh, fl, tv1, tv2, tv3;
  LongNumber temp, xh, xl, rts;
  LongNumber fltZero_;
  LongNumber Two_(2UL, NumberSign::PLUS);

  tv1.SetPrecision(x2.GetPrecision());
  tv2.SetPrecision(x2.GetPrecision());
  tv3.SetPrecision(x2.GetPrecision());  
  df.SetPrecision(x2.GetPrecision());
  dx.SetPrecision(x2.GetPrecision());
  dxold.SetPrecision(x2.GetPrecision());
  f.SetPrecision(x2.GetPrecision());
  fh.SetPrecision(x2.GetPrecision());
  fl.SetPrecision(x2.GetPrecision());
  temp.SetPrecision(x2.GetPrecision());
  xh.SetPrecision(x2.GetPrecision());
  xl.SetPrecision(x2.GetPrecision());
  rts.SetPrecision(x2.GetPrecision());
  fltZero_.SetPrecision(x2.GetPrecision());


  FindIntRoot(x1, &fl, &df, Intercept_, Root_);
  FindIntRoot(x2, &fh, &df, Intercept_, Root_);

  if ((fl > fltZero_ && fh > fltZero_) || (fl < fltZero_ && fh < fltZero_))
  {
    ErrObj_.SetFindRootFunctionError();
    return ErrObj_;
  }

  if (fl == fltZero_)
    return x1;

  if (fh == fltZero_)
    return x2;

  if (fl < fltZero_)         // Orient the search so that f(xl) < 0
  {
    xl = x1;
    xh = x2;
  }
  else
  {
    xh = x1;
    xl = x2;    
  }

  rts = x1;
  rts += x2;
  rts /= Two_;                         // Initialize the guess for root,
  dxold = x2;
  dxold -= x1;
  dxold.AbsoluteValue();               // the "stepsize before last,"
  dx = dxold;                          // and the last step
  FindIntRoot(rts, &f, &df, Intercept_, Root_);

  for (j = 1; j <= LONG_MAX; j++)          // Loop over allowed iterations
  {
    tv1 = rts;
    tv1 -= xh;
    tv1 *= df;
    tv1 -= f;    
    tv2 = rts;
    tv2 -= xl;
    tv2 *= df;
    tv2 -= f;    
    tv1 *= tv2;

    tv2 = Two_;
    tv2 *= f;
    tv2.AbsoluteValue();

    tv3 = dxold;
    tv3 *= df;
    tv3.AbsoluteValue();
  
    if ((tv1 > fltZero_) ||        // Bisect if Newton out of range.
        (tv2 > tv3))               // or not decreasing fast enough
    {
      dxold = dx;
      dx = xh;
      dx -= xl;
      dx /= Two_;      
      rts = xl;
      rts += dx;
      if (xl == rts)
        return rts;        
    }
    else                                                                // Change in root is negligible
    {                                                                   // Newton step acceptable. Take it.
      dxold = dx;
      dx = f;
      dx /= df;
      temp = rts;
      rts -= dx;
      if (temp == rts)
        return rts;
    }

    tv1 = dx;
    tv1.AbsoluteValue();
    
    if (tv1 < xacc)        // convergence criterion
      return rts;

    // The one new function evaluation per iteration
    FindIntRoot(rts, &f, &df, Intercept_, Root_);

    if (f < fltZero_)
      xl = rts;
    else
      xh = rts;
  }

  ErrObj_.SetRootNotFoundError();
  return ErrObj_;
}

/****************************************************************************/
void LongNumber::OmitConstants(Boolean Flag_)
{
  _OmitConsts = Flag_;
  LongNumber Test_;
}

/****************************************************************************/
void LongNumber::SetSpeedCritical()
{
  _UseSeries = 0;
}

/****************************************************************************/
void LongNumber::UseSeriesApprox(int BitStr_)
{
  _UseSeries = BitStr_;
}

/****************************************************************************/
LongNumber LongNumber::Cos_Series(const LongNumber& InVal_)
{
  if (_UseSeries & COS_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;
  
    LongNumber Term_(1UL, NumberSign::PLUS);
    LongNumber TermSqr_(InVal_);
    TermSqr_ *= InVal_;
    TermSqr_.Round(LastExp_);
  
    LongNumber FactVal_(1UL, NumberSign::PLUS);
    LongNumber Sum_;
    LongInt ValWrap_;

    Term_.SetPrecision(InVal_.GetPrecision());
    TermSqr_.SetPrecision(InVal_.GetPrecision());
    FactVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());

    long neg, x, n, Exp_, Max_;
    Max_ = LONG_MAX / 2L;

    for (n = Exp_ = 0; (Exp_ = 2L * n) < Max_; n++)
    {
      neg = (n % 2L) ? -1L:1L;

      if (Exp_ && (x = Exp_ - 1L))
      {
        ValWrap_ = x;
        FactVal_ = ValWrap_;
        FactVal_ *= Exp_;
      }

      Term_ /= FactVal_;
      Term_ *= neg;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

#     if LONGNUM_DEBUG_ASSERT5a
        cout <<"Epsilon = " <<Epsilon_ <<endl;
        cout <<"Cos, Term(" <<n <<") = " <<Term_ <<endl;
        cout <<"Cos, Sum(" <<n <<") = " <<Sum_ <<endl;
#     endif

      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Term_ *= TermSqr_;
      Term_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::cosl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::cos(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Sin_Series(const LongNumber& InVal_)
{
  if (_UseSeries & SIN_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;

    LongNumber Term_(InVal_);
    LongNumber TermSqr_(Term_);
    TermSqr_ *= Term_;
    Term_.Round(LastExp_);
    TermSqr_.Round(LastExp_);

    LongNumber FactVal_(1UL, NumberSign::PLUS);
    LongNumber Sum_;
    LongInt ValWrap_;

    Term_.SetPrecision(InVal_.GetPrecision());
    TermSqr_.SetPrecision(InVal_.GetPrecision());
    FactVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());

    long x, n, neg, Exp_, Max_;
    Max_ = (LONG_MAX - 1L) / 2L;
  
    for (n = Exp_ = 0; (Exp_ = 2L * n + 1L) < Max_; n++)
    {
      neg = (n % 2L) ? -1L:1L;

      if (Exp_ && (x = Exp_ - 1L))
      {
        ValWrap_ = x;
        FactVal_ = ValWrap_;
        FactVal_ *= Exp_;
      }

      Term_ /= FactVal_;
      Term_ *= neg;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

#     if LONGNUM_DEBUG_ASSERT5a
        cout <<"Epsilon = " <<Epsilon_ <<endl;
        cout <<"Sin, Term(" <<n <<") = " <<Term_ <<endl;
        cout <<"Sin, Sum(" <<n <<") = " <<Sum_ <<endl;
#     endif

      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Term_ *= TermSqr_;
      Term_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::sinl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::sin(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Atan_Series(const LongNumber& InVal_)
{
  if (_UseSeries & ATAN_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;

    LongNumber Term_;
    LongNumber Numer_(InVal_);
    LongNumber TermSqr_(Numer_);
    TermSqr_ *= Numer_;
    Numer_.Round(LastExp_);
    TermSqr_.Round(LastExp_);
    
    LongNumber FactVal_(1UL, NumberSign::PLUS);
    LongNumber Sum_;
    LongInt ValWrap_;

    Term_.SetPrecision(InVal_.GetPrecision());
    TermSqr_.SetPrecision(InVal_.GetPrecision());
    FactVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());

    long n, neg, Exp_, Max_;
    Max_ = (LONG_MAX - 1L) / 2L;
  
    for (n = Exp_ = 0; (Exp_ = 2UL * n + 1UL) < Max_; n++)
    {
      neg = (n % 2UL) ? -1UL:1UL;

      if (Exp_ - 1UL)
      {
        ValWrap_ = Exp_;
        FactVal_ = ValWrap_;
      }

      Term_ = Numer_;
      Term_ /= FactVal_;
      Term_ *= neg;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

#     if LONGNUM_DEBUG_ASSERT5b
        cout <<"Epsilon = " <<Epsilon_ <<endl;
        cout <<"Atan, Numer(" <<n <<") = " <<Numer_ <<endl;
        cout <<"Atan, FactVal(" <<n <<") = " <<FactVal_ <<endl;
        cout <<"Atan, Term(" <<n <<") = " <<Term_ <<endl;
        cout <<"Atan, Sum(" <<n <<") = " <<Sum_ <<endl;
#     endif

      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Numer_ *= TermSqr_;
      Numer_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::atanl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::atan(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Asin_Series(const LongNumber& InVal_)
{
  if (_UseSeries & ASIN_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;
  
    LongNumber Term_(InVal_);
    LongNumber TermSqr_(Term_);
    TermSqr_ *= Term_;
    Term_.Round(LastExp_);
    TermSqr_.Round(LastExp_);

    LongNumber TopVal_(1UL, NumberSign::PLUS);
    LongNumber BotVal_(1UL, NumberSign::PLUS);
    LongNumber Sum_;
    LongInt ValWrap_;

    Term_.SetPrecision(InVal_.GetPrecision());
    TermSqr_.SetPrecision(InVal_.GetPrecision());
    TopVal_.SetPrecision(InVal_.GetPrecision());
    BotVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());

    long n, x, Last_, Exp_, Max_;
    Max_ = (LONG_MAX - 1L) / 2L;
    Last_ = 1UL;
  
    for (n = Exp_ = 0; (Exp_ = 2UL * n + 1UL) < Max_; n++)
    {
      x = Exp_ - 1UL;
  
      if (x)
      {
        ValWrap_ = x - 1L;
        TopVal_ = ValWrap_;
        TopVal_ *= Last_;
        ValWrap_ = x;
        BotVal_ = ValWrap_;
        BotVal_ *= Exp_;
        Last_ = Exp_;
      }

      Term_ *= TopVal_;
      Term_ /= BotVal_;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

#     if LONGNUM_DEBUG_ASSERT5b
        cout <<"Epsilon = " <<Epsilon_ <<endl;
        cout <<"Asin, Exp(" <<n <<") = " <<Exp_ <<endl;
        cout <<"Asin, TopVal(" <<n <<") = " <<TopVal_ <<endl;
        cout <<"Asin, BotVal(" <<n <<") = " <<BotVal_ <<endl;
        cout <<"Asin, Term(" <<n <<") = " <<Term_ <<endl;
        cout <<"Asin, Sum(" <<n <<") = " <<Sum_ <<endl;
#     endif

      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Term_ *= TermSqr_;
      Term_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::asinl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;
  #else  
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::asin(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::E_Series(const LongNumber& InVal_)
{
  if (_UseSeries & E_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;
  
    LongNumber Term_(1UL, NumberSign::PLUS);
    LongNumber TermVal_(InVal_);
    LongNumber FactVal_(1UL, NumberSign::PLUS);
    LongNumber Sum_;
    LongInt ValWrap_;

    Term_.SetPrecision(InVal_.GetPrecision());
    TermVal_.SetPrecision(InVal_.GetPrecision());
    FactVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());
    TermVal_.Round(LastExp_);

    long n;
  
    for (n = 0; n < LONG_MAX; n++)
    {
      if (n)
      {
        ValWrap_ = n;
        FactVal_ = ValWrap_;
      }

      Term_ /= FactVal_;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Term_ *= TermVal_;
      Term_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::expl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::exp(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Ln_Series(const LongNumber& InVal_)
{
  if (_UseSeries & LN_SERIES)
  {
    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', InVal_.GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);
    long LastExp_ = -InVal_.GetPrecision();
    LastExp_ -= 3;
  
    LongNumber TermVal_(InVal_);
    TermVal_ -= 1L;

    LongNumber Numer_ = TermVal_;
    LongNumber FactVal_(1UL, NumberSign::PLUS);
    LongNumber Term_;
    LongNumber Sum_;
    LongInt ValWrap_;

    Numer_.SetPrecision(InVal_.GetPrecision());
    TermVal_.SetPrecision(InVal_.GetPrecision());
    FactVal_.SetPrecision(InVal_.GetPrecision());
    Sum_.SetPrecision(InVal_.GetPrecision());
    Term_.SetPrecision(InVal_.GetPrecision());
    TermVal_.Round(LastExp_);
    Numer_.Round(LastExp_);

    long n, neg;
  
    for (n = 1; n < LONG_MAX; n++)
    {
      ValWrap_ = n;
      neg = (n % 2L) ? 1L:-1L;
      FactVal_ = ValWrap_;
      Term_ = Numer_;
      Term_ /= FactVal_;
      Term_ *= neg;
      Sum_ += Term_;
      Sum_.Round(LastExp_);

#     if LONGNUM_DEBUG_ASSERT5e
        cout <<"Epsilon = " <<Epsilon_ <<endl;
        cout <<"Ln, Term(" <<n <<") = " <<Term_ <<endl;
        cout <<"Ln, TermVal(" <<n <<") = " <<TermVal_ <<endl;
        cout <<"Ln, Numer(" <<n <<") = " <<Numer_ <<endl;
        cout <<"Ln, Sum(" <<n <<") = " <<Sum_ <<endl;
#     endif

      Numer_.AbsoluteValue();
      if (Numer_ < Epsilon_)
        break;

      Numer_ *= TermVal_;
      Numer_.Round(LastExp_);
    }

    LastExp_ += 3;
    Sum_.Round(LastExp_);
    return Sum_;
  }

  #if HAS_LONGDOUBLE_STDFNCS
    LongDouble Dval_(InVal_.ToString());
    Ldouble dval_ = (Ldouble)Dval_;
    dval_ = ::logl(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
    return Result_;  
  #else
    Double Dval_(InVal_.ToString());
    double dval_ = (double)Dval_;
    dval_ = ::log(dval_);
    Dval_ = dval_;
    LongNumber Result_(Dval_.ToString(Double::TYPE_DOUBLE_PRECISION));
    return Result_;
  #endif
}

/****************************************************************************/
LongNumber LongNumber::Pi_Series()
{
  if (_UseSeries & PI_SERIES)
  {
    LongNumber PreValue_ = LongNumber::vROOT3();
    PreValue_ *= 2L;

    long neg, i;
    LongNumber Sum_;
    LongNumber Term_;
    LongNumber Two_(2UL, NumberSign::PLUS);
    LongNumber Three_(3UL, NumberSign::PLUS);
    LongNumber Dterm_(1UL, NumberSign::PLUS);

    ChrString Epstr_("0.");
    Epstr_ += ChrString('0', LongNumber::vROOT3().GetPrecision());
    Epstr_ += "5";
    LongNumber Epsilon_(Epstr_);

    Term_.SetPrecision(LongNumber::vROOT3().GetPrecision());
    Two_.SetPrecision(LongNumber::vROOT3().GetPrecision());
    Three_.SetPrecision(LongNumber::vROOT3().GetPrecision());
    Dterm_.SetPrecision(LongNumber::vROOT3().GetPrecision());
    Sum_.SetPrecision(LongNumber::vROOT3().GetPrecision());
    long LastExp_ = -LongNumber::vROOT3().GetPrecision();
    LastExp_ -= 3;

    for (i = 0; i < LONG_MAX; i++)
    {
      neg = (i % 2L) ? -1L:1L;
      Term_ = neg * (1L / (Dterm_ * (Two_ * i + 1L)));
      Sum_ += Term_;
      Sum_.Round(LastExp_);
    
      Term_.AbsoluteValue();
      if (Term_ < Epsilon_)
        break;

      Dterm_ *= Three_;
    }

    Sum_ *= PreValue_;
    LastExp_ += 3;
    Sum_.Round(LastExp_);
  
    return Sum_;
  }

  LongDouble Dval_ = LongDouble::vPI;
  LongNumber PiVal_(Dval_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION));
  return PiVal_;
}

/****************************************************************************/
void LongNumber::ReadPresets()
{
    LongNumber* Temp_ = NULL;

    if (!LongNumber::p_ROOT2)
    {
      LongNumber::p_ROOT2 = this;
      Temp_ = new LongNumber(PRESET_ROOT2);
      LongNumber::p_ROOT2 = Temp_;
    }
      
    if (!LongNumber::p_ROOT3)
    {
      LongNumber::p_ROOT3 = this;
      Temp_ = new LongNumber(PRESET_ROOT3);
      LongNumber::p_ROOT3 = Temp_;
    }
            
    if (!LongNumber::p_E)
    {
      LongNumber::p_E = this;
      Temp_ = new LongNumber(PRESET_E);
      LongNumber::p_E = Temp_;
    }
      
    if (!LongNumber::p_PI)
    {
      LongNumber::p_PI = this;
      Temp_ = new LongNumber(PRESET_PI);
      LongNumber::p_PI = Temp_;
    }

    if (!LongNumber::p_NAN)
    {
      LongNumber::p_NAN = this;
      Double NanVal_ = Double::vNAN;
      Temp_ = new LongNumber(NanVal_, 0, 0, 0);
      Temp_->SetNan(true);
      LongNumber::p_NAN = Temp_;
    }
    
    if (!LongNumber::p_INFINITY)
    {
      LongNumber::p_INFINITY = this;
      Double InfVal_ = Double::vINFINITY;
      Temp_ = new LongNumber(InfVal_, 0, 0, 0);
      Temp_->SetInfinite(true);
      LongNumber::p_INFINITY = Temp_;
    }    
}

/****************************************************************************/
void LongNumber::StoreConstants()
{
  ifstream Fin_;
  ofstream Fout_;
  
  PATHTOKENIZER(Pathtok_, "PATH");
  size_t Max_ = Pathtok_.Max();
  size_t i;

  char* AbsPath_ = new_char_array(300 + strlen(_DataFile), _DataFile);    
  Fin_.open(AbsPath_, ios::in);
  
  for (i = 0; !Fin_.good() && i < Max_; i++)
  {
    strcpy(AbsPath_, Pathtok_[i]);
    strcat(AbsPath_, Pathtok_.DirSeparator());
    strcat(AbsPath_, _DataFile);
    Fin_.clear();
    Fin_.open(AbsPath_, ios::in);
  }

  if (!Fin_.good())
  {
    int SavedSettings_ = _UseSeries;
    _UseSeries = E_SERIES | PI_SERIES;
    SetConstants();
    _UseSeries = SavedSettings_;
    
    Fout_.open(_DataFile, ios::out);

    if (Fout_.good() && !_OmitConsts)
    {
      Fout_ <<GetPrecision() <<endl;
      Fout_ <<LongNumber::p_ROOT2->ToString().c_str() <<endl;
      Fout_ <<LongNumber::p_ROOT3->ToString().c_str() <<endl;
      Fout_ <<LongNumber::p_E->ToString().c_str() <<endl;
      Fout_ <<LongNumber::p_PI->ToString().c_str() <<endl;
    }

    Fout_.close();    
  }
  else
  {
    int LineBufSize_ = 256;
    char* Buffer_ = new_char_array(LineBufSize_, NULL);
    LongNumber* Temp_ = NULL;
  
    Buffer_ = ReadStringUntil(Fin_, Buffer_, LineBufSize_, '\n');    
    long Prec_ = atol(Buffer_);
    
    if (Prec_ == GetPrecision())
    {
      Buffer_ = ReadStringUntil(Fin_, Buffer_, LineBufSize_, '\n');
      if (!LongNumber::p_ROOT2)
      {
        LongNumber::p_ROOT2 = this;
        Temp_ = new LongNumber(Buffer_);
        LongNumber::p_ROOT2 = Temp_;
      }      
      
      Buffer_ = ReadStringUntil(Fin_, Buffer_, LineBufSize_, '\n');
      if (!LongNumber::p_ROOT3)
      {
        LongNumber::p_ROOT3 = this;
        Temp_ = new LongNumber(Buffer_);
        LongNumber::p_ROOT3 = Temp_;
      }      
            
      Buffer_ = ReadStringUntil(Fin_, Buffer_, LineBufSize_, '\n');
      if (!LongNumber::p_E)
      {
        LongNumber::p_E = this;
        Temp_ = new LongNumber(Buffer_);
        LongNumber::p_E = Temp_;
      }      
      
      Buffer_ = ReadStringUntil(Fin_, Buffer_, LineBufSize_, '\n');
      if (!LongNumber::p_PI)
      {
        LongNumber::p_PI = this;
        Temp_ = new LongNumber(Buffer_);
        LongNumber::p_PI = Temp_;
      }

      if (!LongNumber::p_NAN)
      {
        LongNumber::p_NAN = this;
        Double NanVal_ = Double::vNAN;
        Temp_ = new LongNumber(NanVal_, 0, 0, 0);
        Temp_->SetNan(true);
        LongNumber::p_NAN = Temp_;
      }

      if (!LongNumber::p_INFINITY)
      {
        LongNumber::p_INFINITY = this;
        Double InfVal_ = Double::vINFINITY;
        Temp_ = new LongNumber(InfVal_, 0, 0, 0);
        Temp_->SetInfinite(true);
        LongNumber::p_INFINITY = Temp_;
      }

      Fin_.close();
    }
    else
    {
      int SavedSettings_ = _UseSeries;
      _UseSeries = E_SERIES | PI_SERIES;
      SetConstants();
      _UseSeries = SavedSettings_;
      
      Fin_.close();
      Fout_.open(_DataFile, ios::out);

      if (Fout_.good() && !_OmitConsts)
      {
        Fout_ <<GetPrecision() <<endl;
        Fout_ <<LongNumber::p_ROOT2->ToString().c_str() <<endl;
        Fout_ <<LongNumber::p_ROOT3->ToString().c_str() <<endl;
        Fout_ <<LongNumber::p_E->ToString().c_str() <<endl;
        Fout_ <<LongNumber::p_PI->ToString().c_str() <<endl;
      }

      Fout_.close();      
    }

    ::DeleteArray(Buffer_);    
  }

  ::DeleteArray(AbsPath_);
}

/****************************************************************************/
void LongNumber::SetConstants()
{
  if (_OmitConsts)
    return;

  LongNumber* Temp_ = NULL;

  if (!LongNumber::p_ROOT2)
  {
    LongNumber::p_ROOT2 = this;
    Temp_ = new LongNumber(LongNumber::IntRoot(LongNumber(2UL, NumberSign::PLUS), 2));
    LongNumber::p_ROOT2 = Temp_;
  }
  
  if (!LongNumber::p_ROOT3)
  {
    LongNumber::p_ROOT3 = this;
    Temp_ = new LongNumber(LongNumber::IntRoot(LongNumber(3UL, NumberSign::PLUS), 2));
    LongNumber::p_ROOT3 = Temp_;
  }

  if (!LongNumber::p_E)
  {
    LongNumber::p_E = this;
    Temp_ = new LongNumber(E_Series(LongNumber(1UL, NumberSign::PLUS)));
    LongNumber::p_E = Temp_;
  }  

  if (!LongNumber::p_PI)
  {
    LongNumber::p_PI = this;
    Temp_ = new LongNumber(Pi_Series());
    LongNumber::p_PI = Temp_;
  }

  if (!LongNumber::p_NAN)
  {
    LongNumber::p_NAN = this;
    Double NanVal_ = Double::vNAN;
    Temp_ = new LongNumber(NanVal_, 0, 0, 0);
    Temp_->SetNan(true);
    LongNumber::p_NAN = Temp_;
  }
    
  if (!LongNumber::p_INFINITY)
  {
    LongNumber::p_INFINITY = this;
    Double InfVal_ = Double::vINFINITY;
    Temp_ = new LongNumber(InfVal_, 0, 0, 0);
    Temp_->SetInfinite(true);
    LongNumber::p_INFINITY = Temp_;
  }
}

/****************************************************************************/
int LongNumber::ClassID() const
{
  return TypeInfo::LONGNUMBER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* LongNumber::GiveObjectAcceptor() const
{
  return (new LongNumberAcceptor(this));
}

/****************************************************************************/
long LongNumber::DataSize () const
{
  return
  (
    sizeof(int) +
    (2 * sizeof(long)) +
    sizeof(short) +
    sizeof(Byte_t) +
    (_Head ? _Head->DataSize():0) +
    (_DecimalPt ? _DecimalPt->DataSize():0)
  );
}

/****************************************************************************/
Object& LongNumber::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  LongNumberAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromLongNumber(TrgComp_)));
}

/****************************************************************************/
Object* LongNumber::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new LongNumber(*this));

  LongNumberAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromLongNumber(TrgComp_)));
}

/****************************************************************************/
istream& LongNumber::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.seekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& LongNumber::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_)
  {
    *Ok_ =
      Is_.read((char*)(&_Sign), sizeof(int)).good() &&
      Is_.read((char*)(&_NumNodes), sizeof(long)).good() &&
      Is_.read((char*)(&_DivPrecision), sizeof(long)).good() &&
      Is_.read((char*)(&_Base), sizeof(short)).good() &&
      Is_.read((char*)(&_SciNote), sizeof(Byte_t)).good() &&
      (_Head ? _Head->BinaryRead(Is_, Ok_):1) &&
      (_DecimalPt ? _DecimalPt->BinaryRead(Is_, Ok_):1);

    if (*Ok_)
      SetIOstate(TRUE, IO_CLEAR, TRUE);    
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& LongNumber::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.write((char*)(&_Sign), sizeof(int));
    if (OstreamCheck(Os_, Ok_))
    {
      Os_.write((char*)(&_NumNodes), sizeof(long));
      if (OstreamCheck(Os_, Ok_))
      {
        Os_.write((char*)(&_DivPrecision), sizeof(long));
        if (OstreamCheck(Os_, Ok_))
        {
          Os_.write((char*)(&_Base), sizeof(short));

          if (OstreamCheck(Os_, Ok_))
          {
            Os_.write((char*)(&_SciNote), sizeof(Byte_t));

            if (_Head)
            {
              *Ok_ = _Head->BinaryWrite(this, Os_, Ok_);
              if (_DecimalPt)
                *Ok_ = _DecimalPt->BinaryWrite(this, Os_, Ok_);
            }
          }
        }
      }
    }
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& LongNumber::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.iseekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& LongNumber::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_)
  {
    *Ok_ =
      Is_.iread((char*)(&_Sign), sizeof(int)).good() &&
      Is_.iread((char*)(&_NumNodes), sizeof(long)).good() &&
      Is_.iread((char*)(&_DivPrecision), sizeof(long)).good() &&
      Is_.iread((char*)(&_Base), sizeof(short)).good() &&
      Is_.iread((char*)(&_SciNote), sizeof(Byte_t)).good() &&
      (_Head ? _Head->BinaryRead(Is_, Ok_):1) &&
      (_DecimalPt ? _DecimalPt->BinaryRead(Is_, Ok_):1);

    if (*Ok_)
      SetIOstate(TRUE, IO_CLEAR, TRUE);          
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& LongNumber::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.owrite((char*)(&_Sign), sizeof(int));
    if (OstreamCheck(((ostream&)Os_), Ok_))
    {
      Os_.owrite((char*)(&_NumNodes), sizeof(long));
      if (OstreamCheck(((ostream&)Os_), Ok_))
      {
        Os_.owrite((char*)(&_DivPrecision), sizeof(long));
        if (OstreamCheck(((ostream&)Os_), Ok_))
        {
          Os_.owrite((char*)(&_Base), sizeof(short));

          if (OstreamCheck(((ostream&)Os_), Ok_))
          {
            Os_.owrite((char*)(&_SciNote), sizeof(Byte_t));

            if (_Head)
            {
              *Ok_ = _Head->BinaryWrite(this, Os_, Ok_);
              if (_DecimalPt)
                *Ok_ = _DecimalPt->BinaryWrite(this, Os_, Ok_);
            }
          }
        }
      }
    }
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& LongNumber::TextRead(istream& Is_, Boolean* Ok_)
{
  ChrString TempStr_;
  TempStr_.TextRead(Is_, Ok_);
  *this = TempStr_;
  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& LongNumber::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  ChrString TempStr_ = ToString();
  TempStr_.TextWrite(Os_, Ok_);
  return ObjectImp::TextWrite(Os_, Ok_);  
}

/****************************************************************************/
const char* LongNumber::GiveHashableData() const
{
  return ToString().c_str();
}

/****************************************************************************/
long LongNumber::GiveHashableDataLength() const
{
  return ::SafeStrLen(ToString().c_str());
}

/****************************************************************************/
LongNumber& LongNumber::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static LongNumber _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& LongNumber::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongNumber::GiveNullObject();
}

/****************************************************************************/
Object& LongNumber::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongNumber::GiveNullObject();
}

/****************************************************************************/
LongNumber& LongNumber::DerivedClass()
{
  return *this;
}

/****************************************************************************/
const LongNumber& LongNumber::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
Boolean LongNumber::IsEqualToLongNumber(const LongNumber* Ptr_) const
{
  if (!Ptr_)
    return FALSE;

  long Exp_, Max_, Min_;
  
  Boolean PtSame_ = *_DecimalPt == *Ptr_->_DecimalPt;
  Boolean SignSame_ = _Sign == Ptr_->_Sign;  
  Max_ = TotalDigits();
  Boolean TotalSame_ = Max_ == Ptr_->TotalDigits();

  if (!SignSame_ || (PtSame_ && !TotalSame_))
    return FALSE;

  Max_ = HighestExp();
  if (Ptr_->HighestExp() > Max_)
    Max_ = Ptr_->HighestExp();

  Min_ = LowestExp();
  if (Ptr_->LowestExp() < Min_)
    Min_ = Ptr_->LowestExp();

  short ObjDigit_, SrcDigit_;
  for (Exp_ = Max_; Exp_ >= Min_; --Exp_)
  {
    SrcDigit_ = GetDigit(Exp_).Digit();
    ObjDigit_ = Ptr_->GetDigit(Exp_).Digit();
    
    if (SrcDigit_ != ObjDigit_)
      return FALSE;
  }

  return TRUE;
}

/****************************************************************************/
Boolean LongNumber::IsLesserToLongNumber(const LongNumber* Ptr_) const
{
  if (!Ptr_)
    return FALSE;  

  long Exp_, Max_, Min_;
  
  Boolean PtSame_ = *_DecimalPt == *Ptr_->_DecimalPt;
  Boolean SignSame_ = _Sign == Ptr_->_Sign;  
  Max_ = TotalDigits();
  Boolean TotalSame_ = Max_ == Ptr_->TotalDigits();

  if (!SignSame_)
    return ((_Sign == NumberSign::MINUS) ? TRUE:FALSE);
  else if (PtSame_ && !TotalSame_)
    return ((_Sign == NumberSign::MINUS) ?
                   (Max_ > Ptr_->TotalDigits()):
                   (Max_ < Ptr_->TotalDigits()));

  Max_ = HighestExp();
  if (Ptr_->HighestExp() > Max_)
    Max_ = Ptr_->HighestExp();

  Min_ = LowestExp();
  if (Ptr_->LowestExp() < Min_)
    Min_ = Ptr_->LowestExp();

  short ObjDigit_, SrcDigit_;
  for (Exp_ = Max_; Exp_ >= Min_; --Exp_)
  {
    SrcDigit_ = GetDigit(Exp_).Digit();
    ObjDigit_ = Ptr_->GetDigit(Exp_).Digit();
    
    if (SrcDigit_ < ObjDigit_)
      return TRUE;
    else if (SrcDigit_ > ObjDigit_)
      return FALSE;
  }

  return FALSE;  
}

/****************************************************************************/
Boolean LongNumber::IsGreaterToLongNumber(const LongNumber* Ptr_) const
{
  if (!Ptr_)
    return FALSE;    

  long Exp_, Max_, Min_;
  
  Boolean PtSame_ = *_DecimalPt == *Ptr_->_DecimalPt;
  Boolean SignSame_ = _Sign == Ptr_->_Sign;  
  Max_ = TotalDigits();
  Boolean TotalSame_ = Max_ == Ptr_->TotalDigits();

  if (!SignSame_)
    return ((_Sign == NumberSign::MINUS) ? FALSE:TRUE);
  else if (PtSame_ && !TotalSame_)
    return ((_Sign == NumberSign::MINUS) ?
                   (Max_ < Ptr_->TotalDigits()):
                   (Max_ > Ptr_->TotalDigits()));

  Max_ = HighestExp();
  if (Ptr_->HighestExp() > Max_)
    Max_ = Ptr_->HighestExp();

  Min_ = LowestExp();
  if (Ptr_->LowestExp() < Min_)
    Min_ = Ptr_->LowestExp();

  short ObjDigit_, SrcDigit_;    
  for (Exp_ = Max_; Exp_ >= Min_; --Exp_)
  {
    SrcDigit_ = GetDigit(Exp_).Digit();
    ObjDigit_ = Ptr_->GetDigit(Exp_).Digit();
    
    if (SrcDigit_ > ObjDigit_)
      return TRUE;
    else if (SrcDigit_ < ObjDigit_)
      return FALSE;
  }

  return FALSE;  
}

/****************************************************************************/
Boolean LongNumber::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongNumberAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToLongNumber(TrgComp_);
}

/****************************************************************************/
Boolean LongNumber::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongNumberAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToLongNumber(TrgComp_);
}

/****************************************************************************/
Boolean LongNumber::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongNumberAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToLongNumber(TrgComp_);
}

/****************************************************************************/
char* LongNumber::NewString(const char* SrcStr_)
{
  char* ns = NULL;

  // Size of the string must be in multiples of BcdDigit::DigitsPerLong()
  // 16 for a 8 byte long, 8 for a 4 byte long
  if (SrcStr_)
  {
    size_t Size_ = strlen(SrcStr_);
    if (Size_ % BcdDigit::DigitsPerLong())
      Size_ += BcdDigit::DigitsPerLong() - (Size_ % BcdDigit::DigitsPerLong());

    return ((ns = new_char_array(Size_ + 1, NULL)) ? strcpy(ns, SrcStr_):NULL);
  }

  return NULL;
}

/****************************************************************************/
short LongNumber::SubtractFromTensCmp(LongNumber& NewResult_, LongNumber& Result_,
                                     long Exp_, long Min_, short Unit_)
{
  short Diff_ = Unit_ - Result_.GetDigit(Exp_).Digit();

  if (Exp_ > Min_)
    Diff_ += SubtractFromTensCmp(NewResult_, Result_, Exp_ - 1, Min_, Unit_) - 1;

  NewResult_.SetDigit(Diff_ % 10, Exp_);
  return (Diff_ / 10);
}

/****************************************************************************/
LongNumber& LongNumber::AbsoluteValue()
{
  _Sign = NumberSign::PLUS;
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::Ceiling()
{
  if (IsInfinite() || IsNan())
    return *this;

  Boolean NoFrac_ = TRUE;
  long Low_ = LowestExp();
  long High_ = HighestExp();
  long i;
  
  if (Low_ < 0)
  {
    for (i = Low_; i; i++)
    {
      if (NoFrac_ && GetDigit(i).Digit())
        NoFrac_ = FALSE;
        
      SetDigit(BcdDigit(0), i);        
    }

    if (High_ < 0)
    {
      if (NoFrac_)
        SetValue(0, NumberSign::PLUS);
      else
      {
        if (_Sign == NumberSign::PLUS)
          SetValue(1UL, NumberSign::PLUS);
        else
          SetValue(0, NumberSign::PLUS);        
      }
    }
    else
    {
      if (!NoFrac_ && _Sign == NumberSign::PLUS)
        (*this)++;      
    }
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::Floor()
{
  if (IsInfinite() || IsNan())
    return *this;

  Boolean NoFrac_ = TRUE;
  long Low_ = LowestExp();
  long High_ = HighestExp();
  long i;

  if (Low_ < 0)
  {
    for (i = Low_; i; i++)
    {
      if (NoFrac_ && GetDigit(i).Digit())
        NoFrac_ = FALSE;
        
      SetDigit(BcdDigit(0), i);        
    }
  
    if (High_ < 0)
    {
      if (NoFrac_)
        SetValue(0, NumberSign::PLUS);
      else
      {    
        if (_Sign == NumberSign::MINUS)
          SetValue(1UL, NumberSign::MINUS);
        else
          SetValue(0, NumberSign::PLUS);
      }
    }
    else
    {
      if (!NoFrac_ && _Sign == NumberSign::MINUS)
        (*this)--;
    }
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::Round(long Exp_)
{
  if (IsInfinite() || IsNan())
    return *this;

  if (Exp_ && !AtZero())
  {
    LongNumber RndNum_(ChrString(".5"));
    RndNum_ *= IntPow(LongNumber(10UL, NumberSign::PLUS), Exp_);
    long Aexp_ = labs(Exp_);

    if (_Sign == NumberSign::PLUS)
    {
      *this += RndNum_;

      if (Exp_ < 0)
      {
        IncExponent(Aexp_);
        Floor();
        IncExponent(Aexp_, TRUE);
      }
      else
      {
        DecExponent(Aexp_);
        Floor();
        DecExponent(Aexp_, TRUE);
      }      
    }
    else
    {
      *this -= RndNum_;

      if (Exp_ < 0)
      {
        IncExponent(Aexp_);
        Ceiling();
        IncExponent(Aexp_, TRUE);
      }
      else
      {
        DecExponent(Aexp_);
        Ceiling();
        DecExponent(Aexp_, TRUE);
      }      
    }
  }
  else
  {
    if (_Sign == NumberSign::PLUS)
    {
      *this += LongNumber(ChrString(".5"));
      Floor();
    }
    else
    {
      *this -= LongNumber(ChrString(".5"));
      Ceiling();
    }
  }
    
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::Round()
{
  return Round(0);
}

/****************************************************************************/
const LongNumber& LongNumber::operator ++ ()
{
  LongNumber Num1_(1, NumberSign::PLUS);
  *this += Num1_;
  return *this;
}

/****************************************************************************/    
const LongNumber& LongNumber::operator ++ (int)
{
  static LongNumber OldVal_ = *this;
  LongNumber Num1_(1, NumberSign::PLUS);
  *this += Num1_;
  return OldVal_;
}

/****************************************************************************/
const LongNumber& LongNumber::operator -- ()
{
  LongNumber Num1_(1, NumberSign::PLUS);
  *this -= Num1_;
  return *this;
}

/****************************************************************************/    
const LongNumber& LongNumber::operator -- (int)
{
  static LongNumber OldVal_ = *this;
  LongNumber Num1_(1, NumberSign::PLUS);
  *this -= Num1_;
  return OldVal_;
}

/****************************************************************************/
LongNumber& LongNumber::operator += (long Val_)
{
  return ((Val_ < 0) ?
    (*this += LongNumber(Ulong(-Val_), NumberSign::MINUS)):
    (*this += LongNumber(Val_, NumberSign::PLUS)));
}

/****************************************************************************/
LongNumber& LongNumber::operator -= (long Val_)
{
  return ((Val_ < 0) ?
    (*this -= LongNumber(Ulong(-Val_), NumberSign::MINUS)):
    (*this -= LongNumber(Val_, NumberSign::PLUS)));
}

/****************************************************************************/
LongNumber& LongNumber::operator *= (long Val_)
{
  return ((Val_ < 0) ?
    (*this *= LongNumber(Ulong(-Val_), NumberSign::MINUS)):
    (*this *= LongNumber(Val_, NumberSign::PLUS)));
}

/****************************************************************************/
LongNumber& LongNumber::operator /= (long Val_)
{
  return ((Val_ < 0) ?
    (*this /= LongNumber(Ulong(-Val_), NumberSign::MINUS)):
    (*this /= LongNumber(Val_, NumberSign::PLUS)));
}

/****************************************************************************/
LongNumber& LongNumber::operator %= (long Val_)
{
  return ((Val_ < 0) ?
    (*this %= LongNumber(Ulong(-Val_), NumberSign::MINUS)):
    (*this %= LongNumber(Val_, NumberSign::PLUS)));
}

/****************************************************************************/
LongNumber& LongNumber::DivInt(long Val_)
{
  return ((Val_ < 0) ?
    (DivInt(LongNumber(Ulong(-Val_), NumberSign::MINUS))):
    (DivInt(LongNumber(Val_, NumberSign::PLUS))));
}

/****************************************************************************/
LongNumber& LongNumber::operator += (const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
    return *this;
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    *this = Obj_.DerivedClass();
    return *this;
  }
  
  long Exp_, Max_, Min_;
  Boolean SignSame_ = _Sign == Obj_.DerivedClass()._Sign;

  if (Obj_.DerivedClass().GetPrecision() > GetPrecision())
    SetPrecision(Obj_.DerivedClass().GetPrecision());

  if (Obj_.DerivedClass().AtZero())
    return *this;
  else if (AtZero())
  {
    *this = Obj_.DerivedClass();
    return *this;
  }    

  if (!SignSame_)
  {
    if (_Sign == NumberSign::MINUS)
    {
      LongNumber Source_(*this);
      Source_._Sign = NumberSign::PLUS;
      *this = (Obj_.DerivedClass() - Source_);
    }
    else
    {
      LongNumber Augend_(Obj_.DerivedClass());
      Augend_._Sign = NumberSign::PLUS;
      *this -= Augend_;
    }

    return *this;
  }

  Max_ = HighestExp();
  if (Obj_.DerivedClass().HighestExp() > Max_)
    Max_ = Obj_.DerivedClass().HighestExp();

  Min_ = LowestExp();
  if (Obj_.DerivedClass().LowestExp() < Min_)
    Min_ = Obj_.DerivedClass().LowestExp();

  LongNumber Result_;
  Result_.SetPrecision(GetPrecision());
  Result_.SetSign(_Sign);
  BcdDigit Bcd_;
  
  for (Exp_ = Min_; Exp_ <= Max_; ++Exp_)
  {
    Bcd_.AddCarry(GetDigit(Exp_), Obj_.DerivedClass().GetDigit(Exp_));
    Result_.SetDigit(Bcd_.Digit(), Exp_);
  }

  if (Bcd_.Carry())
    Result_.SetDigit(Bcd_.Carry(), Exp_);

  *this = Result_;
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator -= (const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
    return *this;
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    *this = Obj_.DerivedClass();
    return *this;
  }

  long Exp_, Max_, Min_;

  if (Obj_.DerivedClass().GetPrecision() > GetPrecision())
    SetPrecision(Obj_.DerivedClass().GetPrecision());

  if (Obj_.DerivedClass().AtZero())
    return *this;
  else if (AtZero())
  {
    *this = Obj_.DerivedClass();
    _Sign = (Obj_.DerivedClass().GetSign() == NumberSign::PLUS) ? NumberSign::MINUS:NumberSign::PLUS;
    return *this;
  }

  if (_Sign == NumberSign::MINUS && Obj_.DerivedClass()._Sign == NumberSign::MINUS)
  {
    LongNumber Src_(*this);
    Src_._Sign = NumberSign::PLUS;
    LongNumber Trg_(Obj_.DerivedClass());
    Trg_._Sign = NumberSign::PLUS;    
    *this = (Trg_ - Src_);
    return *this;
  }
  else if (_Sign == NumberSign::MINUS && Obj_.DerivedClass()._Sign == NumberSign::PLUS)
  {
    LongNumber Trg_(Obj_.DerivedClass());
    Trg_._Sign = NumberSign::MINUS;
    *this += Trg_;
    return *this;
  }
  else if (_Sign == NumberSign::PLUS && Obj_.DerivedClass()._Sign == NumberSign::MINUS)
  {
    LongNumber Trg_(Obj_.DerivedClass());
    Trg_._Sign = NumberSign::PLUS;    
    *this += Trg_;
    return *this;
  }  

  Max_ = HighestExp();
  if (Obj_.DerivedClass().HighestExp() > Max_)
    Max_ = Obj_.DerivedClass().HighestExp();

  Min_ = LowestExp();
  if (Obj_.DerivedClass().LowestExp() < Min_)
    Min_ = Obj_.DerivedClass().LowestExp();

  LongNumber Result_;
  Result_.SetPrecision(GetPrecision());
  Result_.SetSign(_Sign);
  BcdDigit Bcd_;
  
  for (Exp_ = Min_; Exp_ <= Max_; ++Exp_)
  {
    Bcd_.Subtract(GetDigit(Exp_), Obj_.DerivedClass().GetDigit(Exp_), Exp_ == Min_);
    Result_.SetDigit(Bcd_.Digit(), Exp_);
  }

  if (Bcd_.Carry())
    *this = Result_;  
  else
  {
    LongNumber NewResult_;
    NewResult_.SetPrecision(GetPrecision());
    
    short Carry_ = SubtractFromTensCmp(NewResult_, Result_, Max_, Min_, 10);
    *this = NewResult_;

    if (!Carry_)
      _Sign = NumberSign::MINUS;
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator *= (const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
  {
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
    
    return *this;
  }
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    int Sign_ = _Sign;
    *this = Obj_.DerivedClass();
    _Sign = Sign_;
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
    
    return *this;
  }

  long x, y, Exp_, Max1_, Min1_, Max2_, Min2_;

  if (Obj_.DerivedClass().GetPrecision() > GetPrecision())
    SetPrecision(Obj_.DerivedClass().GetPrecision());

  if (Obj_.DerivedClass().AtZero())
  {
    *this = LongNumber();
    return *this;
  }
  else if (AtZero())
    return *this;

  Max1_ = HighestExp();
  Max2_ = Obj_.DerivedClass().HighestExp();

  Min1_ = LowestExp();
  Min2_ = Obj_.DerivedClass().LowestExp();

  LongNumber TmpResult_;
  LongNumber Result_;
  Result_.SetPrecision(GetPrecision());
  TmpResult_.SetPrecision(GetPrecision());  

  for (x = Min1_; x <= Max1_; x++)
    for (y = Min2_; y <= Max2_; y++)
    {
      Exp_ = x + y;
      BcdDigit Bcd_(GetDigit(x).Digit());
      Bcd_.Multiply(Obj_.DerivedClass().GetDigit(y));
      TmpResult_.SetDigit(Bcd_, Exp_);
      TmpResult_.SetCarry(Bcd_, Exp_);
      Result_ += TmpResult_;
      TmpResult_.ClearDigit(Exp_)
                .ClearDigit(Exp_ + 1);
    }

  if (_Sign == Obj_.DerivedClass()._Sign)
    Result_._Sign = NumberSign::PLUS;
  else
    Result_._Sign = NumberSign::MINUS;

  *this = Result_;  
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator /= (const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
  {
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
    
    return *this;
  }
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    int Sign_ = _Sign;
    *this = Obj_.DerivedClass();
    _Sign = Sign_;    
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
    
    return *this;
  }

  LongNumber* Dum_ = NULL;
  return DivProc(Obj_.DerivedClass(), Dum_, FALSE);
}

/****************************************************************************/
LongNumber& LongNumber::operator %= (const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
    return *this;
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    *this = Obj_.DerivedClass();
    return *this;
  }

  LongNumber Rem_;  
  DivProc(Obj_.DerivedClass(), &Rem_, TRUE);
  *this = Rem_;
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::DivInt(const MixedNumType<LongNumber>& Obj_)
{
  if (IsInfinite() || IsNan())
  {
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
    
    return *this;
  }
  else if (Obj_.DerivedClass().IsInfinite() || Obj_.DerivedClass().IsNan())
  {
    int Sign_ = _Sign;
    *this = Obj_.DerivedClass();
    _Sign = Sign_;    
    if (IsInfinite() && Obj_.DerivedClass().GetSign() == NumberSign::MINUS)
      FlipSign();
  
    return *this;
  }

  LongNumber* Dum_ = NULL;
  return DivProc(Obj_.DerivedClass(), Dum_, TRUE);
}

/****************************************************************************/
long LongNumber::FindDivGuess(LongNumber* Src_, LongNumber* Trg_)
{
  if (Trg_->AtZero() || Src_->AtZero())
    return 1;

  Ulong MaxSrc_ = Src_->HighestExp();
  Ulong MaxTrg_ = Trg_->HighestExp();
  long GuessSrc_ = 0;
  long GuessTrg_ = 0;

  for (;;)
  {
    GuessSrc_ = Src_->GetDigit(MaxSrc_).Digit();
    if (!MaxSrc_ || GuessSrc_) break;
    else MaxSrc_--;
  }
  
  for (;;)
  {
    GuessTrg_ = Trg_->GetDigit(MaxTrg_).Digit();    
    if (!MaxTrg_ || GuessTrg_) break;
    else MaxTrg_--;
  }

  ++GuessTrg_;  
  if (!GuessSrc_ || GuessTrg_ > GuessSrc_)
    return 1;

  return (GuessSrc_ / GuessTrg_);
}

/****************************************************************************/
LongNumber& LongNumber::DivProc(const LongNumber& Obj_, LongNumber* Remainder_, Boolean IntDiv_)
{
  if (this == &Obj_)
  {
    *this = LongNumber(1, NumberSign::PLUS);
    return *this;
  }
  else if (Obj_.AtZero())
  {
    // cannot divide by zero
    SetDivideByZero();
    return *this;
  }
  else if (AtZero())
    return *this;

  if (Obj_.GetPrecision() > GetPrecision())
    SetPrecision(Obj_.GetPrecision());

  if (Remainder_)
    Remainder_->SetPrecision(GetPrecision());  

  LongNumber ZeroVal_;
  LongNumber Quotient_;
  LongNumber TmpResult_;
  Quotient_.SetPrecision(GetPrecision());
  TmpResult_.SetPrecision(GetPrecision());
  
  LongNumber* Src_;
  LongNumber* Trg_;
  long Max1_, Max2_, Diff_, Prod_, GuessQ_;
  int SrcSign_, TrgSign_;
  Boolean StartLoop_;  

  Trg_ = new LongNumber(Obj_);
  Src_ = new LongNumber(*this);
  Trg_->SetPrecision(GetPrecision());
  Src_->SetPrecision(GetPrecision());
  
  if (Obj_._Sign == NumberSign::MINUS)
  {
    Trg_->_Sign = NumberSign::PLUS;
    TrgSign_ = NumberSign::MINUS;
  }
  else
    TrgSign_ = NumberSign::PLUS;    

  if (_Sign == NumberSign::MINUS)
  {
    Src_->_Sign = NumberSign::PLUS;
    SrcSign_ = NumberSign::MINUS;
  }
  else
    SrcSign_ = NumberSign::PLUS;

  Max1_ = Src_->HighestExp();
  Max2_ = Trg_->HighestExp();
  Diff_ = Max1_ - Max2_;
  StartLoop_ = TRUE;
  Prod_ = 0;

  if (Diff_ > 0)
    Trg_->IncExponent(Diff_);
  else if (Diff_ < 0)
  {
    if (IntDiv_)
    {
      if (Remainder_)
        *Remainder_ = *this;
        
      *this = ZeroVal_;
      StartLoop_ = FALSE;
    }
    else
      Trg_->DecExponent(labs(Diff_));
  }

  while (StartLoop_)
  {
    TmpResult_ = *Src_ - *Trg_;
    
    if (TmpResult_ < ZeroVal_)
    {
      if (Prod_)
        Quotient_.SetDigit(Prod_, Diff_);

      if ((-Diff_ >= _DivPrecision) || (IntDiv_ && Diff_ <= 0))
      {
        *this = Quotient_;
        if (Remainder_)
          *Remainder_ = *Src_;
        break;
      }
      
      if (Diff_ <= 0)
        Trg_->DecExponent(1);
      else
        Trg_->IncExponent(1, TRUE);
          
      --Diff_;
      Prod_ = 0;
    }
    else if (TmpResult_ == ZeroVal_)
    {
      ++Prod_;
      Quotient_.SetDigit(Prod_, Diff_);
      *this = Quotient_;
      break;
    }
    else
    {
      GuessQ_ = FindDivGuess(Src_, Trg_);
      ++Prod_;
      --GuessQ_;

      if (GuessQ_)
      {
        TmpResult_ -= (*Trg_ * GuessQ_);
        Prod_ += GuessQ_;
      }
      
      *Src_ = TmpResult_;
    }
  }

  if (SrcSign_ == TrgSign_)
    _Sign = NumberSign::PLUS;
  else
    _Sign = NumberSign::MINUS;

  delete Trg_;
  delete Src_;
          
  return *this;
}

/****************************************************************************/
Boolean LongNumber::IsInSciNote(const ChrString& Str_)
{
  return
  (
    IsNumber(Str_.c_str(), 1, 1, 1, 10) &&
    !IsNumber(Str_.c_str(), 1, 1, 0, 10)
  );
}

/****************************************************************************/
Boolean LongNumber::IsValue(const ChrString& Str_)
{
  return
  (
    IsNumber(Str_.c_str(), 1, 1, 1, 10) ||
    IsNumber(Str_.c_str(), 1, 0, 0, 16) ||
    IsNumber(Str_.c_str(), 1, 0, 0, 8)
  );
}

/****************************************************************************/
LongNumber LongNumber::ValueOf(const ChrString& Str_)
{
  LongNumber ThisNum_(Str_);
  return ThisNum_;
}

/****************************************************************************/
void LongNumber::Clear()
{
  delete _Head;
  _Head = _Tail = NULL;  
  delete _DecimalPt;
}

/****************************************************************************/
LongNumber& LongNumber::SetValue(Ulong Value_, int Sign_)
{
  _Base = 10;
  _Sign = (Sign_ < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;
  FeedNumberValue(Value_);

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (long Value_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = (Value_ < 0) ? NumberSign::MINUS:NumberSign::PLUS;  
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;
  Value_ = ::labs(Value_);
  FeedNumberValue(Value_);

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const LongNumber& Obj_)
{
  if (this != &Obj_)
  {
    _NanVal = Obj_._NanVal;
    _InfVal = Obj_._InfVal;
    _Base =  Obj_._Base;  
    _Sign = Obj_._Sign;
    _NumNodes = Obj_._NumNodes;
    if (Obj_._SciNote)
      _SciNote = Obj_._SciNote;
    _DivPrecision = Obj_.GetPrecision();

    Clear();
    _DecimalPt = new DecimalPoint(*Obj_._DecimalPt);
    _Head = Obj_._Head ? (new LongNode(Obj_._Head, Obj_._Head ? Obj_._Head->GetPrevious():NULL)):NULL;    
    _Tail = FindTail();

    TypeWrapperImp::operator = (Obj_);
  }
  
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const ChrString& NumStr_)
{
  ChrString Temp_ = NumStr_;
  Temp_.ToLower();
  bool Neg_ = false;

  if (Temp_ == "nan")
    *this = vNAN();
  else if ((Temp_ == "inf" || Temp_ == "+inf") || (Neg_ = Temp_ == "-inf"))
  {
    LongNumber Lnum_ = Neg_ ? -vINFINITY():vINFINITY();
    *this = Lnum_;
  }
  else
  {
    _NanVal = _InfVal = false;

    // Non-decimal bases are only valid for integers
    _OriginalBase = _Base = FindRadix(NumStr_.c_str());
    if (_Base != 10 && strchr(NumStr_.c_str(), '.'))
      return *this;

    _Sign = (*NumStr_ == '-') ? NumberSign::MINUS:NumberSign::PLUS;
    _NumNodes = 0;
    _DivPrecision = (NumStr_.strlen() > LongNumber::_DefaultPrecision) ?
                     NumStr_.strlen():LongNumber::_DefaultPrecision;

    Clear();
    _DecimalPt = new DecimalPoint(NumStr_.c_str());
    FeedNumberStr(NumStr_.c_str());
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const LongDouble& FltObj_)
{
  _NanVal = _InfVal = false;
  _Sign = (FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;
  _OriginalBase = _Base = 10;

  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(LongDouble::TYPE_LONGDOUBLE_PRECISION);

    Clear();
    _DecimalPt = new DecimalPoint(TempStr_.c_str());
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    if (FltObj_.IsNan())
      LongNumber::operator = (vNAN());
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      LongNumber::operator = (Lnum_);
    }
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const Double& FltObj_)
{
  _NanVal = _InfVal = false;
  _Sign = (FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;
  _OriginalBase = _Base = 10;

  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(Double::TYPE_DOUBLE_PRECISION);

    Clear();
    _DecimalPt = new DecimalPoint(TempStr_.c_str());
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    if (FltObj_.IsNan())
      LongNumber::operator = (vNAN());
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      LongNumber::operator = (Lnum_);
    }
  }  

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const Float& FltObj_)
{
  _NanVal = _InfVal = false;
  _Sign = (FltObj_.FloatValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;
  _OriginalBase = _Base = 10;

  if (!FltObj_.IsNan() && !FltObj_.IsInfinite())
  {
    ChrString TempStr_ = FltObj_.ToString(Float::TYPE_FLOAT_PRECISION);

    Clear();
    _DecimalPt = new DecimalPoint(TempStr_.c_str());
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    if (FltObj_.IsNan())
      LongNumber::operator = (vNAN());
    else
    {
      LongNumber Lnum_ = (FltObj_.FloatValue() < 0) ? -vINFINITY():vINFINITY();
      LongNumber::operator = (Lnum_);
    }
  }  

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const Integer& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = (Obj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  if (_Sign == NumberSign::MINUS && Obj_.AtMinValue())
  {
    ChrString TempStr_ = Obj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -Obj_.IntValue():Obj_.IntValue();
    FeedNumberValue(Value_);
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const LongInt& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = (Obj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  if (_Sign == NumberSign::MINUS && Obj_.AtMinValue())
  {
    ChrString TempStr_ = Obj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -Obj_.IntValue():Obj_.IntValue();
    FeedNumberValue(Value_);
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const ShortInt& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = (Obj_.IntValue() < 0) ? NumberSign::MINUS:NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  if (_Sign == NumberSign::MINUS && Obj_.AtMinValue())
  {
    ChrString TempStr_ = Obj_.ToString();
    FeedNumberStr(TempStr_.c_str());
  }
  else
  {
    Ulong Value_ = (_Sign == NumberSign::MINUS) ? -Obj_.IntValue():Obj_.IntValue();
    FeedNumberValue(Value_);
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const UInteger& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  Ulong Value_ = Obj_.UnsignedValue();
  FeedNumberValue(Value_);

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const ULongInt& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  Ulong Value_ = Obj_.UnsignedValue();
  FeedNumberValue(Value_);

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::operator = (const UShortInt& Obj_)
{
  _NanVal = _InfVal = false;
  _OriginalBase = _Base = 10;
  _Sign = NumberSign::PLUS;
  _NumNodes = 0;

  Clear();
  _DecimalPt = new DecimalPoint;

  Ulong Value_ = Obj_.UnsignedValue();
  FeedNumberValue(Value_);

  return *this;
}

/****************************************************************************/
LongNode* LongNumber::FindTail()
{
  LongNode* NodePtr_ = _Head;

  if (NodePtr_)
    while (NodePtr_->GetNext())
      NodePtr_ = NodePtr_->GetNext();

  return NodePtr_;
}

/****************************************************************************/
LongNumber& LongNumber::IncExponent(long Shift_, Boolean Back_)
{
  _DecimalPt->IncreaseExponent(Shift_, Back_);
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::DecExponent(long Shift_, Boolean Back_)
{
  _DecimalPt->DecreaseExponent(Shift_, Back_);
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::AddNodeWithStr(const char* NumStr_, Ushort Radix_, Boolean RightOfPoint_)
{
  if (_Head)
  {
    LongNode* Node_ = _Head;
    _Head = Node_->AddNodeWithStr(NumStr_, Radix_);
  }
  else
    _Tail = _Head = new LongNode(NumStr_, Radix_);

  ++_NumNodes;
  if (RightOfPoint_)
    _DecimalPt->ShiftRight(BcdDigit::DigitsPerLong());
  
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::AddNodeWithValue(Ulong NumVal_, Boolean RightOfPoint_)
{
  if (_Head)
  {
    LongNode* Node_ = _Head;  
    _Head = Node_->AddNodeWithValue(NumVal_);
  }
  else
    _Tail = _Head = new LongNode(NumVal_);

  ++_NumNodes;
  if (RightOfPoint_)
    _DecimalPt->ShiftRight(BcdDigit::DigitsPerLong());
  
  return *this;
}

/****************************************************************************/    
LongNumber& LongNumber::AppendNodeWithStr(const char* NumStr_, Ushort Radix_)
{
  if (_Tail)
  {
    LongNode* Node_ = _Tail;
    _Tail = Node_->AppendNodeWithStr(NumStr_, Radix_);
  }
  else
    _Head = _Tail = new LongNode(NumStr_, Radix_);

  ++_NumNodes;
  return *this;
}

/****************************************************************************/    
LongNumber& LongNumber::AppendNodeWithValue(Ulong NumVal_)
{
  if (_Tail)
  {
    LongNode* Node_ = _Tail;
    _Tail = Node_->AppendNodeWithValue(NumVal_);
  }
  else
    _Head = _Tail = new LongNode(NumVal_);

  ++_NumNodes;
  return *this;
}

/****************************************************************************/
Ulong LongNumber::CharToBcd(char DigitChar_, Ushort Exp_)
{
  return
  (
    (isxdigit(DigitChar_) && !isdigit(DigitChar_)) ?
        ((Ulong(DigitChar_ - 'A' + 0x0A) & 0x0F) << ((Exp_ % BcdDigit::DigitsPerLong()) * 4)):
        ((Ulong(DigitChar_ - '0') & 0x0F) << ((Exp_ % BcdDigit::DigitsPerLong()) * 4))
  );
}

/****************************************************************************/
char LongNumber::BcdToChar(Ushort DigitVal_)
{
  return
  (
    (0x0A <= DigitVal_ && DigitVal_ <= 0x0F) ? ((DigitVal_ - 0x0A) + 'A'):
    (DigitVal_ <= 9) ? (DigitVal_ + '0'):0
  );
}

/****************************************************************************/
// PURPOSE:
//   Method to remove the point and sign from a number string leaving only
//   the digits. If Hex digits are present they are converted to upper case.
//   Scientific notation symbols 'e', 'E' are also removed
//
//	The characters in s must match this generic format:
//	  þ [ws] [sn] [0] [x] [ddd] [.] [ddd]
//
//	where
//
//	  þ [ws]   =  optional whitespace
//	  þ [sn]   =  optional sign (+ or -)
//	  þ [ddd]  =  optional digits
//	  þ [.]    =  optional decimal point
//	  þ [0]    =  optional zero (0)
//	  þ [x]    =  optional x or X
//
char* LongNumber::RemoveNonDigits(char* NumStr_, Ushort Radix_)
{
  Ulong i, x, Max_;

  // Remove scientific notation symbols if base is 10
  if (Radix_ == 10)
  {
    char* ptr_;
    if ((ptr_ = strchr(NumStr_, 'e')) ||
        (ptr_ = strchr(NumStr_, 'E')))
      *ptr_ = 0;
  }
  
  Max_ = strlen(NumStr_);
  for (i = x = 0; i < Max_; i++)
  {
    for (;i < Max_ && !IsValidDigit(NumStr_[i], Radix_); i++);

    // convert hex digits to upper case.
    // Should probably be in another function to preserve functional
    // integrity, but good optimization to do it here while filtering.
    NumStr_[x++] = toupper(NumStr_[i]);
  }
  NumStr_[x] = '\0';
  
  return NumStr_;
}

/****************************************************************************/
char* LongNumber::FormatForBcdNode(char* NumStr_)
{
  size_t Size_ = strlen(NumStr_);

  if (Size_ % BcdDigit::DigitsPerLong())
  {
    size_t NewLen_ = Size_ + (BcdDigit::DigitsPerLong() - Size_ % BcdDigit::DigitsPerLong());
    size_t Remain_ = NewLen_ - Size_;
    memmove(NumStr_ + Remain_, NumStr_, strlen(NumStr_) + 1);
    memset(NumStr_, '0', Remain_);    
  }

  return NumStr_;
}

/****************************************************************************/
char* LongNumber::ConvertDecToBase(Ushort Radix_) const
{
  if (AtZero())
    return strcpy(new_char_array(_DivPrecision + 20, NULL), "0");

  ChrString TempStr_;

  LongNumber* Source_ = new LongNumber(*this);
  LongNumber* Remainder_ = new LongNumber;
  LongNumber* BaseUnit_ = new LongNumber(Ulong(Radix_), NumberSign::PLUS);
  ShortInt Digit_;

  while (!Source_->AtZero())
  {
    *Remainder_ = *Source_;
    *Remainder_ %= *BaseUnit_;
    Digit_ = Remainder_->ToString(10);
    Digit_ = ::AbsoluteValue(Digit_);
    TempStr_ += Character::FromDigit(Digit_.IntValue(), Radix_).ToString();
    Source_->DivInt(*BaseUnit_);
  }

  delete Source_;
  delete Remainder_;
  delete BaseUnit_;

  char* NumStr_ = strcpy(new_char_array(strlen(TempStr_.c_str()) + _DivPrecision + 20, NULL), TempStr_.c_str());
  StrReverse(NumStr_);

  return NumStr_;
}

/****************************************************************************/
LongNumber* LongNumber::ConvertBaseToDec(const char* NumStr_, Ushort Radix_) const
{
  size_t i, Max_;
  Max_ = strlen(NumStr_);

  for (i = 0; i < Max_; i++)
    if (NumStr_[i] != '0')
      break;

  if (i == Max_)
    return (new LongNumber);

  LongNumber* Result_ = new LongNumber;
  LongNumber* BaseUnit_ = new LongNumber(Ulong(Radix_), NumberSign::PLUS);
  LongNumber* Augend_;
    
  for (i = 0; i < Max_; i++)
  {
    if (i)
      *Result_ *= *BaseUnit_;

    if (NumStr_[i] == '0')
      continue;      
    else
    {
      Augend_ = new LongNumber(Character::ToDigit(NumStr_[i], Radix_), NumberSign::PLUS);
      *Result_ += *Augend_;
      delete Augend_;
    }
  }

  Result_->_Sign = _Sign;
  Result_->_Base = 10;
  *Result_->_DecimalPt = DecimalPoint();

  LongNumber* Divisor_ = new LongNumber(1UL, NumberSign::PLUS);
  Max_ = OffsetExponent(0);
  
  for (i = 0; i < Max_; i++)
    if (i)
      *Divisor_ *= *BaseUnit_;

  if (i)
    *Result_ /= *Divisor_;

  delete BaseUnit_;
  delete Divisor_;
  
  return Result_;
}

/****************************************************************************/
void LongNumber::FeedNumberStr(const char* NumStr_)
{
  char* Buffer_ = new_char_array(BcdDigit::DigitsPerLong() + 1, NULL);
  size_t i, Max_;

  // Strip the sign and point to prepare for feeding number string as
  // pieces into a linked list of LongNode bcd string objects.
  char* IntStr_ = StripLeadingZeros(RemoveNonDigits(NewString(NumStr_), _Base));
  Max_ = strlen(IntStr_);

  if (Max_)
  {
    if (_Base != 10)
    {
      LongNumber* Result_ = ConvertBaseToDec(IntStr_, _Base);
      *this = *Result_;
      delete Result_;
    }
    else
    {    
      // Format the input string so pieces of it can be assign to each
      // bcd node in correct order.
      Max_ = strlen(FormatForBcdNode(IntStr_));
  
      // If length of string is non zero then make initial LongNode object
      // assign this to the head of the node.
      AddNodeWithStr(Substring(IntStr_, Buffer_, 0, BcdDigit::DigitsPerLong()), _Base, FALSE);

      // Append new LongNode objects to the linked list as required until
      // number string is completely processed. The head pointer will be placed
      // at the new head of the list with each assignment of a new LongNode
      // object.
      if (_Head)
        for (i = BcdDigit::DigitsPerLong(); i < Max_; i += BcdDigit::DigitsPerLong())
          AddNodeWithStr(Substring(IntStr_, Buffer_, i, i + BcdDigit::DigitsPerLong()), _Base, FALSE);
    }
  }
  else
  {
    _Base = 10;  
    AddNodeWithValue(0, FALSE);
  }

  // Deallocate the stripped number string
  ::DeleteArray(Buffer_);
  ::DeleteArray(IntStr_);
}

/****************************************************************************/
void LongNumber::FeedNumberValue(Ulong Value_)
{
  Ulong UnitMax_ = (Ulong)floor(::pow(10, BcdDigit::DigitsPerLong()));
  Ulong BcdMax_ = UnitMax_ - 1;
  Ulong NodeValue_ = Value_;
  Ulong Nodes_;
  
  for (Nodes_ = 0;
       NodeValue_ > BcdMax_;
       NodeValue_ /= UnitMax_, ++Nodes_);

  for (;;)
  {
    Ulong BcdNum_, Exp_;
    for (BcdNum_ = Exp_ = 0; NodeValue_ / Ulong(floor(::pow(10, Exp_))); ++Exp_);
    for (--Exp_; NodeValue_; --Exp_)
    {
      Ulong Divisor_ = (Ulong)floor(::pow(10, Exp_));
      Ulong Mask_ = (NodeValue_ / Divisor_) << (Exp_ * 4);
      NodeValue_ %= Divisor_;
      BcdNum_ |= Mask_;
    }

    AddNodeWithValue(BcdNum_, FALSE);

    if (Nodes_)
    {
      NodeValue_ = Value_ % Ulong(floor(::pow(UnitMax_, Nodes_)));
      --Nodes_;
      NodeValue_ /= Ulong(floor(::pow(UnitMax_, Nodes_)));
    }
    else
      break;
  }
}

/****************************************************************************/
Boolean LongNumber::IsInteger() const
{
  return (_DecimalPt->GetRightShift() == 0);
}

/****************************************************************************/
// PURPOSE:
//   Method to return the total number of digits currently written to the
//   bcd string
//
Ulong LongNumber::TotalDigits() const
{
  LongNode* NodePtr_ = _Tail;
  Ulong HighestDigit_ = AvailableDigits();
  Ulong DigitsInNode_ = 0;  

  while (!DigitsInNode_ && NodePtr_)
  {
    DigitsInNode_ = NodePtr_->TotalDigits();
    HighestDigit_ -= (BcdDigit::DigitsPerLong() - DigitsInNode_);
    NodePtr_ = NodePtr_->GetPrevious();
  }
  
  return HighestDigit_;
}

/****************************************************************************/
BcdDigit LongNumber::RawGetDigit(Ulong Exp_) const
{
  // Check if specified exponent position is filled with a digit
  // otherwise return the default 0 value bcd digit.
  if (Exp_ >= TotalDigits())
    return BcdDigit();

  // Find the index of the bcd list node containing the exponent position
  Ulong NodeIndex_ = Exp_ / BcdDigit::DigitsPerLong();

  // only if the node index is within the current number of nodes then
  // Iterate from the head of the list up to the specified node and
  // get the digit at the specified exponent position.
  if (NodeIndex_ < _NumNodes)
  {
    Ulong i;
    LongNode* NodePtr_ = _Head;

    if (NodePtr_)
    {
      for (i = 0; i < NodeIndex_; ++i)
        NodePtr_ = NodePtr_->GetNext();

      return NodePtr_->GetDigit(Exp_);
    }    
  }

  // return 0-value bcd digit if all else fails
  return BcdDigit();
}

/****************************************************************************/
LongNumber& LongNumber::RawSetDigit(const BcdDigit& Digit_, Ulong Exp_)
{
  // The bcd list should have enough digits to fit the carry,
  // otherwise do nothing and return
  if (Exp_ >= AvailableDigits())
    return *this;

  // Find the index of the bcd list node containing the exponent position
  Ulong NodeIndex_ = Exp_ / BcdDigit::DigitsPerLong();

  // only if the node index is within the current number of nodes then
  // Iterate from the head of the list up to the specified node and
  // set the carry in that node.
  if (NodeIndex_ < _NumNodes)
  {
    Ulong i;
    LongNode* NodePtr_ = _Head;

    if (NodePtr_)
    {
      for (i = 0; i < NodeIndex_; ++i)
        NodePtr_ = NodePtr_->GetNext();
      
      NodePtr_->SetDigit(Digit_, Exp_);
    }
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::RawSetCarry(const BcdDigit& Digit_, Ulong Exp_)
{
  // The bcd list should have enough digits to fit the carry,
  // otherwise do nothing and return
  if ((Exp_ + 1) >= AvailableDigits())
    return *this;

  // The carry will be set at the next exponent position rather than
  // the current position
  ++Exp_;  

  // Find the index of the bcd list node containing the exponent position
  Ulong NodeIndex_ = Exp_ / BcdDigit::DigitsPerLong();

  // only if the node index is within the current number of nodes then
  // Iterate from the head of the list up to the specified node and
  // set the carry in that node.
  if (NodeIndex_ < _NumNodes)
  {
    Ulong i;
    LongNode* NodePtr_ = _Head;

    if (NodePtr_)
    {
      for (i = 0; i < NodeIndex_; ++i)
        NodePtr_ = NodePtr_->GetNext();
      
      NodePtr_->SetCarry(Digit_, Exp_);
    }
  }

  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::RawClearDigit(Ulong Exp_)
{
  return RawSetDigit(BcdDigit(0), Exp_);
}

/****************************************************************************/
char* LongNumber::RawToString(int Radix_) const
{
  if (IsInteger() && Radix_ != _Base && _Base == 10)
    return ConvertDecToBase(Radix_);
  else
    Radix_ = _Base;

  Ulong i, Max_, Exp_;
  LongNode* NodePtr_ = _Head;
  Boolean ErrConvert_;
  
  Max_ = TotalDigits();
  char* Buffer_ = new_char_array(Max_ + _DivPrecision + 20, NULL);

  if (!Max_)
    return strcpy(Buffer_, "0");
  
  for (i = Exp_ = 0; i < _NumNodes && NodePtr_ && Exp_ < Max_; i++)
  {
    Buffer_[Exp_] = BcdToChar(NodePtr_->GetDigit(Exp_).Digit());
    
    if (!Buffer_[Exp_])
      break;

    ErrConvert_ = FALSE;      
    for (++Exp_; Exp_ % BcdDigit::DigitsPerLong() && Exp_ < Max_; ++Exp_)
    {
      Buffer_[Exp_] = BcdToChar(NodePtr_->GetDigit(Exp_).Digit());
      if (ErrConvert_ = !Buffer_[Exp_])
        break;
    }

    if (ErrConvert_)
      break;

    NodePtr_ = NodePtr_->GetNext();
  }

  Buffer_[Exp_] = 0;
  StrReverse(Buffer_);
  return Buffer_;
}

/****************************************************************************/
long LongNumber::LowestExp() const
{
  return (0L - OffsetExponent(0));
}

/****************************************************************************/
long LongNumber::HighestExp() const
{
  Ulong TotDig_ = TotalDigits();
  return (TotDig_ ? (TotDig_ - OffsetExponent(0) - 1):0);
}

/****************************************************************************/
long LongNumber::OffsetExponent(long Exp_) const
{
  return (Exp_ +
          long(_DecimalPt->GetRightShift()) +
          long(_DecimalPt->GetDecExponent()) -
          long(_DecimalPt->GetIncExponent()));
}

/****************************************************************************/
Ushort LongNumber::ConvertedFromBase() const
{
  return _OriginalBase;
}

/****************************************************************************/
long LongNumber::GetPrecision() const
{
  return _DivPrecision;
}

/****************************************************************************/
long LongNumber::SetDefaultPrecision(long Prec_)
{
  _DefaultPrecision = Prec_;
  return Prec_;
}

/****************************************************************************/
LongNumber& LongNumber::SetPrecision(long Prec_)
{
  _DivPrecision = Prec_;
  return *this;
}

/****************************************************************************/
BcdDigit LongNumber::GetDigit(long Exp_) const
{
  Exp_ = OffsetExponent(Exp_);
  return ((Exp_ < 0 || Exp_ >= TotalDigits()) ? BcdDigit():RawGetDigit(Exp_));
}

/****************************************************************************/
LongNumber& LongNumber::ClearDigit(long Exp_)
{
  Exp_ = OffsetExponent(Exp_);  
  return ((Exp_ < 0 || Exp_ >= TotalDigits()) ? *this:RawClearDigit(Exp_));
}

/****************************************************************************/
LongNumber& LongNumber::SetDigit(const BcdDigit& Digit_, long Exp_)
{
  long OldExp_ = Exp_;
  Exp_ = OffsetExponent(Exp_);
  return ((Exp_ < 0) ? AddNodeWithValue(0, TRUE).SetDigit(Digit_, OldExp_):
          (Exp_ >= AvailableDigits()) ? AppendNodeWithValue(0).SetDigit(Digit_, OldExp_):
                                        RawSetDigit(Digit_, Exp_));
}

/****************************************************************************/    
LongNumber& LongNumber::SetCarry(const BcdDigit& Digit_, long Exp_)
{
  long OldExp_ = Exp_;
  Exp_ = OffsetExponent(Exp_);
  long CarryExp_ = Exp_ + 1;
  return ((CarryExp_ < -1) ? AddNodeWithValue(0, TRUE).SetCarry(Digit_, OldExp_):
          (CarryExp_ + 1 >= AvailableDigits()) ? AppendNodeWithValue(0).SetCarry(Digit_, OldExp_):
                                                 RawSetCarry(Digit_, Exp_));
}

/****************************************************************************/
LongNumber& LongNumber::SetSciNotation(Boolean Flag_)
{
  _SciNote = Flag_;
  return *this;
}

/****************************************************************************/
LongNumber& LongNumber::SetFixedPoint(Boolean Flag_)
{
  _SciNote = !Flag_;
  return *this;
}

/****************************************************************************/
char* LongNumber::PrependSciNote(char* NumStr_, Ushort Radix_) const
{
  char Buffer_[10];
  size_t i, x, Max_;
  Boolean NzFound_, PtFound_, Zero_;
  long Shift_ = -OffsetExponent(0);  
  long Diff_;  
  
  Max_ = strlen(NumStr_);
  Zero_ = NzFound_ = PtFound_ = FALSE;

  if (!Max_ || !_SciNote)
    return NumStr_;

  if (Shift_ < 0)
    Shift_ = 0;
  
  for (x = i = 0; i < Max_; i++)
    if (NumStr_[i] == '.')
    {
      PtFound_ = TRUE;

      if (!NzFound_)
      {
        for (x = i + 1; x < Max_; x++)
          if (IsValidDigit(NumStr_[x], Radix_) && NumStr_[x] != '0')
            break;

        Zero_ = x == Max_;
      }
      
      break;
    }
    else if (!NzFound_ && IsValidDigit(NumStr_[i], Radix_) && NumStr_[i] != '0')
    {
      NzFound_ = TRUE;
      x = i;
    }

  if (PtFound_)
  {
    // non-zero digit found before decimal point
    if (NzFound_)
    {
      x++;                 // location of 2nd digit after 1st non-zero digit
      Diff_ = i - x;       // difference from decimal point to 2nd digit after 1st non-zero digit
      
      if (!Diff_ && !Shift_)
        return NumStr_;
      else
      {
        if (Diff_)
        {
          memmove(&NumStr_[x + 1], &NumStr_[x], Diff_);
          NumStr_[x] = '.';
          if (!NumStr_[x + 1])
            strcat(NumStr_, "0");
        }
        else
          strcat(NumStr_, ".0");
        
        Buffer_[0] = 'e';
        IntToStr(Diff_ + Shift_, &Buffer_[1]);
        strcat(NumStr_, Buffer_);
      }
    }

    // non-zero digit found after decimal point
    // so negative exponent
    else if (!Zero_)
    {
      Diff_ = x - i;
      NumStr_[i] = '0';
      NumStr_[x - 1] = NumStr_[x];
      NumStr_[x] = '.';
      
      if (Max_ == (x + 1))
        strcat(NumStr_, "0");
      
      StripLeadingZeros(NumStr_);
      strcpy(Buffer_, "e-");
      IntToStr(Diff_, &Buffer_[2]);
      strcat(NumStr_, Buffer_);
    }
  }

  // No decimal point found
  else
  {
    // A non-zero digit found
    if (NzFound_)
    {
      x++;                 // location of 2nd digit after 1st non-zero digit
      Diff_ = Max_ - x;    // difference from end of string to 2nd digit after 1st non-zero digit
      
      if (!Diff_ && !Shift_)
        return NumStr_;
      else
      {
        if (Diff_)
        {      
          memmove(&NumStr_[x + 1], &NumStr_[x], Diff_ + 1);
          NumStr_[x] = '.';
          if (!NumStr_[x + 1])
            strcat(NumStr_, "0");
        }
        else
          strcat(NumStr_, ".0");
        
        Buffer_[0] = 'e';
        IntToStr(Diff_ + Shift_, &Buffer_[1]);
        strcat(NumStr_, Buffer_);
      }      
    }
    else
      return NumStr_;
  }

  return NumStr_;
}

/****************************************************************************/
char* LongNumber::PrependBaseModifier(char* NumStr_, Ushort Radix_) const
{
  if (Radix_ != 10)
  {
    if (Radix_ == 8)
    {
      memmove(NumStr_ + 1, NumStr_, strlen(NumStr_) + 1);
      *NumStr_ = '0';      
    }
    else if (Radix_ == 16)
    {
      memmove(NumStr_ + 2, NumStr_, strlen(NumStr_) + 1);
      memmove(NumStr_, "0x", 2);
    }      
  }

  return NumStr_;
}

/****************************************************************************/
char* LongNumber::PrependSign(char* NumStr_) const
{
  Boolean NonZero_ = FALSE;
  long Len_ = strlen(NumStr_);
  long i;
  
  for (i = 0; i < Len_; i++)
    if (NumStr_[i] != '0' && NumStr_[i] != '.')
    {
      NonZero_ = TRUE;
      break;
    }

  if (_Sign == NumberSign::MINUS && NonZero_)
  {
    memmove(NumStr_ + 1, NumStr_, strlen(NumStr_) + 1);
    *NumStr_ = '-';
  }

  return NumStr_;
}

/****************************************************************************/
// Convert all digits including digits left of decimal point
//
ChrString LongNumber::ToString(int Radix_) const
{
  // handle nan and infinite values first
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (_Sign == NumberSign::MINUS)
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }  

  // Only valid for radix in the range 1 to 16
  // Non-decimal bases are only valid for integers
  // Division by zero is invalid.  
  if (Radix_ < 1 || Radix_ > 16 ||
      Radix_ != 10 && !IsInteger())
    return ChrString("ERROR: Invalid Radix Value");
  else if (DivideByZero())
    return ChrString("ERROR: Divide By Zero");

  char* NumStr_ = RawToString(Radix_);
  long Shift_ = OffsetExponent(0);
  long PosShift_;
  long Len_ = strlen(NumStr_);
  long i;

  if (Shift_ < 0)
  {
    PosShift_ = labs(Shift_);
    Shift_ = 0;
  }
  else
    PosShift_ = 0;
  
  if (Shift_)
    if (Shift_ <= Len_)
    {
      Boolean NoFrac_ = TRUE;
      char* FracPart_ = &NumStr_[Len_ - Shift_];
      Len_ = strlen(FracPart_);

      for (i = 0; i < Len_ && NoFrac_; i++)
        if (FracPart_[i] != '0')
          NoFrac_ = 0;

      if (FracPart_ == NumStr_)
      {
        if (!NoFrac_)
        {
          memmove(FracPart_ + 2, FracPart_, Len_ + 1);
          FracPart_[0] = '0';          
          FracPart_[1] = '.';
        }
        else
        {          
          FracPart_[0] = '0';
          FracPart_[1] = 0;
        }
      }
      else
      {
        if (!NoFrac_)
        {
          memmove(FracPart_ + 1, FracPart_, Len_ + 1);          
          FracPart_[0] = '.';
        }
        else
          FracPart_[0] =  0;
      }
    }
    else if (strcmp(NumStr_, "0") != 0)
    {
      Boolean NoFrac_ = TRUE;      
      Ulong Diff_ = Shift_ - Len_;

      for (i = 0; i < Len_ && NoFrac_; i++)
        if (NumStr_[i] != '0')
          NoFrac_ = 0;

      if (!NoFrac_)
      {
        memmove(NumStr_ + Diff_ + 2, NumStr_, Len_ + 1);
        NumStr_[0] = '0';
        NumStr_[1] = '.';
        Diff_ += 2;
        
        for (i = 2; i < Diff_; i++)
          NumStr_[i] = '0';
      }
      else
      {
        NumStr_[0] = '0';
        NumStr_[1] = 0;
      }
    }
  
  if (PosShift_ && !_SciNote)
  {
    ChrString Trailz_('0', PosShift_);
    strcat(NumStr_, Trailz_.c_str());
  }  

  StripTrailingZeros(PrependSign(PrependBaseModifier(PrependSciNote(StripPaddingZeros(NumStr_, Radix_), Radix_), Radix_)));
  ChrString RetStr_(NumStr_);
  ::DeleteArray(NumStr_);
  
  return RetStr_;
}

/****************************************************************************/
// Only convert digits left of decimal point (no fractions)
//
ChrString LongNumber::ToTruncString(int Radix_) const
{
  // handle nan and infinite values first
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (_Sign == NumberSign::MINUS)
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }  

  // Only valid for radix in the range 1 to 16
  // Non-decimal bases are only valid for integers
  // Division by zero is invalid.
  if (Radix_ < 1 || Radix_ > 16 ||
      Radix_ != 10 && !IsInteger())
    return ChrString("ERROR: Invalid Radix Value");
  else if (DivideByZero())
    return ChrString("ERROR: Divide By Zero");  

  char* NumStr_ = RawToString(Radix_);
  long Shift_ = OffsetExponent(0);
  long PosShift_;
  long Len_ = strlen(NumStr_);

  if (Shift_ < 0)
  {
    PosShift_ = labs(Shift_);
    Shift_ = 0;
  }
  else
    PosShift_ = 0;
  
  if (Shift_)
    if (Shift_ <= Len_)
    {  
      char* FracPart_ = &NumStr_[Len_ - Shift_];

      if (FracPart_ == NumStr_)
        strcpy(NumStr_, "0");
      else
        FracPart_[0] = 0;
    }
    else if (strcmp(NumStr_, "0") != 0)
      strcpy(NumStr_, "0");

  if (PosShift_ && !_SciNote)
  {
    ChrString Trailz_('0', PosShift_);
    strcat(NumStr_, Trailz_.c_str());
  }

  StripTrailingZeros(PrependSign(PrependBaseModifier(PrependSciNote(StripPaddingZeros(NumStr_, Radix_), Radix_), Radix_)));
  ChrString RetStr_(NumStr_);
  ::DeleteArray(NumStr_);
  
  return RetStr_;
}

/****************************************************************************/
ChrString LongNumber::ToString(const LongNumber& Int_, int Radix_)
{
  LongNumber Tmp_(Int_);
  return Tmp_.ToString(Radix_);
}

/****************************************************************************/
ChrString LongNumber::ToTruncString(const LongNumber& Int_, int Radix_)
{
  LongNumber Tmp_(Int_);
  return Tmp_.ToTruncString(Radix_);
}

/****************************************************************************/
LongNumber& LongNumber::FromString(const ChrString& Str_)
{
  *this = Str_;
  return *this;
}

/****************************************************************************/
Boolean LongNumber::AtZero() const
{
  return (!_NumNodes || _NumNodes == 1 && _Head->TotalDigits() == 0);
}

/****************************************************************************/
Boolean LongNumber::operator ! () const
{
  return (!_NumNodes || _NumNodes == 1 && _Head->TotalDigits() == 0);
}

/****************************************************************************/
LongNumber LongNumber::operator - () const
{
  LongNumber Obj_(*this);
  Obj_._Sign = NumberSign::MINUS;
  return Obj_;
}

/****************************************************************************/
LongNumber LongNumber::operator + () const
{
  LongNumber Obj_(*this);
  return Obj_;
}

/****************************************************************************/
LongNumber LongNumber::Complement() const
{
  Ulong Exp_, Max_;
  Max_ = TotalDigits();
  LongNumber ThisNum_(*this);
  
  for (Exp_ = 0; Exp_ < Max_; ++Exp_)
  {
    BcdDigit NewDigit_ = RawGetDigit(Exp_);
    ThisNum_.RawSetDigit(NewDigit_.Complement(), Exp_);
  }

  return ThisNum_;
}

/****************************************************************************/
LongNumber LongNumber::UnsignedValue() const
{
  LongNumber Obj_(*this);
  Obj_._Sign = NumberSign::PLUS;
  return Obj_;
}

/****************************************************************************/
void LongNumber::SetData(const ChrString& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
const ChrString& LongNumber::GetData() const
{
  LongNumber *const LocalThis_ = (LongNumber *const)this;
  delete LocalThis_->_StringRep;
  LocalThis_->_StringRep = new ChrString(ToString());
  return *(LocalThis_->_StringRep);
}

/****************************************************************************/      
LongNumber::operator ChrString () const
{
  return ToString();
}

/****************************************************************************/
#if OVERLOAD_NEW
void* LongNumber::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void LongNumber::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* LongNumber::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void LongNumber::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(LongNumberAcceptor::LongNumberAcceptor, LongNumber)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(LongNumberAcceptor::AcceptDataPtr, LongNumber)
OBJACCEPTOR_GIVEDATA_TOPDEFN(LongNumberAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromLongNumber, LongNumber, LongNumber)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromLongNumber, LongNumber)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToLongNumber, LongNumber)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToLongNumber, LongNumber)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToLongNumber, LongNumber)
MEMORYOPS_DEFN(LongNumberAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromLongInt, LongNumber, LongInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromLongInt, LongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToLongNumber, LongNumber, LongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToLongNumber, LongNumber, LongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToLongNumber, LongNumber, LongInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromShortInt, LongNumber, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToLongNumber, LongNumber, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToLongNumber, LongNumber, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToLongNumber, LongNumber, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromInteger, LongNumber, Integer)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromInteger, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToLongNumber, LongNumber, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToLongNumber, LongNumber, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToLongNumber, LongNumber, Integer)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromULongInt, LongNumber, ULongInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromULongInt, ULongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToLongNumber, LongNumber, ULongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToLongNumber, LongNumber, ULongInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToLongNumber, LongNumber, ULongInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromUShortInt, LongNumber, UShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromUShortInt, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToLongNumber, LongNumber, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToLongNumber, LongNumber, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToLongNumber, LongNumber, UShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromUInteger, LongNumber, UInteger)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromUInteger, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToLongNumber, LongNumber, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToLongNumber, LongNumber, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToLongNumber, LongNumber, UInteger)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromChrString, LongNumber, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToLongNumber, LongNumber, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToLongNumber, LongNumber, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToLongNumber, LongNumber, ChrString)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromFloat, LongNumber, Float)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromFloat, Float)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToLongNumber, LongNumber, Float)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToLongNumber, LongNumber, Float)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToLongNumber, LongNumber, Float)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromDouble, LongNumber, Double)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromDouble, Double)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToLongNumber, LongNumber, Double)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToLongNumber, LongNumber, Double)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToLongNumber, LongNumber, Double)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongNumberAcceptor::CreateFromLongDouble, LongNumber, LongDouble)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongNumberAcceptor::AssignFromLongDouble, LongDouble)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToLongNumber, LongNumber, LongDouble)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToLongNumber, LongNumber, LongDouble)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongNumberAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToLongNumber, LongNumber, LongDouble)

/****************************************************************************/
/****************************************************************************/
Boolean LongNumberAcceptor::IsLongNumber() const
{
  return TRUE;
}

/****************************************************************************/
Boolean LongNumberAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
#if LONGNUM_DEBUG
int main()
{
#if LONGNUM_DEBUG_SHOWOUTPUT
  LongNumber Num1_;
  LongNumber Num2_("11223344556677889900");
  LongNumber Num3_(Num2_);

  LongNumber* MultNum_ = Num2_.Make(4294294294UL, NumberSign::PLUS);
  LongNumber* AddNum_ = Num3_.Make(4123123123UL, NumberSign::MINUS);
  Num1_ = "998877665544332211";

  cout <<MultNum_->TotalDigits() <<endl;
  cout <<MultNum_->AvailableDigits() <<endl;  

  cout <<AddNum_->TotalDigits() <<endl;
  cout <<AddNum_->AvailableDigits() <<endl;
  
  cout <<Num1_.TotalDigits() <<endl;
  cout <<Num1_.AvailableDigits() <<endl;

  Ulong i, Max_;
  
  Max_ = Num1_.TotalDigits();
  for (i = 0; i < Max_; ++i)
    cout <<Num1_.GetDigit(i).Digit();
  cout <<endl;

  ChrString NewStr_ = AddNum_->ToString();
  cout <<NewStr_ <<endl;

  NewStr_ = MultNum_->ToString();
  cout <<NewStr_ <<endl;

  NewStr_ = Num1_.ToString();
  cout <<NewStr_ <<endl;

  LongNumber Num1Cmp_ = Num1_.Complement();
  NewStr_ = Num1Cmp_.ToString();
  cout <<NewStr_ <<endl;

  NewStr_ = Num2_.ToString();
  cout <<NewStr_ <<endl;

  NewStr_ = Num3_.ToString();
  cout <<NewStr_ <<endl;

  *MultNum_ = Num3_;
  for (i = 0; i < 10; ++i)
    MultNum_->SetDigit(BcdDigit(i), i);

  BcdDigit Temp_(9);
  Temp_.Add(BcdDigit(2));
  MultNum_->ClearDigit(10);
  
  for (;i < 20; ++i)
    MultNum_->SetCarry(Temp_, i);

  Max_ = MultNum_->TotalDigits();
  for (i = 0; i < Max_; ++i)
    cout <<MultNum_->GetDigit(i).Digit();
  cout <<endl;

  // Testing decimal conversion
  LongNumber NumDec1_("1122334455.6677889900");
  NumDec1_
    .SetDigit(BcdDigit(1), -9)
    .SetDigit(BcdDigit(2), -10)
    .SetDigit(BcdDigit(3), 9)
    .SetDigit(BcdDigit(4), 8)
    .SetCarry(Temp_, -1)
    .ClearDigit(-1);

  long Expn_;
  for (Expn_ = -10; Expn_ < 10; ++Expn_)
    cout <<NumDec1_.GetDigit(Expn_).Digit();
  cout <<endl;

  NewStr_ = NumDec1_.ToString();
  cout <<NewStr_ <<endl;

  // Testing base conversions
  LongNumber HexNum1_("0xDEADBEEFFEEDCAD");
  cout.setf(ios::hex, ios::basefield);  
  for (Expn_ = 0; Expn_ < 15; ++Expn_)
    cout <<HexNum1_.GetDigit(Expn_).Digit();
  cout <<endl;

  cout.setf(ios::dec, ios::basefield);
  NewStr_ = HexNum1_.ToString(16);
  cout <<NewStr_ <<endl;

  LongNumber CmpHex1_ = HexNum1_.Complement();
  NewStr_ = CmpHex1_.ToString(16);
  cout <<NewStr_ <<endl;

  HexNum1_ = ChrString("0xD01DCAFEFEEDDAD");
  NewStr_ = HexNum1_.ToString(16);
  cout <<NewStr_ <<endl;

  HexNum1_ = "-0xBADCAFEBADDEED";
  NewStr_ = HexNum1_.ToString(16);
  cout <<NewStr_ <<endl;

  *MultNum_ = HexNum1_;
  NewStr_ = MultNum_->ToString(16);
  cout <<NewStr_ <<endl;

  MultNum_->SetValue(4889507, NumberSign::PLUS);
  NewStr_ = MultNum_->ToString();
  cout <<NewStr_ <<endl;

  delete MultNum_;
  delete AddNum_;
#endif

#if LONGNUM_DEBUG_ASSERT
  LongNumber Num1_;
  LongNumber Num2_("11223344556677889900");
  LongNumber Num3_(Num2_);
  
  LongNumber* MultNum_ = Num2_.Make(4294294294UL, NumberSign::PLUS);
  LongNumber* AddNum_ = Num3_.Make(4123123123UL, NumberSign::MINUS);
  Num1_ = "998877665544332211";

  assert(MultNum_->TotalDigits() == 10);
  assert(MultNum_->AvailableDigits() == 16);
  assert(MultNum_->HighestExp() == 9);

  assert(AddNum_->TotalDigits() == 10);
  assert(AddNum_->AvailableDigits() == 16);
  assert(AddNum_->HighestExp() == 9);
  
  assert(Num1_.TotalDigits() == 18);
  assert(Num1_.AvailableDigits() == 24);
  assert(Num1_.HighestExp() == 17);

  Ulong i, x, Max_;
  
  Max_ = Num1_.TotalDigits();
  for (i = 0, x = 1; i < Max_; ++i)
  {
    assert(Num1_.GetDigit(i).Digit() == x);
    if (i % 2)
      ++x;
  }

  ChrString NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "-4123123123"));

  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "4294294294"));

  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "998877665544332211"));
  assert(Num2_ > Num1_);  

  LongNumber Num1Cmp_ = Num1_.Complement();
  NewStr_ = Num1Cmp_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1122334455667788"));

  NewStr_ = Num2_.ToString();
  assert(!strcmp(NewStr_.c_str(), "11223344556677889900"));

  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "11223344556677889900"));

  // Testing comparison methods
  assert(*MultNum_ > *AddNum_);
  assert(Num2_ > Num1_);
  assert(Num2_ == Num3_);

  *MultNum_ = Num3_;
  for (i = 0; i < 10; ++i)
    MultNum_->SetDigit(BcdDigit(i), i);

  BcdDigit Temp_(9);
  Temp_.Add(BcdDigit(2));
  MultNum_->ClearDigit(10);
  
  for (;i < 20; ++i)
    MultNum_->SetCarry(Temp_, i);

  Max_ = MultNum_->TotalDigits();
  for (i = 0; i < Max_; ++i)
    if (i < 10)
      assert(MultNum_->GetDigit(i).Digit() == i);
    else if (i == 10)
      assert(MultNum_->GetDigit(i).Digit() == 0);
    else
      assert(MultNum_->GetDigit(i).Digit() == 1);

  // Testing decimal conversion
  LongNumber NumDec1_("1122334455.6677889900");
  LongNumber DifDec_ = NumDec1_;
  LongNumber AddDec_ = NumDec1_;
  NumDec1_
    .SetDigit(BcdDigit(1), -9)
    .SetDigit(BcdDigit(2), -10)
    .SetDigit(BcdDigit(3), 9)
    .SetDigit(BcdDigit(4), 8)
    .SetCarry(Temp_, -1)
    .ClearDigit(-1);

  assert(NumDec1_.LowestExp() == -10);
  assert(NumDec1_.HighestExp() == 9);

  NewStr_ = NumDec1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "3422334451.0677889912"));

  AddDec_ += NumDec1_;
  DifDec_ -= NumDec1_;
  NewStr_ = DifDec_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-2299999995.4000000012"));
  NewStr_ = AddDec_.ToString();
  assert(!strcmp(NewStr_.c_str(), "4544668906.7355779812"));

  DifDec_ -= DifDec_;
  NewStr_ = DifDec_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));

  *MultNum_ = "36";
  DifDec_ = "45";
  assert(MultNum_->HighestExp() == 1);
  *MultNum_ *= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "1620"));
  assert(MultNum_->HighestExp() == 3);

  // new div test
  DifDec_.SetValue(15, NumberSign::PLUS);
  assert(DifDec_.HighestExp() == 1);
  *MultNum_ /= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "108"));
  
  DifDec_ = *MultNum_;
  *MultNum_ /= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "1"));

  *MultNum_ = "108";
  DifDec_.SetValue(12158, NumberSign::PLUS);
  assert(DifDec_.HighestExp() == 4);
  *MultNum_ /= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strncmp(NewStr_.c_str(), "0.00888303997367988", 19));
  /// ---

  *MultNum_ = "567890";
  DifDec_ = "12345";
  *MultNum_ *= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "7010602050"));
  MultNum_->SetSciNotation();
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "7.01060205e9"));
  MultNum_->SetSciNotation(FALSE);

  // new div test
  DifDec_.SetValue(823, NumberSign::MINUS);
  *MultNum_ /= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "-8518350"));

  *MultNum_ *= DifDec_;
  DifDec_.SetValue(850, NumberSign::MINUS);
  *MultNum_ /= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strncmp(NewStr_.c_str(), "-8247767.11764706", 16));
  MultNum_->SetSciNotation();
  NewStr_ = MultNum_->ToString();
  cout <<"MultNum(1) = " <<NewStr_ <<endl;
  MultNum_->SetSciNotation(FALSE);
  /// ---
  
  *MultNum_ = "111111.111111";
  DifDec_ = "999999.999999";
  *MultNum_ *= DifDec_;
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "111111111110.888888888889"));
  MultNum_->SetSciNotation();
  NewStr_ = MultNum_->ToString();
  cout <<"MultNum(2) = " <<NewStr_ <<endl;
  MultNum_->SetSciNotation(FALSE);

  // Testing base conversions
  LongNumber HexNum1_("0xDEADBEEFFEEDCAD");
  NewStr_ = HexNum1_.ToString(16);
  assert(!strcmp(NewStr_.c_str(), "0xDEADBEEFFEEDCAD"));

  HexNum1_ = ChrString("0xD01DCAFEFEEDDAD");
  NewStr_ = HexNum1_.ToString(16);
  assert(!strcmp(NewStr_.c_str(), "0xD01DCAFEFEEDDAD"));

  HexNum1_ = "0xBADCAFEBADDEED";
  NewStr_ = HexNum1_.ToString(16);
  assert(!strcmp(NewStr_.c_str(), "0xBADCAFEBADDEED"));

  *MultNum_ = HexNum1_;
  NewStr_ = MultNum_->ToString(16);
  assert(!strcmp(NewStr_.c_str(), "0xBADCAFEBADDEED"));
  assert(*MultNum_ == HexNum1_);

  // testing signed values
  MultNum_->SetValue(4889507, NumberSign::PLUS);
  NewStr_ = MultNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "4889507"));
  assert(*MultNum_ != HexNum1_);

  *AddNum_ = -(*MultNum_);
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "-4889507"));
  assert(*MultNum_ != *AddNum_);

  // Testing number expansion
  AddNum_->SetDigit(BcdDigit(9), 9);
  AddNum_->SetDigit(BcdDigit(2), 8);
  AddNum_->SetDigit(BcdDigit(4), -2);
  AddNum_->SetDigit(BcdDigit(6), -4);
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "-9204889507.0406"));

  *AddNum_ = "0";
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));

  AddNum_->SetDigit(BcdDigit(9), 9);
  AddNum_->SetDigit(BcdDigit(3), 8);
  AddNum_->SetDigit(BcdDigit(3), -1);
  AddNum_->SetDigit(BcdDigit(9), -2);
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "9300000000.39"));
  
  // Testing string validation and conversion
  assert(LongNumber::IsValue("-45231"));
  assert(!LongNumber::IsValue("45-231"));
  assert(LongNumber::IsValue("-3515.51501"));
  assert(!LongNumber::IsInSciNote("-3515.51501"));
  assert(LongNumber::IsValue("-0xdeadbeef"));
  assert(!LongNumber::IsValue("0x-deadbeef"));
  assert(LongNumber::IsValue("-3515.51501e15"));
  assert(LongNumber::IsInSciNote("-3515.51501e15"));
  assert(!LongNumber::IsValue("-0xdeadbeefe+12"));
  assert(LongNumber::IsValue("-2147483648"));
  assert(LongNumber::IsValue("4294967295"));

  Num1_ = "9876.5432";
  Num2_ = "987.65432";
  Num3_ = "9877.5432";
  *AddNum_ = "98765.432";
  *MultNum_ = "9875.5432";

  // Testing comparison methods
  assert(Num1_ > Num2_);
  assert(Num1_ < *AddNum_);
  assert(Num1_ > *MultNum_);
  assert(Num1_ < Num3_);
  assert(Num3_ > Num2_);
  assert(*AddNum_ > *MultNum_);  

  // testing boundary conditions
  Num3_.SetFixedPoint();
  AddNum_->SetFixedPoint();
  Num1_.SetFixedPoint();
  Num2_.SetFixedPoint();
  
  // addition boundaries
  Num3_ = "5555";
  *AddNum_ = "5555";  
  Num1_ = "1111";
  Num2_ = "9999";
  Num1_ += Num2_;
  *AddNum_ += Num3_;
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "11110"));  
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "11110"));

  Num3_ = "12345";
  *AddNum_ = "98765";
  Num1_ = "1111";
  Num2_ = "-1111";
  Num1_ += Num2_;
  *AddNum_ += Num3_;
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "111110"));  
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));
  
  Num3_ = "101010";
  *AddNum_ = "20202";
  Num1_ = "121212";
  Num2_ = "989898";
  Num1_ += Num2_;
  *AddNum_ += Num3_;
  NewStr_ = AddNum_->ToString();
  assert(!strcmp(NewStr_.c_str(), "121212"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1111110"));

  // subtraction boundaries
  Num3_ = "1111";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "1111";  
  Num3_ -= *AddNum_;
  Num1_ -= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1111"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-1111"));

  Num3_ = "1";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "1";
  Num3_ -= *AddNum_;
  Num1_ -= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-1"));

  Num3_ = "10000";
  *AddNum_ = "55555";
  Num1_ = "55555";
  Num2_ = "10000";
  Num3_ -= *AddNum_;
  Num1_ -= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-45555"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "45555"));

  Num3_ = "10000";
  *AddNum_ = "1";
  Num1_ = "1";
  Num2_ = "10000";
  Num3_ -= *AddNum_;
  Num1_ -= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "9999"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-9999"));

  Num3_ = "99999";
  *AddNum_ = "12345";
  Num1_ = "12345";
  Num2_ = "99999";
  Num3_ -= *AddNum_;
  Num1_ -= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "87654"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-87654"));

  // multiplication boundaries
  Num3_ = "1";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "1";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));

  Num3_ = "11111";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "11111";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));
  Num1_.SetSciNotation();
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));
  Num1_.SetSciNotation(FALSE);

  Num3_ = "12345";
  *AddNum_ = "1";
  Num1_ = "1";
  Num2_ = "12345";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "12345"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "12345"));

  Num3_ = "11";
  *AddNum_ = "11";
  Num1_ = "11";
  Num2_ = "-11";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "121"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-121"));

  Num3_ = "-11";
  *AddNum_ = "11";
  Num1_ = "-11";
  Num2_ = "-11";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "-121"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "121"));

  Num3_ = "11111";
  *AddNum_ = "99999";
  Num1_ = "55555";
  Num2_ = "55555";
  Num3_ *= *AddNum_;
  Num1_ *= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1111088889"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "3086358025"));

  Num1_ = "12345";
  Num2_ = "98765";
  Num1_ *= Num2_;
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1219253925"));

  // division boundaries
  Num3_ = "1";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "1";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "ERROR: Divide By Zero"));
  assert(AtZero(*AddNum_));
  assert(!NotZero(*AddNum_));
  assert(NotZero(Num3_));
  assert(!AtZero(Num3_));
  Num3_.ClearError();
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));

  Num3_ = "111";
  *AddNum_ = "0";
  Num1_ = "0";
  Num2_ = "111";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "ERROR: Divide By Zero"));
  Num3_.ClearError();
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "0"));

  Num3_ = "99999";
  *AddNum_ = "11111";
  Num1_ = "88888";
  Num2_ = "22222";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "9"));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "4"));

  Num3_ = "55555";
  *AddNum_ = "55555";
  Num1_ = "98765";
  Num2_ = "12345";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strcmp(NewStr_.c_str(), "1"));
  NewStr_ = Num1_.ToString();
  assert(!strncmp(NewStr_.c_str(), "8.000405022", 11));

  Num3_ = "11111";
  *AddNum_ = "99999";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strncmp(NewStr_.c_str(), "0.1111111111111", 15));

  Num3_ = "12345";
  *AddNum_ = "98765";
  Num1_ = "999";
  Num2_ = "1";
  Num3_ /= *AddNum_;  
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strncmp(NewStr_.c_str(), "0.1249936718", 12));
  NewStr_ = Num1_.ToString();
  assert(!strcmp(NewStr_.c_str(), "999"));

  Num3_ = "1";
  *AddNum_ = "999";
  Num1_ = "1";
  Num2_ = "12345";
  Num3_ /= *AddNum_;
  Num1_ /= Num2_;
  NewStr_ = Num3_.ToString();
  assert(!strncmp(NewStr_.c_str(), "0.001001001", 11));
  NewStr_ = Num1_.ToString();
  assert(!strncmp(NewStr_.c_str(), "0.00008100445524", 16));

  // Testing Scientific Notation
  Num1_.SetSciNotation();
  NewStr_ = Num1_.ToString();
  cout <<"NewStr = " <<NewStr_ <<endl;

  Num1_ = "7.5465e-9";
  Num2_ = "4.5237e6";
  cout <<"Num1_ = " <<Num1_.ToString() <<endl;
  cout <<"Num2_ = " <<Num2_.ToString() <<endl;
  Num2_.SetSciNotation();
  cout <<"Num2_ = " <<Num2_.ToString() <<endl;
  Num3_ = Num1_;
  Num1_ /= Num2_;
  NewStr_ = Num1_.ToString();
  cout <<Num3_ <<" / " <<Num2_ <<" = " <<NewStr_ <<endl;

  Num1_ = Num3_;
  Num3_ = Num2_;
  Num2_ /= Num1_;
  NewStr_ = Num2_.ToString();
  cout <<Num3_ <<" / " <<Num1_ <<" = " <<NewStr_ <<endl;
  
  delete MultNum_;
  delete AddNum_;  

  cout <<"SUCCESS Testing LongNumber class" <<endl;
#endif

#if LONGNUM_DEBUG_ASSERT2a
  cout <<"/****************************************************************************/" <<endl;

  LongNumber::OmitConstants(TRUE);
  LongNumber IntPart_;

  LongNumber Num2a_("-4544668906.7355779812");
  LongNumber Rem_ = Num2a_.modf(IntPart_);

  cout <<"intpart  = " <<IntPart_ <<endl;
  cout <<"fracpart = " <<Rem_ <<endl;

  IntPart_ = LongInt(13);
  Num2a_ = ChrString("4544668906");
  Rem_ = Num2a_.fmod(IntPart_);
  cout <<"Rem = " <<Rem_ <<endl;

  LongNumber Num1a_(2, NumberSign::PLUS);
  LongNumber Answer_;
  Answer_ = LongNumber::IntRoot(Num1a_, 2, 3);
  cout <<"Root2 = " <<Answer_ <<endl;

  Num1a_.SetValue(121, NumberSign::PLUS);
  Answer_ = LongNumber::IntRoot(Num1a_, 2, 3);
  cout <<"Root121 = " <<Answer_ <<endl;

  Answer_ = LongNumber::IntRoot(Num1a_, 0, 3);
  if (Answer_.ErrorStatus())
    cout <<"Root0 = ERROR" <<endl;
  else
    cout <<"Root0 = " <<Answer_ <<endl;

  cout <<"SUCCESS Testing LongNumber class" <<endl;
#endif

// Testing rounding methods
#if LONGNUM_DEBUG_ASSERT2b
  system("clear");

  LongNumber l1 = ChrString("-9.9999");
  LongNumber l2 = ChrString("9.9999");
  LongNumber l3 = ChrString("-9.12345");
  LongNumber l4 = ChrString("9.12345");
  LongNumber Val;

  cout <<"l1: " <<l1 <<endl;
  Val = l1;  
  Val.Ceiling();
  cout <<"Ceiling: " <<Val <<endl;
  Val = l1;
  Val.Floor();
  cout <<"Floor: " <<Val <<endl;
  Val = l1;
  Val.Round();
  cout <<"Round: " <<Val <<endl;
  Val = l1;
  Val.Round(-3);
  cout <<"Round(-3): " <<Val <<endl;

  cout <<"l2: " <<l2 <<endl;
  Val = l2;  
  Val.Ceiling();
  cout <<"Ceiling: " <<Val <<endl;
  Val = l2;  
  Val.Floor();
  cout <<"Floor: " <<Val <<endl;
  Val = l2;
  Val.Round();
  cout <<"Round: " <<Val <<endl;
  Val = l2;
  Val.Round(-3);
  cout <<"Round(-3): " <<Val <<endl;

  cout <<"l3: " <<l3 <<endl;
  Val = l3;  
  Val.Ceiling();
  cout <<"Ceiling: " <<Val <<endl;
  Val = l3;
  Val.Floor();
  cout <<"Floor: " <<Val <<endl;
  Val = l3;
  Val.Round();
  cout <<"Round: " <<Val <<endl;
  Val = l3;
  Val.Round(-3);
  cout <<"Round(-3): " <<Val <<endl;
  Val = l3;
  Val.Round(-4);
  cout <<"Round(-4): " <<Val <<endl;
  
  cout <<"l4: " <<l4 <<endl;
  Val = l4;  
  Val.Ceiling();
  cout <<"Ceiling: " <<Val <<endl;
  Val = l4;  
  Val.Floor();
  cout <<"Floor: " <<Val <<endl;
  Val = l4;
  Val.Round();
  cout <<"Round: " <<Val <<endl;
  Val = l4;
  Val.Round(-3);
  cout <<"Round(-3): " <<Val <<endl;
  Val = l4;
  Val.Round(-4);
  cout <<"Round(-4): " <<Val <<endl;
  
#endif

#if LONGNUM_DEBUG_ASSERT3
  cout <<"/****************************************************************************/" <<endl;

//  LongNumber::OmitConstants(FALSE);
  cout <<"ROOT2 = " <<LongNumber::vROOT2() <<endl;
  cout <<"ROOT3 = " <<LongNumber::vROOT3() <<endl;
  cout <<"PI = " <<LongNumber::vPI() <<endl;
  cout <<"E = " <<LongNumber::vE() <<endl;
  
  LongNumber Sin45_(LongNumber::vPI() / 4L);
  LongNumber Result1_ = Sin45_.sin();
  cout <<"Sin(45) = " <<Result1_ <<endl;
  LongNumber Cos30_(LongNumber::vPI() / 6L);
  LongNumber Result2_ = Cos30_.cos();
  cout <<"Cos(30) = " <<Result2_ <<endl;
  LongNumber Tan30_(LongNumber::vPI() / 6L);
  LongNumber Result3_ = Tan30_.tan();
  cout <<"Tan(30) = " <<Result3_ <<endl;

  Result1_ = Result1_.arcsin();
  Result2_ = Result2_.arccos();
  Result3_ = Result3_.arctan();
  Result1_ /= LongNumber::vPI();
  Result2_ /= LongNumber::vPI();
  Result3_ /= LongNumber::vPI();
  Result1_ *= 180L;
  Result2_ *= 180L;
  Result3_ *= 180L;

  cout <<"ArcSin(Sin(45)) = " <<Result1_ <<endl;
  cout <<"ArcCos(Cos(30)) = " <<Result2_ <<endl;
  cout <<"ArcTan(Tan(30)) = " <<Result3_ <<endl;
#endif

#if LONGNUM_DEBUG_ASSERT4
  long texp, v;
  LongNumber retv(1UL, NumberSign::MINUS);

  retv = LongNumber::IntRoot(retv, 2);
  assert(retv.NoComplexRootError());
  LongNumber Obj_(retv);  
  retv.ClearError();
  assert(retv.NoError());
  assert(Obj_.NoComplexRootError());
  Obj_.ClearError();
  assert(Obj_.NoError());

  system("clear");
  for (v = -8192; v <= -2; v /= 2)
  {
    Obj_ = LongInt(v);
    retv = Obj_.frexp(&texp);
    cout <<"x = " <<v <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  }

  Obj_ = LongInt(-1);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = Double(-0.5);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = Double(-0.2);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = LongNumber();
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = Double(0.2);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = Double(0.5);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  Obj_ = LongInt(1);
  retv = Obj_.frexp(&texp);
  cout <<"x = " <<Obj_ <<", exp = " <<texp <<", frexp() = " <<retv <<endl;

  for (v = 2; v <= 8192; v *= 2)
  {
    Obj_ = LongInt(v);  
    retv = Obj_.frexp(&texp);
    cout <<"x = " <<v <<", exp = " <<texp <<", frexp() = " <<retv <<endl;
  }
#endif

#if LONGNUM_DEBUG_ASSERT5
#if LONGNUM_DEBUG_ASSERT5c
  float dval = 3.14 / 6;
  Float testval;
  LongNumber v1, v2;
  int z;

  for (z = 1; z <= 12; z++)
  {
    testval = dval * z;
    v1 = testval;
    
    v2 = v1.sin();
    cout <<"sin(" <<testval <<") = " <<v2 <<endl;
    cout <<"arcsin(" <<v2 <<") = " <<v2.arcsin() <<endl;
    
    v2 = v1.cos();    
    cout <<"cos(" <<testval <<") = " <<v2 <<endl;
    cout <<"arccos(" <<v2 <<") = " <<v2.arccos() <<endl;

    if (z != 9 && z != 6)
    {
      v2 = v1.tan();
      cout <<"tan(" <<testval <<") = " <<v2 <<endl;
      cout <<"arctan(" <<v2 <<") = " <<v2.arctan() <<endl;
    }
  }
#endif

#if LONGNUM_DEBUG_ASSERT5d
  LongNumber posv1(5, NumberSign::PLUS);
  LongNumber posv2(10, NumberSign::PLUS);
  LongNumber negv1(5, NumberSign::MINUS);
  LongNumber negv2(10, NumberSign::MINUS);
  LongNumber Result;

  Result = (posv1 - posv2);
  cout <<posv1 <<" - " <<posv2 <<" = " <<Result <<endl;
  Result = (posv1 - negv2);
  cout <<posv1 <<" - " <<negv2 <<" = " <<Result <<endl;
  Result = (posv1 - negv1);
  cout <<posv1 <<" - " <<negv1 <<" = " <<Result <<endl;

  Result = (posv1 + posv2);
  cout <<posv1 <<" + " <<posv2 <<" = " <<Result <<endl;
  Result = (posv1 + negv2);
  cout <<posv1 <<" + " <<negv2 <<" = " <<Result <<endl;
  Result = (posv1 + negv1);
  cout <<posv1 <<" + " <<negv1 <<" = " <<Result <<endl;
  
  Result = (posv2 - posv1);
  cout <<posv2 <<" - " <<posv1 <<" = " <<Result <<endl;
  Result = (posv2 - negv2);
  cout <<posv2 <<" - " <<negv2 <<" = " <<Result <<endl;
  Result = (posv2 - negv1);
  cout <<posv2 <<" - " <<negv1 <<" = " <<Result <<endl;

  Result = (posv2 + posv1);
  cout <<posv2 <<" + " <<posv1 <<" = " <<Result <<endl;
  Result = (posv2 + negv2);
  cout <<posv2 <<" + " <<negv2 <<" = " <<Result <<endl;
  Result = (posv2 + negv1);
  cout <<posv2 <<" + " <<negv1 <<" = " <<Result <<endl;

  Result = (negv1 - negv2);
  cout <<negv1 <<" - " <<negv2 <<" = " <<Result <<endl;
  Result = (negv1 - posv2);
  cout <<negv1 <<" - " <<posv2 <<" = " <<Result <<endl;
  Result = (negv1 - posv1);
  cout <<negv1 <<" - " <<posv1 <<" = " <<Result <<endl;

  Result = (negv1 + negv2);
  cout <<negv1 <<" + " <<negv2 <<" = " <<Result <<endl;
  Result = (negv1 + posv2);
  cout <<negv1 <<" + " <<posv2 <<" = " <<Result <<endl;
  Result = (negv1 + posv1);
  cout <<negv1 <<" + " <<posv1 <<" = " <<Result <<endl;
  
  Result = (negv2 - negv1);
  cout <<negv2 <<" - " <<negv1 <<" = " <<Result <<endl;
  Result = (negv2 - posv2);
  cout <<negv2 <<" - " <<posv2 <<" = " <<Result <<endl;
  Result = (negv2 - posv1);
  cout <<negv2 <<" - " <<posv1 <<" = " <<Result <<endl;

  Result = (negv2 + negv1);
  cout <<negv2 <<" + " <<negv1 <<" = " <<Result <<endl;
  Result = (negv2 + posv2);
  cout <<negv2 <<" + " <<posv2 <<" = " <<Result <<endl;
  Result = (negv2 + posv1);
  cout <<negv2 <<" + " <<posv1 <<" = " <<Result <<endl;


  Result = (posv1 / posv2);
  cout <<posv1 <<" / " <<posv2 <<" = " <<Result <<endl;
  Result = (posv1 / negv2);
  cout <<posv1 <<" / " <<negv2 <<" = " <<Result <<endl;
  Result = (posv1 / negv1);
  cout <<posv1 <<" / " <<negv1 <<" = " <<Result <<endl;

  Result = (posv1 * posv2);
  cout <<posv1 <<" * " <<posv2 <<" = " <<Result <<endl;
  Result = (posv1 * negv2);
  cout <<posv1 <<" * " <<negv2 <<" = " <<Result <<endl;
  Result = (posv1 * negv1);
  cout <<posv1 <<" * " <<negv1 <<" = " <<Result <<endl;
  
  Result = (posv2 / posv1);
  cout <<posv2 <<" / " <<posv1 <<" = " <<Result <<endl;
  Result = (posv2 / negv2);
  cout <<posv2 <<" / " <<negv2 <<" = " <<Result <<endl;
  Result = (posv2 / negv1);
  cout <<posv2 <<" / " <<negv1 <<" = " <<Result <<endl;

  Result = (posv2 * posv1);
  cout <<posv2 <<" * " <<posv1 <<" = " <<Result <<endl;
  Result = (posv2 * negv2);
  cout <<posv2 <<" * " <<negv2 <<" = " <<Result <<endl;
  Result = (posv2 * negv1);
  cout <<posv2 <<" * " <<negv1 <<" = " <<Result <<endl;

  Result = (negv1 / negv2);
  cout <<negv1 <<" / " <<negv2 <<" = " <<Result <<endl;
  Result = (negv1 / posv2);
  cout <<negv1 <<" / " <<posv2 <<" = " <<Result <<endl;
  Result = (negv1 / posv1);
  cout <<negv1 <<" / " <<posv1 <<" = " <<Result <<endl;

  Result = (negv1 * negv2);
  cout <<negv1 <<" * " <<negv2 <<" = " <<Result <<endl;
  Result = (negv1 * posv2);
  cout <<negv1 <<" * " <<posv2 <<" = " <<Result <<endl;
  Result = (negv1 * posv1);
  cout <<negv1 <<" * " <<posv1 <<" = " <<Result <<endl;
  
  Result = (negv2 / negv1);
  cout <<negv2 <<" / " <<negv1 <<" = " <<Result <<endl;
  Result = (negv2 / posv2);
  cout <<negv2 <<" / " <<posv2 <<" = " <<Result <<endl;
  Result = (negv2 / posv1);
  cout <<negv2 <<" / " <<posv1 <<" = " <<Result <<endl;

  Result = (negv2 * negv1);
  cout <<negv2 <<" * " <<negv1 <<" = " <<Result <<endl;
  Result = (negv2 * posv2);
  cout <<negv2 <<" * " <<posv2 <<" = " <<Result <<endl;
  Result = (negv2 * posv1);
  cout <<negv2 <<" * " <<posv1 <<" = " <<Result <<endl;  
#endif

#if LONGNUM_DEBUG_ASSERT5e
  float incr = 2.0 / 5;
  float dvale = incr;
  Float testvale;
  LongNumber v1e, v2e;
  int zy;

//  v1.UseSeriesApprox(LongNumber::E_SERIES | LongNumber::LN_SERIES);

  for (zy = 1; zy < 5; zy++)
  {
    testvale = dvale;
    v1e = testvale;
    
    v2e = v1e.log();
    cout <<"log(" <<testvale <<") = " <<v2e <<endl;
    cout <<"exp(" <<v2e <<") = ";
    v1e = v2e.exp();
    cout <<v1e <<endl;
    
    dvale += incr;
  }  
#endif

#if LONGNUM_DEBUG_ASSERT5f
  LongNumber posv1a(5, NumberSign::PLUS);
  LongNumber posv2a(10, NumberSign::PLUS);
  LongNumber negv1a(5, NumberSign::MINUS);
  LongNumber negv2a(10, NumberSign::MINUS);
  Boolean Resultf;

  Resultf = (posv1a < posv2a);
  cout <<posv1a <<" < " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a < negv2a);
  cout <<posv1a <<" < " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a < negv1a);
  cout <<posv1a <<" < " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (posv1a > posv2a);
  cout <<posv1a <<" > " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a > negv2a);
  cout <<posv1a <<" > " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a > negv1a);
  cout <<posv1a <<" > " <<negv1a <<" = " <<Resultf <<endl;
  
  Resultf = (posv2a < posv1a);
  cout <<posv2a <<" < " <<posv1a <<" = " <<Resultf <<endl;
  Resultf = (posv2a < negv2a);
  cout <<posv2a <<" < " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv2a < negv1a);
  cout <<posv2a <<" < " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (posv2a > posv1a);
  cout <<posv2a <<" > " <<posv1a <<" = " <<Resultf <<endl;
  Resultf = (posv2a > negv2a);
  cout <<posv2a <<" > " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv2a > negv1a);
  cout <<posv2a <<" > " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (negv1a < negv2a);
  cout <<negv1a <<" < " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a < posv2a);
  cout <<negv1a <<" < " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a < posv1a);
  cout <<negv1a <<" < " <<posv1a <<" = " <<Resultf <<endl;

  Resultf = (negv1a > negv2a);
  cout <<negv1a <<" > " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a > posv2a);
  cout <<negv1a <<" > " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a > posv1a);
  cout <<negv1a <<" > " <<posv1a <<" = " <<Resultf <<endl;
  
  Resultf = (negv2a < negv1a);
  cout <<negv2a <<" < " <<negv1a <<" = " <<Resultf <<endl;
  Resultf = (negv2a < posv2a);
  cout <<negv2a <<" < " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv2a < posv1a);
  cout <<negv2a <<" < " <<posv1a <<" = " <<Resultf <<endl;

  Resultf = (negv2a > negv1a);
  cout <<negv2a <<" > " <<negv1a <<" = " <<Resultf <<endl;
  Resultf = (negv2a > posv2a);
  cout <<negv2a <<" > " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv2a > posv1a);
  cout <<negv2a <<" > " <<posv1a <<" = " <<Resultf <<endl;

  negv1a.AbsoluteValue();
  negv2a.AbsoluteValue();

  Resultf = (posv1a == posv2a);
  cout <<posv1a <<" == " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a == negv2a);
  cout <<posv1a <<" == " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a == negv1a);
  cout <<posv1a <<" == " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (posv1a != posv2a);
  cout <<posv1a <<" != " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a != negv2a);
  cout <<posv1a <<" != " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv1a != negv1a);
  cout <<posv1a <<" != " <<negv1a <<" = " <<Resultf <<endl;
  
  Resultf = (posv2a == posv1a);
  cout <<posv2a <<" == " <<posv1a <<" = " <<Resultf <<endl;
  Resultf = (posv2a == negv2a);
  cout <<posv2a <<" == " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv2a == negv1a);
  cout <<posv2a <<" == " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (posv2a != posv1a);
  cout <<posv2a <<" != " <<posv1a <<" = " <<Resultf <<endl;
  Resultf = (posv2a != negv2a);
  cout <<posv2a <<" != " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (posv2a != negv1a);
  cout <<posv2a <<" != " <<negv1a <<" = " <<Resultf <<endl;

  Resultf = (negv1a == negv2a);
  cout <<negv1a <<" == " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a == posv2a);
  cout <<negv1a <<" == " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a == posv1a);
  cout <<negv1a <<" == " <<posv1a <<" = " <<Resultf <<endl;

  Resultf = (negv1a != negv2a);
  cout <<negv1a <<" != " <<negv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a != posv2a);
  cout <<negv1a <<" != " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv1a != posv1a);
  cout <<negv1a <<" != " <<posv1a <<" = " <<Resultf <<endl;
  
  Resultf = (negv2a == negv1a);
  cout <<negv2a <<" == " <<negv1a <<" = " <<Resultf <<endl;
  Resultf = (negv2a == posv2a);
  cout <<negv2a <<" == " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv2a == posv1a);
  cout <<negv2a <<" == " <<posv1a <<" = " <<Resultf <<endl;

  Resultf = (negv2a != negv1a);
  cout <<negv2a <<" != " <<negv1a <<" = " <<Resultf <<endl;
  Resultf = (negv2a != posv2a);
  cout <<negv2a <<" != " <<posv2a <<" = " <<Resultf <<endl;
  Resultf = (negv2a != posv1a);
  cout <<negv2a <<" != " <<posv1a <<" = " <<Resultf <<endl;
#endif
#endif

#if LONGNUM_DEBUG_ASSERT6
  Double dNanVal_ = Double::vNAN;
  Double dInfVal_ = Double::vINFINITY;

  LongNumber InfVal1_ = dInfVal_;
  LongNumber NanVal1_ = dNanVal_;
  LongNumber NormVal_ = 12;
  LongNumber InfVal2_ = NormVal_;
  LongNumber NanVal2_;

  assert(InfVal1_.IsInfinite() && !InfVal1_.IsNan());
  assert(!NanVal1_.IsInfinite() && NanVal1_.IsNan());
  assert(!NormVal_.IsInfinite() && !NormVal_.IsNan());
  assert(!InfVal2_.IsInfinite() && !InfVal2_.IsNan());
  assert(!NanVal2_.IsInfinite() && !NanVal2_.IsNan());

  InfVal2_ = InfVal1_;
  NanVal2_ = NanVal1_;

  assert(InfVal2_.IsInfinite() && !InfVal2_.IsNan());
  assert(!NanVal2_.IsInfinite() && NanVal2_.IsNan());

  InfVal2_ = NormVal_;
  NanVal2_ = NormVal_;

  assert(!InfVal2_.IsInfinite() && !InfVal2_.IsNan());
  assert(!NanVal2_.IsInfinite() && !NanVal2_.IsNan());

  InfVal2_ = dInfVal_;
  NanVal2_ = dNanVal_;

  assert(InfVal2_.IsInfinite() && !InfVal2_.IsNan());
  assert(!NanVal2_.IsInfinite() && NanVal2_.IsNan());

  NormVal_ = InfVal2_;
  assert(NormVal_.IsInfinite() && !NormVal_.IsNan());
  NormVal_ = NanVal2_;
  assert(!NormVal_.IsInfinite() && NormVal_.IsNan());

  NormVal_ = LongNumber::vINFINITY();
  assert(NormVal_.IsInfinite() && !NormVal_.IsNan());
  NormVal_ = LongNumber::vNAN();
  assert(!NormVal_.IsInfinite() && NormVal_.IsNan());
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




