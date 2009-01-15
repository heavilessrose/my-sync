#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
  #include "stdafx.h"
#endif
#ifndef TEMPSTREAM_H
  #include "tempstream.h"
#endif
#ifndef VARLENGTHFILE_H
  #include "varlengthfile.h"
#endif
#ifndef USHORTINT_H
  #include "ushortint.h"
#endif
#ifndef UNSIGNED_H
  #include "unsigned.h"
#endif
#ifndef ULONGINT_H
  #include "ulongint.h"
#endif
#ifndef CHARSET_H
  #include "charset.h"
#endif
#ifndef BTREE_CPP
  #include "btree.cpp"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef ARRAYBINTREE_CPP
  #include "arraybintree.cpp"
#endif
#ifndef TIMEOFDAY_H
  #include "timeofday.h"
#endif
#ifndef RANDDEV_H
  #include "randdev.h"  // uniform deviates
#endif
#ifndef RANDTABLE_H
  #include "randtable.h" // table of number created by RandGen
#endif
#ifndef POLYNOMIAL
  #include "polynomial.h"
#endif
#ifndef MATRIX_H
  #include "matrix.h"
#endif
#ifndef INTARRAY_H
  #include "intarray.h"
#endif
#ifndef STATARRAY_CPP
  #include "statarray.cpp"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif
#ifndef STRINGSTORE_H
  #include "strstore.h"
#endif
#ifndef STRIO_H
  #include "strio.h"
#endif

#ifndef STREAMREADER_H
  #include "streamreader.h"
#endif
#ifndef STRINGBROWSER_H
  #include "strbrowser.h"
#endif

#ifndef SORTS_H
  #include "sorts.h"
#endif
#ifndef SORTEDLIST_H
  #include "sortlist.h"
#endif
#ifndef SMARTPTR_H
  #include "smartptr.h"
#endif
#ifndef SLOTTEDFILE_H
  #include "slotfile.h"
#endif
#ifndef SIMPLELIST_H
  #include "simplelist.h"
#endif
#ifndef SIMPLEARRAY_H
  #include "simplearray.h"
#endif
#ifndef SHORTINT_H
  #include "shortint.h"
#endif
#ifndef SET_CPP
  #include "set.cpp"
#endif
#ifndef SERIESCOMPARE_H
  #include "seriescmp.h"
#endif
#ifndef SAFESET_CPP
  #include "safeset.cpp"
#endif
#ifndef RECT_CPP
  #include "rect.cpp"
#endif
#ifndef PRIMESTABLE_H
  #include "primetbl.h"
#endif
#ifndef POINT_CPP
  #include "point.cpp"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef LONGNUM_H
  #include "longnum.h"
#endif
#ifndef LONGINT_H
  #include "longint.h"
#endif
#ifndef LONESTRING_H
  #include "lonestr.h"
#endif
#ifndef LONGDOUBLE_H
  #include "ldouble.h"
#endif
#ifndef INTEGER_H
  #include "integer.h"
#endif
#ifndef QUICKINT_H
  #include "quickint.h"
#endif
#ifndef INIFILE_H
  #include "inifile.h"
#endif
#ifndef IMAGEPTR_CPP
  #include "imageptr.cpp"
#endif
#ifndef HASHTABLE_CPP
  #include "hashtable.cpp"
#endif
#ifndef FLOAT_H
  #include "float.h"
#endif
#ifndef ENIGMA_H
  #include "enigma.h"
#endif
#ifndef DISCRETE_CPP
  #include "discrete.cpp"
#endif
#ifndef DATASTORE_CPP
  #include "datastore.cpp"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef COMPLEX_CPP
  #include "complex.cpp"
#endif
#ifndef BYTESTR_H
  #include "bytestr.h"
#endif
#ifndef BITSTRING_H
  #include "bitstr.h"
#endif
#ifndef BITS_CPP
  #include "bits.cpp"
#endif
#ifndef AVLTREE_CPP
  #include "avltree.cpp"
#endif
#ifndef LONESTRING_H
  #include "lonestr.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef ADVFORLOOP_CPP
  #include "advforloop.cpp"
#endif
#ifndef FORLOOP_CPP
  #include "forloop.cpp"
#endif
#ifndef PTRARRAY_CPP
  #include "ptrarray.cpp"
#endif
#ifndef ARGARRAY_CPP
  #include "argarray.cpp"
#endif
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif
#ifndef REFERENCECOUNT_H
  #include "refcount.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif
#ifndef DOUBLE_H
  #include "double.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
#ifndef HASHABLE_CPP
  #include "hashable.cpp"
#endif
#ifndef FUZZYSET_CPP
  #include "fuzzyset.cpp"
#endif
#ifndef BITVECTOR_H
  #include "bitvect.h"
#endif
#ifndef BUILTINSERIESCOMPARE_H
  #include "bltincmp.h"
#endif
#ifndef BST_H
  #include "bst.h"
#endif
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef CHAR_H
  #include "char.h"
#endif
#ifndef COMPARE_H
  #include "compare.h"
#endif
#ifndef DYNARRAY_CPP
  #include "dynarray.cpp"
#endif
#ifndef FULLDATE_H
  #include "fulldate.h"
#endif
#ifndef PTRDYNARRAY_CPP
  #include "ptrdynarray.cpp"
#endif
#ifndef DOUBLELIST_CPP
  #include "dlnklist.cpp"
#endif
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif
#ifndef PRIORITYQUEUE_CPP
  #include "priqueue.cpp"
#endif
#ifndef STACK_H
  #include "stack.h"
#endif
#ifndef QUEUE_H
  #include "queue.h"
#endif
#ifndef DEQUE_H
  #include "deque.h"
#endif
#ifndef REFPTRS_CPP
  #include "refptrs.cpp"
#endif
#ifndef TRISTATE_H
  #include "tristate.h"
#endif
#ifndef TEXTGRAPHICS_H
  #include "txtgraph.h"
#endif

// Function prototypes
void TestBuiltIn();
void TestAdvForLp();
void TestArgArray();
void TestPtrArray();
void TestAssoc();
void TestBstTree();
void TestAvlTree();
void TestBaseptr();
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
void TestBits();
#endif
void TestBitstr();
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
void TestBitvect();
#endif
void TestBltinCmp();
void TestByteStr();
void TestChar();
void TestCompare();
void TestComplex();
void TestDatastor();
void TestDeque();
void TestDlnklist();
void TestDouble();
void TestDynarray();
void TestEnigma();
void TestFloat();
void TestForLoop();
void TestFuzzyset();
void TestHashtab();
void TestImagePtr();
void TestIniFile();
void TestInteger();
void TestLdouble();
void TestLonestr();
void TestLongint();
void TestLongnum();
void TestObjimp();
void TestPoint();
void TestPrimeTbl();
void TestPriqueue();
void TestQueue();
void TestQuickint();
void TestRational();
void TestRect();
void TestRefptrs();
void TestSafeSet();
void TestSercmp();
void TestSet();
void TestShortint();
void TestSimpArray();
void TestSimpList();
void TestSlnklist();
void TestSlotFile();
void TestSmartPtr();
void TestSortList();
void TestSorts();
void TestStack();
void TestStrBrows();
void TestStreamReader();
void TestString();
void TestStrio();
void TestStrStore();
void TestStrutil();
void TestDate();
void TestPtrDynarray();
void TestTempStream();
void TestArray();
void TestMatrix();
void TestPoly();
void TestRand();
void TestTimeOfDay();
void TestArrayBinTree();
void TestBtree();
void TestCharSet();
void TestULongInt();
void TestUnsigned();
void TestUShortInt();
void TestVlenFile();
void TestDiscrete();
void TestTristate();
void TestTxtgraph();

// function prototypes
//void ArrayTest1(ostream & buffer);
//void ArrayTest2(ostream & buffer);
void ArrayTestA(ostream & buffer);
void ArrayTest3(ostream & buffer);
void ArrayTest4(ostream & buffer);
void ArrayTest5(ostream & buffer);
void ArrayTest6(ostream & buffer);
void ArrayTest7(ostream & buffer);
//void ArrayTest8(ostream & buffer);
void ArrayTest9(ostream & buffer);
void ArrayTest10(ostream & buffer);
void ArrayTest11(ostream & buffer);
//void ArrayTest12(ostream & buffer);
void ArrayTest13(ostream & buffer);

void checkDate(int, int, int);
const char* DayText[] =
{
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday"
};

void ShowArray(int* arr, int size_)
{
  int i;
  for (i = 0; i < size_; i++)
    cout <<arr[i] <<",";
  cout <<endl;
}

struct SomeDumStruct
{
  DEFAULT_ROOTOBJECT_DEFN(SomeDumStruct)
};

class Sample
{
  DEFAULT_EXTERNALCLASS_DEFN(Sample)

  friend ostream& operator << (ostream& os, const Sample& obj)
    { os <<"x=" <<obj.Getx() <<", y=" <<obj.Gety(); return os; }

  private:
    char* _own;
    int _x;
    int _y;

  public:
	Sample():
      _own(new_char_string("(NONE)")),
      _x(0), _y(0) {}	  
    Sample(int x, int y):
      _own(new_char_string("(NONE)")),
      _x(x), _y(y) {}
    Sample(const Sample& Obj_):
      _own(new_char_string(Obj_._own)),
      _x(Obj_._x), _y(Obj_._y) {}
    ~Sample()
    { cout <<"Sample class destructor called, owned by: " <<_own <<endl;
      ::Delete(_own); }
      
    int Getx() const { return _x; }
    int Gety() const { return _y; }
    void Setxy(int x, int y) { _x = x; _y = y; }
    void SetOwner(const char* Str_)
    { ::Delete(_own);
      _own = new_char_string(Str_); }

    Sample& operator = (const Sample& Obj_)
    { if (this != &Obj_) {
        _x = Obj_._x;
        _y = Obj_._y;
        ::Delete(_own);
        _own = new_char_string(Obj_._own);
      }
      return *this; }

	MEMORYOPS_DECL(Sample)
};

template <class T>
class SimpleContainer
{
  DEFAULT_EXTERNALCLASS_DEFN(SimpleContainer)

  private:
    char* _name;
    DataStorePtr<T> _dsptr;

    static char* GetCount();

  public:
    SimpleContainer();
    SimpleContainer(T* Data_, int Flags_);
    SimpleContainer(const SimpleContainer<T>& cont_);
    ~SimpleContainer();

    SimpleContainer<T>& operator = (const SimpleContainer<T>& cont_);

    void SetData(T* Data_, int Flags_);
    DataStorePtr<T>& GetData();
    const DataStorePtr<T>& GetData() const;

	MEMORYOPS_DECL(SampleContainer)
};

MEMORYOPS_DEFN(Sample)
MEMORYOPS_TEMPLATE_DEFN(SimpleContainer)

template <class T>
SimpleContainer<T>::SimpleContainer():
_name(SimpleContainer<T>::GetCount())
{}

template <class T>
SimpleContainer<T>::SimpleContainer(T* Data_, int Flags_):
_name(SimpleContainer<T>::GetCount()),
_dsptr(Data_, Flags_)
{
  Data_->SetOwner(_name);
  _dsptr.Pointee()->SetOwner(_name);
}

template <class T>
SimpleContainer<T>::SimpleContainer(const SimpleContainer<T>& cont_):
_name(SimpleContainer<T>::GetCount()),
_dsptr(cont_._dsptr)
{
  _dsptr.Pointee()->SetOwner(_name);
}

template <class T>
SimpleContainer<T>::~SimpleContainer()
{
  ::Delete(_name);
  _name = NULL;
}

template <class T>
char* SimpleContainer<T>::GetCount()
{
  static size_t InstCnt_ = 0;
  ++InstCnt_;  
  char* Buf_ = UintToStr(InstCnt_, NULL);
  size_t Len_ = strlen(Buf_);
  memmove(&Buf_[4], Buf_, Len_ + 1);
  memmove(Buf_, "cont", 4);
  return Buf_;
}

template <class T>
SimpleContainer<T>& SimpleContainer<T>::operator = (const SimpleContainer<T>& cont_)
{
  if (this != &cont_)
  {
    _dsptr = cont_._dsptr;
    _dsptr.Pointee()->SetOwner(_name);    
  }

  return *this;
}

template <class T>
void SimpleContainer<T>::SetData(T* Data_, int Flags_)
{
  _dsptr.AssignPtr(Data_, Flags_);
  Data_->SetOwner(_name);
  _dsptr.Pointee()->SetOwner(_name);  
}
    
template <class T>
DataStorePtr<T>& SimpleContainer<T>::GetData()
{
  return _dsptr;
}

template <class T>
const DataStorePtr<T>& SimpleContainer<T>::GetData() const
{
  return _dsptr;
}

int operator == (FuzzySetTypeDefn<char>::NodeStore& x, const char* s)
{
  FuzzySetTypeDefn<char>::NodeIter Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->_Value != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

struct Dumstruct
{ DEFAULT_ROOTOBJECT_DEFN(Dumstruct) };

class Somestruct
{
  DEFAULT_ROOTOBJECT_DEFN(Somestruct)

  friend Boolean _IsMatchable(const Somestruct& Obj_)
      { return TRUE; }
  friend Boolean _IsEqualTo(const Somestruct& Src_, const Somestruct& Trg_)
      { return (Src_.xval == Trg_.xval); }
  
  public:
    int xval;
    void Set(int x_) { xval = x_; }
};

class SimpleStr
{
  DEFAULT_EXTERNALCLASS_DEFN(SimpleStr)

  private:
    char* _str;

  public:
    SimpleStr():
      _str(NULL) {}
    SimpleStr(const char* str_):
      _str(new_char_string(str_)) {}
    ~SimpleStr()
      { DeleteArray(_str); }

    SimpleStr& operator = (const char* str_)
      { DeleteArray(_str);
        _str = new_char_string(str_);
        return *this; }

    char* Str()
      { return _str; }
      
    MEMORYOPS_DEFN(SimpleStr)
};

class EnclosedClass
{
  DEFAULT_EXTERNALCLASS_DEFN(EnclosedClass)

  private:
    long x;
    char* _own;
    static StaticObjPtr<EnclosedClass> _StaObj;

    EnclosedClass* Make()
	{ return (new EnclosedClass(999)); }

  public:
    EnclosedClass():
    x(0), _own(new_char_string("NONE"))
	{ if (_StaObj.IsNull())
	    _StaObj = EnclosedClass::Make();
	  else
	    _StaObj.Increment(); }
    EnclosedClass(long n):
    x(n), _own(new_char_string("NONE")) {}
    EnclosedClass(const EnclosedClass& Obj_):
    x(Obj_.x), _own(new_char_string(Obj_._own)) {}
    ~EnclosedClass()
    { cout <<"EnclosedClass class destructor called, owned by: " <<_own <<endl;
      ::Delete(_own); }
            
    void SetOwner(const char* Owner_)
        { ::Delete(_own);
          _own = new_char_string(Owner_); }
    void SetX(long newx)
	{ x = newx; }
    long showX()
	{ return x; }
    EnclosedClass* Clone() const
	{ return new EnclosedClass(*this); }

    MEMORYOPS_DEFN(EnclosedClass)
};

class MasterClass
{
  DEFAULT_EXTERNALCLASS_DEFN(MasterClass)

  public:
    long dupv;
    EnclosedClass what;
    MasterClass* _Next;
    UpdatePtr<EnclosedClass> _Uptr;

    MasterClass() : _Next(NULL)
	{ what.SetX(10); dupv = 10; }
    ~MasterClass()
	{ delete _Next; _Next = NULL; }

    void MakeAnother()
	{ _Next = new MasterClass; }

    Boolean Update(const EnclosedClass& w, long IdCode_);
    const Boolean& ConstUpdateReturnRef(const EnclosedClass& w, long IdCode_) const;
    const Boolean* ConstUpdateReturnPtr(const EnclosedClass& w, long IdCode_) const;
    Boolean& UpdateReturnRef(const EnclosedClass& w, long IdCode_);
    Boolean* UpdateReturnPtr(const EnclosedClass& w, long IdCode_);    
    
    TypedObjectUpdater<MasterClass, EnclosedClass>* Updater()
	{ SPMARKER("start/end: MasterClass::Updater()")
          return TypedObjectUpdater<MasterClass, EnclosedClass>::Make(this, &MasterClass::Update, 0); }

    UpdatePtr<EnclosedClass> ReturnAndUpdate()
	{ SPMARKER("start: MasterClass::ReturnAndUpdate()")
        
          CountedObjPtr<EnclosedClass> ObjPtr_(&what);
	  UpdatePtr<EnclosedClass> Ptr_;
	  if (_Next)
            Ptr_.SetUpdatablePtr(ObjPtr_, Updater()->AddNext(_Next->Updater()), NULL);
	  else
	    Ptr_.SetUpdatablePtr(ObjPtr_, Updater(), NULL);
            
          SPMARKER("end: MasterClass::ReturnAndUpdate()")
	  return Ptr_; }

    long ShowX()
	{ return what.showX(); }
    long ShowDupv()
	{ return dupv; }

    MEMORYOPS_DEFN(MasterClass)
};

Boolean MasterClass::Update(const EnclosedClass& w, long IdCode_)
	{ EnclosedClass& nw = (EnclosedClass&)w;
          dupv = nw.showX();
	  if (&what != &nw)
	    what = nw;            
	  return TRUE; }

const Boolean& MasterClass::ConstUpdateReturnRef(const EnclosedClass& w, long IdCode_) const
	{ static Boolean Dummy_ = TRUE;
          MasterClass *const LocalThis_ = (MasterClass *const)this;
          EnclosedClass& nw = (EnclosedClass&)w;
          LocalThis_->dupv = nw.showX();
	  if (&what != &nw)
	    LocalThis_->what = nw;
	  return Dummy_; }
const Boolean* MasterClass::ConstUpdateReturnPtr(const EnclosedClass& w, long IdCode_) const
	{ static Boolean Dummy_ = TRUE;
          ConstUpdateReturnRef(w, IdCode_);
	  return &Dummy_; }

Boolean& MasterClass::UpdateReturnRef(const EnclosedClass& w, long IdCode_)
	{ static Boolean Dummy_ = TRUE;
          MasterClass *const LocalThis_ = (MasterClass *const)this;
          EnclosedClass& nw = (EnclosedClass&)w;
          LocalThis_->dupv = nw.showX();
	  if (&what != &nw)
	    LocalThis_->what = nw;
	  return Dummy_; }
Boolean* MasterClass::UpdateReturnPtr(const EnclosedClass& w, long IdCode_)
	{ static Boolean Dummy_ = TRUE;
          UpdateReturnRef(w, IdCode_);
	  return &Dummy_; }                    

inline EnclosedClass* Clone(const EnclosedClass& Obj_)
	{ return Obj_.Clone(); }
inline int* Clone(int Val_)
	{ return (new int(Val_)); }

class REFPTRSTestClass
{
  DEFAULT_ROOTOBJECT_DEFN(REFPTRSTestClass)

  public:
    int* member;
    static StaticPtr<int> _StatMem;

    REFPTRSTestClass(){}
    REFPTRSTestClass(int m):
	member(new int(m))
	{ _StatMem.Create(m); }
    REFPTRSTestClass(const REFPTRSTestClass& obj):
	member(new int(*obj.member))
	{ _StatMem.Create(*obj.member); }
    ~REFPTRSTestClass()
	{ delete member;
	  _StatMem.Destroy(); }

    REFPTRSTestClass& operator = (REFPTRSTestClass& obj)
	{ if (this != &obj){
	    delete member;
	    member = new int(*obj.member);
	  }
	  *_StatMem = *obj.member;
	  return *this; }

    void SetMember(int x)
	{ delete member;
	  member = new int(x); }

    int GetMember()
	{ return *member; }

    StaticPtr<int>& GetPointer()
	{ return _StatMem; }
};

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

void ShowResultCodeString(Parser* p, char* Text_, ParseResultInfo* Results_, QuickParseStatus* Status_)
{
  int i;
  char CodeStr_[4096];
  
  CodeStr_[0] = 0;
  cout <<"################################################################################################################" <<endl;
  cout <<"Parsing Line: " <<Text_ <<endl;
  cout <<"Remaining Line: " <<Status_->NewInputStr <<endl;
  cout <<"Deepest Level: " <<Status_->DeepestLevel <<endl;
  
  for (i = 0; i <= Status_->DeepestLevel; i++)
  {
    cout <<p->GetResultCodeString(Results_, CodeStr_, 0, i) <<endl;
    CodeStr_[0] = 0;
  }
}

class YoClass
{
  DEFAULT_ROOTOBJECT_DEFN(YoClass)

  private:
    int x;

  public:
    int Getx() const
	{ return x; }
    void Setx(int p)
	{ x = p; }
};

int operator == (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() == y2.Getx()); }

int operator < (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() < y2.Getx()); }

int operator > (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() < y2.Getx()); }

class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

  friend Boolean operator == (const Char& obj1, const Char& obj2)
      { return (obj1.val() == obj2.val()); }
  friend Boolean operator != (const Char& obj1, const Char& obj2)
      { return (obj1.val() != obj2.val()); }
  friend Boolean operator > (const Char& obj1, const Char& obj2)
      { return (obj1.val() > obj2.val()); }
  friend Boolean operator < (const Char& obj1, const Char& obj2)
      { return (obj1.val() < obj2.val()); }
  friend Boolean operator >= (const Char& obj1, const Char& obj2)
      { return (obj1.val() >= obj2.val()); }
  friend Boolean operator <= (const Char& obj1, const Char& obj2)
      { return (obj1.val() <= obj2.val()); }  

  friend ostream& operator << (ostream& os, const Char& obj)
  {
    os <<obj.val();
    return os;
  }
  friend ostream& _WriteToOutput(ostream& os, const Char& Obj_)
	{ os <<Obj_.val(); return os; }    

  private:
    char ch;

  public:
    Char(char c = 'X'):
	ch(c) {};
    Char& Set(char c)
        { ch = c; return *this; }
    char val() const
	{ return (ch); }
};

int* Square(int* x)
{ (*x) *= (*x); return x; }

int Square(int x)
{ return (x * x); }

int operator == (SimpleList<Char>& x, const char* s)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

int operator == (SimpleList<Char>& x, const Char* s)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->val() != s[i].val())
      return 0;

  return 1;
}

void ShowTheList(SimpleList<Char>& x)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
  {
    if (Iter_.IndexNode(i) && Iter_.IndexNode(i)->_Object)
      cout <<"[" <<i <<"] = "
           <<Iter_.IndexNode(i)->_Object->val() <<", ";

    cout <<"\r\n";
  }

  cout <<endl;
}

int operator == (const PtrDynArray<Char>& x, const char* s)
{
  int i;
  for (i = 0; i < x.RunLength(); ++i)
    if (x[i]->val() != s[i])
      return 0;
  return (s[x.RunLength()] == '\0');
}

void ShowTheList(const PtrDynArray<Char>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    cout <<x[i]->val();
    cout <<",";
  }

  cout <<endl;
}

void ShowTheIntList(const PtrDynArray<int>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
    cout <<*(x[i]) <<",";

  cout <<endl;
}

int operator == (const DynArray<Char>& x, const char* s)
{
  int i;
  for (i = 0; i < x.RunLength(); ++i)
    if (x[i].val() != s[i])
      return 0;
  return (s[x.RunLength()] == '\0');
}

void ShowTheList(const DynArray<Char>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    cout <<x[i].val();
    cout <<",";
  }

  cout <<endl;
}

void ShowTheIntList(const DynArray<int>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
    cout <<x[i] <<",";

  cout <<endl;
}

DynArray<Char> operator + (const DynArray<Char>&, const DynArray<Char>&);
DynArray<Char> operator + (const DynArray<Char>&, const Char&);
DynArray<Char> operator + (const Char&, const DynArray<Char>&);

int operator == (const PtrArray<Char>& x, const char* s)
{
  for (int i = 0; i < x.RunLength(); ++i)
    if (x[i]()->val() != s[i])
      return 0;

  return (s[x.RunLength()] == '\0');
}

void ShowTheArray(const PtrArray<Char>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    if (!x[i].IsVoid())
      cout <<x[i]()->val();
    cout <<",";
  }

  cout <<endl;
}

void ShowTheIntArray(const PtrArray<int>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    if (!x[i].IsVoid())
      cout <<*(x[i]);
    cout <<",";
  }

  cout <<endl;
}

int operator == (const PtrArray<Char>& x, Char** s)
{
  for (int i = 0; i < x.RunLength(); ++i)
    if (x[i]() != s[i] ||
	x[i]()->val() != s[i]->val())
      return 0;

  return 1;
}

int operator == (const PtrArray<Char>& x1, const PtrArray<Char>& x2)
{
  if (x1.RunLength() != x2.RunLength())
    return 0;

  for (int i = 0; i < x1.RunLength(); ++i)
    if (x1[i]()->val() != x2[i]()->val())
      return 0;

  return 1;
}

PtrArray<Char> operator + (const PtrArray<Char>&, const Char&);
PtrArray<Char> operator + (const Char&, const PtrArray<Char>&);
PtrArray<Char> operator + (const Char&, const PtrArray<Char>&);

int operator == (const ArgArray<Char>& x, const char* s)
{
  for (int i = 0; i < x.RunLength(); ++i)
    if (x[i]().val() != s[i])
      return 0;

  return (s[x.RunLength()] == '\0');
}

void ShowTheArray(const ArgArray<Char>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    if (!x[i].IsVoid())
      cout <<x[i]().val();
    cout <<",";
  }

  cout <<endl;
}

void ShowTheIntArray(const ArgArray<int>& x)
{
  for (int i = 0; i < x.RunLength(); ++i)
  {
    if (!x[i].IsVoid())
      cout <<x[i]();
    cout <<",";
  }

  cout <<endl;
}

int operator == (const ArgArray<Char>& x, const Char* s)
{
  for (int i = 0; i < x.RunLength(); ++i)
    if (x[i]().val() != s[i].val())
      return 0;

  return 1;
}

int operator == (const ArgArray<Char>& x1, const ArgArray<Char>& x2)
{
  if (x1.RunLength() != x2.RunLength())
    return 0;

  for (int i = 0; i < x1.RunLength(); ++i)
    if (x1[i]().val() != x2[i]().val())
      return 0;

  return 1;
}

ArgArray<Char> operator + (const ArgArray<Char>&, const Char&);
ArgArray<Char> operator + (const Char&, const ArgArray<Char>&);
ArgArray<Char> operator + (const Char&, const ArgArray<Char>&);

int CompChar(const Char& c1, const Char& c2)
{
  return (c1.val() - c2.val());
}

int CompChar(const Char* c1, const Char* c2)
{
  return (c1->val() - c2->val());
}

int CompChar(const char& c1, const char& c2)
{
  return (c1 - c2);
}

int CompCharPtr(const Char* c1, const Char* c2)
{
  return ((c1 && c2) ? (c1->val() - c2->val()):0);
}

int PriorityChar(const Char& c1)
{
  return c1.val();
}

int operator == (const SingleList<Char>& x, const char* s)
{
  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

void ShowTheList(const SingleSublist<Char>& x)
{
  for (int i = 0; i < x.NumNodes(); ++i)
  {
    if (x.GetNode(i) && x.GetNode(i)->GetItem())
      cout <<x.GetNode(i)->GetItem()->val();
    cout <<",";
  }

  cout <<endl;
}

int operator == (const SingleList<Char>& x, const Char* s)
{
  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i].val())
      return 0;

  return 1;
}

SingleList<Char> operator + (const SingleList<Char>&, const Char&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);

int operator == (const DoubleList<Char>& x, const char* s)
{
  if (!s)
    return 0;

  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

void ShowTheList(const DoubleSublist<Char>& x)
{
  for (int i = 0; i < x.NumNodes(); ++i)
  {
    if (x.GetNode(i) && x.GetNode(i)->GetItem())
      cout <<x.GetNode(i)->GetItem()->val();
    cout <<",";
  }

  cout <<endl;
}

int operator == (const DoubleList<Char>& x, const Char* s)
{
  if (!s)
    return 0;

  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i].val())
      return 0;

  return 1;
}

DoubleList<Char> operator + (const DoubleList<Char>&, const Char&);
DoubleList<Char> operator + (const Char&, const DoubleList<Char>&);
DoubleList<Char> operator + (const Char&, const DoubleList<Char>&);

const int MAX_STRING = 12;

class LString
{
   protected:
     char str[MAX_STRING];

   public:
     LString()
       { memset(str, 0, MAX_STRING); }
     LString(const char* s);
     LString(const LString& s)
       { strcpy(str, s.str); }
     char* getstr()
       { return str; }
     unsigned getLen()
       { return strlen(str); }

     LString& operator =(LString& s);
     char& operator[](unsigned index)
      { return str[index]; }

     int operator <(LString& s)
       { return strcmp(str, s.str) < 0; }
     int operator <=(LString& s)
       { return strcmp(str, s.str) <= 0; }
     int operator >(LString& s)
       { return strcmp(str, s.str) > 0; }
     int operator >=(LString& s)
       { return strcmp(str, s.str) >= 0; }
     int operator ==(LString& s)
       { return strcmp(str, s.str) == 0; }
     int operator !=(LString& s)
       { return strcmp(str, s.str) != 0; }

    friend ostream& operator <<(ostream& os, LString& s);
};

LString::LString(const char* s)
{
  if (strlen(s) < MAX_STRING)
    strcpy(str, s);
  else {
    memcpy(str, s, MAX_STRING - 1);
    str[MAX_STRING] = '\0';
  }
}

LString& LString::operator =(LString& s)
{
  strcpy(str, s.str);
  return *this;
}

ostream& operator <<(ostream& os, LString& s)
{
  os << s.str;
  return os;
}

template <class T>
class AdvForLpViewer;

template <class T>
class ForLpViewer;

template <class T>
void Work(ForLoop<T>* LoopPtr_, void* ExecBlock_)
{
  ((ForLpViewer<T>*)ExecBlock_)->Accept(LoopPtr_);
}

template <class T>
void Work(AdvForLoop<T>* LoopPtr_, void* ExecBlock_)
{
  ((AdvForLpViewer<T>*)ExecBlock_)->Accept(LoopPtr_);
}

template <class T>
class ForLpViewer
{
  private:
    long _Count;

  public:
    ForLpViewer(): _Count(0) {}
    long Cnt() const
	{ return _Count; }
    void Accept(ForLoop<T>* LoopPtr_)
	{ cout <<*LoopPtr_->GetIndex() <<" ";
	  ++_Count; }
};

template <class T>
class AdvForLpViewer
{
  private:
    long _Count;

  public:
    AdvForLpViewer(): _Count(0) {}
    long Cnt() const
	{ return _Count; }
    void Accept(AdvForLoop<T>* LoopPtr_)
	{ cout <<*LoopPtr_->GetIndex() <<" ";
	  ++_Count; }
};

class TestClass
{
  private:
    int x;

  public:
    TestClass():
      x(0) {}
    TestClass(int v):
      x(v) {}

    void Set(int v)
      { x = v; }
    int Get()
      { return x; }

    TestClass* Clone() const
      { return (new TestClass(*this)); }
};

StaticPtr<int> REFPTRSTestClass::_StatMem;

StaticObjPtr<EnclosedClass> EnclosedClass::_StaObj;

/****************************************************************************/
void Function1(Tristate& Ts_ = Tristate::Default())
{
  if ((int)Ts_ == Tristate::Indeterminate)
    cout <<"Indeterminate State" <<endl;

  cout <<"STATE : " <<Ts_.ToString() <<endl;
}

/****************************************************************************/
void showTree(Btree<LString>& t, const char* msg)
{
  LString str;
  Boolean ok = t.getFirstNode(str);

//  clrscr();
  cout << msg
       << " (tree has " << t.getNumNodes() << " nodes)"
       << "\n\n";
  while (ok) {
    cout << str << '\n';
    ok = t.getNextNode(str);
  }
//  gotoxy(1, 23);
  cout << "Press any key ... ";
//  getch();
}

/****************************************************************************/
void searchNode(Btree<LString>& t, LString& s)
{
//  gotoxy(1, 23);
//  clreol();
//  gotoxy(1, 22);
  cout << "Press any key to search for " << s;
//  getch();
//  gotoxy(1, 22);
//  clreol();
  if (t.search(s))
    cout << "Found " << s << " in tree\n";
  else
    cout << "No match for " << s << '\n';
  cout << "Press any key to continue ... ";
//  getch();
}

/****************************************************************************/
void displayLink(unsigned n)
{
  if (n != BTREE_NIL)
    cout << '(' << n << ") ";
  else
    cout << "(NIL) ";
}

/****************************************************************************/
void ShowFuzzySet(FuzzySetTypeDefn<char>::NodeStore& x)
{
  FuzzySetTypeDefn<char>::NodeIter Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
  {
    if (Iter_.IndexNode(i) && Iter_.IndexNode(i)->_Object)
    {
      cout <<"[" <<i <<"] = "
           <<"(" <<Iter_.IndexNode(i)->_Object->_Value
           <<"," <<Iter_.IndexNode(i)->_Object->_Key
           <<"), ";
    }
    cout <<"\r\n";
  }

  cout <<endl;
}

