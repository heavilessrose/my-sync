#ifndef BUILTINHND_CPP
#define BUILTINHND_CPP
#ifndef BUILTINHND_H
  #include "bltinhnd.h"
#endif

/****************************************************************************/
BuiltInDeleter::BuiltInDeleter():
_Next(NULL)
{}

/****************************************************************************/
BuiltInDeleter::BuiltInDeleter(const BuiltInDeleter& Obj_):
_Next(Obj_._Next ? Obj_._Next->Clone():NULL)
{}

/****************************************************************************/
BuiltInDeleter::~BuiltInDeleter()
{
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
BuiltInDeleter* BuiltInDeleter::Clone() const
{
  return (new BuiltInDeleter(*this));
}

/****************************************************************************/
void BuiltInDeleter::SetNextPtr(ObjectUpdater* Ptr_)
{
  if (!Ptr_ || Ptr_->IsDeleter())
    _Next = Ptr_;
}

/****************************************************************************/
ObjectUpdater* BuiltInDeleter::GetNextPtr()
{
  return _Next;
}

/****************************************************************************/
ObjectUpdater* BuiltInDeleter::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (_Next)
    _Next->AddNext(Next_, Done_);
  else if (Next_ && Next_->IsDeleter())
  {
    _Next = Next_;
    *Done_ = TRUE;
  }
  else
    *Done_ = FALSE;

  return this;
}

/****************************************************************************/
ObjectUpdater* BuiltInDeleter::TransferBasePtr(ObjectUpdater*)
{
  return this;
}

/****************************************************************************/
Boolean BuiltInDeleter::IsUpdater()
{
  return TRUE;
}

/****************************************************************************/
Boolean BuiltInDeleter::IsDeleter()
{
  return TRUE;
}

/****************************************************************************/
Boolean BuiltInDeleter::DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
{
  Boolean RetCode_ = RawDelete(Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
Boolean BuiltInDeleter::DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
{
  Boolean RetCode_ = RawDelete((VOIDPTR)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* BuiltInDeleter::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void BuiltInDeleter::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* BuiltInDeleter::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void BuiltInDeleter::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif





