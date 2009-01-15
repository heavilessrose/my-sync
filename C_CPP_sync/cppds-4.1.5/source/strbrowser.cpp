#ifndef STRINGBROWSER_CPP
#define STRINGBROWSER_CPP
#ifndef STRINGBROWSER_H
  #include "strbrowser.h"
#endif

/****************************************************************************/
StringBrowser::StringBrowser():
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser::StringBrowser(const char* s):
_Str(s),
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser::StringBrowser(char c, Subscript n):
_Str(c, n),
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser::StringBrowser(const char* s, Subscript n):
_Str(s, n),
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser::StringBrowser(const ChrString& s) :
_Str(s),
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser::StringBrowser(const StringBrowser& s) :
_Str(s._Str),
_Tokenizer(_Str.c_str(), 0, NULL, " \t\n\r")
{}

/****************************************************************************/
StringBrowser* StringBrowser::Make()
{
  return (new StringBrowser());
}

/****************************************************************************/
StringBrowser* StringBrowser::Make(char Ch_, Subscript n)
{
  return (new StringBrowser(Ch_, n));
}

/****************************************************************************/
StringBrowser* StringBrowser::Make(const char* Str_)
{
  return (new StringBrowser(Str_));
}

/****************************************************************************/
StringBrowser* StringBrowser::Make(const char* Str_, Subscript n)
{
  return (new StringBrowser(Str_, n));
}

/****************************************************************************/
StringBrowser* StringBrowser::Make(const Object& Obj_)
{
  StringBrowserAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& StringBrowser::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  StringBrowserAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromStringBrowser(TrgComp_)));
}

/****************************************************************************/
Object* StringBrowser::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new StringBrowser(*this));

  StringBrowserAcceptor TrgComp_(this);
  return ((StringBrowser*) (ObjComp_->CreateFromStringBrowser(TrgComp_)));
}

/****************************************************************************/
StringBrowser& StringBrowser::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static StringBrowser _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& StringBrowser::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StringBrowser::GiveNullObject();
}

/****************************************************************************/
Object& StringBrowser::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StringBrowser::GiveNullObject();
}

/****************************************************************************/
int StringBrowser::ClassID() const
{
  return TypeInfo::STRINGBROWSER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* StringBrowser::GiveObjectAcceptor() const
{
  return (new StringBrowserAcceptor(this));
}

/****************************************************************************/
ChrString& StringBrowser::FindToken()
{
  char* Buffer_ = NULL;
  _Tokenizer.FindToken(Buffer_, TRUE);
  _Token = Buffer_;
  ::Delete(Buffer_);
  return _Token;
}

/****************************************************************************/
StringBrowser& StringBrowser::operator = (const ChrString& Str_)
{
  if (&_Str != &Str_)
  {
    _Str = Str_;
    _Tokenizer.SetString(_Str.c_str());
  }

  return *this;
}

/****************************************************************************/
StringBrowser& StringBrowser::operator = (const StringBrowser& Str_)
{
  if (this != &Str_)
  {
    _Str = Str_._Str;
    _Tokenizer.SetString(_Str.c_str());
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringBrowser::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringBrowser::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* StringBrowser::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void StringBrowser::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
StringBrowserAcceptor::StringBrowserAcceptor(const StringBrowser* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
// Data transfer methods
void StringBrowserAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const StringBrowser*)Ptr_;
}

/****************************************************************************/
const void* StringBrowserAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
// ChrString class specific acceptor methods
Boolean StringBrowserAcceptor::IsStringBrowser() const
{
  return TRUE;
}

/****************************************************************************/
void* StringBrowserAcceptor::CreateFromStringBrowser(const ObjectAcceptor& Obj_)
{
  _VarPtr = new StringBrowser(*((const StringBrowser*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* StringBrowserAcceptor::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new StringBrowser(*((const ChrString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* StringBrowserAcceptor::AssignFromStringBrowser(const ObjectAcceptor& Obj_)
{
  const StringBrowser* StrBrowserPtr_ = (const StringBrowser*)Obj_.GiveDataPtr();
  *_VarPtr = *StrBrowserPtr_;
  return _VarPtr;
}

/****************************************************************************/
void* StringBrowserAcceptor::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const ChrString* StrPtr_ = (const ChrString*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringBrowserAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringBrowserAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if STRINGBROWSER_DEBUG
int main()
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
  return 0;
}
#endif
/****************************************************************************/
#endif