/****************************************************************************/
void TestFuzzySets(ostream& os)
{
  os <<"Fuzzy set testing\r\n"
     <<"-----------------\r\n";

  const char* iname     = "abcd";
  const float amemb[4]  = { 0.9F, 0.4F, 0.5F, 0.0F };
  const float bmemb[4]  = { 0.7F, 0.6F, 0.3F, 0.8F };

  const size_t n1       = 6;
  const char* name1     = "ABCDEF";
  const float memb1[6]  = { 0.1F, 0.6F, 0.3F, 0.4F, 0.9F, 0.1F };
  const size_t n2       = 4;
  const char* name2     = "AEGH";
  const float memb2[4]  = { 0.7F, 0.5F, 0.6F, 0.4F };

  FuzzySet<char> A;
  FuzzySet<char> B;
  FuzzySet<char> C;
  FuzzySet<char> D;
  FuzzyNode<double,char> Node_;

  size_t n;
  for (n = 0; n < 4; ++n)
  {
    Node_ = FuzzyNode<double,char>(amemb[n], iname[n]);
    A |= &Node_;
    Node_ = FuzzyNode<double,char>(bmemb[n], iname[n]);
    B |= &Node_;
  }

  os <<"\r\nset A =\r\n";
  ShowFuzzySet(A.GiveSetData());
  os <<"\r\nset B =\r\n";
  ShowFuzzySet(B.GiveSetData());

  C = A | B;
  D = C.Complement();

  os <<"\r\nset c = union(A,B)\r\n";
  ShowFuzzySet(C.GiveSetData());

  os <<"\r\nset d = complement(C)\r\n";
  ShowFuzzySet(D.GiveSetData());

  FuzzySet<char> fs1;
  FuzzySet<char> fs2;
  FuzzySet<char> fs3;
  FuzzySet<char> fs4;
  FuzzySet<char> fs5;

  for (n = 0; n < n1; ++n)
  {
    Node_ = FuzzyNode<double,char>(memb1[n], name1[n]);
    fs1 |= &Node_;
  }

  for (n = 0; n < n2; ++n)
  {
    Node_ = FuzzyNode<double,char>(memb2[n], name2[n]);
    fs2 |= &Node_;
  }  

  fs3 = fs1 | fs2;
  fs4 = fs1 & fs2;
  fs5 = fs3.Complement();

  os <<"\r\nset fs1 =\r\n";
  ShowFuzzySet(fs1.GiveSetData());

  os <<"\r\nset fs2 =\r\n";
  ShowFuzzySet(fs2.GiveSetData());

  os <<"\r\nset fs3 = union(fs1,fs2)\r\n";
  ShowFuzzySet(fs3.GiveSetData());

  os <<"\r\nset fs4 = intersection(fs1,fs2)\r\n";
  ShowFuzzySet(fs4.GiveSetData());

  os <<"\r\nset fs5 = complement(fs3)\r\n";
  ShowFuzzySet(fs5.GiveSetData());
}

/****************************************************************************/
void TestComplex(ostream & buffer)
{
    buffer << "Testing Complex\r\n"
              "===============\r\n\r\n";

    ComplexNumber<double> one(1.0, 0);
    ComplexNumber<double> a (2.0, 3.0);
    ComplexNumber<double> b (4.0, 5.0);
    ComplexNumber<double> c, d;

    buffer << "      a = " << a << "\r\n";
    buffer << "      b = " << b << "\r\n";

    buffer << "\r\n";
    d = a + one;
    buffer << "a + one = " << d << "\r\n";

    d = a - one;
    buffer << "a - one = " << d << "\r\n";

    d = a * one;
    buffer << "a * one = " << d << "\r\n";

    d = a / one;
    buffer << "a / one = " << d << "\r\n";

    d = a + 2.0;
    buffer << "a + 2.0 = " << d << "\r\n";
    
    d = a - 2.0;
    buffer << "a - 2.0 = " << d << "\r\n";
    
    d = a * 2.0;
    buffer << "a * 2.0 = " << d << "\r\n";
    
    d = a / 2.0;
    buffer << "a / 2.0 = " << d << "\r\n";
    
    d = 2.0 + a;
    buffer << "2.0 + a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) - a;
    buffer << "2.0 - a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) * a;
    buffer << "2.0 * a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) / a;
    buffer << "2.0 / a = " << d << "\r\n";
    
    d = d * a;
    buffer << "  d * a = " << d << "\r\n\r\n";

    d = a + b;
    buffer << "  a + b = " << d << "\r\n";
    
    d = a - b;
    buffer << "  a - b = " << d << "\r\n";
    
    d = a * b;
    buffer << "  a * b = " << d << "\r\n";
    
    d = a / b;
    buffer << "  a / b = " << d << "\r\n";

    d = d * b;
    buffer << "  d * b = " << d << "\r\n\r\n";

    a = ComplexNumber<double>(2.0,3.0);
    b = ComplexNumber<double>(4.0,5.0);

    c = a; c += b;
    buffer << " a += b = " << c << "\r\n";

    c = a; c -= b;
    buffer << " a -= b = " << c << "\r\n";

    c = a; c *= b;
    buffer << " a *= b = " << c << "\r\n";

    c = a; c /= b;
    buffer << " a /= b = " << c << "\r\n";

    c = a; c += 2.0;
    buffer << " a += 2 = " << c << "\r\n";

    c = a; c -= 2.0;
    buffer << " a -= 2 = " << c << "\r\n";

    c = a; c *= 2.0;
    buffer << " a *= 2 = " << c << "\r\n";

    c = a; c /= 2.0;
    buffer << " a /= 2 = " << c << "\r\n\r\n";

    d = -a;
    buffer << "     -a = " << d << "\r\n";
    
    d = conj(a);
    buffer << "conj(a) = " << d << "\r\n";

    d = ComplexNumber<double>(norm(a));
    buffer << "norm(a) = " << d << "\r\n";

    d = ComplexNumber<double>(abs(a));
    buffer << " abs(a) = " << d << "\r\n";

    d = ComplexNumber<double>(arg(a));
    buffer << " arg(a) = " << d << "\r\n\r\n";

    // trigonometry tests
    d = tan(a);
    buffer << " tan(a) = " << d << "\r\n";

    d = sin(a);
    buffer << " sin(a) = " << d << "\r\n";

    c = cos(a);
    buffer << " cos(a) = " << c << "\r\n";

    c = c * c + d * d;
    buffer << "sin * sin + cos * cos = "
           << c << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3, N_DPI);
    buffer << "      d = " << d << "\r\n";

    c = tan(d);
    buffer << " tan(d) = " << c << "\r\n";

    c = atan(c);
    buffer << "atan(c) = " << c << "\r\n\r\n";

    double dt = atan(1.0);
    ComplexNumber <double> ct = atan(ComplexNumber<double>(1.0,0.0));

    buffer << "     dt = " << dt << "\r\n";
    buffer << "     ct = " << ct << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3,N_DPI);
    buffer << "      d = " << d << "\r\n";

    c = sin(d);
    buffer << " sin(d) = " << c << "\r\n";

    c = asin(c);
    buffer << "asin(c) = " << c << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3,N_DPI);
    buffer << "      d = " << d << "\r\n";
    
    c = cos(d);
    buffer << " cos(d) = " << c << "\r\n";
    
    c = acos(c);
    buffer << "acos(c) = " << c << "\r\n";

    double          qd = tanh(N_DPI);
    ComplexNumber<double> qc = tanh(ComplexNumber<double>(N_DPI));

    buffer << "\r\n" << qd << " should equal " << qc << "\r\n";

    qd = atanh(qd);
    qc = atanh(qc);
    buffer << qd << " should equal " << qc << "\r\n";

    ComplexNumber<double> qx(0.5,0.3);
    buffer << "qx = " << qx << "\r\n";

    qc = tanh(qx);
    buffer << "qc = " << qc << "\r\n";
    
    qc = atanh(qc);
    buffer << "qc = " << qc << "\r\n";

    a = ComplexNumber<double>(0.5,0.3);
    buffer << "\r\n      a  = " << a << "\r\n";

    b = tanh(a);
    buffer << " tanh(a) = " << b << "\r\n";
    
    c = sinh(a);
    buffer << " sinh(a) = " << c << "\r\n";

    d = cosh(a);
    buffer << " cosh(a) = " << d << "\r\n";

    b = atanh(b);
    buffer << "atanh(b) = " << b << "\r\n";

    c = asinh(c);
    buffer << "asinh(c) = " << c << "\r\n";

    d = acosh(d);
    buffer << "acosh(d) = " << d << "\r\n\r\n";

    // logarithm tests
    a = ComplexNumber<double>(4.0);
    buffer << "      a = " << a << "\r\n";

    b = ComplexNumber<double>(-1.0);
    buffer << "      b = " << b << "\r\n";

    c = ComplexNumber<double>(3.14,15.92);
    buffer << "      c = " << c << "\r\n";

    d = sqrt(a);
    buffer << "sqrt(a) = " << d << "\r\n";

    d = sqrt(b);
    buffer << "sqrt(b) = " << d << "\r\n";

    d = sqrt(c);
    buffer << "sqrt(c) = " << d << "\r\n";

    d *= d;
    buffer << "  d * d = " << d << "\r\n";

    d = pow(a,0.5);
    buffer << "pow(a,0.5) = " << d << "\r\n";

    d = log(c);
    buffer << " log(a) = " << d << "\r\n";

    d = exp(d);
    buffer << " exp(d) = " << d << "\r\n";

    d = exp(ComplexNumber<double>(1.0));
    buffer << " exp(1) = " << d << "\r\n";

    d = log(d);
    buffer << " log(e) = " << d << "\r\n";

    d = log10(ComplexNumber<double>(10.0));
    buffer << "   log10(10) = " << d << "\r\n";

    d = log10(ComplexNumber<double>(217.3));
    buffer << "log10(217.3) = " << d << "\r\n";


    a = ComplexNumber<double>(N_DPI,N_DE); // value with lotsa digits
    b = SigDig(a,8);
    c = SigDig(a,2);

    buffer << setprecision(15);
    buffer << "      a = " << a << "\r\n";
    buffer << "      b = " << b << "\r\n";
    buffer << "      c = " << c << "\r\n";
    buffer << setprecision(6) << "\r\n";

    // conversion tests
//    ComplexNumber<float> fa(a);
//    ComplexNumber<long double> la(fa);
//    buffer << "     fa = " << fa << "\r\n";
//    buffer << "     la = " << la << "\r\n";
}

/****************************************************************************/
void showPages(Btree<LString>& t, const char* msg, LString& s)
// displays the tree nodes for a B-tree with no deleted nodes
{
   Bstruct<LString> page;
//   clrscr();
   cout << msg << s << '\n';
   for (unsigned i = 0; i < t.getNumNodes(); i++) {
     t.getPage(&page, i);
     cout << "Page " << i << " : ";
     for (unsigned j = 0; j <= page.count; j++) {
       if (j > 0)
         cout << page.data[j] << " ";
       displayLink(page.nodeLink[j]);
     }
     cout << '\n';
   }
   cout << "\n\nPress any key to continue";
//   getch();
}

/****************************************************************************/
// utility routine to display a matrix
template <class T>
void ShowMatrix(ostream& os, const Matrix<T>& m)
{
  const size_t MaxR = m.GetRows();
  const size_t MaxC = m.GetCols();
  size_t r, c;

  for (r = 0; r < MaxR; ++r)
  {
    os << "[ ";

    for (c = 0; c < MaxC; ++c)
      os << setw(3) << m.Get(r,c) << " ";

    os << "]\r\n";
  }
}

int Times2(const int & x)
{
  return x * 2;
}

/****************************************************************************/
// template function to display array
template <class T>
ostream & operator << (ostream & buffer, StatArray<T> & a)
{
  buffer << "[" <<"0" << "," << a.RunLength() << "] ";

  for (int i = 0; i <= a.RunLength(); ++i)
    buffer << a[i] << " ";

  return buffer;
}

/****************************************************************************/
void showTree(ArrayBinTree<ChrString>& t, ChrString msg)
{
  ChrString str;
  Boolean ok = t.getFirstNode(str);

  cout << msg << "\n\n";
  while (ok) {
    cout << str << '\n';
    ok = t.getNextNode(str);
  }
  
  cout << "Press any key ... ";
}

/****************************************************************************/
void searchNode(ArrayBinTree<ChrString>& t, ChrString& s)
{
  cout << "Press any key to search for " << s;
  if (t.search(s))
    cout << "Found " << s << " in tree\n";
  else
    cout << "No match for " << s << '\n';
  cout << "Press any key to continue ... ";
}

/****************************************************************************/
// utility routine to display a matrix
template <class T>
void ShowPoly(ostream & buffer, const Polynomial<T> p)
{
        const size_t D = p.Degree();

        buffer << "[ ";

        size_t i = D - 1;

        while(1)
        {
            buffer << p.Get(i) << " ";

            if (i == 0)
                break;

            --i;
        }

        buffer << "]";
}

template <class T>
void BinaryOut(ostream & str, T x)
{
        T mask = 1 << (CHAR_BIT * sizeof(T) - 1);

        while (1)
        {
            str << ((x & mask) ? "1" : "0");

            mask >>= 1;

            if (mask == 0)
                break;
        }
}

/****************************************************************************/
void TestRandom(ostream & buffer)
{
    buffer << "Uniform Deviate Test\r\n\r\n";

    const int TEST_SIZE  = 31231;
    const int ARRAY_SIZE =  1000;
    float chiSq = 0.0F;
    int i, j;
    RandDev udev;
    unsigned int numbers[ARRAY_SIZE];

    // perform a chi-square test
    // fill array with zeros
    for (i = 0; i < ARRAY_SIZE; ++i)
         numbers[i] = 0;

    // increment
    for (i = 0; i < TEST_SIZE; ++i)
         numbers[udev(ARRAY_SIZE)]++;

    // calculate chi-square value
    for (i = 0; i < ARRAY_SIZE; ++i)
         chiSq += (float)numbers[i] * (float)numbers[i];

    chiSq = (ARRAY_SIZE * chiSq / TEST_SIZE) - TEST_SIZE;

    // display chi-square value
    buffer << "Chi-Square test value: " << chiSq << "\r\n\r\n";

    // output a set of values for each generator
    for (i = 0; i < 20; ++i)
        {
        for (j = 0; j < 5; ++j)
            {
            buffer << '\t' << setw(10) << udev();
            }

        buffer << "\r\n";
        }

    buffer << "\r\nRandom Number Table Test\r\n\r\n";

    RandTable<100> rtable;

    for (size_t n1 = 0; n1 < 15; ++n1)
        {
        for (size_t n2 = 0; n2 < 10; ++n2)
            {
            buffer << "\t" << setw(3) << rtable();
            }

        buffer << "\r\n";
        }
}

/****************************************************************************/
void TestPolynomials(ostream& buffer)
{
    buffer << "Polynomial test\r\n===============\r\n\r\n";

    Polynomial<int> p1(4), p2(4), p3(2), p4(4), p5(8);

    p1[0U] =  1; p2[0U] =  2; p3[0U] =  2;
    p1[1U] =  2; p2[1U] = -1; p3[1U] = -1;
    p1[2U] =  0; p2[2U] =  0;
    p1[3U] = -3; p2[3U] =  0;

    buffer << "p1 = "; ShowPoly(buffer,p1); buffer << "\r\n";
    buffer << "p2 = "; ShowPoly(buffer,p2); buffer << "\r\n";
    buffer << "p3 = "; ShowPoly(buffer,p3); buffer << "\r\n";

    buffer << "p1 + p2 = "; ShowPoly(buffer,p1 + p2); buffer << "\r\n";
    buffer << "p1 + p3 = "; ShowPoly(buffer,p1 + p3.Stretch(4)); buffer << "\r\n";

    p4 = p1;
    p4 += p2;
    buffer << "p1 += p2 = "; ShowPoly(buffer,p4); buffer << "\r\n";

    p5 = p1 * p2;
    buffer << "p5 = p1 * p2"; ShowPoly(buffer,p5); buffer << "\r\n";
    buffer << "p1(3) = " << p1(3) << "\r\n";
    buffer << "p5(2) = " << p5(2) << "\r\n";

    int a1[] = { 9, -10, 7,  6};
    int a2[] = {-5,   4, 0, -2};

    Polynomial<int> x1(4,a1), x2(4), x3(7);
    x2 = a2;

    buffer << "x1 = "; ShowPoly(buffer,x1); buffer << "\r\n";
    buffer << "x2 = "; ShowPoly(buffer,x2); buffer << "\r\n";

    x3 = x1 * x2;
    buffer << "x3 = x1 * x2"; ShowPoly(buffer,x3); buffer << "\r\n\r\n";

    int finit[] = {-6,0,1,-3};
    int ginit[] = {1,1,0,5};
    int hinit[] = {1,2,3,4,5};
    Polynomial<int> f(4,finit), g(4,ginit), h(5,hinit);

    buffer << "f = "; ShowPoly(buffer,f); buffer << "\r\n";
    buffer << "g = "; ShowPoly(buffer,g); buffer << "\r\n";
    buffer << "h = "; ShowPoly(buffer,h); buffer << "\r\n";

    buffer << "g + f = "; ShowPoly(buffer,g + f); buffer << "\r\n";
    buffer << "f - g = "; ShowPoly(buffer,f - g); buffer << "\r\n";
    buffer << "f * g = "; ShowPoly(buffer,f * g); buffer << "\r\n";
    buffer << "f - h = "; ShowPoly(buffer,f - h); buffer << "\r\n";
    buffer << "h - f = "; ShowPoly(buffer,h - f); buffer << "\r\n";
    buffer << "f + h = "; ShowPoly(buffer,f + h); buffer << "\r\n";
    buffer << "h + f = "; ShowPoly(buffer,h + f); buffer << "\r\n\r\n";

    // test FFT multiply
    double px1init[] = { 9.0, -10.0, 7.0,  6.0};
    double px2init[] = {-5.0,   4.0, 0.0, -2.0};
    DPoly px1(4,px1init);
    DPoly px2(4,px2init);

    buffer << "px1 = "; ShowPoly(buffer,px1); buffer << "\r\n";
    buffer << "px2 = "; ShowPoly(buffer,px2); buffer << "\r\n";

    buffer << "\r\n           px1 * px2 = ";
    ShowPoly(buffer,px1 * px2);
    buffer << "\r\n";

    buffer << "FFTMultiply(px1,px2) = ";
    ShowPoly(buffer,FFTMultiply(px1,px2));
    buffer << "\r\n\r\n";

    double z1init[] = { 1.0, 1.1, 2.0, 2.2, 3.0, 3.3, 4.0, 4.4,
                        5.0, 5.5, 6.0, 6.6, 7.0, 7.7, 8.0, 8.8 };

    double z2init[] = { 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8,
                        2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8 };

    DPoly z1(16,z1init);
    DPoly z2(16,z2init);

    buffer << "z1 = "; ShowPoly(buffer,z1); buffer << "\r\n";
    buffer << "z2 = "; ShowPoly(buffer,z2); buffer << "\r\n";

    buffer << "\r\n           z1 * z2 = ";
    ShowPoly(buffer,z1 * z2);
    buffer << "\r\n";

    buffer << "FFTMultiply(z1,z2) = ";
    ShowPoly(buffer,FFTMultiply(z1,z2));
    buffer << "\r\n";
}

/****************************************************************************/
void TestMatrix(ostream& os)
{
    // display a headline
    os << "Matrix test\r\n===========\r\n";

    Matrix<int> A(3,3), B(3,3), C(3,3), D(3,3);

    A(0,0) =   1;
    A(0,1) =   3;
    A(0,2) =  -4;
    A(1,0) =   1;
    A(1,1) =   1;
    A(1,2) =  -2;
    A(2,0) =  -1;
    A(2,1) =  -2;
    A(2,2) =   5;

    B(0,0) =   8;
    B(0,1) =   3;
    B(0,2) =   0;
    B(1,0) =   3;
    B(1,1) =  10;
    B(1,2) =   2;
    B(2,0) =   0;
    B(2,1) =   2;
    B(2,2) =   6;

    D(0,0) =   1;
    D(0,1) =   2;
    D(0,2) =  -1;
    D(1,0) =   2;
    D(1,1) =  -1;
    D(1,2) =  -3;
    D(2,0) =   0;
    D(2,1) =  -2;
    D(2,2) =   4;

    os << "\r\nMatrix A = \r\n";
    ShowMatrix(os,A);

    os << "\r\nMatrix B = \r\n";
    ShowMatrix(os,B);

    C = A % B;
    os << "\r\nMatrix C (A % B) = \r\n";
    ShowMatrix(os,C);

    C = A + B;
    os << "\r\nMatrix C (A + B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C += B;
    os << "\r\nMatrix C (= A, += B) =\r\n";
    ShowMatrix(os,C);

    C = A + 1;
    os << "\r\nMatrix C (= A + 1) =\r\n";
    ShowMatrix(os,C);

    C += 1;
    os << "\r\nMatrix C (+= 1) =\r\n";
    ShowMatrix(os,C);

    C = A - B;
    os << "\r\nMatrix C (A - B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C -= B;
    os << "\r\nMatrix C (= A, -= B) =\r\n";
    ShowMatrix(os,C);

    C = A - 1;
    os << "\r\nMatrix C (= A - 1) =\r\n";
    ShowMatrix(os,C);

    C -= 1;
    os << "\r\nMatrix C (-= 1) =\r\n";
    ShowMatrix(os,C);

    C = A * B;
    os << "\r\nMatrix C (A * B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C *= B;
    os << "\r\nMatrix C (= A, *= B) =\r\n";
    ShowMatrix(os,C);

    C = A * 2;
    os << "\r\nMatrix C (= A * 2) =\r\n";
    ShowMatrix(os,C);

    C *= 2;
    os << "\r\nMatrix C (*= 2) =\r\n";
    ShowMatrix(os,C);

    C = B / A;
    os << "\r\nMatrix C (B / A) = \r\n";
    ShowMatrix(os,C);

    C = B;
    C /= A;
    os << "\r\nMatrix C (= B, /= A) =\r\n";
    ShowMatrix(os,C);

    C = A / 2;
    os << "\r\nMatrix C (= A / 2) =\r\n";
    ShowMatrix(os,C);

    C /= 2;
    os << "\r\nMatrix C (/= 2) =\r\n";
    ShowMatrix(os,C);

    C = -A;
    os << "\r\nMatrix C (-A) = \r\n";
    ShowMatrix(os,C);

    // test comparisons
    os << "\r\nMatrix A = \r\n";
    ShowMatrix(os,A);

    os << "\r\nMatrix D = \r\n";
    ShowMatrix(os,D);

    if (A.Equals(D))
        os << "\r\nERROR: A should not equal D";
    else
        os << "\r\nOKAY: A not equal D";

    C = A;
    if (A.Equals(C))
        os << "\r\nOKAY: A equals C\r\n";
    else
        os << "\r\nERROR: A should equal C\r\n";

    Matrix<bool> I(3,3);

    I = (A == D);
    os << "\r\nMatrix I = (A == D)\r\n";
    ShowMatrix(os,I);

    I = (A != D);
    os << "\r\nMatrix I = (A != D)\r\n";
    ShowMatrix(os,I);

    I = (A < D);
    os << "\r\nMatrix I = (A < D)\r\n";
    ShowMatrix(os,I);

    I = (A <= D);
    os << "\r\nMatrix I = (A <= D)\r\n";
    ShowMatrix(os,I);

    I = (A > D);
    os << "\r\nMatrix I = (A > D)\r\n";
    ShowMatrix(os,I);

    I = (A >= D);
    os << "\r\nMatrix I = (A >= D)\r\n";
    ShowMatrix(os,I);

    // check fill function
    C.Fill(9);
    os << "\r\nC filled with 9 =\r\n";
    ShowMatrix(os,C);

    // check Apply functions
    C = Apply(A, Times2);
    os << "\r\nC = A.Apply(Times2)\r\n";
    ShowMatrix(os,C);

    C.Apply(Times2);
    os << "\r\nApply(C,Times2)\r\n";
    ShowMatrix(os,C);

    // check row and column vector functions
    Matrix<int>   S(1,1);
    Matrix<int> r1A(3,1);
    Matrix<int> c0B(1,3);

    r1A = A.VectorRow(1);
    c0B = B.VectorCol(0);

    os << "\r\nMatrix S = \r\n";
    ShowMatrix(os,S);

    os << "\r\nMatrix R1A = \r\n";
    ShowMatrix(os,r1A);

    os << "\r\nMatrix C0B = \r\n";
    ShowMatrix(os,c0B);

    if (r1A.IsRowVector())
        os << "\r\nOKAY: R1A is row vector";
    else
        os << "\r\nERROR: R1A should be a row vector";

    if (!r1A.IsColVector())
        os << "\r\nOKAY: R1A is not a column vector";
    else
        os << "\r\nERROR: R1A should not be a column vector";

    if (!c0B.IsRowVector())
        os << "\r\nOKAY: C0B is not a row vector";
    else
        os << "\r\nERROR: C0B should not be a row vector";

    if (c0B.IsColVector())
        os << "\r\nOKAY: C0B is column vector";
    else
        os << "\r\nERROR: C0B should be a column vector";

    if (c0B.IsVector())
        os << "\r\nOKAY: C0B is a vector";
    else
        os << "\r\nERROR: C0B should be a vector";

    if (!A.IsVector())
        os << "\r\nOKAY: A is not a vector";
    else
        os << "\r\nERROR: A should not be a vector";

    if (!c0B.IsSquare())
        os << "\r\nOKAY: C0B is not square";
    else
        os << "\r\nERROR: C0B should not be square";

    if (A.IsSquare())
        os << "\r\nOKAY: A is square";
    else
        os << "\r\nERROR: A should be square";

    B.Fill(0);

    if (B.IsZero())
        os << "\r\nOKAY: B is zero";
    else
        os << "\r\nERROR: B should be zero";

    if (!A.IsZero())
        os << "\r\nOKAY: A is not zero";
    else
        os << "\r\nERROR: A should not be zero";

    // test inner product
    int ip = r1A.InnerProduct(c0B);
    os << "\r\n\r\ninner product of R1A and C0B = " << ip << "\r\n";

    // make some bigger matrices
    Matrix<int> M1(5,5), M2(5,5,3), M3(5,5), M4(5,5);

    const int junk[]  = { 1, 5, 3, 0, 1,
                          0, 2, 0, 4, 5,
                          1, 0, 0, 2, 3,
                          7, 1, 3, 0, 0,
                          2, 1, 0, 4, 6 };

    const int ident[] = { 1, 0, 0, 0, 0,
                          0, 1, 0, 0, 0,
                          0, 0, 1, 0, 0,
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 1 };

    const int tridi[] = { 1, 1, 0, 0, 0,
                          1, 1, 1, 0, 0,
                          0, 1, 1, 1, 0,
                          0, 0, 1, 1, 1,
                          0, 0, 0, 1, 1 };

    const int utri[]  = { 1, 1, 1, 1, 1,
                          0, 1, 1, 1, 1,
                          0, 0, 1, 1, 1,
                          0, 0, 0, 1, 1,
                          0, 0, 0, 0, 1 };

    const int ltri[]  = { 1, 0, 0, 0, 0,
                          1, 1, 0, 0, 0,
                          1, 1, 1, 0, 0,
                          1, 1, 1, 1, 0,
                          1, 1, 1, 1, 1 };

    const int perm[]  = { 0, 1, 0, 0, 0,
                          1, 0, 0, 0, 0,
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 1,
                          0, 0, 1, 0, 0 };

    const int det[]  = { 3, 5, 3, 8, 1,
                         2, 6, 3, 4, 5,
                         1, 4, 5, 2, 3,
                         7, 1, 3, 6, 8,
                         2, 4, 1, 4, 9 };
    M1 = ident;
    M3 = M1 * 2;
    M4 = junk;

    os << "\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    os << "\r\nmatrix M2 = \r\n";
    ShowMatrix(os,M2);

    os << "\r\nmatrix M3 = \r\n";
    ShowMatrix(os,M3);

    os << "\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    if (M1.IsDiagonal())
        os << "\r\nOKAY: M1 is diagonal";
    else
        os << "\r\nERROR: M1 should be diagonal";

    if (M1.IsIdentity())
        os << "\r\nOKAY: M1 is an identity matrix";
    else
        os << "\r\nERROR: M1 should be an identity matrix";

    if (!M2.IsDiagonal())
        os << "\r\nOKAY: M2 is not diagonal";
    else
        os << "\r\nERROR: M2 should not be diagonal";

    if (!M2.IsIdentity())
        os << "\r\nOKAY: M2 is not an identity matrix";
    else
        os << "\r\nERROR: M2 should not be an identity matrix";

    if (M3.IsDiagonal())
        os << "\r\nOKAY: M3 is diagonal";
    else
        os << "\r\nERROR: M3 should be diagonal";

    if (!M3.IsIdentity())
        os << "\r\nOKAY: M3 is not an identity matrix";
    else
        os << "\r\nERROR: M3 should not be an identity matrix";

    if (!M4.IsDiagonal())
        os << "\r\nOKAY: M4 is not diagonal";
    else
        os << "\r\nERROR: M4 should not be diagonal";

    if (!M4.IsIdentity())
        os << "\r\nOKAY: M4 is not an identity matrix";
    else
        os << "\r\nERROR: M4 should not be an identity matrix";

    // tridiagonal tests
    M1 = tridi;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsTridiagonal())
        os << "\r\nOKAY: M1 is tridiagonal";
    else
        os << "\r\nERROR: M1 should be tridiagonal";

    if (!M4.IsTridiagonal())
        os << "\r\nOKAY: M4 is not tridiagonal";
    else
        os << "\r\nERROR: M1 should not be tridiagonal";

    // upper triangular tests
    M1 = utri;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsUpperTriangular())
        os << "\r\nOKAY: M1 is upper-triangular";
    else
        os << "\r\nERROR: M1 should be upper-triangular";

    if (!M4.IsUpperTriangular())
        os << "\r\nOKAY: M4 is not upper-triangular";
    else
        os << "\r\nERROR: M4 should not be upper-triangular";

    // lower triangular tests
    M1 = ltri;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsLowerTriangular())
        os << "\r\nOKAY: M1 is lower-triangular";
    else
        os << "\r\nERROR: M1 should be lower-triangular";

    if (!M4.IsLowerTriangular())
        os << "\r\nOKAY: M4 is not lower-triangular";
    else
        os << "\r\nERROR: M4 should not be lower-triangular";

    // permutation tests
    M1 = perm;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    M2 = ident;
    os << "\r\n\r\nmatrix M2 = \r\n";
    ShowMatrix(os,M2);

    if (M1.IsPermutation())
        os << "\r\nOKAY: M1 is permutation matrix";
    else
        os << "\r\nERROR: M1 should be permutation";

    if (M2.IsPermutation())
        os << "\r\nOKAY: M2 is permutation matrix";
    else
        os << "\r\nERROR: M2 should be permutation";

    if (!M4.IsPermutation())
        os << "\r\nOKAY: M4 is not permutation";
    else
        os << "\r\nERROR: M4 should not be permutation";

    // check singularity function
    M1(0,1) = 0;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsSingular())
        os << "\r\nOKAY: M1 is singular";
    else
        os << "\r\nERROR: M1 should be singular";

    if (!M2.IsSingular())
        os << "\r\nOKAY: M2 is not singular";
    else
        os << "\r\nERROR: M2 should not be singular";

    if (!M4.IsSingular())
        os << "\r\nOKAY: M4 is not singular";
    else
        os << "\r\nERROR: M4 should not be singular";

    // change main window heading
    os <<endl
       <<"Matrix Tests (manipulations)" <<endl
       <<"============================" <<endl;
    
    // test minors and determinants
    os << "\r\n\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    os << "\r\nminor M4(1,1) = \r\n";
    ShowMatrix(os,M4.Minor(1,1));

    os << "\r\nminor M4(0,4) = \r\n";
    ShowMatrix(os,M4.Minor(0,4));

    Matrix<int> M5(2,2), M6(3,3);

    M5(0,0) = 1;
    M5(0,1) = 2;
    M5(1,0) = 3;
    M5(1,1) = 4;

    M6(0,0) = 1;
    M6(0,1) = 3;
    M6(0,2) = 2;
    M6(1,0) = 5;
    M6(1,1) = 4;
    M6(1,2) = 7;
    M6(2,0) = 6;
    M6(2,1) = 9;
    M6(2,2) = 8;

    M4 = det;
    Matrix<int> T4(5,5), T5(2,2), T6(3,3);

    T4 = M4.Transpose();
    T5 = M5.Transpose();
    T6 = M6.Transpose();

    os << "\r\nmatrix M5 = \r\n";
    ShowMatrix(os,M5);

    os << "\r\ndeterminant of M5 = "
           << M5.Determinant() << "\r\n";

    os << "\r\nmatrix T5 = \r\n";
    ShowMatrix(os,T5);

    os << "\r\ndeterminant of T5 = "
           << T5.Determinant() << "\r\n";

    os << "\r\nmatrix M6 = \r\n";
    ShowMatrix(os,M6);

    os << "\r\ndeterminant of M6 = "
           << M6.Determinant() << "\r\n";

    os << "\r\nmatrix T6 = \r\n";
    ShowMatrix(os,T6);

    os << "\r\ndeterminant of T6 = "
           << T6.Determinant() << "\r\n";

    os << "\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    os << "\r\ndeterminant of M4 = "
           << M4.Determinant() << "\r\n";

    os << "\r\nmatrix T4 = \r\n";
    ShowMatrix(os,T4);

    os << "\r\ndeterminant of T4 = "
           << T4.Determinant() << "\r\n";

    Matrix<int> R;
    os << "\r\nMatrix R (def. constr.) = \r\n";
    ShowMatrix(os,R);

    R.Resize(10,10);
    os << "\r\nMatrix R (now 10x10) = \r\n";
    ShowMatrix(os,R);

    // change main window heading
    os <<endl
       <<"Matrix Tests (double)" <<endl
       <<"=====================" <<endl;
    
    // check <double> Matrix
    os << "\r\nFLOATING POINT!";

    Matrix<double> X(3,4), Y(4,3), Z(3,3);

    X(0,0) =  1.0; X(1,0) =  5.0; X(2,0) =  2.0;
    X(0,1) =  2.0; X(1,1) =  2.0; X(2,1) =  4.0;
    X(0,2) =  0.0; X(1,2) =  3.0; X(2,2) =  3.0;
    X(0,3) =  1.0; X(1,3) =  2.0; X(2,3) =  1.0;

    Y(0,0) =  0.0; Y(2,0) =  1.0;
    Y(0,1) =  1.0; Y(2,1) =  0.0;
    Y(0,2) =  2.0; Y(2,2) =  5.0;
    Y(1,0) =  1.0; Y(3,0) =  3.0;
    Y(1,1) =  3.0; Y(3,1) =  1.0;
    Y(1,2) =  2.0; Y(3,2) =  2.0;

    os << "\r\nMatrix X = \r\n";
    ShowMatrix(os,X);
    os << "\r\nMatrix Y = \r\n";
    ShowMatrix(os,Y);

    Z = X % Y;
    os << "\r\nMatrix Z (X % Y) = \r\n";
    ShowMatrix(os,Z);

    // check transposition
    Matrix<double> tX;
    tX = X.Transpose();
    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);
    os << "\r\nTranspose X =\r\n";
    ShowMatrix(os,tX);

    X(0,0) =  1; X(0,1) =  3; X(0,2) = -4; X(0,3) =  8;
    X(1,0) =  1; X(1,1) =  1; X(1,2) = -2; X(1,3) =  2;
    X(2,0) = -1; X(2,1) = -2; X(2,2) =  5; X(2,3) = -1;

    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);

    Matrix<double> lX(X.LinSolve());
    os << "\r\nX after elimination =\r\n";
    ShowMatrix(os,X);

    os << "\r\nlinear equation solution =\r\n";
    ShowMatrix(os,lX);

    X(0,0) =  1.0; X(1,0) =  3.0; X(2,0) =  5.0;
    X(0,1) =  2.0; X(1,1) =  5.0; X(2,1) =  6.0;
    X(0,2) =  0.0; X(1,2) =  4.0; X(2,2) =  3.0;
    X(0,3) =  0.1; X(1,3) = 12.5; X(2,3) = 10.3;

    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);

    lX = X.LinSolve();
    os << "\r\nX after elimination =\r\n";
    ShowMatrix(os,X);

    os << "\r\nlinear equation solution =\r\n";
    ShowMatrix(os,lX);

    Matrix<double> Adbl(3,3), Bdbl(3,1);

    Adbl(0,0) =  1.0; Adbl(0,1) =  2.0; Adbl(0,2) =  0.0;
    Adbl(1,0) =  3.0; Adbl(1,1) =  5.0; Adbl(1,2) =  4.0;
    Adbl(2,0) =  5.0; Adbl(2,1) =  6.0; Adbl(2,2) =  3.0;

    Bdbl(0,0) =  0.1;
    Bdbl(1,0) = 12.5;
    Bdbl(2,0) = 10.3;

    os << "\r\n\r\nmatrix Adbl = \r\n";
    ShowMatrix(os,Adbl);

    os << "\r\nmatrix Bdbl = \r\n";
    ShowMatrix(os,Bdbl);

    Matrix<double> alup(Adbl); // copy Adbl before LUP decomp
    os << "\r\nLU decomp of Adbl (before) = \r\n";
    ShowMatrix(os,alup);

    Matrix<size_t> aperm = alup.LUPDecompose();
    os << "\r\nLU decomp of Adbl (after) = \r\n";
    ShowMatrix(os,alup);

    os << "\r\nPermutation of Adbl = \r\n";
    ShowMatrix(os,aperm);

    Matrix<double> asol = alup.LUPSolve(aperm,Bdbl);
    os << "\r\nlinear solution of Adbl and Bdbl = \r\n";
    ShowMatrix(os,asol);

    Matrix<double> ainv = alup.LUPInvert(aperm);
    os << "\r\ninverse of Adbl and Bdbl = \r\n";
    ShowMatrix(os,ainv);

    Matrix<double> aid = Adbl % ainv;
    os << "\r\ninverse dot Adbl = \r\n";
    ShowMatrix(os,aid);

    Grid<size_t> iperm = ainv.LUPDecompose();
    Matrix<double> invinv = ainv.LUPInvert(iperm);
    os << "\r\ninverse of inverse =\r\n";
    ShowMatrix(os,invinv);
}

