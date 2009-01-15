#ifndef PRIMESTABLE_H
#define PRIMESTABLE_H

#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_IOMANIP_H
  #include <iomanip.h>
  #define INCL_IOMANIP_H
#endif
#ifndef LANGSUPP_H
  #include "langsupp.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef BITVECTOR_H
  #include "bitvect.h"
#endif
#ifndef PATHTOKEN_H
  #include "pathtoken.h"
#endif

#define PRIMESTABLE_DEBUG1        0     // Primes table testing
#define PRIMESTABLE_DEBUG_OUTPUT  0     // Show debugging output
#define PRIMESTABLE_DEBUG2        0     // Primes table generation

/****************************************************************************/
class PrimesTable
{
  public:
    static const Ulong MAXENTRIES;

    enum
    {
      TABLESIZE = 256,
      LOOKAHEAD = 20
    };
  
  protected:
    struct TableBuffer
    {
      Ulong _Limit;
      Ulong _Distance;
      Ulong _Table[PrimesTable::TABLESIZE];

      TableBuffer():
        _Limit(0),
        _Distance(0) {}
      TableBuffer(const TableBuffer& Obj_);

      static void Xmem()
          { SHOWFATAL("Error: memory allocation failure"); }

      Ulong TableSize()
          { return (_Limit - LowBound()); }
      Ulong LowBound()
          { return ((_Limit < _Distance) ? 0:(_Limit - _Distance)); }
      Boolean InRange(Ulong Index_)
          { return (_Limit && (LowBound() <= Index_) && (Index_ < _Limit)); }
      void SetLimit(Ulong Start_, Ulong Distance_)
          { _Limit = Start_ + Distance_;
            _Distance = Distance_; }
          
      Ulong GetNumber(Ulong Index_);
      Boolean RetrieveBlock(Ulong Index_, long Offset_, fstream& Fin_);
    };
    
    const char* _DataFile;
    fstream _File;
    long _Offset;
    
    TableBuffer _CurTable;
    Ulong _TotalSize;
    Ulong _TableIndex;
    BitVector _Bitv;

    static void Xmem()
        { SHOWFATAL("Error: memory allocation failure"); }

    void StoreTable();
    Boolean RetrieveTable(Ulong BlockNum_);
    void GenerateTable();

    Boolean SieveFindPrime(Ulong Num_);
    Ulong GetTableElementOnGenerate(Ulong Index_, PrimesTable::TableBuffer& TempTable_);
    Ulong GetTableElement(Ulong Index_, PrimesTable::TableBuffer& TempTable_);
    void GenerateTableHelper(Ulong Number_, PrimesTable::TableBuffer& TempTable_);
    void InitTable();
        
  public:
    // Constructors
    PrimesTable();

    // Destructor
    ~PrimesTable();

    Ulong MaxPrimes();
    Ulong Ceiling(double Value_);
    Ulong Floor(double Value_);
    Ulong NthPrime(Ulong Index_, Boolean* Found_=NULL);
    Boolean IsPrime(Ulong Num_, Boolean* Computable_=NULL);
    Ulong NextPrimeFactor(Ulong Number_, Ulong& Quotient_, Ulong& Exponent_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





