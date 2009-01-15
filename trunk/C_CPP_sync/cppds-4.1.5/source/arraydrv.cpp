#if defined(__TURBOC__)
  #ifndef INCL_CONIO_H
    #include <conio.h>
    #define INCL_CONIO_H
  #endif
#endif
#ifndef PTRARRAY_CPP
  #include "ptrarray.cpp"
#endif
#ifndef ARGARRAY_CPP
  #include "argarray.cpp"
#endif

#define ARRAY_CLONE_TEST   	        1
#define STLCONT_ARRAY_DEBUG		     0
#define GENERAL_ARRAY_DEBUG	        1
#define ARGARRAY_DEBUG		           0
#define PTRARRAY_DEBUG		           0
#define ARRAY_SHOW_DEBUG_OUTPUT		  1
#define ARRAY_NOTIFY_SUCCESS		     1

class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

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

#if ARGARRAY_DEBUG
int Square(int x)
{ return (x * x); }

int operator == (const Char& c1, const Char& c2)
	{ return (c1.val() == c2.val()); }

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
#endif

#if PTRARRAY_DEBUG
int* Square(int* x)
{ (*x) *= (*x); return x; }

int operator == (const Char& c1, const Char& c2)
	{ return (c1.val() == c2.val()); }

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

int CompChar(const Char* c1, const Char* c2)
{
  return (c1->val() - c2->val());
}
#endif