/****************************************************************************/
// main test routine
void TestArray(ostream & buffer)
{
    ArrayTestA(buffer);
    ArrayTest3(buffer);
    ArrayTest4(buffer);
    ArrayTest5(buffer);
    ArrayTest6(buffer);
    ArrayTest7(buffer);    
    ArrayTest9(buffer);
    ArrayTest10(buffer);
    ArrayTest11(buffer);
    ArrayTest13(buffer);
}

/****************************************************************************/
void ArrayTestA(ostream& buffer)
{
  Boolean Error_;
  ifstream fin("stats.in", ios::in);

  cout <<endl;
  double val_ = StatArray<double>::AvgMean(fin, Error_);
  cout <<"Mean: " <<val_ <<endl;                // == 1325.18
  val_ = StatArray<double>::StdDeviation(fin, Error_);
  cout <<"Std. dev.: " <<val_ <<endl;           // == 2069.23
}

/****************************************************************************/
// basic array tests
void ArrayTest3(ostream & buffer)
{
    // banner
    buffer << "\r\nArrays: Integer Fills and Arithmetic"
              "\r\n------------------------------------\r\n";

        Range r(1, 10);
	NumericArray<int> ia(size_t(11), default_size);

	ia.AssignArg(0, ia.RunLength());
	buffer << "                  Filled with zero = " << ia << "\r\n";

	ia.FillArithmetic(2,3);
	buffer << "      Filled arithmetic (2, +3) ia = " << ia << "\r\n";

	NumericArray<int> ib(size_t(11), default_size);
	ib.FillGeometric(1,2);
	buffer << "       Filled geometric (1, *2) ib = " << ib << "\r\n";

	NumericArray<int> ic(size_t(11), default_size);
	ic = ia + ib; 
	buffer << "                 Addition: ia + ib = " << ic << "\r\n";

	ic = ia - ib;
	buffer << "              Subtraction: ia - ib = " << ic << "\r\n";

	ic = ia * ib;
	buffer << "           Multiplication: ia * ib = " << ic << "\r\n";

	ic = ia / ib;
	buffer << "                 Division: ia / ib = " << ic << "\r\n";

	ic = ia;
	ic += ib;
	buffer << "      Shorthand Addition: ia += ib = " << ic << "\r\n";

	ic = ia;
	ic -= ib;
	buffer << "   Shorthand Subtraction: ia -= ib = " << ic << "\r\n";

	ic = ia;
	ic *= ib;
	buffer << "Shorthand Multiplication: ia *= ib = " << ic << "\r\n";

	ic = ia;
	ic /= ib;
	buffer << "      Shorthand Division: ia /= ib = " << ic << "\r\n";
}

void ArrayTest4(ostream & buffer)
{
    buffer << "\r\nArrays: float Fills and Arithmetic"
              "\r\n----------------------------------\r\n";

	Range r(1,10);
	NumericArray<float> fa(size_t(11), default_size), fb(size_t(11), default_size);

	fa.AssignArg(0.0F, fa.RunLength());
	buffer << "                  Filled with zero = " << fa << "\r\n";

	fa.FillArithmetic(2.0F,1.3F);
	buffer << "   Filled arithmetic (2.0, 1.3) fa = " << fa << "\r\n";

	fb.FillGeometric(1.0F,1.1F);
	buffer << "     Filled geometric (1, *1.1) fb = " << fb << "\r\n";

	NumericArray<float> fc(size_t(11), default_size);
	fc = fa + fb; 
	buffer << "                 Addition: fa + fb = " << fc << "\r\n";

	fc = fa - fb;
	buffer << "              Subtraction: fa - fb = " << fc << "\r\n";

	fc = fa * fb;
	buffer << "           Multiplication: fa * fb = " << fc << "\r\n";

	fc = fa / fb;
	buffer << "                 Division: fa / fb = " << fc << "\r\n";

	fc  = fa;
	fc += fb;
	buffer << "      Shorthand Addition: fa += fb = " << fc << "\r\n";

	fc = fa;
	fc -= fb;
	buffer << "   Shorthand Subtraction: fa -= fb = " << fc << "\r\n";

	fc = fa;
	fc *= fb;
	buffer << "Shorthand Multiplication: fa *= fb = " << fc << "\r\n";

	fc = fa;
	fc /= fb;
	buffer << "      Shorthand Division: fa /= fb = " << fc << "\r\n";

	fa.FillGeometric(1701.0F,-.25F);
	buffer << "    Geometric test: (1701.0, -.25) = " << fa << "\r\n";

	fc = Abs(fa);
	buffer << "           absolute value (friend) = " << fc << "\r\n";

	fa.Abs();
	buffer << "           absolute value (member) = " << fa << "\r\n";

	fa = -fa;
	buffer << "                    unary negation = " << fa << "\r\n";

	NumericArray<float> fd(fa); fd += fb;
	buffer << "           concatenation (fa & fb) = " << fd << "\r\n";

	static const float x[] = {1.0F,1.0F,2.0F,3.0F,5.0F,8.0F,13.0F,21.0F,34.0F,55.0F,89.0F};
	NumericArray<float> fe(x, 11);
	buffer << "           construction from array = " << fe << "\r\n";

	fc = fa;
	NumericArray<Boolean> b1 = (fc == fa);

	fc[ 3]  = 0.0F;
	NumericArray<Boolean> b2 = fc == fa;

	fc[ 3]  = fa[3];
	fc[ 1]  = 0.0F;
	NumericArray<Boolean> b3 = fc == fa;

	fc[ 1]  = fa[1];
	fc[10]  = 0.0F;
	NumericArray<Boolean> b4 = fc == fa;

	buffer << "   equality tests (should be TFFF) = "
           << b1 << " "
           << b2 << " "
           << b3 << " "
           << b4 << "\r\n";
}

void ArrayTest5(ostream & buffer)
{
    buffer << "\r\nArrays: Integer Comparison Tests"
              "\r\n--------------------------------\r\n";

    static const int MaxVal = 4;

    Range r(-5,5);
    NumericArray<int> i1(size_t(11), default_size), i2(size_t(11), default_size);
    NumericArray<Boolean> res(size_t(11), default_size);

    // fill arrays with random numbers
    for (int i = 0; i <= 10; ++i)
    {
	i1[i] = RANDOM(MaxVal);
	i2[i] = RANDOM(MaxVal);
    }

    buffer << "Comp array i1 = " << i1 << "\r\n";
    buffer << "Comp array i2 = " << i2 << "\r\n";

    res = i1 < i2;
    buffer << "      i1 < i2 = " << res << "\r\n";

    res = i1 <=i2;
    buffer << "     i1 <= i2 = " << res << "\r\n";

    res = i1 == i2;
    buffer << "     i1 == i2 = " << res << "\r\n";

    res = i1 != i2;
    buffer << "     i1 != i2 = " << res << "\r\n";

    res = i1 >= i2;
    buffer << "     i1 >= i2 = " << res << "\r\n";

    res = i1 > i2;
    buffer << "      i1 > i2 = " << res << "\r\n";
}

void ArrayTest6(ostream & buffer)
{
    buffer << "\r\nArrays: Long logical operations"
              "\r\n--------------------------------\r\n";

    Range r(-5,5);
    IntegerArray<long>
    la(size_t(11), default_size),
    lb(size_t(11), default_size),
    lc(size_t(11), default_size);

    for (int i = 0; i <= 10; ++i)
    {
	la[i] = RANDOM(16);
	lb[i] = RANDOM(5) + 1;
    }

    buffer << "                   long array la = " << la << "\r\n";
    buffer << "                   long array lb = " << lb << "\r\n";

    lc = la % lb;
    buffer << "       binary modulus:   la % lb = " << lc << "\r\n";

    lc = la << lb;
    buffer << "    binary shift left:  la << lb = " << lc << "\r\n";

    lc = la >> lb;
    buffer << "   binary shift right:  la >> lb = " << lc << "\r\n";

    lc = la & lb;
    buffer << "           binary AND:   la & lb = " << lc << "\r\n";

    lc = la | lb;
    buffer << "            binary OR:   la | lb = " << lc << "\r\n";

    lc = la ^ lb;
    buffer << "           binary XOR:   la ^ lb = " << lc << "\r\n";

    lc  = la;
    lc %= lb;
    buffer << "    shorthand modulus:  la %= lb = " << lc << "\r\n";

    lc  = la;
    lc <<= lb;
    buffer << "shorthand shift  left: la <<= lb = " << lc << "\r\n";

    lc  = la;
    lc >>= lb;
    buffer << "shorthand shift right: la >>= lb = " << lc << "\r\n";

    lc  = la;
    lc &= lb;
    buffer << "        shorthand AND:  la &= lb = " << lc << "\r\n";

    lc  = la;
    lc |= lb;
    buffer << "        shorthand  OR:  la |= lb = " << lc << "\r\n";

    lc  = la;
    lc ^= lb;
    buffer << "        shorthand XOR:  la ^= lb = " << lc << "\r\n";

    lc = !la;
    buffer << "                  unary NOT: !la = " << lc << "\r\n";

    lc = ~la;
    buffer << "           unary complement: ~la = " << lc << "\r\n";
}

void ArrayTest7(ostream & buffer)
{
  buffer << "\r\nArrays: Function Application & Conversion Tests"
            "\r\n-----------------------------------------------\r\n";

  Range r(0,7);
  NumericArray<double> da(size_t(8), default_size);

  for (int i = 0; i < 8; ++ i)
    da[i] = (double)i * 3.1415927;

  buffer << "   double array da = " << da << "\r\n";

  da.ApplyUnary(sqrt);
  buffer << "square roots of da = " << da << "\r\n";

  // conversion tests
  DynArray<double> ad(da);
  buffer << "   create DynArray<double> from NumericArray<double> = " << ad << "\r\n";

  buffer << "     assign DynArray<double> to NumericArray<double> = " << da << "\r\n";
  ((DynArray<double>&)da) = ad;

  NumericArray<float> fa(size_t(8), default_size);
  fa.AssignArg(1.0F, fa.RunLength());
        
  #ifdef std_cplusplus
    da = Convert<float,double>(fa);
    buffer << "assign NumericArray<float> to NumericArray<double> = " << da << "\r\n";
  #endif
}

void ArrayTest9(ostream& buffer)
{
    buffer << "\r\nArrays: Statistics Tests #1"
              "\r\n---------------------------\r\n";

    StatArray<double> dsa(15, default_size);
    size_t i;
//    Index i(r);

    for (i = 0; i < 15; ++i)
      dsa[i] = (double)RANDOM(100);

    buffer << "stat array = " << dsa << "\r\n";

    double minimum = dsa.Min();
    double maximum = dsa.Max();
    double sum     = dsa.Sum();
    double prod    = dsa.Product();
    double mean    = dsa.Mean();
    double avgdev  = dsa.AvgDeviation();
    double stddev  = dsa.StdDeviation();
    double var     = dsa.Variance();
    double skew    = dsa.Skew();
    double kurt    = dsa.Kurt();

    buffer << "  sum            = " << sum     << "\r\n";
    buffer << "  prod           = " << prod    << "\r\n\r\n";

    buffer << "\r\nSingle function stats:\r\n";
    buffer << "  minimum        = " << minimum << "\r\n";
    buffer << "  maximum        = " << maximum << "\r\n";
    buffer << "  mean           = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
    buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    dsa.MinMax(minimum,maximum);
    dsa.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "\r\nMulti-function stats:\r\n";
    buffer << "minimum        = " << minimum << "\r\n";
    buffer << "maximum        = " << maximum << "\r\n";
    buffer << "mean           = " << mean    << "\r\n";
    buffer << "avg. deviation = " << avgdev  << "\r\n";
    buffer << "std. deviation = " << stddev  << "\r\n";
    buffer << "variance       = " << var     << "\r\n";
    buffer << "skewness       = " << skew    << "\r\n";
    buffer << "kurtosis       = " << kurt    << "\r\n\r\n";
}

void ArrayTest10(ostream& buffer)
{
    buffer << "\r\nArrays: Statistics Tests #2"
              "\r\n---------------------------\r\n";

    StatArray<double> ds(21, default_size);

    ds[ 1] = 1.0;
    ds[ 2] = 2.0;
    ds[ 3] = 2.0;
    ds[ 4] = 3.0;
    ds[ 5] = 2.0;
    ds[ 6] = 1.0;
    ds[ 7] = 3.0;
    ds[ 8] = 4.0;
    ds[ 9] = 5.0;
    ds[10] = 7.0;
    ds[11] = 6.0;
    ds[12] = 5.0;
    ds[13] = 6.0;
    ds[14] = 4.0;
    ds[15] = 1.0;
    ds[16] = 2.0;
	ds[17] = 3.0;
    ds[18] = 3.0;
    ds[19] = 2.0;
    ds[20] = 4.0;

    buffer << "array before: " << ds << "\r\n";
    buffer << "median          = " << ds.Median() << "\r\n";
    buffer << "median (sorted) = " << ds.MedianSort() << "\r\n";
    buffer << "array  after: " << ds << "\r\n";
}

void ArrayTest11(ostream& buffer)
{
    buffer << "\r\nArrays: Correlation Tests"
              "\r\n-------------------------\r\n";

//    Range r(11);
    StatArray<double> dsa1(11, default_size), dsa2(11, default_size);
    const double rad2deg = 1.0 / 57.29577951;
    double angle1 = 45.0 * rad2deg;
    double angle2 = 45.0 * rad2deg;

//    for (int i = r.GetMin(); i < r.GetMax(); ++i)
    for (int i = 1; i < 11; ++i)
        {
        dsa1[i] = angle1;
		dsa2[i] = angle2;
		angle1 += rad2deg;
		angle2 -= rad2deg;
		}

	double cor = dsa1.Correlation(dsa2);
    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa2 = " << dsa2 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	StatArray<double> dsa3(dsa1);
	cor = dsa1.Correlation(dsa3);
    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa3 = " << dsa3 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	dsa1.FillRandom(  0.0, 1.0);
	dsa2.FillRandom(-10.0,10.0);
	cor = dsa1.Correlation(dsa2);

    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa2 = " << dsa2 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	dsa1.AssignArg(1.0, dsa1.RunLength());
	dsa2.AssignArg(1.0, dsa2.RunLength());

	cor = dsa1.Correlation(dsa2);
	buffer << "correlation        (all ones) = " << cor << "\r\n";

	cor = dsa1.Correlation(dsa3);
	buffer << "correlation (ones and values) = " << cor << "\r\n";
}

void ArrayTest13(ostream & buffer)
{
    buffer << "\r\nStatistics: Coyotes, Jack Rabbits, and Temperature"
              "\r\n--------------------------------------------------\r\n\r\n";

    int  oprec = buffer.precision(4);
    long oflag = buffer.flags();
    buffer.setf(ios::fixed|ios::showpoint|ios::showpos);

    Range r(1971,1990);
    Index ind(r);
    StatArray<double>
    coyote(r.GetMagnitude(), default_size),
    jackrab(r.GetMagnitude(), default_size),
    rainfall(r.GetMagnitude(), default_size);

    coyote[(ind=1971)] =  97.0; jackrab[(ind=1971)] = 200.0; rainfall[(ind=1971)] =  3.0;
    coyote[(ind=1972)] =  86.0; jackrab[(ind=1972)] = 220.0; rainfall[(ind=1972)] = 11.0;
    coyote[(ind=1973)] =  87.0; jackrab[(ind=1973)] = 225.0; rainfall[(ind=1973)] =  4.0;
    coyote[(ind=1974)] =  73.0; jackrab[(ind=1974)] = 233.0; rainfall[(ind=1974)] =  2.0;
    coyote[(ind=1975)] =  70.0; jackrab[(ind=1975)] = 261.0; rainfall[(ind=1975)] =  1.0;
    coyote[(ind=1976)] =  81.0; jackrab[(ind=1976)] = 235.0; rainfall[(ind=1976)] =  4.0;
    coyote[(ind=1977)] =  67.0; jackrab[(ind=1977)] = 257.0; rainfall[(ind=1977)] =  5.0;
    coyote[(ind=1978)] =  75.0; jackrab[(ind=1978)] = 250.0; rainfall[(ind=1978)] =  3.0;
    coyote[(ind=1979)] =  78.0; jackrab[(ind=1979)] = 223.0; rainfall[(ind=1979)] =  3.0;
    coyote[(ind=1980)] =  93.0; jackrab[(ind=1980)] = 187.0; rainfall[(ind=1980)] =  4.0;
    coyote[(ind=1981)] = 101.0; jackrab[(ind=1981)] = 171.0; rainfall[(ind=1981)] =  6.0;
    coyote[(ind=1982)] =  93.0; jackrab[(ind=1982)] = 167.0; rainfall[(ind=1982)] =  2.0;
    coyote[(ind=1983)] =  96.0; jackrab[(ind=1983)] = 154.0; rainfall[(ind=1983)] =  3.0;
    coyote[(ind=1984)] =  82.0; jackrab[(ind=1984)] = 212.0; rainfall[(ind=1984)] =  2.0;
    coyote[(ind=1985)] =  70.0; jackrab[(ind=1985)] = 210.0; rainfall[(ind=1985)] =  4.0;
    coyote[(ind=1986)] =  76.0; jackrab[(ind=1986)] = 219.0; rainfall[(ind=1986)] =  7.0;
    coyote[(ind=1987)] =  50.0; jackrab[(ind=1987)] = 308.0; rainfall[(ind=1987)] = 10.0;
    coyote[(ind=1988)] =  69.0; jackrab[(ind=1988)] = 287.0; rainfall[(ind=1988)] =  1.0;
    coyote[(ind=1989)] =  80.0; jackrab[(ind=1989)] = 244.0; rainfall[(ind=1989)] =  2.0;
    coyote[(ind=1990)] =  81.0; jackrab[(ind=1990)] = 218.0; rainfall[(ind=1990)] =  1.0;

    // general statistics
    double mean, stddev, avgdev, var, skew, kurt;

    coyote.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "coyote stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	 buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    jackrab.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "jack rabbit stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    rainfall.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "rainfall stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	 buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    // correlate information
    double cor;

    cor = coyote.Correlation(jackrab);
    buffer << "correlation of  coyotes and  rabbits: " << cor << "\r\n";

    cor = jackrab.Correlation(coyote);
    buffer << "correlation of  rabbits and  coyotes: " << cor << "\r\n";

    cor = coyote.Correlation(rainfall);
    buffer << "correlation of  coyotes and rainfall: " << cor << "\r\n";

    cor = jackrab.Correlation(rainfall);
    buffer << "correlation of  rabbits and rainfall: " << cor << "\r\n\r\n";

    // looking for weird numbers
    buffer << "\r\nData dump with ZScores:\r\n"
           << "Year  Coyotes       Rabbits       Rainfall\r\n";

    int i;
    for (i = r.GetMin(); i < r.GetMax(); ++i)
        {
        ind = i;
        buffer << setw(4) << i << ": "
               << setw(4) <<   coyote[ind] << " (" << setw(6) <<   coyote.ZScore(ind) << ") "
               << setw(4) <<  jackrab[ind] << " (" << setw(6) <<  jackrab.ZScore(ind) << ") "
               << setw(4) << rainfall[ind] << " (" << setw(6) << rainfall.ZScore(ind) << ")\r\n";
        }

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
    buffer.flags((int)oflag);
#else
    buffer.flags((std::_Ios_Fmtflags)oflag);
#endif

    buffer.precision(oprec);
}

/****************************************************************************/
void checkDate(int year, int month, int day)
{
  try
  {
    Date(year, month, day);
    cout <<"Validity check FAILED" <<endl;    
  }
  catch (SciEngErr& x)
  {
    cout <<"Validity check PASSED" <<endl;
  }
}

/****************************************************************************/
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

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
    for (i = 0; i < 11; i++)
      da[i] = i;
#else
    for (i = 0; i < 11; i++)
      (da.Base())[i] = i;
#endif

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
/****************************************************************************/
void TestBuiltIn()
{
  int* val = new_int(-5);
  int* valarr = new_ints(5, -1, -2, -3, -4, -5);

  unsigned* uval = new_unsigned_int(5);
  unsigned* uvalarr = new_unsigned_ints(5, 1, 2, 3, 4, 5);

  char* ch = new_char('A');
  char* str = new_char_string("New ChrString");

  double* dval = new_double(3.1415927);

  int index;

  cout <<*val <<endl;
  for (index = 0; index < 5; index++)
    cout <<valarr[index];
  cout <<endl;
  cout <<*uval <<endl;
  for (index = 0; index < 5; index++)
    cout <<uvalarr[index];
  cout <<endl;

  cout <<*ch <<" " <<str <<endl;
  cout <<*dval <<endl;

  index = 1;
  Increment(index);
  cout <<"inc(1) == " <<index <<endl;
  Decrement(index);
  cout <<"dec(2) == " <<index <<endl;

  Delete(val);
  DeleteArray(valarr);
  Delete(uval);
  DeleteArray(uvalarr);
  Delete(ch);
  DeleteArray(str);
}

