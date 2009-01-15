//
// Variable length data file algorithm test program
// DON'T ERASE THIS!
//
// Variable length file: variable length record file layout
// --------------------------------------------------------
//
// 1.   Class ID Byte (long)
// 2.   VarLengthFile object Object ID (long)
// 3.   VarLengthFile object null status (byte)
// 4.   FileInfo Record
// 5.   File Header (dependent on 4.5)
// 6.   count-down running total of remaining data (long)
// 7.   variable length record of "Object" objects
//      in standard object data block format
//
// 4.1  chunk size (long)
// 4.2  initial run (long)
// 4.3  file config (long x MAXCONFIG)
// 4.4  highest record (long)
// 4.5  header size (long)
// 4.6  file size (long)
// 4.7  index (unsigned long)
//
// 7.1  Class ID (long)
// 7.2  Object ID (long)
// 7.3  Null object status (byte)
// 7.4  Object data (indefinite)
//
// Variable record format:
//
// [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ][ 7 ]
//
// FileInfo record format:
//
// [ 4.1 ][ 4.2 ][ 4.3 ][ 4.4 ][ 4.5 ][ 4.6 ][ 4.7 ]
//
// Standard object data block format:
//
// [ 7.1 ][ 7.2 ][ 7.3 ][ 7.4 ]
//
// if [6] is non-zero we are midway within a record
// if [6] has a value of zero then we are at the end of the record
//
#ifndef VLENALGO_H
#define VLENALGO_H

#include <assert.h>
#include <fstream.h>
#include <string.h>
#include "userdefs.h"

#define VLENALGO_DEBUG  1
#define VLENALGO_TEST1  0
#define VLENALGO_TEST2  1

#if VLENALGO_DEBUG
  #include "rand.h"
#endif

/****************************************************************************/
class TestClass
{
  struct VarLengthFilePosInfo
  {
    private:
      long SeekLimitSize() const
           { return _InfoPtr->SeekLimitSize(); }
      Boolean IsValidFilePos(long Pos_) const
           { return (0 <= Pos_ && Pos_ <= SeekLimitSize()); }
  
    public:    
      TestClass* _InfoPtr;
      long** _ChunkArray;
      long _RecordNum;
      long _RecordPos;
      long _ChunkNum;
      long _ChunkPos;
      long _RealBytePos;
      long _LinearBytePos;

      VarLengthFilePosInfo(TestClass* ParPtr_);
      VarLengthFilePosInfo(TestClass* ParPtr_, TestClass::VarLengthFilePosInfo* InfoPtr_);
      ~VarLengthFilePosInfo();

      Boolean SeekRecNum(long RecNum_);
      Boolean SeekFilePos(long Pos_);
      Boolean SeekFilePos(long Pos_, ios::seek_dir Dir_);
      Boolean UpdateFilePos();

      long Read(istream& Is_, char* Buffer_, long Max_);         // new
      long Write(ostream& Os_, const char* Buffer_, long Max_);        // new
      void Reset();
  };

  friend VarLengthFilePosInfo;

  protected:
    enum
    {
      FILECHUNK = 32,
      INITIALRUN = 4,
      MAXCONFIG = 32
    };

    static long CONFIGSIZE()
	{ return (MAXCONFIG * sizeof(long)); }

    long _ChunkSize;
    long _InitialRun;
    long _FileConfig[MAXCONFIG];
    
    char* _NullBlock;
    long _HeaderSize;
    long _HighestRecord;
    long _RecordCount;
    long _FileSize;    
    long _Index;

    VarLengthFilePosInfo* _InputPos;
    VarLengthFilePosInfo* _OutputPos;

    long _FileBase;
    long _FileOffset;

    long Pow2(size_t Exp_) const;
    size_t Exponent() const
        { return (_Index - 2); }
    long NestingValue(size_t Exp_) const
        { return Pow2(Exp_); }        
    long NumberOfPages(size_t Exp_) const
        { return _FileConfig[Exp_ + 2]; }
    long PageNumber(long RecNum_, size_t Exp_) const
        { return (RecordNumber(RecNum_, Exp_) / (SegmentsPerPage(Exp_) / NestingValue(Exp_))); }
    long SeekLimitSize() const
        { return (GiveFileOffset() + ((HighestRecord() + 1) * RecordSize())); }
        
    long PageSeek(long RecNum_, size_t Exp_) const;
    long ClusterSeek(size_t Exp_, size_t Index_) const;
    long RecordSeek(long RecNum_) const;
        
    long RecordNumber(long RecNum_, size_t Exp_) const;
    long SegmentsPerPage(size_t Exp_) const;

    void AddPage();
    void IncreaseRecSize();

