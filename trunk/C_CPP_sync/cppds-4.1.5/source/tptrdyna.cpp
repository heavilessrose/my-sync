#include <assert.h>
#include <iostream.h>
#include "pdynarra.cpp"

#define GENERAL_PTRDYNARRAY_DEBUG	0
#define STANDARD_PTRDYNARRAY_DEBUG	1

int* Square(int* x)
{ (*x) *= (*x); return x; }

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

int operator == (const Char& c1, const Char& c2)
	{ return (c1.val() == c2.val()); }

int operator > (const Char& c1, const Char& c2)
	{ return (c1.val() > c2.val()); }

int operator < (const Char& c1, const Char& c2)
	{ return (c1.val() < c2.val()); }

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

PtrDynArray<Char> operator + (const PtrDynArray<Char>&, const PtrDynArray<Char>&);
PtrDynArray<Char> operator + (const PtrDynArray<Char>&, const Char&);
PtrDynArray<Char> operator + (const Char&, const PtrDynArray<Char>&);

int CompCharPtr(const Char* c1, const Char* c2)
{
  return ((c1 && c2) ? (c1->val() - c2->val()):0);
}

int main()
{
#if GENERAL_PTRDYNARRAY_DEBUG
  PtrDynArray<short> Argv_(5, default_size);

  // Must be able to accept pointers to objects of abstract classes
  PtrDynArray<Object> Test_;

  short Num41_ = 41;
  short Num7_ = 7;
  short Num3_ = 3;
  short Num99_ = 99;
  short Num67_ = 67;
  short N64 	= 64;
  short N128 	= 128;
  short N256 	= 256;
  short N1024 	= 1024;
  short N4096 	= 4096;
  short N16384 	= 16384;
  short N1000 	= 1000;
  short N2000 	= 2000;
  short N2041 	= 2041;
  short N207 	= 207;
  short N203 	= 203;
  short N2099 	= 2099;
  short N2067 	= 2067;

  Argv_.Resize(5);
  Argv_[0] = &Num41_;
  Argv_[1] = &Num7_;
  Argv_[2] = &Num3_;
  Argv_[3] = &Num99_;
  Argv_[4] = &Num67_;

  cout <<endl;
  cout <<"PtrDynArray = ";
  PtrDynArrayIterator<short> ArgvIter2_(&Argv_);
  for (;ArgvIter2_.More(); ++ArgvIter2_)
    if (ArgvIter2_.IsValid())
    {
      if (ArgvIter2_.Pointee())
        cout <<*ArgvIter2_ <<" ";
      else
        cout <<"VOID ";
    }
    else
      cout <<"INVALID ";

  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size() <<endl;

  ArgvIter2_.Reset();
  Argv_.Resize(11);
  ArgvIter2_[0], ArgvIter2_.SetData(&N64);
  ArgvIter2_[1], ArgvIter2_.SetData(&N128);
  ArgvIter2_[2], ArgvIter2_.SetData(&N256);
  ArgvIter2_[3], ArgvIter2_.SetData(NULL);
  ArgvIter2_[4], ArgvIter2_.SetData(&N1024);
  ArgvIter2_[5], ArgvIter2_.SetData(NULL);
  ArgvIter2_[6], ArgvIter2_.SetData(&N4096);
  ArgvIter2_[7], ArgvIter2_.SetData(NULL);
  ArgvIter2_[8], ArgvIter2_.SetData(&N16384);
  ArgvIter2_[9], ArgvIter2_.SetData(&N2000);
  ArgvIter2_[10], ArgvIter2_.SetData(&N1000);

  cout <<endl;
  cout <<"PtrDynArray = ";
  PtrDynArrayIterator<short> ArgvIter3_(&Argv_);
  for (;ArgvIter3_.More(); ++ArgvIter3_)
    if (ArgvIter3_.IsValid())
    {
      if (ArgvIter3_.Pointee())
        cout <<*ArgvIter3_ <<" ";
      else
        cout <<"VOID ";
    }
    else
      cout <<"INVALID ";

  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size();

  cout <<endl;
  cout <<"Reverse DynArray = ";
  ReverseRandomAccessIterator<short> RevIter_(ArgvIter3_);
  for (RevIter_.Reset(); RevIter_.More();)
    if (RevIter_.IsValid())
    {
      if (RevIter_.Pointee())
        cout <<*RevIter_++ <<" ";
      else
      {
        cout <<"VOID ";
        ++RevIter_;
      }
    }
    else
    {
      cout <<"INVALID ";
      ++RevIter_;
    }

  cout <<endl <<"Shrinking Down To 6";
  Argv_.Resize(6);
  cout <<endl <<"Shrinking Done";
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;

  Argv_.Flush();
  cout <<"PtrDynArray Flushed";

  cout <<endl;
  cout <<"PtrDynArray = ";
  for (ArgvIter3_.Reset(); ArgvIter3_.More(); ++ArgvIter3_)
    if (ArgvIter3_.IsValid())
    {
      if (ArgvIter3_.Pointee())
        cout <<*ArgvIter3_ <<" ";
      else
        cout <<"VOID ";
    }
    else
      cout <<"INVALID ";

  PtrDynArray<short> Argt_(5, default_size);

  Argt_.Resize(5);
  Argt_[0] = &N2041;
  Argt_[1] = &N207;
  Argt_[2] = &N203;
  Argt_[3] = &N2099;
  Argt_[4] = &N2067;
  Argv_.ShallowAssign(Argt_);

  cout <<endl;
  cout <<"PtrDynArray = ";
  for (ArgvIter3_.Reset(); ArgvIter3_.More(); ++ArgvIter3_)
    if (ArgvIter3_.IsValid())
    {
      if (ArgvIter3_.Pointee())
        cout <<*ArgvIter3_ <<" ";
      else
        cout <<"VOID ";    
    }
    else
      cout <<"INVALID ";

  cout <<endl;
  ShowInfo((ostream&)cout, Argv_);
  ShowInfo((ostream&)cout, Argt_);
  cout <<endl;
///
#endif

#if STANDARD_PTRDYNARRAY_DEBUG
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
  SimpleArray<int> Result_ = s8.SeriesCompare(s10);
  
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);
    
  for (i = 0; i < 10; i++)
    delete s9[i];

  delete s10[8];
  delete s10[10];
#endif

  cout <<"SUCCESS testing <PtrDynArray>" <<endl;
  return 0;
}

