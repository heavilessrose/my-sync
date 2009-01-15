#ifndef SIMPLE_DICTIONARY_H
#define SIMPLE_DICTIONARY_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif

/****************************************************************************/
// Simple Dictionary class (page 189)
// This is a very simplistic implementation. It is intended only
// to illustrate the principles of threaded iteration. Don't plan
// on winning any awards if you use it in a real project.
//
class SimpleDictionary
{
  private:
    struct Node_t
    {
      void* _Key;
      void* _Entry;

      Node_t* _Next;
      Node_t* _Previous;

      Node_t(void* k, void* e, Node_t* n, Node_t* p);
      Node_t();
      ~Node_t()
	{ delete _Next; }
    };

    Node_t _List; 		// Dummy list head

  public:
    SimpleDictionary(){} 			// Creates an empty dictionary
    SimpleDictionary(const SimpleDictionary&); 	// Copy constructor

    SimpleDictionary& operator = (const SimpleDictionary&);

    void AddEntry(void* key, void* value);
    Boolean At(void* key, void*& value);
    void RemoveEntry(void* key);

    typedef void* Slot; 		// Actual type is buried
    Slot First(); 			// Returns Slot for iteration
					// Equivalent to our "Peek"

    // Equivalent to our "Next"
    Boolean GetCurrent(Slot slot, void*& key, void*& value);

    Boolean GetNext(Slot& slot, void*& key, void*& value);
};

/****************************************************************************/
#endif




