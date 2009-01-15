#ifndef SIMPLE_DICTIONARY_CPP
#define SIMPLE_DICTIONARY_CPP
#ifndef SIMPLE_DICTIONARY_H
  #include "diction.h"
#endif

/****************************************************************************/
SimpleDictionary::Node_t::Node_t(void* k, void* e, SimpleDictionary::Node_t* n, SimpleDictionary::Node_t* p):
_Key(k),
_Entry(e),
_Next(n),
_Previous(p)
{
  if (_Next != NULL)
    _Next->_Previous = this;

  if (_Previous != NULL)
    _Previous->_Next = this;
}

/****************************************************************************/
SimpleDictionary::Node_t::Node_t():
_Key(NULL),
_Entry(NULL),
_Next(NULL),
_Previous(NULL)
{}

/****************************************************************************/
/****************************************************************************/
SimpleDictionary::SimpleDictionary(const SimpleDictionary& Dict_)
{
  delete _List._Next; // Get rid of current list
  _List._Next = NULL;

  // Copy d's list
  SimpleDictionary::Node_t* Node_ = Dict_._List._Next;

  for (;Node_ != NULL; Node_ = Node_->_Next)
    _List._Next =
      new SimpleDictionary::Node_t(Node_->_Key, Node_->_Entry, _List._Next, _List._Previous);
}

/****************************************************************************/
SimpleDictionary& SimpleDictionary::operator = (const SimpleDictionary& Dict_)
{
  if (this == &Dict_) return *this;
    delete _List._Next; // Get rid of current list

  // Copy d's list
  SimpleDictionary::Node_t* Node_ = Dict_._List._Next;

  for (;Node_ != NULL; Node_ = Node_->_Next)
    _List._Next =
      new SimpleDictionary::Node_t(Node_->_Key, Node_->_Entry, _List._Next, _List._Previous);

  return *this;
}

/****************************************************************************/
void SimpleDictionary::AddEntry(void* key, void* value)
{
  SimpleDictionary::Node_t* Node_ = _List._Next;

  for (;Node_ != NULL; Node_ = Node_->_Next)
    if (Node_->_Key == key)
    {
      Node_->_Entry = value;
      return;
    }

  _List._Next = new SimpleDictionary::Node_t(key, value, _List._Next, NULL);
}

/****************************************************************************/
Boolean SimpleDictionary::At(void* key, void*& value)
{
  SimpleDictionary::Node_t* Node_ = _List._Next;
  for (;Node_ != NULL; Node_ = Node_->_Next)
    if (Node_->_Key == key)
    {
      value = Node_->_Entry;
      return TRUE;
    }

  return FALSE;
}

/****************************************************************************/
void SimpleDictionary::RemoveEntry(void* key)
{
  SimpleDictionary::Node_t* Node_ = &_List;

  for (;Node_->_Next != NULL; Node_ = Node_->_Next)
    if (Node_->_Next->_Key == key)
    {
      SimpleDictionary::Node_t* OldNext_ = Node_->_Next;

      if (Node_->_Next->_Next != NULL)
	Node_->_Next->_Next->_Previous = Node_;

      Node_->_Next = Node_->_Next->_Next;
      OldNext_->_Next = NULL;
      OldNext_->_Previous = NULL;
      delete OldNext_;
      return;
    }
}

/****************************************************************************/
SimpleDictionary::Slot SimpleDictionary::First()
{
  return (void*)_List._Next;
}

/****************************************************************************/
Boolean SimpleDictionary::GetCurrent(Slot slot, void*& key, void*& value)
{
  if (slot == NULL)
    return FALSE;

  SimpleDictionary::Node_t* Node_ = (SimpleDictionary::Node_t*)slot;
  key = Node_->_Key;
  value = Node_->_Entry;
  return TRUE;
}

/****************************************************************************/
Boolean SimpleDictionary::GetNext(Slot& slot, void*& key, void*& value)
{
  if (slot == NULL)
    return FALSE;

  SimpleDictionary::Node_t* Node_ = (SimpleDictionary::Node_t*)slot;
  Node_ = Node_->_Next;
  slot = Node_;
  if (Node_ != NULL)
  {
    key = Node_->_Key;
    value = Node_->_Entry;
    return TRUE;
  }
  else
  {
    key = NULL;
    value = NULL;
    return FALSE;
  }
}

/****************************************************************************/
#endif