/****************************************************************************/
void TestForLoop()
{
  int cnt;

  // /* select sort loop */
  // int lim = 20;
  // for (int x = 0; x < lim - 1; ++x)
  //   for (int y = x + 1; y < lim; ++y)
  //	 /* Body of Loop should execute (19 * 10) / 2 = 190 times */
  int Lim_ = 20;
  int lp2startoff = 1;
  int lp1indexoff = -1;
  ForLpViewer<int> View_;
  ForLpViewer<int> View2_;

  ForLoop<int> Loop1_(0, Lim_ - 1, 1);
  Loop1_
    .SetIndexLimitPtr(&Lim_, &lp1indexoff)
    .AddLoopBody(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBody(Work, &View2_, ForLoopEnums::POST_LOOP);

  ForLoop<int> Loop2_(0, Lim_, 1);
  Loop2_
    .SetStartValuePtr(Loop1_.GetIndex(), &lp2startoff)
    .SetIndexLimitPtr(&Lim_)
    .AddLoopBody(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBody(Work, &View2_, ForLoopEnums::POST_LOOP);

  Loop1_.AddLoop(&Loop2_);

  for (cnt = 0; Loop1_.Run(); ++cnt);
  Loop1_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  Loop2_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  assert(cnt == 190);
  cout <<endl <<endl;

  // /* 4x4 matrix multiplication loop (incomplete) */
  // int lim = 4
  // for (int i = 0; i < lim; ++i)
  //   for (int j = 0; j < lim; ++j)
  //     for (int k = 0; k < lim; ++k)
  //       /* Body of Loop should execute 64 times */
  ForLoop<int> Loop3_;
  Lim_ = 4;
  Loop1_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop2_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop3_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_)
    .SetStepValue(1);
  Loop2_.AddLoop(&Loop3_);
  for (cnt = 0; Loop1_.Run(); ++cnt);
  cout <<cnt <<endl;
  assert(cnt == 64);

  // /* 8! (The hard & stupid way) */
  // int m = 0;
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         for (int g = 0; g < 4; ++g)
  //           for (int h = 0; h < 3; ++h)
  //             for (int i = 0; i < 2; ++i)
  //               ++m; /* final value should be 40320 */
  ForLoop<long> LoopArr_[7];
  int i; long Step_ = 1;
  for (i = 0, cnt = 8; cnt > 1; --cnt, ++i)
  {
    LoopArr_[i]
      .SetStartValue(0)
      .SetIndexLimit(cnt)
      .SetStepValue(Step_);

    if (i)
      LoopArr_[i - 1].AddLoop(&LoopArr_[i]);
  }
  long longcnt;
  for (longcnt = 0; LoopArr_[0].Run(); ++longcnt);
  cout <<longcnt <<endl;
  assert(longcnt == 40320);

  // /* Test standalone loop function   */
  // /* 8! / 4! = P(8,4) 		*/
  // int m = 0
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         ++m; /* final value should be 1680 */
  ForLpViewer<int> View3_;
  int Startv_[4] = { 0, 0, 0, 0 };
  int Endv_[4] = { 8, 7, 6, 5 };

  Loop1_
    .AddLoopBody(Work, &View3_, ForLoopEnums::PRE_LOOP);
  Loop1_.ForLoopFnc(Startv_, Endv_, 4);
  cout <<endl <<View3_.Cnt() <<endl;
}

/****************************************************************************/
void TestArgArray()
{
  // test basic workings
  Char c0, ca('a'), cb('b'), c3[3];
  ArgArray<Char> s1, s2(30), s3(4, c0);
  ArgArray<Char> s4(s3), s5(1, ca), s6(10, cb);
  const ArgArray<Char> s7(c3, 3);
  Boolean Valid_;
  size_t i;

  Char pc4[3];
  Char pc5[3];

  ArgArray<Char>::SetComparisonFunction(CompChar);
  assert(s1.RunLength() == 0);
  assert(s2.RunLength() == 0);
  assert(s3.RunLength() == 4 && s3 == "XXXX");
  assert(s4.RunLength() == 4 && s4 == "XXXX");
  assert(s5.RunLength() == 1 && s5[0] == ca);
  assert(s6.RunLength() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.RunLength() == 3 && s7 == "XXX");
  s1.GrowTo(2, c0); assert(s1 == "XX");
  s1.GrowTo(4, ca); assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignArg(cb); assert(s1 == "b");
  s1.AssignArg(cb, 5); assert(s1 == "bbbbb");
  s1.AssignArg(&cb), assert(s1 == "b");
  s1.AssignArg(c3, 3); assert(s1 == "XXX");

  // test appends
  s1 = s5, s1 += s5, assert(s1 == "aa");
  s1 += cb, assert(s1 == "aab");
  s1.Append(cb), assert(s1 == "aabb");
  s1.Append(cb, 5), assert(s1 == "aabbbbbbb");
  s1.Append(&ca), assert(s1 == "aabbbbbbba");
  s1.Append(c3, 3), assert(s1 == "aabbbbbbbaXXX");
  s1 = ((const ArgArray<Char>&)s4) + ((const ArgArray<Char>&)s5), assert(s1 == "XXXXa");
  s1 = ((const Char&)ca) + ((const ArgArray<Char>&)s5), assert(s1 == "aa");
  s1 = ((const ArgArray<Char>&)s4) + ((const Char&)cb), assert(s1 == "XXXXb");

  // test inserts, removes, etc.
  s1 = s4, s1.Insert(1, s5), assert(s1 == "XaXXX");
  s1.Insert(2, cb), assert(s1 == "XabXXX");
  s1.Insert(0, ca, 3), assert(s1 == "aaaXabXXX");
  s1.Insert(9, &ca), assert(s1 == "aaaXabXXXa");
  s1.Insert(1, c3, 3), assert(s1 == "aXXXaaXabXXXa");
  ArgArray<Char> s8(s1);
  s1.Remove(7), assert(s1 == "aXXXaaX");
  s1.Remove(2, 1), assert(s1 == "aXXaaX");
  s1.Remove(), assert(s1 == "");
  s6.Sub_Array(s1, 2);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.Sub_Array(s1, 3, 2), assert(s1 == "bb");
  s1.Swap(s3), assert(s1 == "XXXX" && s3 == "bb");

  // test access functions
  s1 = s7, assert(s1[0] == c0);
  s1[0] = ca, assert(s1[0] == ca);
  s1.Put_At(1, cb), assert(s1.Get_At(1, Valid_) == cb);

  s1.ToPtrArray(pc4);
  ArgArray<Char>* cs7p = (ArgArray<Char>*)s7.Clone();
  cs7p->ToPtrArray(pc5);
  assert(pc4[1] == cb && pc5[2] == c0);
  delete cs7p;

  // Test class hierarchy extensions
  // Test virtual object constructor
  ArgArray<Char>* ps1 = ArgArray<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  ps1->ToPtrArray(pc5);
  assert(*ps1 == pc5);

  // Test virtual assignment
  ArgArray<Char>* ps2 = ArgArray<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXXXXXaaaaab");

  ArgArray<int> s9;
  for (i = 0; i < 10; i++)
    s9[i] = i * 2;

  s9.ApplyUnary(Square);
#if (defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__))
  for (i = 0; i < 10; i++)
    assert(s9[i] == (i * 2) * (i * 2));
#endif

  cout <<s9 <<endl;
  ArgArray<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10[8] = Char('c');
  s10[10] = Char('c');
  SimpleArray<int> Result_ = s8.SeriesArrayCompare(s10);

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  delete ps1;
  delete ps2;

  cout <<"SUCCESS testing <ArgArray>" <<endl;
}

/****************************************************************************/
void TestPtrArray()
{
  // test basic workings
  static Char c0, ca('a'), cb('b'), c3[3];
  static Char* pc3[] = {&ca, &cb, &c0};
  PtrArray<Char> s1, s2(30), s3(4, &c0);
  PtrArray<Char> s4(s3), s5(1, &ca), s6(10, &cb);
  PtrArray<Char> s7(pc3, 3);
  size_t i;

  Char* pc4[3];
  Char* pc5[3];
  Boolean Valid_;

  PtrArray<Char>::SetComparisonFunction(CompChar);
  assert(s1.RunLength() == 0);
  assert(s2.RunLength() == 0);
  assert(s3.RunLength() == 4);
  assert(s4.RunLength() == 4);
  assert(s5.RunLength() == 1 && ((const Char*)s5[0]) == &ca);
  assert(s6.RunLength() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.RunLength() == 3 && s7 == "abX");
  s1.GrowTo(2, &c0), s1[0] = &c0, s1[1] = &c0, assert(s1 == "XX");
  s1.GrowTo(4, &ca); assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignArg(&cb); assert(s1 == "b");
  s1.AssignArg(&cb, 5); assert(s1 == "bbbbb");
  s1.AssignArg(pc3), assert(s1 == "a");
  s1.AssignArg(pc3, 3); assert(s1 == "abX");

  // test appends
  s1 = s5, s1 += s5, assert(s1 == "aa");
  s1 += &cb, assert(s1 == "aab");
  s1.Append(&cb), assert(s1 == "aabb");
  s1.Append(&cb, 5), assert(s1 == "aabbbbbbb");
  s1.Append(pc3), assert(s1 == "aabbbbbbba");
  s1.Append(pc3, 3), assert(s1 == "aabbbbbbbaabX");
  s1 = s6 + s7, assert(s1 == "bbbbbbbbbbabX");
  s1 = &ca + s7, assert(s1 == "aabX");
  s1 = s7 + &cb, assert(s1 == "abXb");

  // test inserts, removes, etc.
  s1 = s7, s1.Insert(1, s7), assert(s1 == "aabXbX");
  s1.Insert(2, &cb), assert(s1 == "aabbXbX");
  s1.Insert(0, &ca, 3), assert(s1 == "aaaaabbXbX");
  s1.Insert(10, pc3), assert(s1 == "aaaaabbXbXa");
  s1.Insert(1, pc3, 3), assert(s1 == "aabXaaaabbXbXa");
  PtrArray<Char> s8(s1);
  s1.Remove(7), assert(s1 == "aabXaaa");
  s1.Remove(2, 1), assert(s1 == "aaXaaa");
  s1.Remove(), assert(s1 == "");
  s6.Sub_Array(s1, 2);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.Sub_Array(s1, 3, 2), assert(s1 == "bb");
  s1.Swap(s3), assert(s1 == "XXXX" && s3 == "bb");

  // test access functions
  s1 = s7, assert(((const Char*)s1[0]) == &ca);
  s1[0] = &cb, assert(((const Char*)s1[0]) == &cb);
  s1.Put_At(1, &ca), assert(s1.Get_At(1, Valid_) == &ca);

  s1.ToPtrArray(pc4, 3);
  PtrArray<Char>* cs7p = (PtrArray<Char>*)s7.ShallowClone();
  cs7p->ToPtrArray(pc5);
  assert(pc4[1] == &ca && pc5[2] == &c0);
  delete cs7p;

  // Test class hierarchy extensions
  // Test virtual object constructor
  PtrArray<Char>* ps1 = PtrArray<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  ps1->ToPtrArray(pc5);
  assert(*ps1 == pc5);

  // Test virtual assignment
  PtrArray<Char>* ps2 = PtrArray<Char>::Make();
  assert(!(*ps2 == *ps1));
  NonConstAssign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXaaaaaaabbbb");

  PtrArray<int> s9;
  for (i = 0; i < 10; i++)
    s9[i].Assign(PtrArray<int>::MakeDataStore(new int(i * 2), TRUE));

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*(s9[i]) == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  PtrArray<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10[8].Assign(PtrArray<Char>::MakeDataStore(new Char('c'), TRUE));
  s10[10].Assign(PtrArray<Char>::MakeDataStore(new Char('c'), TRUE));
  SimpleArray<int> Result_ = s8.SeriesArrayCompare(s10);

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  delete ps1;
  delete ps2;

  // Testing auto-delete methods
  PtrArray<Char> sd1;
  sd1[0].Assign(PtrArray<Char>::MakeDataStore(new Char('X'), TRUE));
  sd1[1].Assign(PtrArray<Char>::MakeDataStore(new Char('Y'), TRUE));
  sd1[2].Assign(PtrArray<Char>::MakeDataStore(new Char('Z'), TRUE));
  assert(sd1 == "XYZ");
  sd1.Flush();
  assert(sd1 == "");

  cout <<"SUCCESS testing <PtrArray>" <<endl;
}

/****************************************************************************/
void TestAssoc()
{
  const Ulong tabsize = 2001;

  ChrString Key_("Hello World!");
  ChrString Data_("Guard");
  
  Association<ChrString, ChrString> Node_;

  Node_.SetData(&Data_);
  Node_.SetKey(&Key_);
  
  if (Node_.GetData() && Node_.GetKey())
    cout <<"Key = " <<*Node_.GetKey()
         <<", Value = " <<*Node_.GetData() <<endl;

  Node_.MakeIndex(tabsize);
  cout <<"Index = " <<Node_.GetIndex() <<endl;
  Node_.ProbeAhead(tabsize);
  cout <<"Index = " <<Node_.GetIndex()
       <<", Incr = " <<Node_.GetProbeIncr() <<endl;

  Association<ChrString, ChrString> Node1_;

  ChrString* Key3_ = ::Clone(Key_);
  ChrString Key4_ = *Key3_;
  Node1_.SetData(&Data_);
  Node1_.SetKey(Key3_);
  ::Delete(Key3_);

  Association<ChrString, ChrString>* NodePtr_ = (Association<ChrString, ChrString>*)Node1_.Clone();
  NodePtr_->MakeIndex(tabsize);
  cout <<"Index = " <<NodePtr_->GetIndex() <<endl;
  delete NodePtr_;

  int val = 115;
  ChrString Key2_ = ("This Is A Test Of The Association Class");
  Association<ChrString, int> hash1;

  hash1.SetData(&val);
  hash1.SetKey(&Key2_);
  
  if (hash1.GetData() && hash1.GetKey())
    cout <<"Key = " <<*hash1.GetKey()
         <<", Value = " <<*hash1.GetData() <<endl;

  hash1.MakeIndex(tabsize);
  cout <<"Index = " <<hash1.GetIndex() <<endl;
  hash1.ProbeAhead(tabsize);
  cout <<"Index = " <<hash1.GetIndex()
       <<", Incr = " <<hash1.GetProbeIncr() <<endl;
}

/****************************************************************************/
void TestBstTree()
{
   LoneString* StrPtr_;

   Bst<LoneString, LoneString>::SetComparisonFunction(CompLoneString);   
   Bst<LoneString, LoneString> dict;
   Bst<LoneString, LoneString> another;

   dict.Insert("715", "Nou");
   dict.Insert("720", "Vince");
   dict.Insert("30",  "Habib");
   dict.Insert("216", "Lorene");
   dict.Insert("316", "Pari");
   dict.Insert("740", "Michael");
   dict.Insert("730", "Pierre");
   dict.Insert("615", "Ardeshir");
   dict.Insert("615", "Ardeshir2");

   cout << "Finished Inserting:\n";

   dict.DumpNode(dict.Root(), cout);

   another = dict;
   another = another;
   another = dict;

   cout << "Finished assignment:\n";

   another.DumpNode(another.Root(), cout);

   dict.Del("216");
   dict.Del("316");
   dict.Del("740");
   dict.Del("999");
   dict.Del("715");
   dict.Del("740");

   cout << "Finished deleting:\n";

   dict.DumpNode(dict.Root(), cout);

   //  Note this does searching on the (another) tree without deletions.
   cout << "Key 715 - Value: " << ((StrPtr_ = another.SearchNode("715", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key  30 - Value: " << ((StrPtr_ = another.SearchNode("30", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 720 - Value: " << ((StrPtr_ = another.SearchNode("720", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 615 - Value: " << ((StrPtr_ = another.SearchNode("615", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 740 - Value: " << ((StrPtr_ = another.SearchNode("740", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 730 - Value: " << ((StrPtr_ = another.SearchNode("730", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 999 - Value: " << ((StrPtr_ = another.SearchNode("999", another.Root())) ? *StrPtr_:"(null)") << "\n\n";

   cout << "Finished searching\n\n";
   cout << "Test complete (for now)\n\n";
}

/****************************************************************************/
void TestAvlTree()
{
   LoneString* StrPtr_;
   LoneString TestStr1_ = "Habib";
   LoneString TestStr2_ = "Pierre";
   LoneString* TestStr3_ = new LoneString("Lorene");
   LoneString* TestStr4_ = new LoneString("Ardeshir2");

   AvlTree<LoneString, LoneString>::SetComparisonFunction(CompLoneString);
   AvlTree<LoneString, LoneString> dict;
   AvlTree<LoneString, LoneString> another;

   dict.Insert("715", "Nou");
   dict.Insert("720", "Vince");
   dict.Insert("30",  &TestStr1_);
   dict.Insert("216", TestStr3_, DataStorageAttributes::DESTROY);
   dict.Insert("316", "Pari");
   dict.Insert("740", "Michael");
   dict.Insert("730", &TestStr2_);
   dict.Insert("615", "Ardeshir");
   dict.Insert("615", TestStr4_, DataStorageAttributes::DESTROY);

   cout << "Finished inserting:\n\n";
   dict.DumpNode(dict.Root(), cout);

   another = dict;
   another = another;
   another = dict;

   cout << "Finished assignment:\n\n";
   another.DumpNode(another.Root(), cout);

   dict.Del("216");
   dict.Del("316");
   dict.Del("740");
   dict.Del("999");
   dict.Del("715");
   dict.Del("740");

   cout << "Finished deleting:\n\n";
   dict.DumpNode(dict.Root(), cout);

   //  Note this does searching on the (another) tree without deletions.
   cout << "Key 715 - Value: " << ((StrPtr_ = another.SearchNode("715", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key  30 - Value: " << ((StrPtr_ = another.SearchNode("30", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 720 - Value: " << ((StrPtr_ = another.SearchNode("720", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 615 - Value: " << ((StrPtr_ = another.SearchNode("615", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 740 - Value: " << ((StrPtr_ = another.SearchNode("740", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 730 - Value: " << ((StrPtr_ = another.SearchNode("730", another.Root())) ? *StrPtr_:"(null)") << '\n';
   assert(another.IsFound());   
   cout << "Key 999 - Value: " << ((StrPtr_ = another.SearchNode("999", another.Root())) ? *StrPtr_:"(null)") << "\n\n";
   assert(!another.IsFound());

   cout << "Finished searching\n\n";

   // Test binary IO methods
   AvlTree<LoneString, LoneString> destTree_;
   
   cout << "storing original tree to file\n\n";
   another.DumpNode(another.Root(), cout);   

   cout << "Reading stored tree from file\n\n";
   destTree_.DumpNode(destTree_.Root(), cout);

   cout << "Test complete (for now)\n\n";
}

/****************************************************************************/
void TestBaseptr()
{
  TestClass* tp1 = new TestClass(10);
  BaseCountedPtr<TestClass> Ptr1(tp1);
  BaseCountedPtr<TestClass>* Ptr2 = new BaseCountedPtr<TestClass>(Ptr1);

  assert(Ptr1->Get() == 10);
  assert((*Ptr2)->Get() == 10);
  Ptr1->Set((*Ptr2)->Get() * 2);
  assert((*Ptr2)->Get() == 20);
  assert(Ptr1->Get() == 20);
  Ptr1 = *Ptr2;
  assert(Ptr1->Get() == 20);
  delete Ptr2;
  assert(!Ptr1.IsNull() && Ptr1->Get() == 20);

  TestClass* tp3 = new TestClass(50);
  BaseCopiedPtr<TestClass> Ptr3(tp3);
  BaseCopiedPtr<TestClass>* Ptr4 = new BaseCopiedPtr<TestClass>(Ptr3);

  assert(Ptr3->Get() == 50);
  assert((*Ptr4)->Get() == 50);
  Ptr3->Set((*Ptr4)->Get() * 3);
  assert((*Ptr4)->Get() == 50);
  assert(Ptr3->Get() == 150);
  delete Ptr4;
  assert(!Ptr3.IsNull() && Ptr3->Get() == 150);
}

/****************************************************************************/
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
void TestBits()
{
  // testing basic workings of bist definition
  Bits<5> x50, x51(0xf), x52(ChrString("xx10101ab"), 2, 15);
  Bits<5> x53(x52);

  assert(x50.To_Ulong() == 0x00);
  assert(x51.To_Ulong() == 0x0f);
  assert(x52.To_Ulong() == 0x15);
  assert(x53.To_Ulong() == 0x15);

  // test arithmetic
  x50 |= x51, assert(x50.To_Ulong() == 0x0f);
  x50 ^= x52, assert(x50.To_Ulong() == 0x1a);
  x50 &= x51, assert(x50.To_Ulong() == 0x0a);
  x50 <<= 2, assert(x50.To_Ulong() == 0x08);
  x50 >>= 3, assert(x50.To_Ulong() == 0x01);
  x50.Set(2), assert(x50.To_Ulong() == 0x05);
  x50.Set(0, 0), assert(x50.To_Ulong() == 0x04);
  x50.Set(), assert(x50.To_Ulong() == 0x1f);
  x50.Reset(3), assert(x50.To_Ulong() == 0x17);
  x50.Reset(), assert(x50.To_Ulong() == 0x00);
  x50.Toggle(2), assert(x50.To_Ulong() == 0x04);
  x50.Toggle(), assert(x50.To_Ulong() == 0x1b);

  assert(x50.To_String() == ChrString("11011"));

  assert(x50.Count() == 4 && x52.Count() == 3);
  assert(x50.Length() == 5 && x51.Length() == 5);
  assert(x50 == x50 && x50 != x51);
  assert(x50.Test(1) && !x50.Test(2));
  assert(x50.Any() && !x50.None());
  x50.Reset(), assert(!x50.Any() && x50.None());

  // Test friend arithmetic
  x50 = x51;
  assert((x50 << 2).To_Ulong() == 0x1c
	&& (x50 >> 2).To_Ulong() == 0x03);
  assert((x50 & x52) == Bits<5>(0x05)
	&& (x50 & 0x05) == Bits<5>(0x05)
	&& (0x05 & x50) == Bits<5>(0x05));
  assert((x50 | x52) == Bits<5>(0x1f)
	&& (x50 | 0x15) == (0x15 | x50));
  assert((x50 ^ x52) == Bits<5>(0x1a)
	&& (x50 ^ 0x15) == Bits<5>(ChrString("11010"))
	&& (0x15 ^ x50) == (x50 ^0x15));

  // Test I/O
  std::istringstream ins("1 0101 11000");
  std::ostringstream outs;
  std::string string_;

  ins >>x50; assert(x50.To_Ulong() == 0x01);
  outs <<x50 <<' ';
  ins >>x50; assert(x50.To_Ulong() == 0x05);
  outs <<x50 <<' ';
  ins >>x50; assert(x50.To_Ulong() == 0x18);
  outs <<x50 <<ends;
  string_ = outs.str();
  assert(strcmp(string_.c_str(), "00001 00101 11000") == 0);

  // Test larger bits
  Bits<153> x153a, x153b;
  (x153a.Set(100) >>= 99) <<= 1;
  assert(x153a.To_Ulong() == 0x04);
  x153b.Set(121);
  assert(x153b.Any() && !x153b.Test(105)
	&& x153b.Test(121) && x153b.Count() == 1);
  assert(((x153a | x153b) ^ x153b) == x153a);

  // Testing null object
  Object* Ptr_ = &x153a.NULLOBJECT();
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;

  cout <<"SUCCESS testing <bits>" <<endl;
}
#endif
/****************************************************************************/
void TestBitstr()
{
  // Test basic workings of BitString definitions
  ChrString Str_("11110101");
  BitString x0, x1(0x1230, 1), x2(x1), x3(x1, 1, 8);
  BitString x4(Str_), x5(Str_, 4, 3);
  assert(x0.Length() == 0 && x0.To_String() == ChrString(""));
  assert(x1.Length() == 13 &&
	 x1.To_String() == ChrString("1001000110000"));
  assert(x2.Count() == 4 &&
	 x2.To_String() == ChrString("1001000110000"));
  assert(x3.To_String() == ChrString("00100011"));
  assert(x4.To_String() == ChrString("11110101"));
  assert(x5.To_String() == ChrString("010"));

  // Test assigns and appends
  x0 = x1, assert(x0.To_String() == ChrString("1001000110000"));
  x0.AssignSubstr(x3), assert(x0.To_String() == ChrString("00100011"));
  x0.AssignSubstr(x3, 1), assert(x0.To_String() == ChrString("0100011"));
  x0.AssignSubstr(x3, 2, 3), assert(x0.To_String() == ChrString("100"));
  x0 += x5, assert(x0.To_String() == ChrString("100010"));
  x0.Append(x5), assert(x0.To_String() == ChrString("100010010"));
  x0.Append(x5, 1), assert(x0.To_String() == ChrString("10001001010"));
  x0.Append(x5, 2, 3);
  assert(x0.To_String() == ChrString("100010010100"));
  x0.AssignSubstr(x0, 3, 4), assert(x0.To_String() == ChrString("0100"));
  assert((x1 + x1 + x1).To_String() ==
	 ChrString("1001000110000" "1001000110000" "1001000110000"));

  // Test Logic
  x0 = x3;
  x0 |= x5, assert(x0.To_String() == ChrString("01100011"));
  assert((x3 | x5).To_String() == ChrString("01100011"));
  assert((x3 | Str_).To_String() == ChrString("11110111"));
  x0 &= x4, assert(x0.To_String() == ChrString("01100001"));
  assert((x3 & x4).To_String() == ChrString("00100001"));
  x0 ^= x2, assert(x0.To_String() == ChrString("1111000010000"));
  assert((x4 ^ x5).To_String() == ChrString("10110101"));
  x0 <<= 2, assert(x0.To_String() == ChrString("1100001000000"));
  assert((x4 << 3).To_String() == ChrString("10101000"));
  x0 >>= 4, assert(x0.To_String() == ChrString("0000110000100"));
  assert((x4 >> 2).To_String() == ChrString("00111101"));
  assert((~x3).To_String() == ChrString("11011100"));
  assert(((x1 + x1 + x1) ^ x1) == (x1 + x1 + x1) >> 13);

  // test inserts, removes, replaces
  x0 = x5;
  x0.Insert(1, x5), assert(x0.To_String() == ChrString("001010"));
  x0.Insert(1, x5, 1), assert(x0.To_String() == ChrString("01001010"));
  x0.Insert(1, x5, 1, 1);
  assert(x0.To_String() == ChrString("011001010"));
  x0.Remove(7), assert(x0.To_String() == ChrString("0110010"));
  x0.Remove(2, 3), assert(x0.To_String() == ChrString("0110"));
  x0.Replace(2, 1, x5), assert(x0.To_String() == ChrString("010100"));
  x0.Replace(2, 1, x5, 1);
  assert(x0.To_String() == ChrString("0110100"));
  x0.Replace(2, 3, x5, 1, 1);
  assert(x0.To_String() == ChrString("01100"));
  x0.Remove(), assert(x0.To_String() == ChrString(""));

  // Test bit twiddling
  x0 = x5;
  x0.Set(2), assert(x0.To_String() == ChrString("011"));
  x0.Set(1, 0), assert(x0.To_String() == ChrString("001"));
  x0.Set(), assert(x0.To_String() == ChrString("111"));
  x0.Reset(0), assert(x0.To_String() == ChrString("011"));
  x0.Reset(), assert(x0.To_String() == ChrString("000"));
  x0.Toggle(2), assert(x0.To_String() == ChrString("001"));
  x0.Toggle(), assert(x0.To_String() == ChrString("110"));
  assert(x0.Trim() == 2 && x0.To_String() == ChrString("11"));
  assert(x0.Resize(4) == 2 && x0.To_String() == ChrString("1100"));
  assert(x0.Resize(3) == 4 && x0.To_String() == ChrString("110"));
  assert(x0.Resize(4, 1) == 3 && x0.To_String() == ChrString("1101"));

  // Test finds
  x0 = x3;
  assert(x0.To_String() == ChrString("00100011"));
  assert(x0.Find(0) == 0 && x0.Find(1) == 2);
  assert(x0.Find(0, 2) == 3 && x0.Find(1, 3) == 6);
  assert(x0.Find(0, 2, 1) == SIZET_MAX && x0.Find(1, 1, 2) == 2);

  // Test tests
  x0 = x5;
  assert(x0.Any() && !x0.None());
  assert(!x0.Test(0) && x0.Test(1));
  assert(x0 == x5 && x0 != x3);
  x0.Reset(), assert(!x0.Any() && x0.None());
  x0.Resize(145), x0.Toggle(101);
  assert(!x0.Test(100) && x0.Test(101));

  Object* Ptr_ = &x0.NULLOBJECT();
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;
  cout <<"SUCCESS testing <BitString>" <<endl;
}

/****************************************************************************/
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
void TestBitvect()
{
  Ulong Value_;
  BitVector pv_(20);
  BitVector pv2_(20);

  // testing basic workings of bist definition
  BitVector x50(5), x51(5), x52(5);
  x51.FromIntValue(0x0f);
  x52.FromString("10101");  
  BitVector x53(x52);
  Boolean Conv_;
  char* Str_;

  assert(x50.ToIntValue(Conv_) == 0x00);
  assert(x51.ToIntValue(Conv_) == 0x0f);
  assert(x52.ToIntValue(Conv_) == 0x15);
  assert(x53.ToIntValue(Conv_) == 0x15);

  // test arithmetic
  x50 |= x51, assert(x50.ToIntValue(Conv_) == 0x0f);
  x50 ^= x52, assert(x50.ToIntValue(Conv_) == 0x1a);
  x50 &= x51, assert(x50.ToIntValue(Conv_) == 0x0a);
  x50 <<= 2;
  Str_ = x50.ToString();
  assert(!strcmp(Str_, "101000"));
  ::Delete(Str_);
  x50 >>= 3, assert(x50.ToIntValue(Conv_) == (0x0a >> 1));  
  x50[2] = 1; assert(x50.ToIntValue(Conv_) == 0x05);
  x50[0] = 0; assert(x50.ToIntValue(Conv_) == 0x04);

  x50.Resize(5);
  x50.SetAllBits(), assert(x50.ToIntValue(Conv_) == 0x1f);
  x50[3] = 0; assert(x50.ToIntValue(Conv_) == 0x17);
  x50.UnSetAllBits(), assert(x50.ToIntValue(Conv_) == 0x00);
  x50.ToggleBit(2), assert(x50.ToIntValue(Conv_) == 0x04);
  x50.ToggleAllBits(), assert(x50.ToIntValue(Conv_) == 0x1b);

  Str_ = x50.ToString();
  assert(!strcmp(Str_, "11011"));
  ::Delete(Str_);
  assert(x50.NumBitsSet() == 4 && x52.NumBitsSet() == 3);
  assert(x50.GetLength() == 5 && x51.GetLength() == 5);
  assert(x50 == x50 && x50 != x51);
  assert(x50[1] && !x50[2]);
  assert(x50.Any() && !x50.None());
  x50.UnSetAllBits(), assert(!x50.Any() && x50.None());

  // Test friend arithmetic
  x50 = x51;
  assert((x50 << 2).ToIntValue(Conv_) == 0x3c
	&& (x50 >> 2).ToIntValue(Conv_) == 0x03);
  assert((x50 & x52) == BitVector(5, NULL, 0x05)
	&& (x50 & 0x05) == BitVector(5, NULL, 0x05)
	&& (0x05 & x50) == BitVector(5, NULL, 0x05));
  assert((x50 | x52) == BitVector(5, NULL, 0x1f)
	&& (x50 | 0x15) == (0x15 | x50));
  assert((x50 ^ x52) == BitVector(5, NULL, 0x1a)
	&& (x50 ^ 0x15) == BitVector(5, "11010")
	&& (0x15 ^ x50) == (x50 ^0x15));

  // Test I/O
  std::istringstream ins("1 0101 11000");
  std::ostringstream outs;
  std::string string_;

  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x01);
  outs <<x50 <<" ";
  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x05);
  outs <<x50 <<" ";
  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x18);
  outs <<x50 <<ends;
  string_ = outs.str();
  assert(strcmp(string_.c_str(), "1 101 11000") == 0);

  // Test larger bits
  BitVector x153a(153), x153b(153);
  (x153a.SetBit(100) >>= 99) <<= 1;
  assert(x153a.ToIntValue(Conv_) == 0x04);
  x153b.SetBit(121);
  assert(x153b.Any() && !x153b.GetBit(105)
	&& x153b.GetBit(121) && x153b.NumBitsSet() == 1);
  assert(((x153a | x153b) ^ x153b) == x153a);

  cout <<"SUCCESS testing <bitvect>" <<endl;
}
#endif
/****************************************************************************/
void TestBltinCmp()
{
  cout <<"int series -- ";
  intSeriesCompare IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 51), 3);
  IntCmp_.FindQuotient();
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 1);
  IntCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MIN, new_ints(3, INT_MAX - 100, 50, 51), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == INT_MIN && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 50), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 0);
  IntCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MIN, new_ints(3, INT_MAX - 100, 50, 50), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == INT_MIN && IntCmp_.Quotient() == 0 && IntCmp_.Remainder() == INT_MAX);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 49), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() > 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 0 && IntCmp_.Remainder() == INT_MAX - 1);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 1, 1), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(6, 1, 2, 3, 4, -5, -5), 6);
  IntCmp_.FindQuotient();
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 1, -1), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 0, 0), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;

  cout <<"Uint series -- ";
  UintSeriesCompare UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 51), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == 1 && UIntCmp_.Remainder() == 1);
  UIntCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 51), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == -1 && UIntCmp_.Remainder() == Uint(-1));
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(INT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 50), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == INT_MAX && UIntCmp_.Quotient() == 2 && UIntCmp_.Remainder() == 1);
  UIntCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 50), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == -1 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(INT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 49), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == INT_MAX && UIntCmp_.Quotient() == 2 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(0, new_unsigned_ints(2, 1, 1), 2);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(21, new_unsigned_ints(6, 1, 2, 3, 4, 5, 6), 6);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() == 0 && UIntCmp_.Unit() == 21 && UIntCmp_.Quotient() == 1 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::ProductOfSeriesCmp(8193, new_unsigned_ints(3, 64, 32, 4), 3);
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == 8193);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(0, new_unsigned_ints(2, 0, 0), 2);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() == 0 && UIntCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;

  cout <<"long series -- ";
  longSeriesCompare LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 51L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 1);
  LongCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MIN, new_longs(3, LONG_MAX - 100, 50L, 51L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == LONG_MIN && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 50L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 0);
  LongCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MIN, new_longs(3, LONG_MAX - 100, 50L, 50L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == LONG_MIN && LongCmp_.Quotient() == 0 && LongCmp_.Remainder() == LONG_MAX);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 49L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() > 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 0 && LongCmp_.Remainder() == LONG_MAX - 1);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 20L, 20L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(6, 1L, 2L, 3L, 4L, -5L, -5L), 6);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 1L, -1L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 0L, 0L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;

  cout <<"Ulong series -- ";
  UlongSeriesCompare ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 51UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == 1 && ULongCmp_.Remainder() == 1);
  ULongCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 51UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() > 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == -1 && ULongCmp_.Remainder() == Ulong(-1));
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 50UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == LONG_MAX && ULongCmp_.Quotient() == 2 && ULongCmp_.Remainder() == 1);
  ULongCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 50UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() > 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == -1 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 49UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == LONG_MAX && ULongCmp_.Quotient() == 2 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(0, new_unsigned_longs(2, 1UL, 1UL), 2);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(21, new_unsigned_longs(6, 1UL, 2UL, 3UL, 4UL, 5UL, 6UL), 6);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 21 && ULongCmp_.Quotient() == 1 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::ProductOfSeriesCmp(134217728UL, new_unsigned_longs(6, 4UL, 8UL, 16UL, 32UL, 64UL, 128UL), 6);
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 134217728UL);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(0, new_unsigned_longs(2, 0UL, 0UL), 2);
  ULongCmp_.FindQuotient();    
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
}

/****************************************************************************/
void TestByteStr()
{
  ByteString Str1_;
  ByteString Str2_("Hello World", 11);
  ByteString Str3_("GoodBye World", 8);
  ByteString Str4_('x', 5);
  ByteString Str5_(Str3_);
  ConstByteStringPtr StrPtr1a_(Str1_);
  ConstByteStringPtr StrPtr2_(Str2_);
  ConstByteStringPtr StrPtr2a_(Str2_);

  ByteStringPtr StrPtr6_;
  ByteStringPtr StrPtr7_;
  ByteStringPtr StrPtr8_;
  ByteString* Str6_ = ByteString::Make();
  ByteString* Str7_ = ByteString::Make("Hello World", 11);
  ByteString* Str8_ = ByteString::Make("GoodBye World", 10);
  ByteString* Str9_ = ByteString::Make('x', 5);
  ByteString* Str10_ = ByteString::Make(*Str8_);
  StrPtr6_ = *Str6_;
  StrPtr7_ = *Str7_;
  StrPtr8_ = *Str8_;
  ByteStringPtr StrPtr9_(*Str9_);
  ByteStringPtr StrPtr10_(*Str10_);

  // testing indexing
  Subscript Lp_; Boolean Ok_; Boolean LpOk_;
  for (Lp_ = 0; (LpOk_ = Str2_[Lp_].valid()); ++Lp_)
  {
    Ok_ =
	Lp_ == 0 ? Str2_[Lp_] == 'H':
	Lp_ == 1 ? Str2_[Lp_] == 'e':
	Lp_ == 2 ||
	Lp_ == 3 ||
	Lp_ == 9 ? Str2_[Lp_] == 'l':
	Lp_ == 4 ||
	Lp_ == 7 ? Str2_[Lp_] == 'o':
	Lp_ == 5 ? Str2_[Lp_] == ' ':
	Lp_ == 6 ? Str2_[Lp_] == 'W':
	Lp_ == 8 ? Str2_[Lp_] == 'r':
	Lp_ == 10 ? Str2_[Lp_] == 'd':0;
    assert(Ok_);
  }
  assert(Lp_ == 11);
  assert(!LpOk_);
  assert(!Str2_.IndexInRange(Lp_));
  assert(Str2_.RestrictIndex(Lp_) == 10);

  // testing assignment, case conversion, justification
  Str1_ = Str2_;
  assert(StrPtr1a_.IsOld());
  Str2_.ToUpper();
  StrPtr7_->ToLower();
  Str3_.RightJustify(20);
  StrPtr8_ -= ByteString("Wo", 2);
  StrPtr8_->Delete(0, 4);
  Str8_->LeftJustify(20);
  assert(StrPtr8_.IsOld());
  StrPtr8_ = *Str8_;
  assert(!StrPtr8_.IsOld());
  Str4_.CenterJustify(20);

  // Testing duplication, insertion
  Str1_.Duplicate(3);
  *Str6_ = *Str7_;
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());
  Str6_->Insert(3, ByteString("Inserted", 8));
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());

  assert(Str1_ == "Hello WorldHello WorldHello WorldHello World");
  assert(StrPtr2_.EnclosedObject() == "HELLO WORLD");
  assert(Str3_ == "            GoodBye ");
  assert(Str4_ == "       xxxxx        ");
  assert(Str5_ == "GoodBye ");
  assert(*Str6_ == "helInsertedlo world");
  assert(*Str7_ == "hello world");
  assert(StrPtr8_.EnclosedObject() == "Bye                 ");
  assert(StrPtr9_.EnclosedObject() == "xxxxx");
  assert(*Str10_ == "GoodBye Wo");

  // testing string tokenizing
  ByteString Buffer_;
  ByteStringToken Tok_(Str1_, 0, " ", " ");
  Tok_.FindToken(Buffer_);
  assert(Buffer_ == "Hello");
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(Buffer_ == "WorldHello");
  }
  Tok_.FindToken(Buffer_);
  assert(Buffer_ == "World");

  // testing deletion, overwriting, replacement
  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, ByteString("Bye  ", 5));
  assert(!StrPtr7_.IsOld());
  Str1_.Replace(0, ByteString("ll", 2), ByteString("nn", 2));

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  // testing string searches
  ConstByteStringPtr StrPtr1_(Str1_);
  Fallible<Subscript> Fall1_ = StrPtr1_->find(ByteString("Bye", 3));
  Fallible<Subscript> Fall2_ = StrPtr1_->findlast(ByteString("Good", 4));
  Subscript Pos3_ = StrPtr1_->find(ByteString("World", 5));
  Subscript Pos4_ = StrPtr1_->findlast(ByteString("Henno", 5));
  Subscript Pos5_ = StrPtr1_->findnext(Pos3_, ByteString("World", 5));
  Subscript Pos6_ = StrPtr1_->findprevious(Pos4_, ByteString("Henno", 5));

  // testing BASIC emulation -- Left, Mid, Right methods
  ByteString Str11_ = Str3_.Right(5);
  ByteString Str12_ = Str8_->Left(5);
  ByteString Str13_ = Str4_.Mid(10, 10);

  assert((int)Fall1_.valid() == 0);
  assert((int)Fall2_.valid() == 0);
  assert(Pos3_ == 6);
  assert(Pos4_ == 33);
  assert(Pos5_ == 17);
  assert(Pos6_ == 22);
  assert(Str11_ == "dBye ");
  assert(Str12_ == "Bye  ");
  assert(Str13_ == "xx        ");

  // testing deletion
  ByteString Str14_("JoKCsephKCWong", 14);
  Str14_.Delete(ByteString("KC", 2));
  assert(Str14_ == "JoKCsephWong");
  ByteString Str15_ = Delete(Str14_, ByteString("KC", 2));
  assert(Str15_ == "JosephWong");

  // testing addition
  Str15_ += ByteString(" This is a test of appending", 28);
  assert(Str15_ == "JosephWong This is a test of appending");

  // testing searches again
  Pos3_ = (Subscript)Str15_.find('s');
  Pos4_ = (Subscript)Str15_.findlast('s');
  Pos5_ = (Subscript)Str15_.findnext(Pos3_, 's');
  Pos6_ = (Subscript)Str15_.findprevious(Pos4_, 's');
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  // testing appending
  Str15_ -= ByteString(" This is a test of appending", 28);
  assert(Str15_ == "JosephWong");
  assert((Str15_ + ByteString(" This is a test of appending", 28)) == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  // brk, span and cspan methods
  Fall1_ = Str15_.brk(ByteString("eph", 3));
  assert(Fall1_.valid());
  Pos3_ = (Subscript)Fall1_;
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span(ByteString("toseJy", 6));
  Pos5_ = Str15_.cspan(ByteString("Wpht", 4));

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  // testing null object return method
  Object* Ptr_ = &Str6_->NULLOBJECT();
  assert(Ptr_->IsNullObject());

  // testing data sharing, memory management
  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

  cout <<endl <<"SUCCESS Testing ByteString" <<endl;
}

