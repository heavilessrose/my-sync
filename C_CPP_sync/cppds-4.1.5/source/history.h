#ifndef HISTORY_H
#define HISTORY_H
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

#ifndef TIMESTAMP_H
  #include "timestmp.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
class History;

struct HistoryNode
{
  friend class History;

  private:
    HistoryNode();
    HistoryNode(const HistoryNode& Obj_);
    HistoryNode(const TimeStamp& Obj_, HistoryNode* Next_=NULL, Boolean Exists_=TRUE);

  public:
    TimeStamp _Time;

    Boolean _Exists;
    HistoryNode* _Next;

    ~HistoryNode();

    void Insert(HistoryNode* Node_);
    void ToggleExistence()
	{ _Exists = !_Exists; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    #if HAS_ARRAY_NEW
      void* operator new[] (size_t Bytes_);
      void operator delete[] (void* Space_);
    #endif
#endif
};

class History
{
  private:
    Boolean _Instance;
    HistoryNode* _History; 			// Newest entry first

    Boolean IsInstance() const
	{ return _Instance; }

    Boolean Exists(const TimeStamp& Time_,
		   HistoryNode*& Prev_, HistoryNode*& Node_) const; // As of given time

  public:
    History();
    History(const TimeStamp& Obj_, Boolean Existence_, Boolean Instance_=TRUE);
    History(const History& Obj_);
    virtual ~History();

    History& AsInstance();
    History& operator = (const History& Hist_);
    History& Copy(const History& Hist_);
    
    Boolean Exists(const TimeStamp& Time_);
    Boolean Exists(const History& Hist_) const;
    Boolean Exists() const; 			// As of now

    void Insert(const TimeStamp& Time_);	// An insertion at given time
    void Remove(const TimeStamp& Time_);	// Removal at given time    

    const TimeStamp& TimeOfBirth() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    #if HAS_ARRAY_NEW
      void* operator new[] (size_t Bytes_);
      void operator delete[] (void* Space_);
    #endif
#endif
};

/****************************************************************************/
#endif





