#ifndef BITSTRING_H
#define BITSTRING_H
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef BITSX_H
  #include "bitsx.h"
#endif

#define BITSTRING_DEBUG		0
//#define OVERLOAD_NEW 0

/****************************************************************************/
class BitStringAcceptor : public ObjectAcceptor
{
  friend class BitString;

  protected:
    union
    {
      const BitString* _ObjPtr;
      BitString* _VarPtr;
    };

    BitStringAcceptor(const BitString* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // BitString class specific acceptor methods
    virtual Boolean IsBitString() const;
    virtual void* CreateFromBitString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromBitString(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToBitString(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class BitString : public ObjectImp
{
  friend class BitStringAcceptor;

  friend Object& RootObject(BitString& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const BitString& Obj_)
	{ return *((const Object*)(&Obj_)); }

  Boolean IsEqualToBitString(const BitString* Str_) const;

  public:
    typedef unsigned long Type;
    enum { _Nb = BITS_BYTE * sizeof(Type) };
    enum _Source { _Zeros = 0, _Ones = -1 };

    BitString():
	ObjectImp()
	{ Tidy(); }
    BitString(Ulong, size_t);
    BitString(const BitString& X_, size_t P_ = 0, size_t N_ = SIZET_MAX):
	ObjectImp((Object&)X_)
	{ Tidy(), AssignSubstr(X_, P_, N_); }
    BitString(const ChrString&, size_t = 0, size_t = SIZET_MAX);
    BitString(_Source S_):
	ObjectImp(),
	_Ptr(0), _Src(S_), _Len(0), _Res(0) {}
    ~BitString()
	{ Tidy(1); }

    // Virtual constructor and assignment methods
    static BitString* Make();
    static BitString* Make(Ulong Val_, size_t n);
    static BitString* Make(const ChrString& Str_, size_t Pos_, size_t Len_);
    static BitString* Make(const BitString& Str_, size_t Pos_, size_t Len_);
    static BitString* Make(_Source S_);
    static BitString* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;        

    // Static null object accessor methods
    static BitString& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::BITS_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // Assignment method
    virtual BitString& operator = (const BitString& Rhs_);
    virtual BitString& operator = (const ChrString& Str_);
    virtual BitString& operator = (_Source S_);

    BitString& operator += (const BitString& Rhs_)
	{ return Append(Rhs_); }
    BitString& operator &= (const BitString& Rhs_);
    BitString& operator |= (const BitString& Rhs_);
    BitString& operator ^= (const BitString& Rhs_);
    BitString& operator <<= (size_t Rhs_);
    BitString& operator >>= (size_t Rhs_);

    BitString& Append(const BitString&, size_t = 0, size_t = SIZET_MAX);
    BitString& AssignSubstr(const BitString&, size_t = 0, size_t = SIZET_MAX);
    BitString& Insert(size_t, const BitString&, size_t = 0, size_t = SIZET_MAX);
    BitString& Remove(size_t = 0, size_t = SIZET_MAX);
    BitString& Replace(size_t, size_t, const BitString&, size_t = 0, size_t = SIZET_MAX);

    BitString& Set();
    BitString& Set(size_t, Boolean = TRUE);
    BitString& Reset();
    BitString& Reset(size_t Pos_)
	{ return Set(Pos_, 0); }

    BitString& Toggle();
    BitString& Toggle(size_t);

    ChrString To_String() const;

    size_t Count() const;
    size_t Length() const
	{ return _Len; }

    size_t Resize(size_t, Boolean = FALSE);
    size_t Trim()
	{ Resize(Rfind(1) + 1);
	  return _Len; }

    size_t Find(Boolean, size_t = 0, size_t = SIZET_MAX) const;
    size_t Rfind(Boolean, size_t = 0, size_t = SIZET_MAX) const;

    BitString SubString(size_t Pos_, size_t N_ = SIZET_MAX) const
	{ return BitString(*this, Pos_, N_); }

    Boolean operator == (const BitString&) const;
    Boolean operator != (const BitString& Rhs_) const
	{ return !(*this == Rhs_); }

    Boolean Test(size_t) const;
    Boolean Any() const;
    Boolean None() const
	{ return !Any(); }

    BitString operator << (size_t Rhs_) const
	{ return (BitString(*this) <<= Rhs_); }
    BitString operator >> (size_t Rhs_) const
	{ return BitString(*this) >>= Rhs_; }
    BitString operator ~ () const
	{ return BitString(*this).Toggle(); }

    Type _W(int I_) const
	{ return _Ptr[I_]; }
    Type _X(int I_, int P_) const
	{ int L_ = (_Len == 0) ? -1:((_Len - 1) / _Nb);
	  return ((L_ < I_) ? _Src :
		  (P_ == 0) ? _Ptr[I_]:
		  (L_ == I_) ? _Ptr[I_] << P_:
		  ((_Ptr[I_] << P_) | (_Ptr[I_ + 1] >> (_Nb - P_)))); }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

  private:
    void Copylr(const BitString&, size_t, size_t, size_t);
    void Copyrl(const BitString&, size_t, size_t, size_t);

    Boolean Grow(size_t, Boolean = FALSE);
    void Setl(size_t L_)
	{ _Len = L_, L_ %= _Nb;
	  if (L_ != 0)
	    _Ptr[(_Len - 1) / _Nb] &= ~(~(Type)0 >> L_); }

    void Tidy(Boolean = 0);

    void Xinv() const
	{ THROW (FallibleBase::BitStringInvalidArgumentErr()); }
    void Xlen() const
	{ THROW (FallibleBase::BitStringLengthErr()); }
    void Xran() const
	{ THROW (FallibleBase::BitStringOutOfRangeErr()); }

    Type* _Ptr, _Src;
    size_t _Len, _Res;
};

inline BitString operator + (const BitString& Lhs_, const BitString& Rhs_)
	{ return (BitString(Lhs_) += Rhs_); }
inline BitString operator & (const BitString& Lhs_, const BitString& Rhs_)
	{ return (BitString(Lhs_) &= Rhs_); }
inline BitString operator | (const BitString& Lhs_, const BitString& Rhs_)
	{ return (BitString(Lhs_) |= Rhs_); }
inline BitString operator ^ (const BitString& Lhs_, const BitString& Rhs_)
	{ return (BitString(Lhs_) ^= Rhs_); }

static inline size_t Bytes(size_t Bits_)
	{ return (sizeof(BitString::Type) * (Bits_ / BitString::_Nb
		+ (Bits_ % BitString::_Nb != 0 ? 1:0))); }

/****************************************************************************/
#endif