/****************************************************************************/
void TestChar()
{
  ChrString Str_;

  // Testing to string conversion
  Character si('f');
  Str_ = si.ToString(si);
  assert(!strcmp(Str_.c_str(), "f"));

  // Testing to lower case character conversion
  Character si2('F');
  si2 = si2.ToLowerCase(si2);

  // Testing to digit to character conversion constructor
  Character si3(0xA, 16);
  Str_ = si3.ToString(si3);
  assert(!strcmp(Str_.c_str(), "A"));

  // Testing relational operators
  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  // Testing space character validation method
  si2 = " ";
  assert(si2 == ' ');
  assert(si2.IsSpace(si2));

  // Testing Base-N digit character validation method
  si3.SetData('f');
  assert(si3.GetData() == 'f'); assert(si3.IsBaseDigit(si3, 16));
  si3.SetData('9');
  assert(si3.GetData() == '9'); assert(si3.IsBaseDigit(si3, 10));

  // Testing to upper case conversion
  si = si.ToUpperCase(si);
  assert(si == 'F');

  // Testing to digit conversion
  int Val_ = Character::ToDigit(si3);
  assert(Val_ == 9);

  // Testing from digit to character conversion and
  // from string to character conversion
  assert(Character::FromDigit(Val_) == '9');
  assert(Character::StaticFromString("Z") == 'Z');

  Object* Ptr_ = &si.NULLOBJECT();
  assert(Ptr_->IsNullObject());

  // Testing bounds checking
  Character chk1 = 127;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 127;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 16;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 16;
  chk1 >>= 2;
  assert(CharType(chk1) == 4);
  chk1 <<= 4;
  assert(CharType(chk1) == 64);
  chk1 = chk1 | Character(0x0F);
  assert(CharType(chk1) == 79);
  chk1 = chk1 / Character(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  Character* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new Character(i+1);
  List_[10] = NULL;

  Character ZeroChar(0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<Character> Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing Char" <<endl;
}

/****************************************************************************/
void TestCompare()
{
  Dumstruct dum1;
  Dumstruct dum2;

  Somestruct some1;
  Somestruct some2;
  some1.Set(5);
  some2.Set(5);

  int v1 = 1, v4 = 4, v7 = 7;
  int a1 = 1, a4 = 4, a7 = 7;

  PtrEqual<int> eqcmp;
  PtrRelation<int> relcmp(SortUsingUserDefinedFunctor<int>::Trait(SortUsingBuiltInOperators<int>::Trait()));

  eqcmp(&v1, &a1);
  assert((BOOLTYPE)eqcmp);

  relcmp.Relation(RelationalEnum::LESS_THAN);
  assert((BOOLTYPE)relcmp(&v1, &a4));
  assert(!relcmp(&v4, &a1));
  relcmp.Relation(RelationalEnum::GREATER_THAN);
  assert((BOOLTYPE)relcmp(&v7, &a4));
  assert(!relcmp(&v4, &a7));
  relcmp.Relation(RelationalEnum::EQUAL);
  assert((BOOLTYPE)relcmp(&v7, &a7));
  assert(!relcmp(&v1, &a7));

  assert((BOOLTYPE)relcmp.InRange(&a4, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!relcmp.XInRange(&a1, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert((BOOLTYPE)relcmp.InRange(&a1, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!PtrRelation<int>::InRange(&a7, &v1, &v4, SortUsingBuiltInOperators<int>::Trait()));

  PtrRelation<Dumstruct> dumrelcmp(SortUsingTemplateFunctions<Dumstruct>::Trait());
  assert(!dumrelcmp(&dum1, &dum2));

  PtrEqual<Somestruct> somerelcmp(MatchUsingTemplateFunctions<Somestruct>::Trait());
  assert((BOOLTYPE)somerelcmp(&some1, &some2));
  some1.Set(0);
  assert(!somerelcmp(&some1, &some2));

  cout <<"SUCCESS Testing PtrEqual, PtrRelation classes" <<endl;
}

/****************************************************************************/
void TestComplex()
{
  ComplexNumber<double> Cn0;
  ComplexNumber<double> Cn1(1, -1);
  ComplexNumber<double> Cn2(4, 7);
  ComplexNumber<double> Cn3(-1, 3);
  ComplexNumber<double> Cn4(2, -5);
  ComplexNumber<double> Cn5(2, 5);

  Cn0 = Cn1 + Cn2;
  assert(Cn0.RealVal() == 5);
  assert(Cn0.ImagVal() == 6);  
  Cn0 = Cn1 - Cn2;
  assert(Cn0.RealVal() == -3);
  assert(Cn0.ImagVal() == -8);
  
  Cn0 = Cn3 * Cn4;
  assert(Cn0.RealVal() == 13);
  assert(Cn0.ImagVal() == 11);
  Cn0 = Cn3 / Cn5;
  assert(Cn0.RealVal() == 13.0/29);
  assert(Cn0.ImagVal() == 11.0/29);

///
  ChrString Str_;

  ComplexNumber<double> si(315, 247);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315 + 247i", 10));

  ComplexNumber<double> si2(315, 247);
  ComplexNumber<double> si3(-210, 171);
  Str_ = si3.ToString();
  assert(!strncmp(Str_.c_str(), "-210 + 171i", 11));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si3 = si;
  ComplexNumber<double> si4;
  si4 = si3 * 1000;  
  assert(si4.RealVal() == 315000);
  assert(si4.ImagVal() == 247000);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  ChrString strobj_ = "568 + 123i";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;
  si2 -= 200;
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;
  si2 /= 2;
  cout <<si2 <<endl;

  TestComplex(cout);
  cout <<endl <<"SUCCESS Testing <ComplexNumber>" <<endl;
}

/****************************************************************************/
void TestDatastor()
{
  Sample* spdata = new Sample(4, 16);
  Sample* spdata2 = new Sample(999, 555);
  spdata2->SetOwner("spdata2");
  Sample* spdata3 = new Sample(4, 16);
  
  int* intdata = new int(10);
  int* intdata2 = new int(24);
  int intval = 56;
  Sample spobject(32, 512);
  spobject.SetOwner("spobject");

  // Testing const DataStorePtr objects as class members
  SimpleContainer<Sample> cont1_(spdata3, DataStorageAttributes::DESTROY);
  SimpleContainer<Sample> cont2_;
  const SimpleContainer<Sample> cont3_(cont1_);
  const SimpleContainer<Sample> cont4_(spdata3, DataStorageAttributes::DEEP_COPY);
  const SimpleContainer<Sample> cont5_(cont4_);
  SimpleContainer<Sample> cont6_(cont4_);
  cont2_ = cont1_;
  cont6_ = cont5_;      // note destructor message "owned by cont6"

  // Testing different storage schemes for each DataStorePtr object
  DataStorePtr<Sample> dsp1_(spdata2, DataStorageAttributes::DEEP_COPY);
  dsp1_.Pointee()->SetOwner("dsp1_");
  DataStorePtr<Sample> dsp2_(spdata, DataStorageAttributes::DESTROY);
  dsp2_.Pointee()->SetOwner("dsp2_");  
  DataStorePtr<int>* dsp3_ = new DataStorePtr<int>;
  DataStorePtr<Sample>* dsp4_ = new DataStorePtr<Sample>;
  DataStorePtr<int>* dsp11_ = new DataStorePtr<int>(intval);
  DataStorePtr<Sample>* dsp12_ = new DataStorePtr<Sample>;

  DataStorePtr<int> dsp21_(*dsp11_);
  DataStorePtr<Sample> dsp22_;

  dsp12_->AssignObject(spobject);
  dsp12_->Pointee()->SetOwner("dsp12_");
  dsp3_->AssignPtr(intdata2, 0);
  *dsp4_ = dsp2_;
  DataStorePtr<Sample>* dsp5_ = new DataStorePtr<Sample>(dsp1_);
  dsp5_->Pointee()->SetOwner("dsp5_");
  DataStorePtr<int>* dsp6_ = dsp3_->Clone();
  dsp22_ = *dsp12_;
  dsp22_.Pointee()->SetOwner("dsp22_");

  // testing if values and addresses are consistent
  // for different storage schemes
  assert(dsp1_.Pointee()->Getx() == 999 && dsp1_.Pointee()->Gety() == 555);
  assert(dsp2_.Pointee()->Getx() == 4 && dsp2_.Pointee()->Gety() == 16);
  assert(**dsp3_ == 24);
  assert(dsp4_->Pointee()->Getx() == 4 && dsp4_->Pointee()->Gety() == 16);
  assert(dsp5_->Pointee()->Getx() == 999 && dsp5_->Pointee()->Gety() == 555);
  assert(**dsp6_ == 24);
  assert(dsp2_.Pointee() == dsp4_->Pointee());
  assert(dsp5_->Pointee() != dsp1_.Pointee());
  assert(dsp6_->Pointee() == dsp3_->Pointee());
  
  assert(dsp11_->Pointee() != dsp21_.Pointee());
  assert(*dsp11_->Pointee() == *dsp21_.Pointee());
  assert(dsp12_->Pointee() != dsp22_.Pointee());
  assert(dsp12_->Pointee()->Getx() == 32 && dsp12_->Pointee()->Gety() == 512);
  assert(dsp22_.Pointee()->Getx() == 32 && dsp22_.Pointee()->Gety() == 512);

  // testing if values and addresses are consistent after destruction
  // of copied or shared pointers
  dsp2_.Destroy();       // note delayed destructor message
  delete dsp3_;
  delete dsp5_;          // note destructor message "owned by dsp5"
  delete dsp11_;
  delete dsp12_;         // note destructor message "owned by dsp12"

  assert(dsp1_.Pointee()->Getx() == 999 && dsp1_.Pointee()->Gety() == 555);
  assert(dsp4_->Pointee()->Getx() == 4 && dsp4_->Pointee()->Gety() == 16);
  assert(**dsp6_ == 24);
  assert(*dsp21_ == 56);
  assert(dsp22_.Pointee()->Getx() == 32 && dsp22_.Pointee()->Gety() == 512);

  delete dsp4_;          // note destructor message "owned by dsp2"
  delete spdata2;        // note destructor message "owned by spdata2"

  cout <<"SUCCESS Testing DataStorePtr classes" <<endl;
}

/****************************************************************************/
void TestDeque()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  Deque<Char> s1, s2, s3;
  size_t i;
  Char* pc5 = &c0;

  Deque<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Push(&sNode1_, ListRelation::HEAD);
  for (i = 0; i < 3; i++)
    s3.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
    
  Deque<Char> s4(s3);
  Deque<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Push(&sNode2_, ListRelation::HEAD);
  s6.Push(&sNode3_, ListRelation::HEAD);
  for (i = 0; i < 9; i++)  
    s6.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);

  Deque<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.Push(&sNode4_, ListRelation::HEAD);
  for (i = 1; i < 3; i++)
    s7.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XX");
  s1.Push(SingleListNode<Char>::Make(ca), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(ca), ListRelation::HEAD);
  assert(s1 == "aaXX");
  s1.ShrinkTo(3, ListRelation::HEAD); assert(s1 == "aXX");

  // test assigns
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;  
  s1.Push(Deque<Char>::Make(s5), ListRelation::HEAD); assert(s1 == "aa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "baa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "bbaa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);  
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);  
  assert(s1 == "bbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::HEAD);
  assert(s1 == "abbbbbbbaa");

  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);

  s4 = s1;

  // Test removes -- head end
  SingleListNode<Char>* Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXabbbbbbbaa" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == c0);
  assert(s1 == "Xabbbbbbbaa" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == c0);
  assert(s1 == "abbbbbbbaa" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == ca);
  assert(s1 == "bbbbbbbaa" && s1.Size() == 9);
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == cb);
  ::Delete(Item_);
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == cb);
  ::Delete(Item_);  
  assert(s1 == "bbbbbaa" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem(ListRelation::HEAD);
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "aa" && s1.Size() == 2);  
  Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "a" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == ca);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test removes -- tail end
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXXabbbbbbba" && s1.Size() == 12);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::TAIL);
  assert(*Item_ == ca);
  assert(s1 == "XXXabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXabbbbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {
    Item_ = s1.PopItem(ListRelation::TAIL);
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "XXXa" && s1.Size() == 4);
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXX" && s1.Size() == 3);
  delete Node_;
  for (i = 0; i < 3; i++)
  {
    Item_ = s1.PopItem(ListRelation::TAIL);
    assert(*Item_ == c0);
    ::Delete(Item_);
  }
  assert(s1.Size() == 0);  
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  Deque<Char>* ps1 = Deque<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Deque<Char>* ps2 = Deque<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode7_(c3.Set(x));  
  s1.Push(&sNode7_, ListRelation::TAIL);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode8_(c3.Set(x));  
  s2.Push(&sNode8_, ListRelation::TAIL);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  x = 75;
  SingleListNode<Char> sNode9_(c3.Set(x));  
  s3.Push(&sNode9_, ListRelation::TAIL);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  
  assert(s1 == "ABCDE" && s1.Size() == 5);
  assert(s2 == "ZYXWX" && s2.Size() == 5);
  assert(s3 == "KLMNY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);
  
  cout <<"SUCCESS testing <Deque>" <<endl;
}

/****************************************************************************/
void TestDlnklist()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  DoubleList<Char> s1, s2, s3;
  size_t i;

  Char* pc4;
  Char* pc5;

  DoubleList<Char>::SetComparisonFunction(CompChar);
  DoubleListNode<Char> sNode1_(c0);

  s3.AssignListNode(&sNode1_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 3; i++)
    s3.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
    
  DoubleList<Char> s4(s3);
  DoubleList<Char> s5, s6;
  DoubleListNode<Char> sNode2_(ca);
  DoubleListNode<Char> sNode3_(cb);

  s5.AssignListNode(&sNode2_, 1, DataStorageAttributes::DEEP_COPY);
  s6.AssignListNode(&sNode3_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 9; i++)  
    s6.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);

  DoubleList<Char> s7;
  DoubleListNode<Char> sNode4_(c0);
  
  s7.AssignListNode(&sNode4_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 3; i++)
    s7.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);  
  assert(s1 == "XX");
  s1.AddListNode(DoubleListNode<Char>::Make(ca), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(ca), ListRelation::TAIL);
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  DoubleListNode<Char> sNode5_(cb);
  DoubleListNode<Char> sNode6_(cb);
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, 1, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, 1, DataStorageAttributes::DEEP_COPY);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "bbbbb");
  s1.AssignListNode(
    DoubleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
    1, DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "bXXX");

  // test appends
  s1 = s5;
  s1 += s5; assert(s1 == "aa");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  assert(s1 == "aab");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "aabb");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);    
  assert(s1 == "aabbbbbbb");
  s1.AddListNode(DoubleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::TAIL);
  assert(s1 == "aabbbbbbba");  
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "aabbbbbbbaXXX");
  s1 = s4 + s5; assert(s1 == "XXXXa");

  // test inserts, removes, etc.
  s1 = s4;
  s1.AddListAtIndex(DoubleList<Char>::Make(s5), 0, ListRelation::NEXT);
  ShowTheList(s1);
  assert(s1 == "XaXXX");
  s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(cb), 1, ListRelation::NEXT);
  assert(s1 == "XabXXX");
  for (i = 0; i < 3; i++)
    s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(ca), 0, ListRelation::PREVIOUS);
  assert(s1 == "aaaXabXXX");
  s1.AddListNodeAtIndex(
    DoubleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE),
    8, ListRelation::NEXT);
  assert(s1 == "aaaXabXXXa");

  for (i = 0; i < 3; i++)
    s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(c3), 1, ListRelation::PREVIOUS);
  assert(s1 == "aXXXaaXabXXXa");
  DoubleList<Char> s8(s1);
  
  delete s1.DetachFromMiddleToEnd(7);
  assert(s1 == "aXXXaaX");
  
  delete s1.DetachFromMiddle(2, 1);
  assert(s1 == "aXXaaX");
  s1.Flush();
  assert(s1 == "");

  s1.AssignSublist(
    s6.GetNode(2)->NewSublist(ListRelation::NEXT, 0, TRUE),
    DataStorageAttributes::DESTROY);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.AssignSublist(
    s6.GetNode(3)->NewSublist(ListRelation::NEXT, 2),
    DataStorageAttributes::DESTROY);  
  assert(s1 == "bb");
  assert(s3 == "XXXX");
  
  DoubleList<Char> stemp_ = s1;
  s1 = s3;
  s3 = stemp_;
  assert(s1 == "XXXX" && s3 == "bb");
  
  // test access functions
  s1 = s7; assert(*s1.GetNode(0)->GetItem() == c0);
  s1.GetNode(0)->SetItem(ca); assert(*s1.GetNode(0)->GetItem() == ca);
  s1.GetNode(1)->SetItem(cb); assert(*s1.GetNode(1)->GetItem() == cb);

  pc4 = s1.ListToArray();
  DoubleList<Char>* cs7p = (DoubleList<Char>*)s7.Clone();
  pc5 = cs7p->ListToArray();
  assert(pc4[1] == cb && pc5[2] == c0);
  delete cs7p;
  ::Delete(pc4);
  ::Delete(pc5);

  // Test class hierarchy extensions
  // Test virtual object constructor
  DoubleList<Char>* ps1 = DoubleList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  DoubleList<Char>* ps2 = DoubleList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXXXXXaaaaab");

  DoubleList<int> s9;
  DoubleListNode<int> sNode7_(0);
  
  s9.AssignListNode(&sNode7_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 10; i++)
    s9.AddListNode(DoubleListNode<int>::Make(i * 2), ListRelation::NEXT);

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*s9.GetNode(i)->GetItem() == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  DoubleList<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10.GetNode(8)->SetItem(Char('c'));
  s10.GetNode(10)->SetItem(Char('c'));
  SimpleArray<int> Result_ = s8.SeriesListCompare(s10);

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  SingleList<Char> ss1, ss2, ss3;

  int x = 'a';
  SingleListNode<Char> sNode8_(c3.Set(x));
  ss1.AssignListNode(&sNode8_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    ss1.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 'a' + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  ss2.AssignListNode(&sNode9_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    ss2.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 'k';
  SingleListNode<Char> sNode10_(c3.Set(x));
  ss3.AssignListNode(&sNode10_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    ss3.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s1.Assign(ss2);
  s2.Assign(ss3);
  s3.Assign(ss1);

  assert(s1 == "zyxwv" && s1.Size() == 5);
  assert(s2 == "klmno" && s2.Size() == 5);
  assert(s3 == "abcde" && s3.Size() == 5);

  x = 65;
  DoubleListNode<Char> sNode11_(c3.Set(x));
  s1.AssignListNode(&sNode11_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s1.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 65 + 26 - 1;
  DoubleListNode<Char> sNode12_(c3.Set(x));
  s2.AssignListNode(&sNode12_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    s2.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 75;
  DoubleListNode<Char> sNode13_(c3.Set(x));
  s3.AssignListNode(&sNode13_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s3.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s1.AddList(DoubleSublist<Char>::Make(s3), ListRelation::HEAD);
  s1 += s2;

  assert(s1 == "KLMNOABCDEZYXWV");
  s1.Sort();
  assert(s1 == "ABCDEKLMNOVWXYZ");

  s1.ReplaceListAtIndex(DoubleSublist<Char>::Make(s3), 14);
  ShowTheList(s1);
  s1.ReplaceList(DoubleSublist<Char>::Make(s2), ListRelation::HEAD);
  ShowTheList(s1);

  assert(s1 == "ZYXWVBCDEKLMNOVWXYKLMNO");

  s1.Sort(SortOrder::DECREASING);
  assert(s1 == "ZYYXXWWVVOONNMMLLKKEDCB");

  s1.ReverseList();
  assert(s1 == "BCDEKKLLMMNNOOVVWWXXYYZ");

  s1.ReplaceListAtIndex(DoubleSublist<Char>::Make(s2), 10);
  assert(s1 == "BCDEKKLLMMZYXWVNOOVVWWXXYYZ");

  s1.Flush();
  s1 = s4;

  s1.AddListNode(DoubleSublist<Char>::Make(s3)->GetNode(2), ListRelation::HEAD);
  s1 += s2;

  assert(s1 == "KLMNOABCDEZYXWV");
  s1.Sort();
  assert(s1 == "ABCDEKLMNOVWXYZ");

  s1.ReplaceListNodeAtIndex(DoubleSublist<Char>::Make(s3)->GetNode(2), 14);
  s1.ReplaceListNode(DoubleSublist<Char>::Make(s2)->GetNode(2), ListRelation::HEAD);

  assert(s1 == "ZYXWVBCDEKLMNOVWXYKLMNO");

  s1.Sort(SortOrder::DECREASING);
  assert(s1 == "ZYYXXWWVVOONNMMLLKKEDCB");

  s1.ReverseList();
  assert(s1 == "BCDEKKLLMMNNOOVVWWXXYYZ");

  s1.ReplaceListNodeAtIndex(DoubleSublist<Char>::Make(s2)->GetNode(4), 10);
  assert(s1 == "BCDEKKLLMMZYXWVNOOVVWWXXYYZ");

  size_t Index_;
  DoubleListNode<Char>* FoundNode_ = s1.FindItem(Char('V'), Index_);
  assert(FoundNode_ != NULL && Index_ == 14 && FoundNode_->GetItem()->val() == 'V');
  FoundNode_ = s1.FindItem(Char('V'), Index_, s1.Size() - 1, ListRelation::HEAD);
  assert(FoundNode_ != NULL && Index_ == 19 && FoundNode_->GetItem()->val() == 'V');

  Char** ptrarr_ = new Char*[10];
  for (i = 0; i < 10; i++)
    ptrarr_[i] = new Char('0' + i);

  s1.Flush();
  s1.PtrArrayToList(ptrarr_, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  Char** ptrarr2_ = s1.ListToPtrArray();
  assert(ptrarr2_[0]->val() == '0');
  assert(ptrarr2_[1]->val() == '1');
  assert(ptrarr2_[2]->val() == '2');
  assert(ptrarr2_[3]->val() == '3');
  assert(ptrarr2_[4]->val() == 'F');
  assert(ptrarr2_[5]->val() == 'E');
  assert(ptrarr2_[6]->val() == '6');
  assert(ptrarr2_[7]->val() == '7');
  assert(ptrarr2_[8]->val() == '8');
  assert(ptrarr2_[9]->val() == '9');
  ::Delete(ptrarr2_);

  for (i = 0; i < 10; i++)
    delete ptrarr_[i];
  delete[] ptrarr_;

  pc4 = new Char[10];
  for (i = 0; i < 10; i++)
    pc4[i] = Char('0' + i);

  s1.Flush();
  s1.ArrayToList(pc4, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  pc5 = s1.ListToArray();
  assert(pc5[0].val() == '0');
  assert(pc5[1].val() == '1');
  assert(pc5[2].val() == '2');
  assert(pc5[3].val() == '3');
  assert(pc5[4].val() == 'F');
  assert(pc5[5].val() == 'E');
  assert(pc5[6].val() == '6');
  assert(pc5[7].val() == '7');
  assert(pc5[8].val() == '8');
  assert(pc5[9].val() == '9');
  ::Delete(pc5);

  delete[] pc4;

  s1 = s4;
  s2 = s4;
  s3 = s4;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s2));
  assert(s3.IsGreater(s2));

  assert(s1 == s4);
  assert(s1 < s2);
  assert(s3 > s2);
  assert(s1 != s3);

  s1.Duplicate(0, 3);
  s1.Duplicate(7, 3);
  s1.Duplicate(5, 3);

  assert(s1 == "AAAABCCCCDEEEE");
  cout <<"SUCCESS testing <DoubleList>" <<endl;
}

/****************************************************************************/
void TestDouble()
{
  ChrString Str_;

  Double si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  Double si2(315.15123);
  Double si3(-210.57123);
  Str_ = si3.ToString();
  assert(!strncmp(Str_.c_str(), "-210.5712", 9));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  ChrString strobj_ = "568.123";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;
  si2 -= UnsignedIntValue(200);
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;
  si2 /= UnsignedIntValue(2);
  cout <<si2 <<endl;

  Rational<long> r1, r2, r3;

  r1(1l,2l);
  r2(2l,3l);

  r3 = r1 + r2;
  si.Assign(r1);
  si2.Assign(r2);
  si3.Assign(r3);

  cout <<si <<endl;
  cout <<si2 <<endl;
  cout <<si3 <<endl;

  cout <<endl <<"SUCCESS Testing Double" <<endl;
}

/****************************************************************************/
void TestDynarray()
{
  Char c0, ca('a'), cb('b'), c3[3];
  DynArray<Char> s1, s2(30, reserve), s3(4, default_size);
  DynArray<Char> s4(s3), s5(ca), s6(cb, 10);
  const DynArray<Char> s7(c3, 3);

  s1.SetComparisonFunction(CompChar);
  assert(s1.RunLength() == 0);
  s1.Reserve(10);
  assert(s1.RunLength() == 0 && s1.Reserve() == 10);
  assert(s2.RunLength() == 0 && s2.Reserve() == 30);
  assert(s3.RunLength() == 4 && s3 == "XXXX");

  assert(s4.RunLength() == 4 && s4 == "XXXX");
  assert(s5.RunLength() == 1 && s5[0] == ca);
  assert(s6.RunLength() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.RunLength() == 3 && s7 == "XXX");
  s1.Resize(2); assert(s1 == "XX");
  s1.Resize(4, ca); assert(s1 == "XXaa");
  s1.Resize(3); assert(s1 == "XXa");

  // test assigns
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignArg(cb); assert(s1 == "b");
  s1.AssignArg(cb, 5); assert(s1 == "bbbbb");
  s1.AssignArg(&cb), assert(s1 == "b");
  s1.AssignArg(c3, 3); assert(s1 == "XXX");

  // test appends
  s1 = s5, s1 += s5, assert(s1 == "aa");
  s1 += cb, assert(s1 == "aab");
  s1.Append(cb), assert(s1 == "aabb");
  s1.Append(cb, 5), assert(s1 == "aabbbbbbb");
  s1.Append(&ca), assert(s1 == "aabbbbbbba");
  s1.Append(c3, 3), assert(s1 == "aabbbbbbbaXXX");
  s1 = s4 + s5, assert(s1 == "XXXXa");
  s1 = ca + s5, assert(s1 == "aa");
  s1 = s4 + cb, assert(s1 == "XXXXb");

  // test inserts, removes, etc.
  s1 = s4, s1.Insert(1, s5), assert(s1 == "XaXXX");
  s1.Insert(2, cb), assert(s1 == "XabXXX");
  s1.Insert(0, ca, 3), assert(s1 == "aaaXabXXX");
  s1.Insert(9, &ca), assert(s1 == "aaaXabXXXa");
  s1.Insert(1, c3, 3), assert(s1 == "aXXXaaXabXXXa");
  DynArray<Char> s8(s1);
  s1.Remove(7), assert(s1 == "aXXXaaX");
  s1.Remove(2, 1), assert(s1 == "aXXaaX");
  s1.Remove(), assert(s1 == "");
  s6.Sub_Array(s1, 2);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.Sub_Array(s1, 3, 2), assert(s1 == "bb");
  s1.Swap(s3), assert(s1 == "XXXX" && s3 == "bb");

  // test access functions
  s1 = s7, assert(s1[0] == c0);
  s1[0] = ca, assert(s1[0] == ca);
  s1.Put_At(1, cb), assert(s1.Get_At(1) == cb);
  assert(s1.Base()[1] == cb && s7.Base()[2] == c0);

  s8.Sort(ObjectSortHelper<Char, Char>::Make());
  assert(s8 == "XXXXXXXaaaaab");

  DynArray<int> s9(size_t(10), default_size);
  size_t i;
  for (i = 0; i < 10; i++)
    s9[i] = i * 2;

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(s9[i] == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  DynArray<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10[8] = Char('c');
  s10[10] = Char('c');
  SimpleArray<int> Result_ = s8.SeriesArrayCompare(s10);

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  cout <<"SUCCESS testing <DynArray>" <<endl;
}

/****************************************************************************/
void TestEnigma()
{
  char hold[128];
  char original[128] =
  {"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890\n\
    `!@#$%^&*()-=_+[]{}\\|;:\'\",<.>/?"};

  EnigmaCrypt Crypto_;

  strcpy(hold, original);
  printf("original = %s\n", hold);

  strcpy(hold, Crypto_.Encrypt(hold, -10));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -10));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, -15));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -15));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, -20));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -20));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 10));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 10));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 15));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 15));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt = %s\n", hold);
  assert(!strcmp(hold, original));
}

/****************************************************************************/
void TestFloat()
{
  ChrString Str_;

  Float si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  Float si2(315.15123);
  Float si3(-210.57123);
  Str_ = si3.ToString();
  assert(!strncmp(Str_.c_str(), "-210.5712", 9));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());  
  ChrString strobj_ = "568.123";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;  
  si2 -= UnsignedIntValue(200);
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;  
  si2 /= UnsignedIntValue(2);
  cout <<si2 <<endl;

  Rational<long> r1, r2, r3;

  r1(1l,2l);
  r2(2l,3l);

  r3 = r1 + r2;
  si.Assign(r1);
  si2.Assign(r2);
  si3.Assign(r3);

  cout <<si <<endl;
  cout <<si2 <<endl;
  cout <<si3 <<endl;

  cout <<endl <<"SUCCESS Testing Float" <<endl;
}

/****************************************************************************/
void TestAdvForLp()
{
  int cnt;

  // /* select sort loop */
  // int lim = 20;
  // for (int x = 0; x < lim - 1; ++x)
  //   for (int y = x + 1; y < lim; ++y)
  //	 /* Body of Loop should execute (19 * 10) / 2 = 190 times */
  int Lim_ = 20;
  int lp2startoff = 1;
  int lp1indexoff = -1;
  AdvForLpViewer<int> View_;
  AdvForLpViewer<int> View2_;

  AdvForLoop<int> Loop1_(0, Lim_ - 1, 1);
  Loop1_
    .SetIndexLimitPtr(&Lim_, &lp1indexoff)
    .AddLoopBodyAsVoidPtr(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBodyAsVoidPtr(Work, &View2_, ForLoopEnums::POST_LOOP);

  AdvForLoop<int> Loop2_(0, Lim_, 1);
  Loop2_
    .SetStartValuePtr(Loop1_.GetIndex(), &lp2startoff)
    .SetIndexLimitPtr(&Lim_)
    .AddLoopBodyAsVoidPtr(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBodyAsVoidPtr(Work, &View2_, ForLoopEnums::POST_LOOP);

  Loop1_.AddLoop(&Loop2_);

  for (cnt = 0; Loop1_.Run(); ++cnt);
  Loop1_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  Loop2_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  assert(cnt == 190);
  cout <<endl <<endl;

  // /* 4x4 matrix multiplication loop (incomplete) */
  // int lim = 4
  // for (int i = 0; i < lim; ++i)
  //   for (int j = 0; j < lim; ++j)
  //     for (int k = 0; k < lim; ++k)
  //       /* Body of Loop should execute 64 times */
  AdvForLoop<int> Loop3_;
  Lim_ = 4;
  Loop1_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop2_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop3_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_)
    .SetStepValue(1);
  Loop2_.AddLoop(&Loop3_);
  for (cnt = 0; Loop1_.Run(); ++cnt);
  cout <<cnt <<endl;
  assert(cnt == 64);

  // /* 8! (The hard & stupid way) */
  // int m = 0;
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         for (int g = 0; g < 4; ++g)
  //           for (int h = 0; h < 3; ++h)
  //             for (int i = 0; i < 2; ++i)
  //               ++m; /* final value should be 40320 */
  AdvForLoop<long> LoopArr_[7];
  int i; long Step_ = 1;
  for (i = 0, cnt = 8; cnt > 1; --cnt, ++i)
  {
    LoopArr_[i]
      .SetStartValue(0)
      .SetIndexLimit(cnt)
      .SetStepValue(Step_);

    if (i)
      LoopArr_[i - 1].AddLoop(&LoopArr_[i]);
  }
  long longcnt;
  for (longcnt = 0; LoopArr_[0].Run(); ++longcnt);
  cout <<longcnt <<endl;
  assert(longcnt == 40320);

  // /* Test standalone loop function   */
  // /* 8! / 4! = P(8,4) 		*/
  // int m = 0
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         ++m; /* final value should be 1680 */
  AdvForLpViewer<int> View3_;
  int Startv_[4] = { 0, 0, 0, 0 };
  int Endv_[4] = { 8, 7, 6, 5 };

  Loop1_
    .AddLoopBodyAsVoidPtr(Work, &View3_, ForLoopEnums::PRE_LOOP);
  Loop1_.ForLoopFnc(Startv_, Endv_, 4);
  cout <<endl <<View3_.Cnt() <<endl;
}

/****************************************************************************/
void TestFuzzyset()
{
  FuzzySet<char>::SetComparisonFunction(CompChar);
  TestFuzzySets(cout);

  char c0('0');
  char c1('1');
  char c2('2');
  char c3('3');
  char c4('4');
  char c5('5');
  char c6('6');
  char c7('7');
  char c8('8');
  char c9('9');
  char ca('A');
  char cb('B');
  char cc('C');
  char cd('D');
  char ce('E');
  char cf('F');

  FuzzySet<char> s1;
  FuzzySet<char> s2;
  FuzzySet<char> s3;
  FuzzySet<char> s4;
  FuzzySet<char> s5;
  FuzzySet<char> s6;
  FuzzySet<char> s7;
  FuzzySet<char> s8;
  FuzzyNode<double,char> Node_;
  FuzzyNode<double,char>* NodePtr_;

  Node_ = FuzzyNode<double,char>(0.1, c0);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c1);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c2);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.4, c3);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c4);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.6, c5);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.7, c6);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.8, c7);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.9, c8);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c9);
  s1 |= &Node_;

  Node_ = FuzzyNode<double,char>(0.1, c0);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c1);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c2);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.4, c3);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c4);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.6, c5);
  s2 |= &Node_;

  s3 = s2;

  Node_ = FuzzyNode<double,char>(0.7, c6);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c7);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.9, c8);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.4, c9);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.5, ca);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.6, cb);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.7, cc);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.8, cd);
  s4 |= &Node_;

  s5 = s4;
  Node_ = FuzzyNode<double,char>(0.1, ca);
  s5 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, cb);
  s5 |= &Node_;

  s6 = s1 & s4;
  s7 = s1 | s4;
  s8 = s1 - s4;

  assert(s1 > s2);
  assert(s1 >= s2);
  assert(s3 >= s2);
  assert(s3 == s2);
  assert(s1 != s2);

  ShowFuzzySet(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowFuzzySet(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD0123456789");
  ShowFuzzySet(s8.GiveSetData());
  assert(s8.GiveSetData() == "97543210");

  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c0));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '0');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c4));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '4');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c9));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '9');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, cd));
  assert(NodePtr_ != NULL && NodePtr_->_Value == 'D');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, char('x')));
  assert(NodePtr_ == NULL);

  s8 = s7 = s6 = s1;
  s6 &= s4;
  s7 |= s4;
  s8 -= s4;

  ShowFuzzySet(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowFuzzySet(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD9876543210");
  ShowFuzzySet(s8.GiveSetData());
  assert(s8.GiveSetData() == "97543210");

  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 > &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 > &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(!(s5 == &Node_));
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(!(s5 == &Node_));

  assert(!(s5 > s4));
  assert(s5 >= s4);
  assert(s5 == s4);

  Node_ = FuzzyNode<double,char>(0.1, ca);
  s5 -= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, cb);
  s5 -= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c4);
  s5 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.4, c5);
  s5 |= &Node_;  

  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(!(s5 > &Node_));
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(!(s5 > &Node_));
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 == &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 == &Node_);

  assert(s5 > s4);
  assert(s5 >= s4);
  assert(!(s5 == s4));

  cout <<"SUCCESS testing <FuzzySet>" <<endl;
}