/****************************************************************************/
int main()
{
#if defined(__TURBOC__)
  clrscr();
#endif

#if STLCONT_ARRAY_DEBUG
  PtrArray<short> Args_;
  ArgArray<short> Argv_(5);
  ArgArrayContainer<short> ArgContainer_(&Argv_);
  PtrArrayContainer<short> PtrContainer_(&Args_);

#if ARRAY_NOTIFY_SUCCESS
  cout <<"SUCCESS Array STL container methods testing" <<endl;
#endif
#endif

#if GENERAL_ARRAY_DEBUG
  int i; Boolean Ok_;
  Ushort Index_;
  PtrArray<short> Argt_;
  PtrArray<short> Args_;
  ArgArray<short> Argv_(5);

  short Num41_ = 41;
  short Num7_ = 7;
  short Num3_ = 3;
  short Num99_ = 99;
  short Num67_ = 67;

  short* TempPtr_;
  short TempVal_;

  PtrArrayIterator<short> ArgsIter_(&Args_);

  ArgsIter_[0]; ArgsIter_.SetData(&Num41_);
  ArgsIter_[1]; ArgsIter_.SetData(&Num7_);
  ArgsIter_[2]; ArgsIter_.SetData(&Num3_);
  ArgsIter_[3]; ArgsIter_.SetData(&Num99_);
  ArgsIter_[4]; ArgsIter_.SetData(&Num67_);

// test1
  Argv_[0] = 41;
  Argv_[1] = 7;
  Argv_[2] = 3;
  Argv_[3] = 99;
  Argv_[4] = 67;

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"PtrArray = ";
#endif

  for (ArgsIter_.Reset(), i = 0; ArgsIter_.More(); ArgsIter_++, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    cout <<*ArgsIter_ <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgsIter_ == 41):
	  (i == 1) ? (*ArgsIter_ == 7):
	  (i == 2) ? (*ArgsIter_ == 3):
	  (i == 3) ? (*ArgsIter_ == 99):
	  (i == 4) ? (*ArgsIter_ == 67):0;
    assert(Ok_);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Args_.Size();
  cout <<endl;

  cout <<"ArgArray = ";
#endif
  assert(Args_.Size() == 5);
  
  ArgArrayIterator<short> ArgvIter_(&Argv_);
  for (i = 0; ArgvIter_.More(); ArgvIter_++, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    cout <<*ArgvIter_ <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgvIter_ == 41):
	  (i == 1) ? (*ArgvIter_ == 7):
	  (i == 2) ? (*ArgvIter_ == 3):
	  (i == 3) ? (*ArgvIter_ == 99):
	  (i == 4) ? (*ArgvIter_ == 67):0;
    assert(Ok_);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  assert(Argv_.Size() == 5);

  // Testing IsVoid methods
  TempPtr_ = Args_[2];
  Args_[2] = NULL;
  TempVal_ = (short)Argv_[2];
  Argv_[2].Erase();

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<Argv_[2].IsVoid() <<" "
       <<Argv_[3].IsVoid() <<" "
       <<Argv_[5].IsVoid() <<endl;
  cout <<Args_[2].IsVoid() <<" "
       <<Args_[3].IsVoid() <<" "
       <<Args_[5].IsVoid() <<endl;
#endif

  assert(Argv_[2].IsVoid() == 1);
  assert(Argv_[3].IsVoid() == 0);
  assert(Argv_[5].IsVoid() == 1);
  assert(Args_[2].IsVoid() == 0);
  assert(Args_[3].IsVoid() == 0);
  assert(Args_[5].IsVoid() == 1);
       
  Args_[2] = TempPtr_;
  Argv_[2] = TempVal_;

#if ARRAY_SHOW_DEBUG_OUTPUT  
  cout <<Args_[2].IsVoid() <<endl;  
  cout <<Args_[2].IsVoid() <<endl;
#endif

  assert(Argv_[2].IsVoid() == 0);
  assert(Argv_[3].IsVoid() == 0);
  assert(Argv_[5].IsVoid() == 1);
  assert(Args_[2].IsVoid() == 0);
  assert(Args_[3].IsVoid() == 0);
  assert(Args_[5].IsVoid() == 1);

// test1
#if ARRAY_CLONE_TEST
  ArgArray<short>* Newv_ = (ArgArray<short>*)Argv_.Clone();
  *Newv_ = Argv_;

  (*Newv_)[0] = 128;
  (*Newv_)[4] = 512;

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NewArray = ";
#endif
  ArgArrayIterator<short> NewvIter_(Newv_);
  for (i = 0; NewvIter_.More(); ++NewvIter_, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (NewvIter_.IsValid())
      cout <<*NewvIter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*NewvIter_ == 128):
	  (i == 1) ? (*NewvIter_ == 7):
	  (i == 2) ? (*NewvIter_ == 3):
	  (i == 3) ? (*NewvIter_ == 99):
	  (i == 4) ? (*NewvIter_ == 512):
	  (i == 5) ? (NewvIter_.IsValid() == 0):0;
    assert(Ok_);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Newv_->Size() <<endl;
#endif
  assert(Newv_->Size() == 5);
  
#endif	// Array Clone test
// test1

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  ArgArrayIterator<short> ArgvIter2_(&Argv_);
  for (i = 0; ArgvIter2_.More(); ++ArgvIter2_, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (ArgvIter2_.IsValid())
      cout <<*ArgvIter2_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*ArgvIter2_ == 41):
	  (i == 1) ? (*ArgvIter2_ == 7):
	  (i == 2) ? (*ArgvIter2_ == 3):
	  (i == 3) ? (*ArgvIter2_ == 99):
	  (i == 4) ? (*ArgvIter2_ == 67):
	  (i == 5) ? (ArgvIter2_.IsValid() == 0):0;
    assert(Ok_);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size() <<endl;
#endif

 // test1
#if ARRAY_CLONE_TEST
  delete Newv_;
#endif
 // test1

  ArgvIter2_.Reset();
  ArgvIter2_[0], ArgvIter2_.SetData(64);
  ArgvIter2_[1], ArgvIter2_.SetData(128);
  ArgvIter2_[2], ArgvIter2_.SetData(256);
  ArgvIter2_[3], ArgvIter2_.Erase();
  ArgvIter2_[4], ArgvIter2_.SetData(1024);
  ArgvIter2_[5], ArgvIter2_.Erase();
  ArgvIter2_[6], ArgvIter2_.SetData(4096);
  ArgvIter2_[7], ArgvIter2_.Erase();
  ArgvIter2_[8], ArgvIter2_.SetData(16384);
  ArgvIter2_[9], ArgvIter2_.SetData(2000);
  ArgvIter2_[10], ArgvIter2_.SetData(1000);

  Argt_ = Argv_;

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  ArgArrayIterator<short> ArgvIter3_(&Argv_);
  for (Index_ = 0; ArgvIter3_.More(); ++ArgvIter3_, ++Index_)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (ArgvIter3_.IsValid())
      cout <<*ArgvIter3_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (Index_ == 0) ? (*ArgvIter3_ == 64):
	  (Index_ == 1) ? (*ArgvIter3_ == 128):
	  (Index_ == 2) ? (*ArgvIter3_ == 256):
	  (Index_ == 3) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 4) ? (*ArgvIter3_ == 1024):
	  (Index_ == 5) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 6) ? (*ArgvIter3_ == 4096):
	  (Index_ == 7) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 8) ? (*ArgvIter3_ == 16384):
	  (Index_ == 9) ? (*ArgvIter3_ == 2000):
	  (Index_ == 10) ? (*ArgvIter3_ == 1000):0;
    assert(Ok_);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size();
#endif
  assert(Argv_.Size() == 8);

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"Reverse ArgArray = ";
  ReverseRandomAccessIterator<short> RevIter_(ArgvIter3_);
  for (RevIter_.Reset(); RevIter_.More();)
    if (RevIter_.IsValid())
      cout <<*RevIter_++ <<" ";
    else
    {
      cout <<"VOID ";
      ++RevIter_;
    }

  cout <<endl;
#endif

  &Argv_[6] = (short*)&Argv_[2];
  &Argv_[2] = (short*)&Argv_[8];
  &Argv_[8] = (short*)&Argv_[6];
