#ifndef SETBASE_H
#define SETBASE_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef SIMPLE_DICTIONARY_H
  #include "diction.h"
#endif
#ifndef HISTORY_H
  #include "history.h"
#endif

/****************************************************************************/
class SetBase : private SimpleDictionary
{
  friend class InternalIterator;

  protected:
    SetBase() {} // To make it abstract

  public:
    class Iterator
    {
      public:
        virtual Boolean More() = 0;
        virtual void* Next() = 0;
    };
    
    SetBase::Iterator* ProvideIterator(); // External iterator

    // AddObject and RemoveObject use the current
    // "time" from the class TimeStamp
    void AddObject(void* entry); 			// Add an object
    void RemoveObject(void* entry);			// Remove the object
    Boolean Exists(void* entry, TimeStamp ts); 	// Was it there?
    Boolean Exists(void* entry); 			// Is it there now?
};

/****************************************************************************/
// Internal iterator for SetBase (page 192)
class InternalIterator : public SetBase::Iterator
{
  private:
    SimpleDictionary* _Dictionary;
    SimpleDictionary::Slot _Slot; 	// Current position
    TimeStamp _MyTime; 	// Time this iterator was born

  public:
    InternalIterator(SimpleDictionary* Dict_);

    virtual Boolean More();
    virtual void* Next();
};

/****************************************************************************/
#endif