/****************************************************************************/
void TestHashtab()
{
  HashTable<ChrString, ChrString> HashTab_(1000, ChrString(""), HashTableScheme::DOUBLEHASH);

  ChrString Str1Key_ = "Fred-Flintstone";
  ChrString Str1Val_ = "488-0093";
  ChrString Str2Key_ = "Winnie-Pooh";
  ChrString Str2Val_ = "255-0162";

  HashTab_["Fred-Flintstone"] = ChrString("488-0093");
  cout <<"key = " <<"Fred-Flintstone" <<endl;
  cout <<"data = " <<HashTab_["Fred-Flintstone"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;  

  HashTab_["Winnie-Pooh"] = ChrString("255-0162");
  cout <<"key = " <<"Winnie-Pooh" <<endl;
  cout <<"data = " <<HashTab_["Winnie-Pooh"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;

  HashTab_["Fred-Flintstone"].Erase();
  cout <<"Fred-Flintstone Removed" <<endl;
  cout <<"data = " <<HashTab_["Fred-Flintstone"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;
}

/****************************************************************************/
void TestImagePtr()
{
  YoClass Samp_;
  YoClass Samp2_;
  YoClass Samp3_;
  YoClass Samp4_;

  ImagePtr<YoClass> Yo1_;
  PtrImage<YoClass> Yo2_;
  ImageStackPtr<YoClass> Yo3_;
  ImagePtr<YoClass> Yo4_;
  PtrImage<YoClass> Yo5_;
  ImageStackPtr<YoClass> Yo6_;

  Samp_.Setx(10);
  Samp2_.Setx(5);
  Samp3_.Setx(15);
  Samp4_.Setx(25);

  Yo1_ = &Samp_;
  Yo1_.Snapshot();
  cout <<Yo1_.HaveImage() <<" ";
  cout <<Yo1_->Getx() <<" ";
  Yo1_ = &Samp2_;
  cout <<Yo1_->Getx() <<" ";
  Yo1_.Rollback();
  cout <<Yo1_->Getx() <<" ";
  Yo1_.Snapshot();
  Yo1_ = &Samp2_;
  Yo1_.Commit();
  Yo1_.Rollback();
  cout <<Yo1_->Getx() <<endl;

  Yo2_ = &Samp_;
  Yo2_.Snapshot();
  cout <<Yo2_.HaveImage() <<" ";
  cout <<Yo2_->Getx() <<" ";
  Yo2_ = &Samp2_;
  cout <<Yo2_->Getx() <<" ";
  Yo2_.Rollback();
  cout <<Yo2_->Getx() <<" ";
  Yo2_.Snapshot();
  Yo2_ = &Samp2_;
  Yo2_.Commit();
  Yo2_.Rollback();
  cout <<Yo2_->Getx() <<endl;

  Yo3_ = &Samp_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  cout <<Yo3_.HaveImage() <<" ";
  Yo3_ = &Samp2_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp3_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp4_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PopImage();
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PopImage();
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp4_;
  Yo3_.Commit();
  Yo3_.Rollback();
  cout <<Yo3_->Getx() <<endl;

  Yo4_ = &Samp_;
  Yo1_ = &Samp_;
  cout <<(Yo1_ == Yo4_) <<" ";
  cout <<(Yo1_ != Yo4_) <<endl;

  Yo5_ = &Samp2_;
  Yo2_ = &Samp2_;
  cout <<(Yo2_ == Yo5_) <<" ";
  cout <<(Yo2_ != Yo5_) <<endl;

  Yo6_ = &Samp3_;
  Yo3_ = &Samp3_;
  cout <<(Yo3_ == Yo6_) <<" ";
  cout <<(Yo3_ != Yo6_) <<endl;
}

/****************************************************************************/
void TestIniFile()
{
  IniFile myini("sample.ini", ios::in | ios::out);
  
  if (myini.ReadIntoMemory())
  {
    IniRecord Record_, NewRecord_;

    myini.RetrieveRecord(2, Record_);
    Record_.DeleteEntry(3);
    Record_.DeleteEntry(4);
    Record_.DeleteEntry(5);

    IniEntry Entry_(Record_.GetEntry(12)->Key(), "Canada");
    Record_.ModifyEntry(12, Entry_);
    myini.ModifyRecord(2, Record_);
    myini.DeleteRecord(3);

    myini.RetrieveRecord(1, Record_);
    Entry_.SetKey("User"); Entry_.SetValue("Bugs Bunny");
    Record_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Password"); Entry_.SetValue("Whats Up Doc?");
    Record_.AddEntry(IniEntry::Make(Entry_));
    myini.ModifyRecord(1, Record_);

    NewRecord_.SetName("kill file");
    Entry_.SetKey("User"); Entry_.SetValue("Fred Flintstone");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Password"); Entry_.SetValue("Yaba Daba Doo!");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Drive Letter"); Entry_.SetValue("D:");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Shares"); Entry_.SetValue("//pentium1/PENTIUM_C");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    myini.AddRecord(NewRecord_);
    
    myini.UpdateFile();
  }

  myini.Close();
}

/****************************************************************************/
void TestInteger()
{
  ChrString Str_;

  Integer si(31515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515"));

  Integer si2(31515);
  Integer si3(-21057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057"));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  Integer sint1("-32090");
  si2.Assign(sint1);
  assert(si2 = -32090);

  si2 = "34000";
  assert(si2 == 34000);

  // Testing bounds checking
  Integer chk1 = INT_MAX;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = INT_MAX;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 65536;
  chk1 = chk1 * 32768;
  assert(chk1.Overflow());
  chk1 = 65536;
  chk1 >>= 2;
  assert(int(chk1) == 16384);
  chk1 <<= 4;
  assert(int(chk1) == 262144);
  chk1 |= Integer(0x0F);
  assert(int(chk1) == 262159);
  chk1 = chk1 / 0;
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  Integer* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new Integer(i+1);
  List_[10] = NULL;

  Integer ZeroChar(0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<Integer> Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing Integer" <<endl;
}

/****************************************************************************/
void TestLdouble()
{
  ChrString Str_;

  LongDouble si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  LongDouble si2(315.15123);
  LongDouble si3(-210.57123);
  Str_ = si3.ToString();  
  assert(!strncmp(Str_.c_str(), "-210.5712", 9));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  ChrString strobj_ = "568.123";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;
  si2 -= UnsignedIntValue(200);
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;
  si2 /= UnsignedIntValue(2);
  cout <<si2 <<endl;

  Rational<long> r1, r2, r3;

  r1(1l,2l);
  r2(2l,3l);

  r3 = r1 + r2;
  si.Assign(r1);
  si2.Assign(r2);
  si3.Assign(r3);

  cout <<si <<endl;
  cout <<si2 <<endl;
  cout <<si3 <<endl;

  cout <<endl <<"SUCCESS Testing LongDouble" <<endl;
}

/****************************************************************************/
void TestLonestr()
{
  LoneString Str1_;
  LoneString Str2_("Hello World");
  LoneString Str3_("GoodBye World", 8);
  LoneString Str4_('x', 5);
  LoneString Str5_(Str3_);

  LoneString* Str6_ = LoneString::Make();
  LoneString* Str7_ = LoneString::Make("Hello World");
  LoneString* Str8_ = LoneString::Make("GoodBye World", 8);
  LoneString* Str9_ = LoneString::Make('x', 5);
  LoneString* Str10_ = LoneString::Make(*Str8_);

  Str1_ = Str2_;
  Str2_.ToUpper();
  Str7_->ToLower();
  Str3_.RightJustify(20);
  Str8_->Delete(0, 4);
  Str8_->LeftJustify(20);
  Str4_.CenterJustify(20);

  Str1_.Duplicate(3);
  *Str6_ = *Str7_;
  Str6_->Insert(3, "Inserted");

  cout  <<Str1_ <<endl
	<<Str2_ <<endl
	<<Str3_ <<endl
	<<Str4_ <<endl
	<<Str5_ <<endl
	<<*Str6_ <<endl
	<<*Str7_ <<endl
	<<*Str8_ <<endl
	<<*Str9_ <<endl
	<<*Str10_ <<endl;

  assert(WhatIs(Str1_) == TypeInfo::LONESTRING_TYPE);
  assert(Str1_ == "Hello WorldHello WorldHello WorldHello World");
  assert(Str2_ == "HELLO WORLD");
  assert(Str3_ == "            GoodBye ");
  assert(Str4_ == "       xxxxx        ");
  assert(Str5_ == "GoodBye ");
  assert(*Str6_ == "helInsertedlo world");
  assert(*Str7_ == "hello world");
  assert(*Str8_ == "Bye                 ");
  assert(*Str9_ == "xxxxx");
  assert(*Str10_ == "GoodBye ");

  char Buffer_[20];
  StringToken Tok_(Str1_.c_str(), 0, " ", " ");
  Tok_.FindToken(Buffer_);
  cout <<Buffer_ <<"-";
  assert(!strcmp(Buffer_, "Hello"));
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(!strcmp(Buffer_, "WorldHello"));
    cout <<Buffer_ <<"-";
  }
  Tok_.FindToken(Buffer_);
  assert(!strcmp(Buffer_, "World"));
  cout <<Buffer_ <<endl;

  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, "Bye  ");
  Str1_.Replace(0, "ll", "nn");

  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl;

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  LoneFallible<Subscript> Fall1_ = Str1_.find("Bye");
  LoneFallible<Subscript> Fall2_ = Str1_.findlast("Good");
  Subscript Pos3_ = Str1_.find("World");
  Subscript Pos4_ = Str1_.findlast("Henno");
  Subscript Pos5_ = Str1_.findnext(Pos3_, "World");
  Subscript Pos6_ = Str1_.findprevious(Pos4_, "Henno");

  LoneString Str11_ = Str3_.Right(5);
  LoneString Str12_ = Str8_->Left(5);
  LoneString Str13_ = Str4_.Mid(10, 10);

  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl
	<<Fall1_.valid() <<endl
	<<Fall2_.valid() <<endl
	<<Pos3_ <<endl
	<<Pos4_ <<endl
	<<Pos5_ <<endl
	<<Pos6_ <<endl
	<<Str11_ <<endl
	<<Str12_ <<endl
	<<Str13_ <<endl;

  assert((int)Fall1_.valid() == 0);
  assert((int)Fall2_.valid() == 0);
  assert(Pos3_ == 6);
  assert(Pos4_ == 33);
  assert(Pos5_ == 17);
  assert(Pos6_ == 22);
  assert(Str11_ == "dBye ");
  assert(Str12_ == "Bye  ");
  assert(Str13_ == "xx        ");

  Str1_ -= "no World";
  cout <<Str1_ <<endl;
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHen");

  LoneString Str14_ = "JoKCsephKCWong";
  Str14_.Delete("KC");
  cout <<Str14_ <<endl;
  assert(Str14_ == "JoKCsephWong");
  LoneString Str15_ = Delete(Str14_, "KC");
  cout <<Str15_ <<endl;
  assert(Str15_ == "JosephWong");

  Str15_ += " This is a test of appending";
  cout <<Str15_.c_str() <<endl;
  assert(Str15_ == "JosephWong This is a test of appending");

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__GNUC__))
  Pos3_ = (Subscript&)Str15_.find('s');
  Pos4_ = (Subscript&)Str15_.findlast('s');
  Pos5_ = (Subscript&)Str15_.findnext(Pos3_, 's');
  Pos6_ = (Subscript&)Str15_.findprevious(Pos4_, 's');
#else
  Pos3_ = Str15_.find('s');
  Pos4_ = Str15_.findlast('s');
  Pos5_ = Str15_.findnext(Pos3_, 's');
  Pos6_ = Str15_.findprevious(Pos4_, 's');
#endif
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  Str15_ -= " This is a test of appending";
  assert(Str15_ == "JosephWong");
  assert((Str15_ + " This is a test of appending") == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  LoneFallible<Subscript> Fall3_ = Str15_.brk("eph");
  assert(Fall3_.valid());
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__GNUC__))
  Pos3_ = (Subscript&)Fall3_;
#else
  Pos3_ = Fall3_;
#endif
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span("toseJy");
  Pos5_ = Str15_.cspan("Wpht");

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

  cout <<endl <<"SUCCESS Testing LoneString" <<endl;
}

/****************************************************************************/
void TestLongint()
{
  ChrString Str_;

  LongInt si(31515012);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515012"));

  LongInt si2(31515012);
  LongInt si3(-21057012);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057012"));  

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "568012";
  assert(si2 == 568012);

  ShortInt shi1_ = 21243;
  UShortInt ushi1_ = 41511;

  si2 = shi1_;
  assert(si2 == 21243);
  si2 = ushi1_;
  assert(si2 == 41511);

  // Testing bounds checking
  int OvFl_, UnFl_, DivZ_, NoErr_;
  LongInt chk1 = LONG_MAX;
  chk1 += 5;
  OvFl_ = chk1.Overflow();
  assert(OvFl_);
  chk1.ClearError();
  chk1 = LONG_MAX;
  chk1 = -chk1;
  --chk1;
  UnFl_ = chk1.Underflow();
  assert(!UnFl_);
  --chk1;
  UnFl_ = chk1.Underflow();
  assert(UnFl_);
  chk1.ClearError();
  chk1 = 65536;
  chk1 *= 32769;
  OvFl_ = chk1.Overflow();  
  assert(OvFl_);
  chk1 = 65536;
  chk1 >>= 2;
  assert(long(chk1) == 16384);
  chk1 <<= 4;
  assert(long(chk1) == 262144);
  chk1 |= LongInt(0x0F);
  assert(long(chk1) == 262159);
  chk1 /= LongInt(0);
  DivZ_ = chk1.DivideByZero();
  assert(DivZ_);
  OvFl_ = chk1.Overflow();
  assert(OvFl_);
  NoErr_ = chk1.NoError();
  assert(!NoErr_);

  // Testing series comparison class on user defined type
  LongInt* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new LongInt(i+1);
  List_[10] = NULL;

  LongInt ZeroChar(0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<LongInt> Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing LongInt" <<endl;
}

/****************************************************************************/
void TestLongnum()
{
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
}

/****************************************************************************/
void TestObjimp()
{
  char cvar;
  int ivar;
  unsigned int uivar;
  short svar;
  Ushort usvar;
  long lvar;
  Ulong ulvar;
  float fvar;
  double dvar;
  Ldouble ldvar;
  SomeDumStruct EmptyStruct;

  int filler = 0;
  void* voidp = &filler;
  void** dvoidp = &voidp;  
  int* nullp = NULL;

  assert(IsBuiltInType(&cvar) && WhatIs(cvar) == TypeInfo::BUILTIN_CHAR &&
	 !strcmp(NameOf(cvar), "char"));
  assert(IsBuiltInType(&ivar) && WhatIs(ivar) == TypeInfo::BUILTIN_INT &&
	 !strcmp(NameOf(ivar), "int"));
  assert(IsBuiltInType(&uivar) && WhatIs(uivar) == TypeInfo::BUILTIN_UNSIGNED_INT &&
	 !strcmp(NameOf(uivar), "unsigned int"));
  assert(IsBuiltInType(&svar) && WhatIs(svar) == TypeInfo::BUILTIN_SHORT &&
	 !strcmp(NameOf(svar), "short"));
  assert(IsBuiltInType(&usvar) && WhatIs(usvar) == TypeInfo::BUILTIN_UNSIGNED_SHORT &&
	 !strcmp(NameOf(usvar), "unsigned short"));
  assert(IsBuiltInType(&lvar) && WhatIs(lvar) == TypeInfo::BUILTIN_LONG &&
	 !strcmp(NameOf(lvar), "long"));
  assert(IsBuiltInType(&ulvar) && WhatIs(ulvar) == TypeInfo::BUILTIN_UNSIGNED_LONG &&
	 !strcmp(NameOf(ulvar), "unsigned long"));
  assert(IsBuiltInType(&fvar) && WhatIs(fvar) == TypeInfo::BUILTIN_FLOAT &&
	 !strcmp(NameOf(fvar), "float"));
  assert(IsBuiltInType(&dvar) && WhatIs(dvar) == TypeInfo::BUILTIN_DOUBLE &&
	 !strcmp(NameOf(dvar), "double"));
  assert(IsBuiltInType(&ldvar) && WhatIs(ldvar) == TypeInfo::BUILTIN_LONG_DOUBLE &&
	 !strcmp(NameOf(ldvar), "long double"));
  assert(!IsBuiltInType(voidp) && WhatIsPtr(voidp) == TypeInfo::VOID_POINTER &&
	 !strcmp(NameOfPtr(voidp), "void*"));
  assert(!IsBuiltInType(dvoidp) && WhatIsPtr(dvoidp) == TypeInfo::DOUBLE_VOID_POINTER &&
	 !strcmp(NameOfPtr(dvoidp), "void**"));
  assert(!IsBuiltInType(nullp) && WhatIsPtr(nullp) == TypeInfo::NULL_POINTER &&
	 !strcmp(NameOfPtr(nullp), "NULL"));
  assert(!IsBuiltInType(&EmptyStruct) && WhatIs(EmptyStruct) == TypeInfo::UNDEFINED &&
	 !strcmp(NameOf(EmptyStruct), "Undefined Type"));

  char* cvarp = Clone('z');
  int* ivarp = Clone(2);
  unsigned int* uivarp = Clone(3U);
  short* svarp = Clone(short(4));
  Ushort* usvarp = Clone(Ushort(5));
  long* lvarp = Clone(6L);
  Ulong* ulvarp = Clone(7UL);
  float* fvarp = Clone(float(8.3));
  double* dvarp = Clone(9.6);
  Ldouble* ldvarp = Clone(Ldouble(10.9));
  voidp = Clone(EmptyStruct);

#if defined(__TURBOC__) | defined(__BORLANDC__)
  int** dintp = &ivarp;
  dvoidp = &voidp;
  assert(IsNestedPtr(dintp));
  assert(!IsNestedPtr(voidp));
  assert(IsNestedPtr(dvoidp));
#endif

  assert(IsStreamable(voidp));
  assert(cvarp && *cvarp == 'z');
  assert(ivarp && *ivarp == 2);
  assert(uivarp && *uivarp == 3);
  assert(svarp && *svarp == 4);
  assert(usvarp && *usvarp == 5);
  assert(lvarp && *lvarp == 6);
  assert(ulvarp && *ulvarp == 7);
  cout <<*fvarp <<" ";	// 8.3
  cout <<*dvarp <<" ";	// 9.6
  cout <<*ldvarp <<" ";	// 10.9

  // Testing template functions
  int ivar2;
  ivar = 5; ivar2 = 10;
  Swap(ivar, ivar2); assert(ivar == 10 && ivar2 == 5);

#if defined(__TURBOC__) | defined(__BORLANDC__)
  assert(SizeOfData(ivar) == 2);
  assert(FormWidthOfObject(ivar) == 6);
#else
  assert(SizeOfData(ivar) == 4);
  assert(FormWidthOfObject(ivar) == 8);
#endif

  Assign(ivar, ivar2); assert(ivar == 5 && ivar2 == 5);
  ivar = 29;
  WriteToOutput((ostream&)cout, ivar); // 29
  ivar2 = 30;
  NonConstAssign(ivar, ivar2); assert(ivar == 30 && ivar2 == 30);
  Delete(ivarp);
  ivarp = Clone(ivar);
  assert(*ivarp == 30);

  Delete(voidp);
  Delete(cvarp);
  Delete(ivarp);
  Delete(uivarp);
  Delete(svarp);
  Delete(usvarp);
  Delete(lvarp);
  Delete(ulvarp);
  Delete(fvarp);
  Delete(dvarp);
  Delete(ldvarp);

  // Testing memory manager -- these extra deletes should not cause
  // any problems, but should be ignored by the memory manager.
  Delete(voidp);
  Delete(cvarp);
  Delete(ivarp);
  Delete(uivarp);
  Delete(svarp);
  Delete(usvarp);
  Delete(lvarp);
  Delete(ulvarp);
  Delete(fvarp);
  Delete(dvarp);
  Delete(ldvarp);

  cout <<endl <<"SUCCESS Testing generic template functions" <<endl;
}

/****************************************************************************/
void TestPoint()
{
  Point<int> pt1(1, 8);
  Point<int> pt2(2, 11);
  Point<int> pt3(3, 14);
  Point<int> pt4(4, 17);
  Point<int> npt5(4, 19);

  double Dist_ = pt1.Distance(npt5);
  cout <<Dist_ <<" == " <<sqrt(130) <<endl;

  Dist_ = pt1.Distance(pt4);
  cout <<Dist_ <<" == " <<sqrt(90) <<endl;

  Rational<int> slp_ = pt1.Slope(pt3);
  assert(slp_.GetNum() == 3);
  assert(slp_.GetDen() == 1);
  
  Rational<int> xint_ = pt2.Xintercept(pt4);
  assert(xint_.GetNum() == -5);
  assert(xint_.GetDen() == 3);

  Rational<int> yint_ = pt2.Yintercept(pt3);
  assert(yint_.GetNum() == 5);
  assert(yint_.GetDen() == 1);

  assert(pt1.IsColinear(pt2, pt3));
  assert(pt3.IsColinear(pt2, pt4));
  assert(!pt2.IsColinear(pt1, npt5));
  assert(!pt3.IsColinear(pt2, npt5));

  Point<int>* pt11p = (Point<int>*)pt1.Clone();
  Point<int> pt12(0, 0);
  
  assert(*pt11p == pt1);
  assert(pt12 != pt2);
  pt12.Assign(pt2);
  assert(pt12 == pt2);
  assert(pt12 != pt1);
}

/****************************************************************************/
void TestPrimeTbl()
{
  const Ulong TESTMAX = 10000;

  PrimesTable Tbl_;
  Ulong nthprime_, prevprime_, nextprime_;
  int i, x, y, Max_, Limit_;
  Boolean Error_, Verified_, nthok_, ceilfloorok_;
  char* msg_;

  Max_ = TESTMAX;
  Verified_ = TRUE;
  x = 0; y = 2; i = 0;
  nextprime_ = prevprime_ = 0;
  Limit_ = Max_ * 2;    
  Error_ = FALSE;
  x = 0;

  for (y = 2; y <= Limit_; Max_ *= 2)
  {
    for (i = y; i <= Max_; ++i)
    {
      if (Tbl_.IsPrime(i))
      {
    
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
}

/****************************************************************************/
void TestPriqueue()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  PriorityQueue<Char> s1, s2, s3;
  size_t i;

  PriorityQueue<Char>::SetComparisonFunction(CompChar);
  Char* pc5 = &c0;

  s1.SetPriorityFunction(PriorityChar);
  s2.SetPriorityFunction(PriorityChar);
  s3.SetPriorityFunction(PriorityChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Insert(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Insert(SingleListNode<Char>::Make(c0));
    
  PriorityQueue<Char> s4(s3);
  PriorityQueue<Char> s5, s6;

  s4.SetPriorityFunction(PriorityChar);
  s5.SetPriorityFunction(PriorityChar);
  s6.SetPriorityFunction(PriorityChar);
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Insert(&sNode2_);
  s6.Insert(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Insert(SingleListNode<Char>::Make(cb));

  PriorityQueue<Char> s7;
  s7.SetPriorityFunction(PriorityChar);
  SingleListNode<Char> sNode4_(c0);
  
  s7.Insert(&sNode4_);
  for (i = 1; i < 3; i++)
    s7.Insert(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Insert(SingleListNode<Char>::Make(ca));
  s1.Insert(SingleListNode<Char>::Make(ca));
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);

  // test assigns
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Insert(PriorityQueue<Char>::Make(s5)); assert(s1 == "aa");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aab");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aabb");
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));    
  assert(s1 == "aabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "aaabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXaaabbbbbbb" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXaaabbbbbb" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == cb);
  assert(s1 == "XXXaaabbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXaaabbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {  
    Item_ = s1.PopItem();
    if (i < 4)
      assert(*Item_ == cb);
    else
      assert(*Item_ == ca);
    ::Delete(Item_);
  }
  assert(s1 == "XXXa" && s1.Size() == 4);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXX" && s1.Size() == 3);
  delete Node_;
  for (i = 0; i < 3; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == c0);
    ::Delete(Item_);
  }
  assert(s1.Size() == 0);  
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  PriorityQueue<Char>* ps1 = PriorityQueue<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  PriorityQueue<Char>* ps2 = PriorityQueue<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode8_(c3.Set(x));
  s1.Insert(&sNode8_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s2.Insert(&sNode9_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s3.Insert(&sNode10_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "ABCDE" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "KLMNY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

  cout <<"SUCCESS testing <PriorityQueue>" <<endl;
}

/****************************************************************************/
void TestQueue()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  Queue<Char> s1, s2, s3;
  size_t i;
  Char* pc5 = &c0;

  Queue<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Enqueue(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Enqueue(SingleListNode<Char>::Make(c0));
    
  Queue<Char> s4(s3);
  Queue<Char> s5, s6;
  SingleListNode<Char> sNodea_(ca);
  SingleListNode<Char> sNodeb_(cb);

  s5.Enqueue(&sNodea_);
  s6.Enqueue(&sNodeb_);
  for (i = 0; i < 9; i++)  
    s6.Enqueue(SingleListNode<Char>::Make(cb));

  Queue<Char> s7;
  SingleListNode<Char> sNode2_(c0);
  
  s7.Enqueue(&sNode2_);
  for (i = 1; i < 3; i++)
    s7.Enqueue(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Enqueue(SingleListNode<Char>::Make(ca));
  s1.Enqueue(SingleListNode<Char>::Make(ca));
  assert(s1 == "aaXX");
  s1.ShrinkTo(3); assert(s1 == "aaX");

  // test assigns
  SingleListNode<Char> sNode3_(cb);
  SingleListNode<Char> sNode4_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode3_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode4_, DataStorageAttributes::DEEP_COPY);
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Enqueue(Queue<Char>::Make(s5)); assert(s1 == "aa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  assert(s1 == "baa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  assert(s1 == "bbaa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));    
  assert(s1 == "bbbbbbbaa");
  s1.Enqueue(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "abbbbbbbaa");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXXabbbbbbba" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "XXXabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXabbbbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "XXXa" && s1.Size() == 4);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXX" && s1.Size() == 3);
  delete Node_;
  for (i = 0; i < 3; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == c0);
    ::Delete(Item_);
  }
  assert(s1.Size() == 0);
  
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  Queue<Char>* ps1 = Queue<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Queue<Char>* ps2 = Queue<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode5_(c3.Set(x));
  s1.Enqueue(&sNode5_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode6_(c3.Set(x));
  s2.Enqueue(&sNode6_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode7_(c3.Set(x));
  s3.Enqueue(&sNode7_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "EDCBA" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "ONMLY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

  cout <<"SUCCESS testing <Queue>" <<endl;
}

/****************************************************************************/
void TestQuickint()
{
  Parser* pptr = Parser::MakeParserPtr();

  char Buffer_[512];
  ParseResultInfo* Results_ = new ParseResultInfo();
  ParseResultInfo* Results2_ = new ParseResultInfo();
  QuickParseStatus* Status_ = new QuickParseStatus();
  QuickParseStatus* Status2_ = new QuickParseStatus();

  pptr->UseBrackets(TRUE);
  pptr->ReadDefinitionFile("mysqldefn.txt");

//  system("clear");
  Parser* pptr1 = Parser::MakeParserPtr();
  Parser* pptr2 = Parser::MakeParserPtr();
  Parser* pptr3 = Parser::MakeParserPtr();
  Parser* pptr4 = Parser::MakeParserPtr();
  Parser* pptr5 = Parser::MakeParserPtr();
  Parser* pptr6 = Parser::MakeParserPtr();
  Parser* pptr7 = Parser::MakeParserPtr();

  pptr1->CopyParseSymbols(*pptr);
  pptr2->CopyParseSymbols(*pptr);
  pptr3->CopyParseSymbols(*pptr);
  pptr4->CopyParseSymbols(*pptr);
  pptr5->CopyParseSymbols(*pptr);
  pptr6->CopyParseSymbols(*pptr);
  pptr7->CopyParseSymbols(*pptr);

  strcpy(Buffer_, " visitnum INTEGER(10) UNSIGNED ZEROFILL NOT NULL DEFAULT 0 AUTO_INCREMENT PRIMARY KEY:");
  Status_->Reset();
  Results_->Reset();  
  pptr1->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr1->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr1);  
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, "small_value SMALLINT NOT NULL DEFAULT 0:");
  Status_->Reset();
  Results_->Reset();
  pptr2->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr2->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr2);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, "float_value DOUBLE(10, 2) ZEROFILL NULL DEFAULT 0 PRIMARY KEY:");
  Status_->Reset();
  Results_->Reset();  
  pptr3->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr3->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " tstamp_value TIMESTAMP:");
  Status_->Reset();
  Results_->Reset();    
  pptr4->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr4->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " dt_value DATETIME DEFAULT \"1999-01-01 20:20:20\":");
  Status_->Reset();
  Results_->Reset();
  pptr5->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr5->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr5);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " vchar1 VARCHAR(10) NOT NULL DEFAULT \"sample\":");
  Status_->Reset();
  Results_->Reset();    
  pptr6->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr6->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " charval CHAR(10) BINARY DEFAULT \"\":");
  Status_->Reset();
  Results_->Reset();    
  pptr7->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr7->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/" <<endl;  
  cout <<"/**************************************************************************************************************/" <<endl;

  delete Results_;
  delete Status_;

  Parser::DestroyParser(0, pptr);  
}

/****************************************************************************/
void TestRational()
{
}

/****************************************************************************/
void TestRect()
{
  Rectangle<int> r1(1, 1, 21, 41);
  Rectangle<int> r2(30, 30, 60, 90);
  Rectangle<int> r3(r1);
  Rectangle<int> r4;

  assert(r1.Area() == 800);
  assert(r2.Area() == 1800);
  assert(r2.Perimeter() == 180);
  assert(r1 == r3);
  assert(r1 < r2);
  assert(r2 > r3);
  assert(r2.Width() == 30);
  assert(r2.Height() == 60);
  r2.Rotate90();
  cout <<r2 <<endl;
}

