#ifndef SETBASE_CPP
#define SETBASE_CPP
#ifndef SETBASE_H
  #include "setbase.h"
#endif

/****************************************************************************/
InternalIterator::InternalIterator(SimpleDictionary* Dict_):
_Dictionary(Dict_),
_Slot(Dict_->First()),
_MyTime()
{}

/****************************************************************************/
// Internal iterator for SetBase (page 192)
Boolean InternalIterator::More()
{
  void* key;
  void* h;

  // _Slot is a data member of InternalIterator pointing to the current position
  if (!_Dictionary->GetCurrent(_Slot, key, h))
    return FALSE; // _Slot already beyond bounds

  // loop until the most recently valid object is found in the dictionary
  // that matches the time stamp of the birth of this iterator.
  // if a match is found, the _Slot in which it is found in the dictionary
  // is stored in "_Slot" then true is returned
  do
    if (((History*)h)->Exists(_MyTime))
      return TRUE;
  while (_Dictionary->GetNext(_Slot, key, h));

  // No valid time stamped entry in dictionary is found that is equal or
  // less than the time of birth of this iterator
  // Or the object's existence status is set FALSE.
  return FALSE;
}

/****************************************************************************/
void* InternalIterator::Next()
{
  void* key;
  void* key1;
  void* h;

  // We assume the client called More() first, so
  // the object at GetNext is current "_Slot"
  _Dictionary->GetCurrent(_Slot, key, h); 	// get current item
  _Dictionary->GetNext(_Slot, key1, h);		// pre-increment to next item.
  return key;					// return key of current item.
}

/****************************************************************************/
// Abstract "Set" base class (pages 190-191)
// Create internal iterator with the dictionary argument set to this SetBase
// object.
SetBase::Iterator* SetBase::ProvideIterator()
{
  return (new InternalIterator(this));
}

/****************************************************************************/
void SetBase::AddObject(void* entry)
{
  void* Void_;
  History* Hist_;

  if (At(entry, Void_))
  {
    // Already there - check time
    Hist_ = (History*)Void_;

    // The current Time Stamp is inserted in case other iterators from a
    // different thread "removes" the object from the set by time stamping
    // it at removal time. Removal actually means that the object has an
    // even number of timestamps since the first insertion of the object is
    // the first time stamp. If the object is actually in the list a time
    // stamp must be added during every insertion / removal operation to keep
    // its membership status in the set "current"
    if (!Hist_->Exists()) // Must do an "insertion"
      Hist_->Insert(TimeStamp());
  }
  else
  { // Not there yet
    // The object is not actually physically in the set yet so this would
    // be its first "real" insertion into the actual data structure.
    Hist_ = new History;
    Hist_->Insert(TimeStamp());
    AddEntry(entry, Hist_);
  }
}

/****************************************************************************/
void SetBase::RemoveObject(void* entry)
{
  void* Void_;
  History* Hist_;

  if (At(entry, Void_))
  { // Already there - check time
    Hist_ = (History*)Void_;

    // Note that the history object is used so the actual object is not
    // removed from the set. Instead the current time stamp is placed into
    // the history list of the object. The existence status of the object is
    // set FALSE by the history remove method.
    if (Hist_->Exists()) // Must do a deletion
      Hist_->Remove(TimeStamp());
  }
}

/****************************************************************************/
// existence status of an object as of the specified "current" time
Boolean SetBase::Exists(void* entry, TimeStamp ts)
{
  void* Void_;
  return At(entry, Void_) && ((History*)Void_)->Exists(ts);
}

/****************************************************************************/
// existence status of an object as of the real current time -- NOW
// which means the current existence status stored in the history object
Boolean SetBase::Exists(void* entry)
{
  void* Void_;
  return At(entry, Void_) && ((History*)Void_)->Exists();
}

/****************************************************************************/
#endif