    long FindStartPosHelper(long RecNum_, size_t Index_, size_t Exp_);
    long FindStartPos(long RecNum_, size_t Index_)
        { return (_FileConfig[0] * FindStartPosHelper(RecNum_, Index_, Exponent())); }

    // validate with _HighestRecord
    long** GetRecFnc(long RecNum_);

    TestClass(long ChunkSize_=FILECHUNK, long InitialRun_=INITIALRUN,
              long Allocated_=0, long HeaderSize_=0);
    TestClass(const TestClass& Obj_);              

    static TestClass* Make(const TestClass& Finfo_);

    long** AddFileOffset(long** PosArray_);    

/*
    // Read/Write file information
    istream& ReadFileInfo(istream& Is_);
    ostream& WriteFileInfo(ostream& Os_) const;
*/

  public:  
    virtual ~TestClass();

    // Read/Write file information (in protected section)
    istream& ReadFileInfo(istream& Is_);
    ostream& WriteFileInfo(ostream& Os_) const;

    long HeaderSize() const
	{ return _HeaderSize; }
    long ChunkSize() const
	{ return _ChunkSize; }
    long InitialRun() const
	{ return _InitialRun; }
    long HighestRecord() const
        { return _HighestRecord; }
    const char* NullBlock() const
        { return _NullBlock; }
    long FileSize() const
        { return _FileSize; }

    // current capacity of the data file in number of records
    long FileCapacity() const;

    // size of each record in bytes
    long RecordSize() const
        { return (_FileConfig[0] * Pow2(Exponent())); }
    long ChunksPerRecord() const
        { return (RecordSize() / ChunkSize()); }

    // validate with _HighestRecord
    long** GetRec(long RecNum_);
    long** GetRec()
        { return GetRec(_InputPos->_RecordNum); }
    
    // update number of records
    long PutRec(long RecNum_, long Size_);
    long PutRec(long Size_)
        { return PutRec(_OutputPos->_RecordNum, Size_); }

    static TestClass* Make(long ChunkSize_=FILECHUNK, long InitialRun_=INITIALRUN,
                           long Allocated_=0, long HeaderSize_=0);

    TestClass* SetHeaderSize(long Size_);
    TestClass* SetFileOffset(long Offset_);

    TestClass* AllocateSlot()
	{ return AllocateSlots(1); }
    TestClass* AllocateSlots(long NewSlots_);

    long FormWidth() const
        { return (7 * sizeof(long) + CONFIGSIZE()); }
    long DataSpan() const
	{ return long(FormWidth() + _HeaderSize + _FileSize); }
//    long DataSpan() const
//	{ return long(FormWidth() + _HeaderSize + ((HighestRecord() + 1) * RecordSize())); }

    Boolean operator == (TestClass& Info_);
    Boolean operator != (TestClass& Info_)
	{ return !(*this == Info_); }

    // File operations
    TestClass::VarLengthFilePosInfo* SeekGet(istream& Is_, long Pos_);
    TestClass::VarLengthFilePosInfo* SeekGet(istream& Is_, long Pos_, ios::seek_dir Dir_);
    TestClass::VarLengthFilePosInfo* Seekg(istream& Is_, long Pos_);
    TestClass::VarLengthFilePosInfo* Seekg(istream& Is_, long Pos_, ios::seek_dir Dir_);
    TestClass::VarLengthFilePosInfo* TellGet();

    TestClass::VarLengthFilePosInfo* SeekPut(ostream& Os_, long Pos_);
    TestClass::VarLengthFilePosInfo* SeekPut(ostream& Os_, long Pos_, ios::seek_dir Dir_);
    TestClass::VarLengthFilePosInfo* Seekp(ostream& Os_, long Pos_);
    TestClass::VarLengthFilePosInfo* Seekp(ostream& Os_, long Pos_, ios::seek_dir Dir_);
    TestClass::VarLengthFilePosInfo* TellPut();

    long Read(istream& Is_, char* Buffer_, long Max_);          // new
    long Write(ostream& Os_, const char* Buffer_, long Max_);   // new
    long ReadRec(istream& Is_, char* Buffer_, long Max_);
    long WriteRec(ostream& Os_, const char* Buffer_, long Max_);
    TestClass::VarLengthFilePosInfo* InputPos()
         { return _InputPos; }
    TestClass::VarLengthFilePosInfo* OutputPos()
         { return _OutputPos; }

    long FileSize(istream& Is_);        // new
    long FileSize(ostream& Os_);        // new
    long UpdateFileSize(ostream& Os_);  // new

    // File position offset methods
    long GiveFileOffset() const
         { return (_FileBase + _FileOffset); }
    long** MinusFileOffset(long** PosArray_);
    static long** DeletePosArray(long** PosArray_);

/*
#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
*/
};

/****************************************************************************/
#endif