/****************************************************************************/
void TestRefptrs()
{
  int a = 6, d = 9;

  // Test cases
  RefPtr<int> intp(&a);
  cout <<*intp.Pointee() <<endl;

  // Testing Master pointer semantics
  MemPtr<int> Mint(&d);
  cout <<*Mint.Pointee() <<endl;

  // testing Boolean operator == (RefPtr<T>&, RefPtr<T>&);
  cout <<(Mint == intp) <<endl;
  cout <<(Mint != intp) <<endl;
  cout <<(Mint - intp) <<endl;

  intp.ClearPtr();
  Mint.ClearPtr();

  int m = 11, n = 12, p = 13, k = 14;
  int *Val_;

  REFPTRSTestClass* Testptr_;
  REFPTRSTestClass Testobj_(n);
  RefPtr<REFPTRSTestClass> Testcp_;
  CPtr<REFPTRSTestClass> Cptr(&Testcp_);

  // testing RefPtr<int>::operator = (T*)
  intp = &m;
  cout <<*intp.Pointee() <<endl;

  Testcp_ = MakeAllocator(&Testobj_).AllocMem();
  cout <<Cptr->GetMember() <<endl;
  cout <<Testcp_.Pointee()->GetMember() <<endl;

  Testptr_ = Testcp_.ClearPtr();
  delete Testptr_;

  // testing RefPtr<T>::RefPtr(T*) conversion constructor
  intp = &p;
  cout <<*intp.Pointee() <<endl;

  // testing RefPtr<int>(T*)
  // testing RefPtr<T>& RefPtr<T>::operator = (RefPtr<T>&)
  Val_ = new int(k + 5);
  intp = RefPtr<int>(Val_);
  cout <<*intp.Pointee() <<endl;

  // testing MemPtr<T>::MemPtr(T*) conversion constructor
  Mint = &k;
  cout <<*Mint.Pointee() <<endl;

  // testing virtual RefPtr<T>& RefPtr<T>::operator = (RefPtr<T>&)
  Mint = intp;
  cout <<*Mint.Pointee() <<endl;
  delete Val_;
}

/****************************************************************************/
void TestSafeSet()
{
  SafeSet<short> Sset_;
  short Num13_ = 13, Num26_ = 26, Num37_ = 37, Num81_ = 81, Num78_ = 78;

  Sset_ += &Num13_;
  Sset_ += &Num26_;
  Sset_ += &Num37_;
  Sset_ += &Num81_;
  Sset_ += &Num78_;

  SafeSetIterator<short>* IterPtr_ = Sset_.GiveIterator();

  while (IterPtr_->More())
    cout <<*IterPtr_->Next() <<" ";
  cout <<endl;

  cout <<(Sset_ >= &Num37_) <<endl;
  Sset_ -= &Num37_;
  cout <<(Sset_ >= &Num37_) <<endl;

  delete IterPtr_;
  IterPtr_ = Sset_.GiveIterator();

  while (IterPtr_->More())
    cout <<*IterPtr_->Next() <<" ";
  cout <<endl;

  delete IterPtr_;
}

/****************************************************************************/
void TestSercmp()
{
  short* List_[25];
  int i, RetVal_;

  for (i = 0; i < 10; ++i)
    List_[i] = new short(i+1);
  List_[10] = NULL;

  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(11, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(77, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(1, List_, 0, SHRT_MIN, SHRT_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);

  for (i = 0; i < 10; ++i)
    *List_[i] = -(*List_[i]);

  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(55, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(77, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(1, List_, 0, SHRT_MIN, SHRT_MAX, TRUE).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(11, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(55, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(77, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(1, List_, 0, NumberingScheme::SIGNED_2SCOMP).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-11, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-55, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() == 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-77, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  List_[0] = new short(SHRT_MAX - 100);
  List_[1] = new short(50);
  List_[2] = new short(51);
  List_[3] = NULL;
  List_[4] = new short(*List_[0]);
  List_[5] = new short(50);
  List_[6] = new short(50);
  List_[7] = NULL;
  List_[8] = new short(SHRT_MIN + 100);
  List_[9] = new short(-50);
  List_[10] = new short(-51);
  List_[11] = NULL;
  List_[12] = new short(*List_[8]);
  List_[13] = new short(-50);
  List_[14] = new short(-50);
  List_[15] = NULL;
  List_[16] = new short(SHRT_MAX);
  List_[17] = new short(SHRT_MIN);
  List_[18] = new short(SHRT_MAX - 500);
  List_[19] = new short(500);
  List_[20] = new short(1);
  List_[21] = NULL;
  List_[22] = new short(100);
  List_[23] = new short(SHRT_MAX);

  SeriesCompare<short> Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, List_, 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[4], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  assert(Sobj_.Result() == 0);
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;

  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MIN, &List_[8], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() > 0);
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MIN, &List_[12], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() == 0);

  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() == 0);

  delete List_[20];
  List_[20] = List_[22];
  List_[22] = NULL;
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);

  delete List_[20];
  List_[20] = List_[23];
  List_[23] = NULL;
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);

  for (i = 0; i < 24; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing Series Compare Class" <<endl;    
}

/****************************************************************************/
void TestSet()
{
  Char c0('0');
  Char c1('1');
  Char c2('2');
  Char c3('3');
  Char c4('4');
  Char c5('5');
  Char c6('6');
  Char c7('7');
  Char c8('8');
  Char c9('9');
  Char ca('A');
  Char cb('B');
  Char cc('C');
  Char cd('D');
  Char ce('E');
  Char cf('F');

  Set<Char>::SetComparisonFunction(CompChar);

  Set<Char> s1;
  Set<Char> s2;
  Set<Char> s3;
  Set<Char> s4;
  Set<Char> s5;
  Set<Char> s6;
  Set<Char> s7;
  Set<Char> s8;  
  
  s1 |= &c0;
  s1 |= &c1;
  s1 |= &c2;
  s1 |= &c3;
  s1 |= &c4;
  s1 |= &c5;
  s1 |= &c6;
  s1 |= &c7;
  s1 |= &c8;
  s1 |= &c9;

  s2 |= &c0;
  s2 |= &c1;
  s2 |= &c2;
  s2 |= &c3;
  s2 |= &c4;
  s2 |= &c5;

  s3 = s2;

  s4 |= &c6;
  s4 |= &c7;
  s4 |= &c8;
  s4 |= &c9;
  s4 |= &ca;
  s4 |= &cb;
  s4 |= &cc;
  s4 |= &cd;

  s5 = s4;
  s5 |= &ca;
  s5 |= &cb;

  s6 = s1 & s4;
  s7 = s1 | s4;
  s8 = s1 - s4;

  assert(s1 > s2);
  assert(s1 >= s2);
  assert(s3 >= s2);
  assert(s3 == s2);
  assert(s1 != s2);

  ShowTheList(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowTheList(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD0123456789");
  ShowTheList(s8.GiveSetData());
  assert(s8.GiveSetData() == "543210");

  Char* CharPtr_;
  CharPtr_ = s7.FindItem(c0);
  assert(CharPtr_ != NULL && CharPtr_->val() == '0');
  CharPtr_ = s7.FindItem(c4);
  assert(CharPtr_ != NULL && CharPtr_->val() == '4');
  CharPtr_ = s7.FindItem(c9);
  assert(CharPtr_ != NULL && CharPtr_->val() == '9');
  CharPtr_ = s7.FindItem(cd);
  assert(CharPtr_ != NULL && CharPtr_->val() == 'D');
  CharPtr_ = s7.FindItem(Char('x'));
  assert(CharPtr_ == NULL);

  s8 = s7 = s6 = s1;
  s6 &= s4;
  s7 |= s4;
  s8 -= s4;

  ShowTheList(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowTheList(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD9876543210");
  ShowTheList(s8.GiveSetData());
  assert(s8.GiveSetData() == "543210");

  assert(s5 > &ca);
  assert(s5 >= &ca);
  assert(s5 > &cb);
  assert(s5 >= &cb);
  assert(!(s5 == &ca));
  assert(!(s5 == &cb));

  assert(!(s5 > s4));
  assert(s5 >= s4);
  assert(s5 == s4);

  s5 -= &ca;
  s5 -= &cb;
  s5 |= &c4;
  s5 |= &c5;

  assert(!(s5 > &ca));
  assert(s5 >= &ca);
  assert(!(s5 > &cb));
  assert(s5 >= &cb);
  assert(s5 == &ca);
  assert(s5 == &cb);

  assert(s5 > s4);
  assert(s5 >= s4);
  assert(!(s5 == s4));

  cout <<"SUCCESS testing <Set>" <<endl;
}

/****************************************************************************/
void TestShortint()
{
  ChrString Str_;

  ShortInt si(31515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515"));

  ShortInt si2(31515);
  ShortInt si3(-21057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057"));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);
  
  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "-32090";
  assert(si2 == -32090);

  si2 = "34000";
  assert(!(si2 == 34000));
  assert(si2 == 0);

  // Testing bounds checking
  ShortInt chk1 = 32767;
  chk1 += ShortInt(5);
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 32767;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 256;
  chk1 = chk1 * ShortInt(128);
  
  assert(chk1.Overflow());
  chk1 = 256;
  chk1 >>= 2;
  assert(short(chk1) == 64);
  chk1 <<= 4;
  assert(short(chk1) == 1024);
  chk1 |= 0x0F;
  assert(short(chk1) == 1039);
  chk1 /= ShortInt(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  ShortInt* List_[25];
  short i;

  for (i = 0; i < 10; ++i)
    List_[i] = new ShortInt(i+1);
  List_[10] = NULL;

  ShortInt ZeroChar(0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<ShortInt> Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];  

  cout <<endl <<"SUCCESS Testing ShortInt" <<endl;
}

/****************************************************************************/
void TestSimpArray()
{
  RANDOMIZE();
  ArrayTest1(cout);
  ArrayTest2(cout);
  ArrayTest8(cout);
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  ArrayTest12(cout);
#endif
}

/****************************************************************************/
void TestSimpList()
{
  SearchableList<short> Alist_;
  SearchableList<short> Alist2_;
  short* Num13_ = new short(13);
  short* Num26_ = new short(26);
  short* Num37_ = new short(37);
  short* Num81_ = new short(81);
  short* Num78_ = new short(78);
  short* Num81b_ = new short(81);

  SimpleNode<short>* Nodep_;
  PtrEqual<short> Comp_(SortUsingBuiltInOperators<short>::Trait());

  Alist_.AppendHead(Num13_);
  Alist_.AppendHead(Num26_);
  Alist_.AppendHead(Num37_);

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.InsertAfter(Alist_.Find(Num26_, Comp_), Num81_);
  Alist_.InsertBefore(Alist_.Find(Num26_, Comp_), Num78_);

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.Remove(Alist_.Find(Num13_));
  Alist_.Remove(Alist_.Find(Num26_));

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.AppendHead(Num81b_);
  Nodep_ = Alist_.Find(Num81b_);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";
  Nodep_ = Alist_.NextMatch(Nodep_);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";

  cout <<endl;
  Alist2_ = Alist_;

  for (Nodep_ = Alist2_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;
}

/****************************************************************************/
void TestSlnklist()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  SingleList<Char> s1, s2, s3;
  size_t i;

  Char* pc4;
  Char* pc5;

  SingleList<Char>::SetComparisonFunction(CompChar);
  SingleListNode<Char> sNode1_(c0);

  s3.AssignListNode(&sNode1_, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 3; i++)
    s3.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
    
  SingleList<Char> s4(s3);
  SingleList<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.AssignListNode(&sNode2_, DataStorageAttributes::DEEP_COPY);
  s6.AssignListNode(&sNode3_, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 9; i++)  
    s6.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);

  SingleList<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.AssignListNode(&sNode4_, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 3; i++)
    s7.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);  
  assert(s1 == "XX");
  s1.AddListNode(SingleListNode<Char>::Make(ca), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(ca), ListRelation::TAIL);
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  SingleListNode<Char> sNode6_(cb);
  SingleListNode<Char> sNode7_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode7_, DataStorageAttributes::DEEP_COPY);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "bbbbb");
  s1.AssignListNode(
    SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE), DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "bXXX");

  // test appends
  s1 = s5;
  s1 += s5; assert(s1 == "aa");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  assert(s1 == "aab");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "aabb");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);    
  assert(s1 == "aabbbbbbb");
  s1.AddListNode(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::TAIL);
  assert(s1 == "aabbbbbbba");  
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "aabbbbbbbaXXX" && s1.Size() == 13);  
  SingleList<Char> s8(s1);
  s1 = s4 + s5; assert(s1 == "XXXXa" && s1.Size() == 5);

  // test inserts, removes, etc.
  s1 = s4;
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(*s5.GetNode(0)), 0);
  ShowTheList(s1);
  assert(s1 == "XaXXX" && s1.Size() == 5);
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(cb), 1);
  assert(s1 == "XabXXX" && s1.Size() == 6);
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), 5);
  assert(s1 == "XabXXXa" && s1.Size() == 7);

  delete s1.DetachFromMiddle(3);
  assert(s1 == "XabXXa" && s1.Size() == 6);

  pc4 = s1.ListToArray();
  SingleList<Char>* cs7p = (SingleList<Char>*)s7.Clone();
  pc5 = cs7p->ListToArray();
  assert(pc4[1] == ca && pc5[2] == c0);
  delete cs7p;
  ::Delete(pc4);
  ::Delete(pc5);

  // Test class hierarchy extensions
  // Test virtual object constructor
  SingleList<Char>* ps1 = SingleList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  SingleList<Char>* ps2 = SingleList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXaaabbbbbbb");

  SingleList<int> s9;
  SingleListNode<int> sNode0_(0);
  s9.AssignListNode(&sNode0_, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 10; i++)
    s9.AddListNode(SingleListNode<int>::Make(i * 2), ListRelation::NEXT);

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*s9.GetNode(i)->GetItem() == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  SingleList<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10.GetNode(8)->SetItem(Char('c'));
  s10.GetNode(10)->SetItem(Char('c'));
  SimpleArray<int> Result_ = s8.SeriesListCompare(s10);

#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s1.AssignListNode(&sNode9_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s1.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s2.AssignListNode(&sNode10_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    s2.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 75;
  SingleListNode<Char> sNode11_(c3.Set(x));
  s3.AssignListNode(&sNode11_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s3.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2 += s3;
  assert(s2.Size() == 10);
  s1 += s2;

  assert(s1 == "ABCDEZYXWVKLMNO" && s1.Size() == 15);
  s1.ReverseList();
  assert(s1 == "ONMLKVWXYZEDCBA" && s1.Size() == 15);

  size_t Index_;
  SingleListNode<Char>* FoundNode_ = s1.FindItem(Char('V'), Index_);
  assert(FoundNode_ != NULL && Index_ == 5 && FoundNode_->GetItem()->val() == 'V');

  s1.Flush();
  s1 = s4;

  Char** ptrarr_ = new Char*[10];
  for (i = 0; i < 10; i++)
    ptrarr_[i] = new Char('0' + i);

  s1.Flush();
  s1.PtrArrayToList(ptrarr_, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  Char** ptrarr2_ = s1.ListToPtrArray();
  assert(ptrarr2_[0]->val() == '0');
  assert(ptrarr2_[1]->val() == '1');
  assert(ptrarr2_[2]->val() == '2');
  assert(ptrarr2_[3]->val() == '3');
  assert(ptrarr2_[4]->val() == 'F');
  assert(ptrarr2_[5]->val() == 'E');
  assert(ptrarr2_[6]->val() == '6');
  assert(ptrarr2_[7]->val() == '7');
  assert(ptrarr2_[8]->val() == '8');
  assert(ptrarr2_[9]->val() == '9');
  ::Delete(ptrarr2_);

  for (i = 0; i < 10; i++)
    delete ptrarr_[i];
  delete[] ptrarr_;

  pc4 = new Char[10];
  for (i = 0; i < 10; i++)
    pc4[i] = Char('0' + i);

  s1.Flush();
  s1.ArrayToList(pc4, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  pc5 = s1.ListToArray();
  assert(pc5[0].val() == '0');
  assert(pc5[1].val() == '1');
  assert(pc5[2].val() == '2');
  assert(pc5[3].val() == '3');
  assert(pc5[4].val() == 'F');
  assert(pc5[5].val() == 'E');
  assert(pc5[6].val() == '6');
  assert(pc5[7].val() == '7');
  assert(pc5[8].val() == '8');
  assert(pc5[9].val() == '9');
  ::Delete(pc5);

  delete[] pc4;

  s1 = s4;
  s2 = s4;
  s3 = s4;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s2));
  assert(s3.IsGreater(s2));

  assert(s1 == s4);
  assert(s1 < s2);
  assert(s3 > s2);
  assert(s1 != s3);

  cout <<"SUCCESS testing <SingleList>" <<endl;
}

/****************************************************************************/
void TestSlotFile()
{
  char Header_[128];
  StrFill(Header_, '\0', 128);
  strcpy(Header_, "This Is The Header Of The File");

  ShortInt NullRec_;
  ShortInt UseRec_;
  ShortInt Record1_(1000);
  ShortInt Record2_(210);
  ShortInt Record3_(3250);
  ShortInt Record4_(440);
  ShortInt Record5_(5700);

  SlottedFile* Sfile_ =
  SlottedFile::Make(SlottedFileInfo::Make(NullRec_, 0, 128));
  Sfile_->SetFileProperties("sample.dat", ios::out|ios::in|ios::binary);

  Sfile_->Open();

  Sfile_->AddRecord(Record1_);
  Sfile_->AddRecord(Record2_);
  Sfile_->AddRecord(Record3_);
  Sfile_->StoreIDToHeader();
  Sfile_->AddRecord(Record4_);
  Sfile_->AddRecord(Record5_);

  long Index_ = 0;
  while ((Index_ = Sfile_->NextRecord(Index_, UseRec_)) != -1)
    cout <<UseRec_ <<endl;

  Sfile_->FirstRecord(UseRec_);
  cout <<UseRec_ <<endl;

  UseRec_ = 21600;
  long Index2_ = Sfile_->NthRecordSlot(2);
  Sfile_->ModifyRecord(Index2_, UseRec_);

  Index_ = Sfile_->NthRecordSlot(1);
  Sfile_->DeleteRecord(Index_);

  if (Sfile_->RetrieveRecord(Index_, UseRec_))
    cout <<UseRec_ <<endl;

  if (Sfile_->RetrieveRecord(Index2_, UseRec_))
    cout <<UseRec_ <<endl;

  long Sz_ = Sfile_->SlotsAllocated() + 1;
  long* IDvalues_ = new long[Sz_];
  Sfile_->ReadIDFromHeader(IDvalues_);
  cout <<"Rec Ids:";

  for (Index_ = 0; Index_ < Sz_; ++Index_)
    cout <<" " <<IDvalues_[Index_];
  delete[] IDvalues_;

  Sfile_->WriteHeader((char*)Header_);
  Sfile_->ReadHeader((char*)Header_);
  cout <<endl <<Header_ <<endl;

  cout <<TypeInfo::GiveClassName(Sfile_->IdentifyRecord(Index2_)) <<endl;

  Sfile_->Close();
  cout <<Sfile_->IsSlottedFile("sample.dat") <<endl;

  cout <<"SUCCESS testing <SlotFile>" <<endl;
}

/****************************************************************************/
void TestSmartPtr()
{
  int val;
  EnclosedClass cpobjp_data(1);
  cpobjp_data.SetOwner("cpobjp");
  CopiedObjPtr<EnclosedClass> cpobjp(cpobjp_data);
  CopiedBuiltInPtr<int>* cpbip = new CopiedBuiltInPtr<int>(new int(5));
  CopiedBuiltInPtr<int> cpbip2(new int(7));

  val = **cpbip;
  assert(val == 5);
  val = *cpbip2;
  assert(val == 7);
  *cpbip = cpbip2;
  val = **cpbip;
  assert(val == 7);
  *cpbip = 9;
  val = **cpbip;
  assert(val == 9);
  delete cpbip;

  int* dap = cpbip2.ReleaseControl();
  assert(*dap == 7);

  CountedVoidPtr<void> vptr;
  EnclosedClass* cnobjp_data = new EnclosedClass(2);
  cnobjp_data->SetOwner("cnobjp");
  CountedObjPtr<EnclosedClass>* cnobjp = new CountedObjPtr<EnclosedClass>(cnobjp_data);
  delete cnobjp;

  CountedBuiltInPtr<int>* cnbip = new CountedBuiltInPtr<int>(new int(15));
  val = **cnbip;
  assert(val == 15);
  CountedBuiltInPtr<int>* cnbip2 = cnbip->Clone();
  val = **cnbip2;
  assert(val == 15);

  int i11 = 11;
  *cnbip2 = &i11;
  val = **cnbip2;
  assert(val == 11);

  *cnbip2 = *cnbip;
  val = **cnbip2;
  assert(val == 15);
  delete cnbip;

  assert(cnbip2->Unique());

  EnclosedClass clon2_data(3);
  clon2_data.SetOwner("clon2");
  CopiedObjPtr<EnclosedClass> clon2(clon2_data);
  CountedBuiltInPtr<int> cnbip3(new int[10]);
  ArrayPtr<int> cap1(cnbip3, 10);

  int i;
  for (i = 0; i < 10; ++i)
    cap1[i] = (i + 1);

  for (i = 0; i < 9; ++i)
  {
    val = *cap1++;
    assert(val == i + 1);
  }
  assert(*cap1 == 10);

  EnclosedClass* w1 = new EnclosedClass();
  EnclosedClass* w2 = new EnclosedClass();
  EnclosedClass* w3 = new EnclosedClass();

  delete w1;
  delete w2;
  delete w3;
  delete cnbip2;

/// Testing object updater classes
  MasterClass mc2;
  MasterClass mc;
  assert(mc.ShowX() == 10);
  assert(mc.ShowDupv() == 10);
  mc.ReturnAndUpdate().Pointee()->SetX(mc2.ReturnAndUpdate().Pointee()->showX() * 100 + 99);
  assert(mc.ShowX() == 1099);
  assert(mc.ShowDupv() == 1099);

  UpdatePtr<EnclosedClass> _Uptr2(mc.ReturnAndUpdate());
  mc.MakeAnother();
  assert(mc.ShowX() == 1099);
  assert(mc.ShowDupv() == 1099);
  assert(mc._Next->ShowX() == 10);
  assert(mc._Next->ShowDupv() == 10);
  mc.ReturnAndUpdate().Pointee()->SetX(mc.ReturnAndUpdate().Pointee()->showX() * 100 + 22);
  assert(mc.ShowX() == 109922);
  assert(mc.ShowDupv() == 109922);
  assert(mc._Next->ShowX() == 109922);
  assert(mc._Next->ShowDupv() == 109922);

/// Testing Const Proxy classes
  EnclosedClass Wsponsor_;
  Wsponsor_.SetX(780021);
  ConstProxyObjectPtr<MasterClass, EnclosedClass, Boolean> ConstProxObj_(&mc2, &MasterClass::ConstUpdateReturnRef, &Wsponsor_);
  if (ConstProxObj_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);

  ConstProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ConstProxPtr1_ = &ConstProxObj_;
  if (*ConstProxPtr1_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);    

  Wsponsor_.SetX(Wsponsor_.showX() + 1100);
  ConstProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ConstProxPtr2_(&mc2, &MasterClass::ConstUpdateReturnPtr, &Wsponsor_);
  if (*ConstProxPtr2_)
  {
    assert(mc2.ShowX() == 781121);
    assert(mc2.ShowDupv() == 781121);
  }
  else
    assert(0);

/// Testing Non-Const Proxy classes
  Wsponsor_.SetX(780021);
  ProxyObjectPtr<MasterClass, EnclosedClass, Boolean> ProxObj_(&mc2, &MasterClass::UpdateReturnRef, &Wsponsor_);
  if (ProxObj_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);

  ProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ProxPtr1_ = &ProxObj_;
  if (*ProxPtr1_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);    

  Wsponsor_.SetX(Wsponsor_.showX() + 1100);
  ProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ProxPtr2_(&mc2, &MasterClass::UpdateReturnPtr, &Wsponsor_);
  if (*ProxPtr2_)
  {
    assert(mc2.ShowX() == 781121);
    assert(mc2.ShowDupv() == 781121);
  }
  else
    assert(0);

/// Testing Built-in updater classes
  int svar = 5;         // sponsor pointer
  int cvar = 10;        // client pointer
  CountedBuiltInPtr<int> cntvar(&svar);
  UpdatePtr<int> cntudp(cntvar, BuiltInUpdater<int, int>::Make(&cvar));
  *cntudp.Pointee() += 10;
  cntudp.Update();
  assert(*cntudp.Pointee() == 15);
  assert(cvar == 15);

/// Testing void pointer classes, Built-in deleter class
  CountedVoidPtr<void> cvp1_(new short(5), BuiltInDeleter::Make());
  CountedVoidPtr<void> cvp2_(new short(10), BuiltInDeleter::Make());
  cvp1_ = cvp2_;

/// Testing temporary object class
  EnclosedClass* Ptr_ = new EnclosedClass;
  TempObject<EnclosedClass> Temp = Ptr_;
  assert(Temp.Pointee() == Ptr_);

// Testing containership of complex classes
  CopiedObjPtr<SimpleStr> cpobjpstr(SimpleStr("Hello"));

  cout <<"SUCCESS Testing SmartPtr classes" <<endl;
}

/****************************************************************************/
void TestSortList()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  SortedList<Char> s1, s2, s3;
  size_t i;
  Char* pc5 = &c0;

  SortedList<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Insert(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Insert(SingleListNode<Char>::Make(c0));
    
  SortedList<Char> s4(s3);
  SortedList<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Insert(&sNode2_);
  s6.Insert(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Insert(SingleListNode<Char>::Make(cb));

  SortedList<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.Insert(&sNode4_);
  for (i = 1; i < 3; i++)
    s7.Insert(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Insert(SingleListNode<Char>::Make(ca));
  s1.Insert(SingleListNode<Char>::Make(ca));
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "Xaa");

  // test assigns
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Insert(SortedList<Char>::Make(s5)); assert(s1 == "aa");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aab");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aabb");
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));    
  assert(s1 == "aabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "aaabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXaaabbbbbbb" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXaaabbbbbbb" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == c0);
  assert(s1 == "Xaaabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "aaabbbbbbb" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "aabbbbbbb" && s1.Size() == 9);
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  ::Delete(Item_);
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  ::Delete(Item_);  
  assert(s1 == "bbbbbbb" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "bb" && s1.Size() == 2);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "b" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  SortedList<Char>* ps1 = SortedList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  SortedList<Char>* ps2 = SortedList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode7_(c3.Set(x));
  s1.Insert(&sNode7_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode8_(c3.Set(x));
  s2.Insert(&sNode8_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s3.Insert(&sNode9_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "ABCDE" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "KLMNY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

  cout <<"SUCCESS testing <SortedList>" <<endl;
}

/****************************************************************************/
void TestSorts()
{
  char filename1[] = "testfile.bin";
  char tempfile1[] = "tempf1.bin";
  char tempfile2[] = "tempf2.bin";

  fstream file;
  int arr[100];
  int i, valid;

  RANDOMIZE();
  ItemSorter<int, int> Sorter_;
  Sorter_.SetSortHelper(ObjectSortHelper<int, int>::Make());

  // testing quicksort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing shellsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing heapsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing mergesort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing selectsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, arr[i]);
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, arr[i]);
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing linear search
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);
  ArraySize out1;

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

/// Reverse Order
  Sorter_.SetOrder(SortOrder::DECREASING);

  // testing quicksort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing shellsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing heapsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing mergesort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing selectsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, arr[i]);
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, arr[i]);
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing linear search
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);  
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);
  cout <<"SUCCESS testing <ItemSorter> with objects" <<endl;
}

