#ifndef HISTORY_CPP
#define HISTORY_CPP
#ifndef HISTORY_H
  #include "history.h"
#endif

/****************************************************************************/
HistoryNode::HistoryNode():
_Exists(TRUE),
_Next(NULL)
{}

/****************************************************************************/
HistoryNode::HistoryNode(const HistoryNode& Obj_):
_Time(Obj_._Time),
_Exists(Obj_._Exists),
_Next(Obj_._Next ? (new HistoryNode(*Obj_._Next)):NULL)
{}

/****************************************************************************/
HistoryNode::HistoryNode(const TimeStamp& Obj_, HistoryNode* Next_, Boolean Exists_):
_Time(Obj_),
_Exists(Exists_),
_Next(Next_)
{}

/****************************************************************************/
HistoryNode::~HistoryNode()
{
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
void HistoryNode::Insert(HistoryNode* Node_)
{
  Node_->_Next = _Next;
  _Next = Node_;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* HistoryNode::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void HistoryNode::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
void* HistoryNode::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if HAS_ARRAY_NEW
void HistoryNode::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
History::History():
_History(NULL),
_Instance(FALSE)
{}

/****************************************************************************/
History::History(const TimeStamp& Obj_, Boolean Existence_, Boolean Instance_):
_History(new HistoryNode(Obj_)),
_Instance(Instance_)
{
  if (!Existence_)
    _History->ToggleExistence();
}

/****************************************************************************/
History::History(const History& Obj_):
_History(Obj_._History ? (new HistoryNode(*Obj_._History)):NULL),
_Instance(Obj_._Instance)
{}

/****************************************************************************/
History::~History()
{
  delete _History;
  _History = NULL;
}

/****************************************************************************/
void History::Insert(const TimeStamp& Time_)
{
  HistoryNode *Ptr_, *Prev_;

  // Not an instance of time
  _Instance = FALSE;

  if (!Exists(Time_, Prev_, Ptr_))
  {
    if (!_History)
      _History = new HistoryNode(Time_, _History);
    else if (Ptr_->_Time != Time_)
    {
      if (Prev_)
	Prev_->Insert(new HistoryNode(Time_));
      else
	_History = new HistoryNode(Time_, _History);
    }
    else
      Ptr_->ToggleExistence();
  }
}

/****************************************************************************/
void History::Remove(const TimeStamp& Time_)
{
  HistoryNode *Ptr_, *Prev_;

  // Not an instance of time
  _Instance = FALSE;

  if (Exists(Time_, Prev_, Ptr_))
  {
    if (Ptr_->_Time != Time_)
    {
      if (Prev_)
	Prev_->Insert(new HistoryNode(Time_, NULL, FALSE));
      else
	_History = new HistoryNode(Time_, _History, FALSE);
    }
    else
      Ptr_->ToggleExistence();
  }
}

/****************************************************************************/
Boolean History::Exists(const TimeStamp& Time_,
			HistoryNode*& Prev_, HistoryNode*& Node_) const
{
  Prev_ = NULL;
  Node_ = _History;

  for (Node_ = _History; Node_ != NULL; Node_ = Node_->_Next)
  {
    if (Node_->_Time <= Time_)
      break;

    Prev_ = Node_;
  }

  if (Node_)
    return Node_->_Exists;

  return FALSE;
}

/****************************************************************************/
Boolean History::Exists(const TimeStamp& Time_)
{
  HistoryNode *Ptr_, *Prev_;
  return Exists(Time_, Prev_, Ptr_);
}

/****************************************************************************/
Boolean History::Exists(const History& Hist_) const
{
  HistoryNode *Ptr_, *Prev_;
  return Exists(Hist_.TimeOfBirth(), Prev_, Ptr_);
}

/****************************************************************************/
Boolean History::Exists() const
{
  return (_History ? _History->_Exists:FALSE);
}

/****************************************************************************/
History& History::AsInstance()
{
  _Instance = TRUE;
  return *this;
}

/****************************************************************************/
History& History::Copy(const History& Hist_)
{
  delete _History;
  _History = (Hist_._History ? (new HistoryNode(*Hist_._History)):NULL);
  _Instance = Hist_._Instance;

  return *this;
}

/****************************************************************************/
History& History::operator = (const History& Hist_)
{
  if (Hist_.IsInstance())
  {
    if (Hist_.Exists())
      Insert(Hist_.TimeOfBirth());
    else
      Remove(Hist_.TimeOfBirth());
  }
  else
  {
    delete _History;
    _History = (Hist_._History ? (new HistoryNode(*Hist_._History)):NULL);
    _Instance = Hist_._Instance;
  }

  return *this;
}

/****************************************************************************/
const TimeStamp& History::TimeOfBirth() const
{
  static TimeStamp Stamp_;

  Stamp_ = TimeStamp();
  return (_History ? _History->_Time:Stamp_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* History::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void History::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
void* History::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if HAS_ARRAY_NEW
void History::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif





