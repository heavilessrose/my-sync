#ifndef STRINGSTORE_CPP
#define STRINGSTORE_CPP
#ifndef STRINGSTORE_H
  #include "strstore.h"
#endif

/****************************************************************************/
/****************************************************************************/
StringStorePtr::StringStorePtr():
ArrayStorePtr<char>()
{}

/****************************************************************************/
StringStorePtr::StringStorePtr(const char* Ptr_):
ArrayStorePtr<char>(Ptr_, strlen(Ptr_) + 1)
{}

/****************************************************************************/
StringStorePtr::StringStorePtr(char* Ptr_, int Flags_):
ArrayStorePtr<char>(Ptr_, strlen(Ptr_) + 1, Flags_)
{}

/****************************************************************************/
StringStorePtr::StringStorePtr(const DataStorePtr<char>& Obj_):
ArrayStorePtr<char>(Obj_)
{}

/****************************************************************************/
StringStorePtr::StringStorePtr(const ArrayStorePtr<char>& Obj_):
ArrayStorePtr<char>(Obj_)
{}

/****************************************************************************/
StringStorePtr::StringStorePtr(const StringStorePtr& Obj_):
ArrayStorePtr<char>(Obj_)
{}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make()
{
  return (new StringStorePtr());
}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make(const char* Ptr_)
{
  return (new StringStorePtr(Ptr_));
}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make(char* Ptr_, int Flags_)
{
  return (new StringStorePtr(Ptr_, Flags_));
}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make(const DataStorePtr<char>& Obj_)
{
  return (new StringStorePtr(Obj_));
}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make(const ArrayStorePtr<char>& Obj_)
{
  return (new StringStorePtr(Obj_));
}

/****************************************************************************/
StringStorePtr* StringStorePtr::Make(const StringStorePtr& Obj_)
{
  return (new StringStorePtr(Obj_));
}

/****************************************************************************/
ArrayStorePtr<char>* StringStorePtr::Clone()
{
  return (new StringStorePtr(*this));
}

/****************************************************************************/
int StringStorePtr::ClassID() const
{
  return TypeInfo::STRINGSTOREPTR_TYPE;
}

/****************************************************************************/
StringStorePtr& StringStorePtr::operator = (const DataStorePtr<char>& rhs)
{
  ArrayStorePtr<char>::operator = (rhs);
  return *this;
}

/****************************************************************************/
StringStorePtr& StringStorePtr::operator = (const ArrayStorePtr<char>& rhs)
{
  ArrayStorePtr<char>::operator = (rhs);
  return *this;
}

/****************************************************************************/
StringStorePtr& StringStorePtr::operator = (const StringStorePtr& rhs)
{
  if (this != &rhs)
    ArrayStorePtr<char>::operator = (rhs);

  return *this;
}

/****************************************************************************/
StringStorePtr& StringStorePtr::AssignPtr(char* Ptr_, int Flags_)
{
  ArrayStorePtr<char>::AssignPtr(Ptr_, strlen(Ptr_) + 1, Flags_);
  return *this;
}

/****************************************************************************/
StringStorePtr& StringStorePtr::AssignObject(const char* Ptr_)
{
  ArrayStorePtr<char>::AssignObject(Ptr_, strlen(Ptr_) + 1);
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringStorePtr::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringStorePtr::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
void* StringStorePtr::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringStorePtr::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if STRINGSTORE_DEBUG
int main()
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

#if STRINGSTORE_NOTIFY_SUCCESS
  cout <<"SUCCESS Testing StringStorePtr classes" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




