#ifndef BITS_H
#define BITS_H
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef BITSX_H
  #include "bitsx.h"
#endif

#define BITS_DEBUG	0

/****************************************************************************/
template <size_t N>
class Bits;

template <size_t N>
class BitsAcceptor : public ObjectAcceptor
{
  friend class Bits<N>;

  protected:
    union
    {
      const Bits<N>* _ObjPtr;
      Bits<N>* _VarPtr;
    };

    BitsAcceptor(const Bits<N>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Bits class specific acceptor methods
    virtual Boolean IsBits() const;
    virtual Boolean IsEqvBits(int Num_) const;
    virtual void* CreateFromBits(const ObjectAcceptor& Obj_);
    virtual void* AssignFromBits(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToBits(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
// template class bits
template <size_t N>
class Bits : public ObjectImp
{
  typedef Ulong Type;

  friend class BitsAcceptor<N>;

  friend Bits<N> operator & (const Bits<N>& Lhs_, const Bits<N>& Rhs_)
	{ return (Bits<N>(Lhs_) &= Rhs_); }
  friend Bits<N> operator | (const Bits<N>& Lhs_, const Bits<N>& Rhs_)
	{ return (Bits<N>(Lhs_) |= Rhs_); }
  friend Bits<N> operator ^ (const Bits<N>& Lhs_, const Bits<N>& Rhs_)
	{ return (Bits<N>(Lhs_) ^= Rhs_); }
  friend istream& operator >> (istream& Is_, Bits<N>& Rhs_)
	{ Rhs_ = _Bitsxstr(Is_, N);
	  return Is_; }
  friend ostream& operator << (ostream& Os_, const Bits<N>& Rhs_)
	{ return (Os_ <<Rhs_.To_String()); }

  friend Object& RootObject(Bits<N>& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const Bits<N>& Obj_)
	{ return *((const Object*)(&Obj_)); }

  Boolean IsEqualToBits(const Bits<N>* Ptr_) const;

  public:
    Bits();
    Bits(Ulong Val_);
    Bits(const Bits<N>& Obj_);
    Bits(const ChrString& Str_, size_t Pos_ = 0, size_t Len_ = SIZET_MAX);

    // Virtual constructor and assignment methods
    static Bits<N>* Make();
    static Bits<N>* Make(Ulong Val_);
    static Bits<N>* Make(const ChrString& Str_, size_t Pos_, size_t Len_);
    static Bits<N>* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Static null object accessor methods
    static Bits<N>& GiveNullObject();

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
    virtual Bits<N>& operator = (Ulong Val_);
    virtual Bits<N>& operator = (const ChrString& Str_);
    virtual Bits<N>& operator = (const Bits<N>& Obj_);

    // Bitwise operators
    Bits<N>& operator &= (const Bits<N>& Rhs_);
    Bits<N>& operator |= (const Bits<N>& Rhs_);
    Bits<N>& operator ^= (const Bits<N>& Rhs_);
    Bits<N>& operator <<= (size_t Pos_);
    Bits<N>& operator >>= (size_t Pos_);

    Bits<N>& Set();
    Bits<N>& Set(size_t Pos_, int Val_ = 1);

    Bits<N>& Reset();
    Bits<N>& Reset(size_t Pos_)
	{ return (Set(Pos_, 0)); }

    Bits<N>& operator ~ () const
	{ return (Bits<N>(*this).Toggle()); }

    Bits<N>& Toggle();
    Bits<N>& Toggle(size_t Pos_);

    Ushort To_Ushort() const;
    Ulong To_Ulong() const;
    ChrString To_String() const;

    size_t Count() const;
    size_t Length() const
	{ return N; }

    int Test(size_t Pos_) const;
    int Any() const;
    int None() const
	{ return !Any(); }

    Bits<N> operator << (size_t Pos_) const
	{ return (Bits<N>(*this) <<= Pos_); }
    Bits<N> operator >> (size_t Pos_) const
	{ return (Bits<N>(*this) >>= Pos_); }

    Type _W(size_t Index_) const
	{ return _Array[Index_]; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

  private:
    enum { Nb = BITS_BYTE * sizeof(Type),
	   Nw = (N == 0) ? 0:((N - 1) / Nb) };

    void Tidy(Type Val_ = 0)
    {
      for (int Index_ = Nw; 0 <= Index_; --Index_)
	_Array[Index_] = Val_;

      if (Val_ != 0)
	Trim();
    }

    void Trim();

    void Xinv() const
	{ THROW (FallibleBase::BitsInvalidArgumentErr()); }
    void Xoflo() const
	{ THROW (FallibleBase::BitsOverFlowErr()); }
    void Xran() const
	{ THROW (FallibleBase::BitsOutOfRangeErr()); }

    int GiveNw() const
	{ return Nw; }

    Type _Array[Nw + 1];
};

/****************************************************************************/
#endif