/****************************************************************************/
void TestStack()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  Stack<Char> s1, s2, s3;
  size_t i;
  Char* pc5 = &c0;

  Stack<Char>::SetComparisonFunction(CompChar);
  SingleListNode<Char> sNode1_(c0);

  s3.Push(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Push(SingleListNode<Char>::Make(c0));
    
  Stack<Char> s4(s3);
  Stack<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Push(&sNode2_);
  s6.Push(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Push(SingleListNode<Char>::Make(cb));

  Stack<Char> s7;
  SingleListNode<Char>sNode4_(c0);
  
  s7.Push(&sNode4_);
  for (i = 1; i < 3; i++)
    s7.Push(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Push(SingleListNode<Char>::Make(ca));
  s1.Push(SingleListNode<Char>::Make(ca));
  assert(s1 == "aaXX");
  s1.ShrinkTo(3); assert(s1 == "aXX");

  // test assigns
  SingleListNode<Char> sNode6_(cb);
  SingleListNode<Char> sNode7_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode7_, DataStorageAttributes::DEEP_COPY);
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Push(Stack<Char>::Make(s5)); assert(s1 == "aa");
  s1.Push(SingleListNode<Char>::Make(cb));
  assert(s1 == "baa");
  s1.Push(SingleListNode<Char>::Make(cb));
  assert(s1 == "bbaa");
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));    
  assert(s1 == "bbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "abbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXabbbbbbbaa" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == c0);
  assert(s1 == "Xabbbbbbbaa" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "abbbbbbbaa" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "bbbbbbbaa" && s1.Size() == 9);
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  ::Delete(Item_);
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  ::Delete(Item_);  
  assert(s1 == "bbbbbaa" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "aa" && s1.Size() == 2);  
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "a" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  Stack<Char>* ps1 = Stack<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Stack<Char>* ps2 = Stack<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode8_(c3.Set(x));
  s1.Push(&sNode8_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Push(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s2.Push(&sNode9_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Push(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s3.Push(&sNode10_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Push(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "EDCBA" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "ONMLY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

  cout <<"SUCCESS testing <Stack>" <<endl;
}

/****************************************************************************/
void TestStrBrows()
{
  ChrString str1_;
  StringBrowser sb_("Once, upon, a, time, in, a, galaxy, far far, away.");
  sb_.Tokenizer().SetDelimiters(",");

  str1_ = sb_.FindToken();
  assert(str1_ == "Once");
  str1_ = sb_.FindToken();
  assert(str1_ == "upon");
  str1_ = sb_.FindToken();  
  assert(str1_ == "a");
  str1_ = sb_.FindToken();  
  assert(str1_ == "time");
  str1_ = sb_.FindToken();  
  assert(str1_ == "in");
  str1_ = sb_.FindToken();  
  assert(str1_ == "a");
  str1_ = sb_.FindToken();  
  assert(str1_ == "galaxy");
  str1_ = sb_.FindToken();  
  assert(str1_ == "far far");
  str1_ = sb_.FindToken();  
  assert(str1_ == "away.");

  cout <<"SUCCESS testing StringBrowser" <<endl;
}

/****************************************************************************/
void TestStreamReader()
{
  StreamReader Sr_;
  ios& iosRef = Sr_.GetIos();

  const char* Array_[] =
  {
    "Fubarred",
    "Anotherd",
    "Yippee Yahoo",
    "Hello World!",
    "Blah Blah Blah",
    NULL
  };

  Sr_.WriteLines(Array_);
  
  IosFormat Form_;
  Form_.SetFormat(ios::skipws | ios::dec);
  Form_.SetWidth(12);
  Form_.SetFill('*');
  Form_.SetPrecision(2);
  Sr_.SetFormat(Form_);

  double SomeFloat_ = 3.1415927;
  (ostream&)(Sr_) <<SomeFloat_ <<endl;

  (ostream&)(Sr_) <<IosReset();
  (ostream&)(Sr_) <<SomeFloat_ <<endl;

  StreamReader* SrPtr_ = new StreamReader;
  (ostream&)(*SrPtr_) <<"testing" <<endl;
  delete SrPtr_;
}

/****************************************************************************/
void TestString()
{
  ChrString Str1_;
  ChrString Str2_("Hello World");
  ChrString Str3_("GoodBye World", 8);
  ChrString Str4_('x', 5);
  ChrString Str5_(Str3_);
  ConstChrStringPtr StrPtr1a_(Str1_);
  ConstChrStringPtr StrPtr2_(Str2_);
  ConstChrStringPtr StrPtr2a_(Str2_);

  ChrStringPtr StrPtr6_;
  ChrStringPtr StrPtr7_;
  ChrStringPtr StrPtr8_;
  ChrString* Str6_ = ChrString::Make();
  ChrString* Str7_ = ChrString::Make("Hello World");
  ChrString* Str8_ = ChrString::Make("GoodBye World", 10);
  ChrString* Str9_ = ChrString::Make('x', 5);
  ChrString* Str10_ = ChrString::Make(*Str8_);
  StrPtr6_ = *Str6_;
  StrPtr7_ = *Str7_;
  StrPtr8_ = *Str8_;
  ChrStringPtr StrPtr9_(*Str9_);
  ChrStringPtr StrPtr10_(*Str10_);

  // testing indexing
  Subscript Lp_; Boolean Ok_; Boolean LpOk_;
  for (Lp_ = 0; (LpOk_ = Str2_[Lp_].valid()); ++Lp_)
  {
    Ok_ =
	Lp_ == 0 ? *Str2_ == 'H':
	Lp_ == 1 ? *Str2_(Lp_) == 'e':
	Lp_ == 2 ||
	Lp_ == 3 ||
	Lp_ == 9 ? Str2_[Lp_] == 'l':
	Lp_ == 4 ||
	Lp_ == 7 ? Str2_[Lp_] == 'o':
	Lp_ == 5 ? Str2_[Lp_] == ' ':
	Lp_ == 6 ? Str2_[Lp_] == 'W':
	Lp_ == 8 ? Str2_[Lp_] == 'r':
	Lp_ == 10 ? Str2_[Lp_] == 'd':
	Lp_ == 11 ? Str2_[Lp_] == 0:0;
    assert(Ok_);
  }
  assert(Lp_ == 12);
  assert(!LpOk_);
  assert(!Str2_.IndexInRange(Lp_));
  assert(Str2_.RestrictIndex(Lp_) == 11);

  // Testing assignment, case conversion, justification
  Str1_ = Str2_;
  assert(StrPtr1a_.IsOld());
  Str2_.ToUpper();
  StrPtr7_->ToLower();
  Str3_.RightJustify(20);
  StrPtr8_ -= "Wo";
  StrPtr8_->Delete(0, 4);
  Str8_->LeftJustify(20);
  assert(StrPtr8_.IsOld());
  StrPtr8_ = *Str8_;
  assert(!StrPtr8_.IsOld());
  Str4_.CenterJustify(20);

  // Testing duplication, insertion
  Str1_.Duplicate(3);
  *Str6_ = *Str7_;
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());
  Str6_->Insert(3, "Inserted");
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());

  assert(Str1_ == "Hello WorldHello WorldHello WorldHello World");
  assert(StrPtr2_.EnclosedObject() == "HELLO WORLD");
  assert(Str3_ == "            GoodBye ");
  assert(Str4_ == "       xxxxx        ");
  assert(Str5_ == "GoodBye ");
  assert(*Str6_ == "helInsertedlo world");
  assert(*Str7_ == "hello world");
  assert(StrPtr8_.EnclosedObject() == "Bye                 ");
  assert(StrPtr9_.EnclosedObject() == "xxxxx");
  assert(*Str10_ == "GoodBye Wo");

  // testing string tokenizing
  char* Buffer_ = new_char_array(20, NULL);
  StringToken Tok_(Str1_.c_str(), 0, " ", " ");
  Tok_.FindToken(Buffer_);
  assert(!strcmp(Buffer_, "Hello"));
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(!strcmp(Buffer_, "WorldHello"));
  }
  Tok_.FindToken(Buffer_);
  assert(!strcmp(Buffer_, "World"));
  ::DeleteArray(Buffer_);

  // testing deletion, overwriting, replacement
  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, "Bye  ");
  assert(!StrPtr7_.IsOld());
  Str1_.Replace(0, "ll", "nn");

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  // testing string searches
  ConstChrStringPtr StrPtr1_(Str1_);
  Fallible<Subscript> Fall1_ = StrPtr1_->find("Bye");
  Fallible<Subscript> Fall2_ = StrPtr1_->findlast("Good");
  Subscript Pos3_ = StrPtr1_->find("World");
  Subscript Pos4_ = StrPtr1_->findlast("Henno");
  Subscript Pos5_ = StrPtr1_->findnext(Pos3_, "World");
  Subscript Pos6_ = StrPtr1_->findprevious(Pos4_, "Henno");

  // testing BASIC emulation -- Left, Mid, Right methods
  ChrString Str11_ = Str3_.Right(5);
  ChrString Str12_ = Str8_->Left(5);
  ChrString Str13_ = Str4_.Mid(10, 10);

  assert((int)Fall1_.valid() == 0);
  assert((int)Fall2_.valid() == 0);
  assert(Pos3_ == 6);
  assert(Pos4_ == 33);
  assert(Pos5_ == 17);
  assert(Pos6_ == 22);
  assert(Str11_ == "dBye ");
  assert(Str12_ == "Bye  ");
  assert(Str13_ == "xx        ");

  // testing subtraction
  Str1_ -= "no World";
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHen");

  // testing deletion
  ChrString Str14_ = "JoKCsephKCWong";
  Str14_.Delete("KC");
  assert(Str14_ == "JoKCsephWong");
  ChrString Str15_ = Delete(Str14_, "KC");
  assert(Str15_ == "JosephWong");

  // testing addition
  Str15_ += " This is a test of appending";
  assert(Str15_ == "JosephWong This is a test of appending");

  // testing searches again
  Pos3_ = (Subscript)Str15_.find('s');
  Pos4_ = (Subscript)Str15_.findlast('s');
  Pos5_ = (Subscript)Str15_.findnext(Pos3_, 's');
  Pos6_ = (Subscript)Str15_.findprevious(Pos4_, 's');
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  // testing appending
  Str15_ -= " This is a test of appending";
  assert(Str15_ == "JosephWong");
  assert((Str15_ + " This is a test of appending") == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  // brk, span and cspan methods
  Fall1_ = Str15_.brk("eph");
  assert(Fall1_.valid());
  Pos3_ = (Subscript)Fall1_;
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span("toseJy");
  Pos5_ = Str15_.cspan("Wpht");

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  // testing null object return method
  Object* Ptr_ = &Str6_->NULLOBJECT();
  assert(Ptr_->IsNullObject());

  // testing data sharing, memory management
  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

  cout <<endl <<"SUCCESS Testing ChrString" <<endl;
}

/****************************************************************************/
void TestStrio()
{
  ShowTitle(cout, "Hello World");
  ShowTitle(cout, "GoodbyeWorld");
}

/****************************************************************************/
void TestStrStore()
{
  char* CharData_ = new_char_string("hello world!");
  char* CharData2_ = new_char_string("testing the array store");
  
  StringStorePtr* asp1_ = new StringStorePtr(CharData_, DataStorageAttributes::DEEP_COPY);
  StringStorePtr asp2_(CharData_, DataStorageAttributes::DESTROY);
  ArrayStorePtr<char> asp3_(CharData2_, strlen(CharData2_) + 1, DataStorageAttributes::DEEP_COPY);
  ArrayStorePtr<char>* asp4_ = new ArrayStorePtr<char>(CharData2_, strlen(CharData_) + 1, DataStorageAttributes::DESTROY);

  DataStorePtr<char> psp1_ = *asp1_;
  DataStorePtr<char> psp2_;
  psp2_ = asp2_;
  assert(strcmp((*asp1_)(6), "world!") == 0);
  assert(asp2_[4] == 'o');
  assert(asp3_.Length() == 24);
  assert(strcmp(asp3_(12), "array store") == 0);
  assert((*asp4_)[12] == 'a');
  assert(asp2_(11) == &CharData_[11]);
  assert(asp2_(12) == &CharData_[12]);
  assert(asp2_(13) == NULL);
  assert(strcmp(asp3_.Pointee(), asp4_->Pointee()) == 0);
  assert(strcmp(asp2_.Pointee(), asp3_.Pointee()) != 0);
  assert(*psp1_ == **asp1_);
  assert(*psp2_ == *asp2_);

  cout <<"SUCCESS Testing StringStorePtr classes" <<endl;
}

/****************************************************************************/
void TestStrutil()
{
  long double ldval;
  char buffer[21];

  LongToStr(10, buffer, 1);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "1111111111"));
  ldval = strtold("-3515781642.5132501e+3", NULL);
  cout <<ldval <<endl;
  //assert(ldval == -3515781642.5132501e+3);
  LongToStr(23500, buffer);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "23500"));
  LongToStr(-57523, buffer);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "-57523"));
  LongToStr(LONG_MAX, buffer);
  cout <<buffer <<endl;
  LongToStr(LONG_MAX, buffer, 16);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MAX, buffer);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MAX, buffer, 16);
  cout <<buffer <<endl;
  LongToStr(LONG_MIN, buffer);
  cout <<buffer <<endl;
  LongToStr(LONG_MIN, buffer, 16);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MIN, buffer);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MIN, buffer, 16);
  cout <<buffer <<endl;
  FloatToStr(-543.212394, buffer, 21, 10);
  cout <<buffer <<endl;
  FloatToStr(-543.212394e8, buffer, 21, 10);
  cout <<buffer <<endl;

  UintToStr(0xCAFE, buffer, 16);
  cout <<AddPosSign(buffer) <<endl;
  assert(!strcmp(buffer, "+0xCAFE"));
  IntToStr(-0x535A, buffer, 16);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "-0x535A"));

  cout <<IsSignedNum("-45231") <<endl;
  cout <<IsSignedNum("45-231") <<endl;
  cout <<IsSignedNum("-3515.51501") <<endl;
  cout <<IsSignedNum("-0xdeadbeef") <<endl;
  cout <<IsFloat("0x-deadbeef") <<endl;
  cout <<IsFloat("-3515.51501e15") <<endl;
  cout <<IsFloat("-3515.51501e-15") <<endl;
  cout <<IsFloat("-351551501e+1.5") <<endl;
  cout <<IsFloat("-3515.51501e") <<endl;
  cout <<IsFloat("0.5150") <<endl;
  cout <<IsFloat(".5150") <<endl;
  cout <<IsFloat("-0xdeadbeefe+12") <<endl;
  cout <<IsFloat("-0xdead.beefe-12") <<endl;
  cout <<IsFloat("-.e+5") <<endl;
  cout <<IsFloat(".e+5") <<endl;
  cout <<IsFloat("-e+5") <<endl;
  cout <<IsFloat("e+5") <<endl;
  cout <<IsFloat(".") <<endl;
  cout <<IsUnsignedNum("-65535") <<endl;
  cout <<IsUnsignedNum("65535") <<endl;
  cout <<IsUnsignedNum("-0xfeedcad") <<endl;
  cout <<IsUnsignedNum("0xfeedcad") <<endl;
  cout <<IsSignedNum("-") <<endl;
  cout <<IsInt("32768") <<endl;
  cout <<IsInt("42767") <<endl;
  cout <<IsInt("33767") <<endl;
  cout <<IsInt("32767") <<endl;

  cout <<IsInt("-32768") <<endl;
  cout <<IsInt("-42767") <<endl;
  cout <<IsInt("-33767") <<endl;
  cout <<IsInt("-32767") <<endl;
  cout <<IsInt("0xFFFF") <<endl;
  cout <<IsInt("0x7FFF") <<endl;
  cout <<IsInt("-0x8000") <<endl;

  cout <<IsUint("65536") <<endl;
  cout <<IsUint("65535") <<endl;
  cout <<IsUint("75535") <<endl;
  cout <<IsUint("66535") <<endl;
  cout <<IsUint("-0xFFFF") <<endl;
  cout <<IsUint("0xFFFF") <<endl;
  cout <<IsUint("-1") <<endl;
  cout <<IsLong("2147483648") <<endl;
  cout <<IsLong("-2147483648") <<endl;
  cout <<IsUlong("-1") <<endl;
  cout <<IsUlong("4294967295") <<endl;
}

/****************************************************************************/
void TestDate()
{
  checkDate(0, 3, 14);          // invalid year
  checkDate(1997, 0, 12);       // invalid month
  checkDate(1998, 13, 1);       // invalid month
  checkDate(97, 1, 12);         // invalid year
  checkDate(1999, 2, 29);       // invalid day
  checkDate(1900, 2, 29);       // invalid day

  Date empty(0, 0, 0);
  cout <<"empty: \"" <<empty.ToString() <<"\"\n";
  Date empty2(ChrString(""));
  cout <<"empty2: \"" <<empty2.ToString() <<"\"\n";

  Date d1 = ChrString("20000101");
  Date d2(2000, 3, 1);
  cout <<"Day of week for d1:" <<d1.ToString() <<" == "
       <<DayText[d1.DayOfWeek()] <<endl;
  cout <<"Day of week for d2:" <<d2.ToString() <<" == "
       <<DayText[d2.DayOfWeek()] <<endl;

  Date p1(1996, 2, 29);
  Date p2("20000229");

  cout <<"p1 == " <<p1.ToString() <<endl;
  cout <<"p2 == " <<p2.ToString() <<endl;

  DateStuff::Duration d = p1.AgeBetween(p2);
  cout <<"Duration between p1 and p2: "
       <<d._Year <<" years, "
       <<d._Month <<" months, "
       <<d._Day <<" days\n";

  DateStuff::Duration dd = p1.AgeBetween(d2);
  cout <<"Duration between p1 and d2 (20000301): "
       <<dd._Year <<" years, "
       <<dd._Month <<" months, "
       <<dd._Day <<" days\n";

  if (p1 < p2)
  {
    p1.AddDays(d._Day);
    p1.AddMonths(d._Month);
    p1.AddYears(d._Year);
  }
  else if (p1 > p2)
  {
    p2.AddDays(d._Day);
    p2.AddMonths(d._Month);
    p2.AddYears(d._Year);
  }

  cout <<"p1 == p2 (after adding Duration(p1,p2)): "
       <<((p1 == p2) ? "Yes":"No") <<endl;

  FullDate today;
  Date f(today);
  cout <<"Today: " <<f.ToString() <<endl;
  f.AddWeekDays(2);
  cout <<" +2 weekdays: " <<f.ToString() <<endl;
  f.AddWeekDays(10);
  cout <<" +10 weekdays: " <<f.ToString() <<endl;
  f.AddWeekDays(30);
  cout <<" +30 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(30);
  cout <<" -30 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(10);
  cout <<" -10 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(2);
  cout <<" -2 weekdays: " <<f.ToString() <<endl;

  cout <<"WeekdaysBetween 12/11/96 and 12/13/96: "
       <<Date(1996, 12, 11).WeekDaysBetween(Date(1996, 12, 13))
       <<endl;
  cout <<"WeekdaysBetween 12/13/96 and 12/27/96: "
       <<Date(1996, 12, 13).WeekDaysBetween(Date(1996, 12, 27))
       <<endl;
  cout <<"WeekdaysBetween 12/27/96 and 2/7/97: "
       <<Date(1996, 12, 27).WeekDaysBetween(Date(1997, 2, 7))
       <<endl;

  FullDate f1(d1), f2(d2);
  DateStuff::Duration f12 = f2 - f1;
  cout <<"d1 - d1 == "
       <<f12._Year <<" years, "
       <<f12._Month <<" months, "
       <<f12._Day <<" days\n";
}

/****************************************************************************/
void TestPtrDynarray()
{
  // test basic workings
  static Char c0, ca('a'), cb('b'), c3[3];
  static Char* pc3[] = {&ca, &cb, &c0};
  PtrDynArray<Char> s1, s2(30, reserve), s3(4, default_size);
  PtrDynArray<Char> s4(s3), s5(&ca), s6(&cb, 10);
  PtrDynArray<Char> s7(pc3, 3);
//  const PtrDynArray<Char> s7(pc3, 3);

  s1.SetComparisonFunction(CompCharPtr);
  assert(s1.RunLength() == 0);
  s1.Reserve(10);
  assert(s1.RunLength() == 0 && s1.Reserve() == 10);
  assert(s2.RunLength() == 0 && s2.Reserve() == 30);
  assert(s3.RunLength() == 4);
  assert(s4.RunLength() == 4);
  assert(s5.RunLength() == 1 && s5[0] == &ca);
  assert(s6.RunLength() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.RunLength() == 3 && s7 == "abX");
  s1.Resize(2), s1[0] = &c0, s1[1] = &c0, assert(s1 == "XX");
  s1.Resize(4, &ca); assert(s1 == "XXaa");
  s1.Resize(3); assert(s1 == "XXa");

  // test assigns
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignArg(&cb); assert(s1 == "b");
  s1.AssignArg(&cb, 5); assert(s1 == "bbbbb");
  s1.AssignArg(pc3), assert(s1 == "a");
  s1.AssignArg(pc3, 3); assert(s1 == "abX");

  // test appends
  s1 = s5, s1 += s5, assert(s1 == "aa");
  s1 += &cb, assert(s1 == "aab");
  s1.Append(&cb), assert(s1 == "aabb");
  s1.Append(&cb, 5), assert(s1 == "aabbbbbbb");
  s1.Append(pc3), assert(s1 == "aabbbbbbba");
  s1.Append(pc3, 3), assert(s1 == "aabbbbbbbaabX");
  s1 = s6;
  s1 += s7, assert(s1 == "bbbbbbbbbbabX");
  s1 = &ca + s7, assert(s1 == "aabX");
  s1 = s7 + &cb, assert(s1 == "abXb");

  // test inserts, removes, etc.
  s1 = s7, s1.Insert(1, s7), assert(s1 == "aabXbX");
  s1.Insert(2, &cb), assert(s1 == "aabbXbX");
  s1.Insert(0, &ca, 3), assert(s1 == "aaaaabbXbX");
  s1.Insert(10, pc3), assert(s1 == "aaaaabbXbXa");
  s1.Insert(1, pc3, 3), assert(s1 == "aabXaaaabbXbXa");
  PtrDynArray<Char> s8(s1);
  s1.Remove(7), assert(s1 == "aabXaaa");
  s1.Remove(2, 1), assert(s1 == "aaXaaa");
  s1.Remove(), assert(s1 == "");
  s6.Sub_Array(s1, 2);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.Sub_Array(s1, 3, 2), assert(s1 == "bb");
  s1.Swap(s6), assert(s1 == "bbbbbbbbbb" && s6 == "bb");

  // test access functions
  s1 = s7, assert(s1[0] == &ca);
  s1[0] = &cb, assert(s1[0] == &cb);
  s1.Put_At(1, &ca), assert(s1.Get_At(1) == &ca);
  assert(s1.Base()[1] == &ca && s1.Base()[2] == &c0);

  // test sort method
  s8.Sort(PointerSortHelper<Char*, Char>::Make());
  assert(s8 == "XXXaaaaaaabbbb");

  PtrDynArray<int> s9(size_t(10), default_size);
  size_t i;
  for (i = 0; i < 10; i++)
    s9[i] = new int(i * 2);

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*s9[i] == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  PtrDynArray<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10[8] = new Char('c');
  s10[10] = new Char('c');
  SimpleArray<int> Result_ = s8.SeriesArrayCompare(s10);
  
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);
#else
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert((Result_.Base())[i] < 0);
    else
      assert((Result_.Base())[i] == 0);
#endif

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);
    
  for (i = 0; i < 10; i++)
    delete s9[i];

  delete s10[8];
  delete s10[10];

  cout <<"SUCCESS testing <PtrDynArray>" <<endl;
}

/****************************************************************************/
void TestTempStream()
{
  OutputBuffer OutBuf_;
  
  *OutBuf_.StreamPtr() <<"hello world!";
  cout <<OutBuf_.Buffer() <<endl;
  *OutBuf_.StreamPtr() <<" Is anybody out there!";
  cout <<OutBuf_.Buffer() <<endl;

  OutBuf_.Buffer() = "";
  *OutBuf_.StreamPtr() <<"ah doo doo doo!\nah da da da!";
  cout <<OutBuf_.Buffer() <<endl;

  cout <<"SUCCESS testing <TempStream>" <<endl;
}

/****************************************************************************/
void TestArray()
{
  RANDOMIZE();
  TestArray(cout);
}

/****************************************************************************/
void TestMatrix()
{
  TestMatrix(cout);
}

/****************************************************************************/
void TestPoly()
{
  TestPolynomials(cout);
}

/****************************************************************************/
void TestRand()
{
  TestRandom(cout);
}

/****************************************************************************/
void TestTimeOfDay()
{
  TimeOfDay td1_(9, 30, 10);
  TimeOfDay td2_(47700);
  TimeOfDay td3_("1:30 pm");
  TimeOfDay td4_("20:15:30");
  TimeOfDay td5_;  

  assert(td3_.IsBetween(td1_, td4_));
  assert(td2_.IsBetween(td1_, td3_));
  assert(td2_ == TimeOfDay("1:15 pm"));

  td5_ = "1:5 pm";
  assert(td5_.Hour() == 13);
  assert(td5_.Minute() == 5);
  assert(td5_.Second() == 0);
  td5_.AddTime(7, 10, 30);
  assert(td5_ == td4_);
  td5_ -= 24330;
  assert(td5_ == td3_);
  td5_ += 900;
  assert(td5_ == TimeOfDay(13, 45, 00));
  assert(td5_.IsBetween(td3_, td4_));
  assert(td3_ < td5_);
  assert(td4_ > td5_);
  td3_.SubtractTime(0, 15, 0);
  assert(td3_ == td2_);
  td3_ += 900;
  assert(td3_ == TimeOfDay(13, 30, 00));

  td5_ = td1_.Now();
  time_t timer;
  struct tm *tmp;
  time(&timer);                 // get time of day
  tmp = localtime(&timer);      // converts date/time to a structure  
  assert(td5_ == TimeOfDay(tmp->tm_hour, tmp->tm_min, tmp->tm_sec));
  assert(TimeOfDay::ParseAndConvert("3:30 pm") == TimeOfDay("15:30:00").ToSeconds());
  cout <<td5_ <<endl;

  TimeOfDay* tdp6_ = TimeOfDay::Make("15:30:00");
  ChrString str_ = tdp6_->AsString();
  assert(!strcmp(str_.c_str(), "15:30:00"));
  str_ = tdp6_->PrintString(TimeOfDay::TIME_NORMAL);
  assert(!strcmp(str_.c_str(), "3:30 pm"));
  str_ = tdp6_->PrintString(TimeOfDay::TIME_MILITARY);
  assert(!strcmp(str_.c_str(), "15:30:00"));
  
  td5_ = *tdp6_ + (7200 + 900 + 15);
  assert(td5_ == TimeOfDay("17:45:15"));
  td5_ = td5_ - (3600 + 450 + 10);
  assert(td5_ == TimeOfDay("16:37:35"));
  long nsecs_ = td5_ - *tdp6_;
  assert(nsecs_ == 4055);

  assert(td5_.StaticID() == tdp6_->ClassID());
  assert(tdp6_->ClassID() == TypeInfo::TIMEOFDAY_TYPE);
  assert(tdp6_->NULLOBJECT() == td5_.NULLOBJECT());
  assert(td5_.NULLOBJECT().IsNullObject());

  TimeOfDay* tdp7_ = TimeOfDay::Make(tdp6_->ToSeconds());
  assert(*tdp7_ == *tdp6_);
  assert(*tdp7_ != td5_);
  delete tdp7_;
  tdp7_ = (TimeOfDay*)td5_.Clone();
  assert(*tdp7_ != *tdp6_);
  assert(*tdp7_ == td5_);
  tdp7_->Assign(td1_);
  assert(*tdp7_ != *tdp6_);
  assert(*tdp7_ != td5_);
  assert(*tdp7_ == td1_);

  delete tdp6_;
  delete tdp7_;
}

/****************************************************************************/
void TestArrayBinTree()
{
  ArrayBinTree<ChrString> tree(25, "!");
  ChrString country[] = { "France",
                        "Germany",
                        "Sweden",
                        "Ireland",
                        "Italy",
                        "Spain",
                        "Portugal",
                        "Switzerland",
                        "Belgium",
                        "England",
                        "Greece",   // not inserted in tree
                        "Denmark"   // not inserted in tree
                       };
  unsigned count = sizeof(country) / sizeof(ChrString);
  unsigned testSize = count - 2;
  unsigned i;

  // insert the names of the countries
  for (i = 0; i < testSize; i++)
    tree.insert(country[i]);

  // display AVL-tree
  showTree(tree, "Complete binary tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);

  for (i = 0; i < count / 2; i++)
    tree.remove(country[i]);

   // display AVL-tree
  tree.TextWrite(cout);
//  showTree(tree, "Trimmed binary tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);
}

/****************************************************************************/
void TestBtree()
{
  Btree<LString> tree("tsbtree.dat");
  LString country[] = { "France",
                        "Sweden",
                        "Spain",
                        "Portugal",
                        "Switzerland",
                        "Belgium",
                        "Ireland",
                        "Italy",
                        "Monaco",
                        "Andora",
                        "Luxembourg",
                        "Germany",
                        "England",
                        "Greece",   // not inserted in tree
                        "Denmark"   // not inserted in tree
                       };
  unsigned count = sizeof(country) / sizeof(LString);
  unsigned testSize = count - 2;
  unsigned numDelete = count / 2;
  unsigned i;

  // insert the names of the countries
  for (i = 0; i < testSize; i++) {
    tree.insert(country[i]);
    showPages(tree, "After inserting ", country[i]);
  }

  // display B-tree
  showTree(tree, "Complete B-tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);

  for (i = 0; i < numDelete; i++) {
    tree.remove(country[i]);
    showPages(tree, "After deleting ", country[i]);
  }

   // display AVL-tree
  showTree(tree, "Trimmed B-tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);

//  gotoxy(1, 22); clreol();
//  gotoxy(1, 23); clreol();
  tree.TextWrite(cout);
}

/****************************************************************************/
void TestCharSet()
{
   CharSet CS1('A','Z'), CS2(0, 25);
   ChrString s("^-");
   CharSet CS3(s);

//   clrscr();
   cout << "Set CS1 is " << CS1 << '\n';
   cout << "Set CS2 is " << CS2 << '\n';
   CS1.clear();
   cout << "Set CS1 is " << CS1 << '\n';
   CS1.mkalphanumeric();
   cout << "Set CS1 is " << CS1 << '\n';
   CS1.remove('a', 'z').add('y', 'z');
   cout << "Set CS1 is " << CS1 << '\n';
   CS1 += CS2;
   cout << "Set CS1 is " << CS1 << '\n';
   CS2 = CS1;
   cout << "Set CS2 is " << CS2 << '\n';
   CS2.remove(1, 4);
   CS2 += CS3;
   cout << "Set CS2 is " << CS2 << '\n';
   cout << "Press any key to end the program ...";
}

/****************************************************************************/
void TestULongInt()
{
  ChrString Str_;

  ULongInt si(61515012);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515012"));

  ULongInt si2(61515012);
  ULongInt si3(41057012);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "41057012"));  

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  
  UInteger uintv("56800012");
  si2.Assign(uintv);
  assert(si2 == ULongInt(56800012));

  UShortInt usintv("65534");
  si2.Assign(usintv);
  assert(si2 == ULongInt(65534));

  // Testing bounds checking
  ULongInt chk1 = ULONG_MAX;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 65536;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 65536;
  chk1 >>= 2;
  assert(Ulong(chk1) == 16384);
  chk1 <<= 4;
  assert(Ulong(chk1) == 262144);
  chk1 |= ULongInt(0x0F);
  assert(Ulong(chk1) == 262159);
  chk1 = ULongInt(chk1) / ULongInt(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  ULongInt* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new ULongInt(i+1);
  List_[10] = NULL;

  ULongInt ZeroChar(0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<ULongInt> Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing ULongInt" <<endl;
}

/****************************************************************************/
void TestUnsigned()
{
  ChrString Str_;

  UInteger si(61515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515"));

  UInteger si2(61515);
  UInteger si3(41057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "41057"));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2.ClearError();
  UShortInt usint("56800");
  cout <<usint <<endl;
  si2.Assign(usint);
  cout <<si2 <<endl;  
  assert(si2 == UInteger(56800));

  si2 = "56800111";
  assert(si2 = 56800111);

  // Testing bounds checking
  UInteger chk1 = UINT_MAX;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 65536;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 65536;
  chk1 >>= 2;
  assert(int(chk1) == 16384);
  chk1 <<= 4;
  assert(int(chk1) == 262144);
  chk1 = chk1 | UInteger(0x0F);
  assert(int(chk1) == 262159);
  chk1 = chk1 / UInteger(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  UInteger* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new UInteger(i+1);
  List_[10] = NULL;

  UInteger ZeroChar(0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<UInteger> Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing UInteger" <<endl;
}

/****************************************************************************/
void TestUShortInt()
{
  ChrString Str_;

  UShortInt si(61515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515"));

  UShortInt si2(61515);
  UShortInt si3(41057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "41057"));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "56800";
  assert(Ushort(si2) == 56800);

  si2 = "56800111";
  assert(Ushort(si2) == 0);

  // Testing bounds checking
  UShortInt chk1 = 65535;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 256;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 256;
  chk1 >>= 2;
  assert(Ushort(chk1) == 64);
  chk1 <<= 4;
  assert(Ushort(chk1) == 1024);
  chk1 |= UShortInt(0x0F);
  assert(Ushort(chk1) == 1039);
  chk1 = chk1 / UShortInt(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  // Testing series comparison class on user defined type
  UShortInt* List_[25];
  short i;

  for (i = 0; i < 10; ++i)
    List_[i] = new UShortInt(i+1);
  List_[10] = NULL;

  UShortInt ZeroChar(0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<UShortInt> Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];  

  cout <<endl <<"SUCCESS Testing UShortInt" <<endl;
}

/****************************************************************************/
void TestVlenFile()
{
  char Header_[128];
  StrFill(Header_, '\0', 128);
  strcpy(Header_, "This Is The Header Of The File");

  ChrString Record1_("String #1, Whoxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record2_("String #2, Canxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record3_("String #3, Sayxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record4_("String #4, Onlyxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record5_("String #5, Timex1bbb11xxx12bbb22xxx23bbb33xxx34bbb44xxx45bbb55xxx56bbb66xxx67bbb77xxx78bbb88xxx89bbb99xxx911b1111x1122b2222x2233b3333x33");
  
  ChrString NullRec_;
  ChrString UseRec_;

  VarLengthFile* Sfile_ =
  VarLengthFile::Make(VarLengthFileInfo::Make(NullRec_));
  Sfile_->SetFileProperties("vsample.dat", ios::out|ios::in|ios::binary);

  Sfile_->Open();

  Sfile_->AddRecord(Record1_);
  Sfile_->AddRecord(Record2_);
  Sfile_->AddRecord(Record3_);
  Sfile_->StoreIDToHeader();
  Sfile_->AddRecord(Record4_);
  Sfile_->AddRecord(Record5_);

  long Index_ = 0;
  while ((Index_ = Sfile_->NextRecord(Index_, UseRec_)) != -1)
    cout <<UseRec_ <<endl;

  Sfile_->FirstRecord(UseRec_);
  cout <<UseRec_ <<endl;

  UseRec_ = ShortInt::ToString(ShortInt(21600));
  long Index2_ = Sfile_->NthRecordSlot(2);
  Sfile_->ModifyRecord(Index2_, UseRec_);

  Index_ = Sfile_->NthRecordSlot(1);
  Sfile_->DeleteRecord(Index_);

  if (Sfile_->RetrieveRecord(Index_, UseRec_))
    cout <<UseRec_ <<endl;

  if (Sfile_->RetrieveRecord(Index2_, UseRec_))
    cout <<UseRec_ <<endl;

  long Sz_ = Sfile_->SlotsAllocated() + 1;
  long* IDvalues_ = new long[Sz_];
  Sfile_->ReadIDFromHeader(IDvalues_);
  cout <<"Rec Ids:";

  for (Index_ = 0; Index_ < Sz_; ++Index_)
    cout <<" " <<IDvalues_[Index_];
  delete[] IDvalues_;

  Sfile_->WriteHeader((Byte_t*)Header_);
  Sfile_->ReadHeader((Byte_t*)Header_);
  cout <<endl <<Header_ <<endl;

  cout <<TypeInfo::GiveClassName(Sfile_->IdentifyRecord(Index2_)) <<endl;

  Sfile_->Close();
  cout <<Sfile_->IsVarLengthFile("vsample.dat") <<endl;

  delete Sfile_;
}

/****************************************************************************/
void TestDiscrete()
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
  
  assert(Discrete<Ulong>::CombinationWithRep(4, 6) == 84);
  assert(Discrete<Ulong>::GCD(456, 624) == 24);
  assert(Discrete<Ulong>::GCD(116, 641) == 1);
  assert(Discrete<Ulong>::LCM(456, 624) == 11856);
  assert(Discrete<Ulong>::LCM(456, 624) *
         Discrete<Ulong>::GCD(456, 624) == (456 * 624));
  assert(Discrete<Ulong>::StirlingNumber2(8, 3) == 966);

  cout <<"SUCCESS Testing Discrete" <<endl;
}

/****************************************************************************/
void TestTristate()
{
  Tristate Ts1_ = Tristate::True;

  Function1();
  Function1(Ts1_);
}

/****************************************************************************/
void TestTxtgraph()
{
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::SINGLE, TextGraphics::HL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::SINGLE, TextGraphics::VL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::DOUBLE, TextGraphics::HL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::DOUBLE, TextGraphics::VL) <<endl;

  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 11; y++)
      cout <<TextGraphics::GRAPHTABLE((TextGraphics::GraphType_t)x, (TextGraphics::GraphChar_t)y);
    cout <<endl;
  }
}

/****************************************************************************/
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
  TestBuiltIn();
  TestAdvForLp();
  TestArgArray();
  TestPtrArray();
  TestAssoc();
  TestBstTree();
  TestAvlTree();
  TestBaseptr();
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  TestBits();
#endif
  TestBitstr();
#if defined(__DJGPP__) | defined(__linux__) | defined(__GNUC__)
  TestBitvect();
#endif
  TestBltinCmp();
  TestByteStr();
  TestChar();
  TestCompare();
  TestComplex();
  TestDatastor();
  TestDeque();
  TestDlnklist();
  TestDouble();
  TestDynarray();
  TestEnigma();
  TestFloat();
  TestForLoop();
  TestFuzzyset();
  TestHashtab();
  TestImagePtr();
  TestIniFile();
  TestInteger();
  TestLdouble();
  TestLonestr();
  TestLongint();
  TestLongnum();
  TestObjimp();
  TestPoint();
  TestPrimeTbl();
  TestPriqueue();
  TestQueue();
  TestQuickint();
  TestRational();
  TestRect();
  TestRefptrs();
  TestSafeSet();
  TestSercmp();
  TestSet();
  TestShortint();
  TestSimpArray();
  TestSimpList();
  TestSlnklist();
  TestSlotFile();
  TestSmartPtr();
  TestSortList();
  TestSorts();
  TestStack();
  TestStrBrows();
  TestStreamReader();
  TestString();
  TestStrio();
  TestStrStore();
  TestStrutil();
  TestDate();
  TestPtrDynarray();
  TestTempStream();
  TestArray();
  TestMatrix();
  TestPoly();
  TestRand();
  TestTimeOfDay();
  TestArrayBinTree();
  TestBtree();
  TestCharSet();
  TestULongInt();
  TestUnsigned();
  TestUShortInt();
  TestVlenFile();
  TestDiscrete();
  TestTristate();
  TestTxtgraph();

  // dummy main function for library building
  cout <<"Built with: djgpp, gcc 3.33 on rhide 1.49b" <<endl;
  cout <<"CPPDS library built SUCCESSFULLY!" <<endl;
  return 0;
}