#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<*(&Argv_[2]) <<"  "
       <<*(&Argv_[6]) <<"  "
       <<*(&Argv_[8]) <<endl;
#endif
  assert(*(&Argv_[2]) == 16384);
  assert(*(&Argv_[6]) == 256);
  assert(*(&Argv_[8]) == 256);

  for (Index_ = 21; Index_ >= 16; Index_--)
    Argv_[Index_].Erase();

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl <<"Shrinking Down To 6";
  Argv_.ShrinkTo(6);
  cout <<endl <<"Shrinking Done";
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  Argv_.ShrinkTo(6);
  assert(Argv_.Size() == 4);

  Argv_.Flush();
#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<"ArgArray Flushed";

  cout <<endl;
  cout <<"ArgArray = ";
#endif
  for (ArgvIter3_.Reset(); ArgvIter3_.More(); ++ArgvIter3_)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (ArgvIter3_.IsValid())
      cout <<*ArgvIter3_ <<" ";
    else
      cout <<"VOID ";
#endif
    assert(ArgvIter3_.IsValid() == 0);
  }

#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TPtrArray = ";
#endif
  PtrArrayIterator<short> Tpiter_(&Argt_);
  for (i = 0; Tpiter_.More(); ++Tpiter_, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (Tpiter_.IsValid())
      cout <<*Tpiter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*Tpiter_ == 64):
	  (i == 1) ? (*Tpiter_ == 128):
	  (i == 2) ? (*Tpiter_ == 256):
	  (i == 3) ? (Tpiter_.IsValid() == 0):
	  (i == 4) ? (*Tpiter_ == 1024):
	  (i == 5) ? (Tpiter_.IsValid() == 0):
	  (i == 6) ? (*Tpiter_ == 4096):
	  (i == 7) ? (Tpiter_.IsValid() == 0):
	  (i == 8) ? (*Tpiter_ == 16384):
	  (i == 9) ? (*Tpiter_ == 2000):
	  (i == 10) ? (*Tpiter_ == 1000):0;
    assert(Ok_);
  }

  Argt_.Flush();
  Argv_[0] = 2141;
  Argv_[1] = 227;
  Argv_[2] = 233;
  Argv_[3] = 2499;
  Argv_[4] = 2567;

  // Testing virtual assignment method
  Argt_.ShallowAssign(Argv_);
#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TPtrArray = ";
#endif
  for (i = 0, Tpiter_.Reset(); Tpiter_.More(); ++Tpiter_, ++i)
  {
#if ARRAY_SHOW_DEBUG_OUTPUT
    if (Tpiter_.IsValid())
      cout <<*Tpiter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*Tpiter_ == 2141):
	  (i == 1) ? (*Tpiter_ == 227):
	  (i == 2) ? (*Tpiter_ == 233):
	  (i == 3) ? (*Tpiter_ == 2499):
	  (i == 4) ? (*Tpiter_ == 2567):(Tpiter_.IsValid() == 0);
    assert(Ok_);
  }

  // Testing null object methods
  Object* ObjPtr1_ = &Args_.NULLOBJECT();
  Object* ObjPtr2_ = &Argv_.NULLOBJECT();
  assert(ObjPtr1_->IsNullObject());
  assert(ObjPtr2_->IsNullObject());
#if ARRAY_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NULL: " <<ObjPtr1_->IsNullObject() <<"\t" <<ObjPtr2_->IsNullObject();
#endif

#if ARRAY_NOTIFY_SUCCESS
  cout <<endl <<"SUCCESS General array classes testing" <<endl;
#endif
///
#endif

#if ARGARRAY_DEBUG
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

#if ARRAY_SHOW_DEBUG_OUTPUT
  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheArray(*ps2);
  cout <<endl;
#endif

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  Write(fout, *ps2);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, s6);
  fin.close();
  assert(s6 == *ps2);

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXXXXXaaaaab");

  ArgArray<int> s9;
  for (i = 0; i < 10; i++)
    s9[i] = i * 2;

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(s9[i] == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  ArgArray<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10[8] = Char('c');
  s10[10] = Char('c');
  SimpleArray<int> Result_ = s8.SeriesArrayCompare(s10);
  
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  delete ps1;
  delete ps2;

#if ARRAY_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <ArgArray>" <<endl;
#endif
#endif

#if PTRARRAY_DEBUG
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

#if ARRAY_SHOW_DEBUG_OUTPUT
  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheArray(*ps2);
  cout <<endl;
#endif

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  Write(fout, *ps2);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, s6);
  fin.close();
  assert(!(s6 == *ps2));

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
  
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);

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

#if ARRAY_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <PtrArray>" <<endl;
#endif
#endif

  return 0;
}

/****************************************************************************/
