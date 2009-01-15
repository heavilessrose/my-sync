#ifndef CHAR_H
#define CHAR_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif

#define CHARACTER_DEBUG		0

/****************************************************************************/
class CharacterAcceptor : public ObjectAcceptor
{
  friend class Character;

  protected:
    union
    {
      const Character* _ObjPtr;
      Character* _VarPtr;
    };

    CharacterAcceptor(const Character* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Character class specific acceptor methods
    virtual Boolean IsCharacter() const;
    virtual Boolean IsTypeWrapper() const;

    virtual void* CreateFromCharacter(const ObjectAcceptor& Obj_);
    virtual void* AssignFromCharacter(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToCharacter(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToCharacter(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToCharacter(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
//typedef unsigned char CharType;
typedef char CharType;

class Character : public IntegerAsBitsType<Character, CharType>
{
  friend class CharacterAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, Character)
  TYPEWRAPPER_INTEGER_OPERATIONS(Character)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(Character)

  friend Character operator + (const Character& Val1_, const Character& Val2_);
  friend Character operator - (const Character& Val1_, const Character& Val2_);
  friend Character operator * (const Character& Val1_, const Character& Val2_);
  friend Character operator / (const Character& Val1_, const Character& Val2_);
  friend Character operator % (const Character& Val1_, const Character& Val2_);
  friend Character operator & (const Character& Val1_, const Character& Val2_);
  friend Character operator | (const Character& Val1_, const Character& Val2_);
  friend Character operator ^ (const Character& Val1_, const Character& Val2_);

  friend Character operator + (CharType Val1_, const Character& Val2_);
  friend Character operator - (CharType Val1_, const Character& Val2_);
  friend Character operator * (CharType Val1_, const Character& Val2_);
  friend Character operator / (CharType Val1_, const Character& Val2_);
  friend Character operator % (CharType Val1_, const Character& Val2_);
  friend Character operator & (CharType Val1_, const Character& Val2_);
  friend Character operator | (CharType Val1_, const Character& Val2_);
  friend Character operator ^ (CharType Val1_, const Character& Val2_);

  friend Character operator + (const Character& Val1_, CharType Val2_);
  friend Character operator - (const Character& Val1_, CharType Val2_);
  friend Character operator * (const Character& Val1_, CharType Val2_);
  friend Character operator / (const Character& Val1_, CharType Val2_);
  friend Character operator % (const Character& Val1_, CharType Val2_);
  friend Character operator & (const Character& Val1_, CharType Val2_);
  friend Character operator | (const Character& Val1_, CharType Val2_);
  friend Character operator ^ (const Character& Val1_, CharType Val2_);
  
  protected:
    CharType _Value;

    // Character class specific relational test methods
    virtual Boolean IsEqualToCharacter(const Character* Ptr_) const;
    virtual Boolean IsLesserToCharacter(const Character* Ptr_) const;
    virtual Boolean IsGreaterToCharacter(const Character* Ptr_) const;

  public:
    // Constructors and destructor
    Character(CharType IntVal_=0);
    Character(short IntVal_, int Radix_);
    Character(const ChrString& Str_);
    Character(const Character& CharObj_);
    virtual ~Character() {}

    // Virtual constructor and assignment methods
    static Character* Make(CharType IntVal_=0);
    static Character* Make(short IntVal_, int Radix_);
    static Character* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;        

    virtual Character& DerivedClass();
    virtual const Character& DerivedClass() const;

    // Typed Wrapper methods
    virtual void SetData(const CharType& Obj_);
    virtual const CharType& GetData() const;
    virtual operator CharType () const
	{ return _Value; }

    // Static null object accessor methods
    static Character& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Character Testing methods
    static Boolean IsUpperCase(CharType Ch_)
	{ return isupper(Ch_); }
    static Boolean IsLowerCase(CharType Ch_)
	{ return islower(Ch_); }
    static Boolean IsDigit(CharType Ch_)
	{ return isdigit(Ch_); }
    static Boolean IsOctDigit(CharType Ch_)
	{ return IsOctc(Ch_); }
    static Boolean IsHexDigit(CharType Ch_)
	{ return isxdigit(Ch_); }
    static Boolean IsBaseDigit(CharType Ch_, int Radix_)
	{ return IsBasec(Ch_, Radix_); }
    static Boolean IsAlphaNumeric(CharType Ch_)
	{ return isalnum(Ch_); }
    static Boolean IsAlpha(CharType Ch_)
	{ return isalpha(Ch_); }
    static Boolean IsSpace(CharType Ch_)
	{ return isspace(Ch_); }

    // Character conversion methods
    static Character ToUpperCase(CharType Ch_)
	{ return Character(toupper(Ch_)); }
    static Character ToLowerCase(CharType Ch_)
	{ return Character(tolower(Ch_)); }
    static short ToDigit(CharType Ch_,  int Radix_=10);
    static Character FromDigit(short Val_, int Radix_=10);
    static Character StaticFromString(const ChrString& Str_);
    static ChrString ToString(const CharType Ch_);
    static ChrString ToTruncString(const CharType Ch_);
    virtual ChrString ToString(int = 10) const;    
    virtual ChrString ToTruncString(int = 10) const;
    virtual Character& FromString(const ChrString& Str_);

    // Numeric conversion methods
    double FloatValue() const
	{ return _Value; }
    short IntValue() const
	{ return _Value; }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static Character UpperBound()
	{ return Character(CHAR_MAX); }
    static Character LowerBound()
	{ return Character(CHAR_MIN); }
    static int StaticID()
	{ return TypeInfo::CHARACTER_TYPE; }
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
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // At zero/maximum/minimum value test functions
    virtual Boolean AtZero() const;    
    virtual Boolean AtMaxValue() const;
    virtual Boolean AtMinValue() const;

    // Assignment method
    virtual Character& operator = (const Character& Obj_);
    virtual Character& operator = (const CharType& IntVal_);
    virtual Character& operator = (const ChrString& Str_);

    // Unary operators
    virtual Boolean operator ! () const;
    Character operator - () const;
    Character operator + () const;
    Character operator ~ () const;
    Character operator << (size_t Shift_) const;
    Character operator >> (size_t Shift_) const;

    // Increment and decrement operators
    virtual const Character& operator ++ ();
    virtual const Character& operator ++ (int);
    virtual const Character& operator -- ();
    virtual const Character& operator -- (int);

    // Arithmetic assignment operators
    virtual Character& operator += (const NumOrdinalType<Character>& Obj_);
    virtual Character& operator -= (const NumOrdinalType<Character>& Obj_);
    virtual Character& operator *= (const NumOrdinalType<Character>& Obj_);
    virtual Character& operator /= (const NumOrdinalType<Character>& Obj_);
    virtual Character& operator %= (const NumOrdinalType<Character>& Obj_);

    virtual Character& operator += (const CharType& IntVal_);
    virtual Character& operator -= (const CharType& IntVal_);
    virtual Character& operator *= (const CharType& IntVal_);
    virtual Character& operator /= (const CharType& IntVal_);
    virtual Character& operator %= (const CharType& IntVal_);

    // Bitwise assignment operators
    virtual Character& operator |= (const NumOrdinalAsBitsType<Character>& Obj_);
    virtual Character& operator &= (const NumOrdinalAsBitsType<Character>& Obj_);
    virtual Character& operator ^= (const NumOrdinalAsBitsType<Character>& Obj_);
    virtual Character& operator |= (const CharType& IntVal_);
    virtual Character& operator &= (const CharType& IntVal_);
    virtual Character& operator ^= (const CharType& IntVal_);
    virtual Character& operator <<= (size_t Shift_);
    virtual Character& operator >>= (size_t Shift_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





