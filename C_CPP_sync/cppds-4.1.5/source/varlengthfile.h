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
// 5.   File Header (dependent on 4.6)
// 6.   variable length record of "Object" objects
//      in standard object data block format
//
// 4.1  object record size (long)
// 4.2  object record (indefinite)
// 4.3  chunk size (long)
// 4.4  initial run (long)
// 4.5  file config (long x MAXCONFIG), MAXCONFIG == 32
// 4.6  header size (long)
// 4.7  highest record (long)
// 4.8  record count
// 4.9  file size (long)
// 4.10 index (unsigned long)
//
// 6.1  Class ID (long)
// 6.2  Object ID (long)
// 6.3  Null object status (byte)
// 6.4  Object data (indefinite)
//
// Variable record format:
//
// [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ]
//
// FileInfo record format:
//
// [ 4.1 ][ 4.2 ][ 4.3 ][ 4.4 ][ 4.5 ][ 4.6 ][ 4.7 ][ 4.8 ][ 4.9 ][ 4.10 ]
//
// Standard object data block format:
//
// [ 6.1 ][ 6.2 ][ 6.3 ][ 6.4 ]
//
#ifndef VARLENGTHFILE_H
#define VARLENGTHFILE_H
#ifndef FILE_H
  #include "file.h"
#endif

#define VARLENGTHFILE_DEBUG		   0
#define VARLENGTHFILE_RUNTIMECHECK	0
#define VARLENGTHFILE_TRACE		   0

#if VLENFILE_TRACE
#  define VLFTRACE(x)	TRACE(x)
#  define VLFMARKER(x)	MARKER(x)
#else
#  define VLFTRACE(x)
#  define VLFMARKER(x)
#endif

/****************************************************************************/
class VarLengthFileAcceptor : public ObjectAcceptor
{
  friend class VarLengthFile;

  protected:
    union
    {
      const VarLengthFile* _ObjPtr;
      VarLengthFile* _VarPtr;
    };

    VarLengthFileAcceptor(const VarLengthFile* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // VarLengthFile class specific acceptor methods
    virtual Boolean IsFile() const;
    virtual Boolean IsVarLengthFile() const;
    virtual void* CreateFromVarLengthFile(const ObjectAcceptor& Obj_);
    virtual void* AssignFromVarLengthFile(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class VarLengthFileInfo
{
  struct VarLengthFilePosInfo
  {
    private:
      long SeekLimitSize() const
           { return _InfoPtr->SeekLimitSize(); }
      Boolean IsValidFilePos(long Pos_) const
           { return (0 <= Pos_ && Pos_ <= SeekLimitSize()); }
  
    public:    
      VarLengthFileInfo* _InfoPtr;
      long** _ChunkArray;
      long _RecordNum;
      long _RecordPos;
      long _ChunkNum;
      long _ChunkPos;
      long _RealBytePos;
      long _LinearBytePos;

      VarLengthFilePosInfo(VarLengthFileInfo* ParPtr_);
      VarLengthFilePosInfo(VarLengthFileInfo* ParPtr_, VarLengthFileInfo::VarLengthFilePosInfo* InfoPtr_);
      ~VarLengthFilePosInfo();

      Boolean SeekRecNum(long RecNum_);
      Boolean SeekFilePos(long Pos_);
      Boolean SeekFilePos(long Pos_, int Dir_);
      Boolean UpdateFilePos();

      long Read(istream& Is_, char* Buffer_, long Max_);         // new
      long Write(ostream& Os_, const char* Buffer_, long Max_);        // new
      void Reset();

#if OVERLOAD_NEW
      void* operator new (size_t Bytes_);
      void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
      void* operator new[] (size_t Bytes_);
      void operator delete[] (void* Space_);
#endif
#endif      
  };

  friend class VarLengthFilePosInfo;
  friend class VarLengthFile;

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
    long _ObjectRecordSize;
    long _HeaderSize;
    long _HighestRecord;
    long _RecordCount;
    long _FileSize;    
    long _Index;    

    VarLengthFilePosInfo* _InputPos;
    VarLengthFilePosInfo* _OutputPos;

    long _FileBase;
    long _FileOffset;
    Object* _ObjectRecord;    

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

    VarLengthFileInfo(const Object& Record_,
                      long ChunkSize_=FILECHUNK, long InitialRun_=INITIALRUN,
                      long Allocated_=0, long HeaderSize_=0);
    VarLengthFileInfo(const VarLengthFileInfo& Obj_);              

    static VarLengthFileInfo* Make(const VarLengthFileInfo& Finfo_);

    long** AddFileOffset(long** PosArray_);    

    // Read/Write file information
    istream& ReadFileInfo(istream& Is_);
    ostream& WriteFileInfo(ostream& Os_) const;

  public:  
    virtual ~VarLengthFileInfo();

    VarLengthFileInfo& operator = (const VarLengthFileInfo& Obj_);

    const Object* NullRecord() const
	{ return _ObjectRecord; }
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
    long ObjectRecordSize() const
        { return _ObjectRecordSize; }

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

    static VarLengthFileInfo* Make(const Object& Record_,
                                   long ChunkSize_=FILECHUNK, long InitialRun_=INITIALRUN,
                                   long Allocated_=0, long HeaderSize_=1024);

    VarLengthFileInfo* SetHeaderSize(long Size_);
    VarLengthFileInfo* SetFileOffset(long Offset_);

    VarLengthFileInfo* AllocateSlot()
	{ return AllocateSlots(1); }
    VarLengthFileInfo* AllocateSlots(long NewSlots_);

    long FormWidth() const
        { return (8 * sizeof(long) + CONFIGSIZE() + ObjectRecordSize()); }
    long DataSpan() const
	{ return long(FormWidth() + _HeaderSize + _FileSize); }
//    long DataSpan() const
//	{ return long(FormWidth() + _HeaderSize + ((HighestRecord() + 1) * RecordSize())); }

    Boolean operator == (VarLengthFileInfo& Info_);
    Boolean operator != (VarLengthFileInfo& Info_)
	{ return !(*this == Info_); }

    // File operations
    VarLengthFileInfo::VarLengthFilePosInfo* SeekGet(istream& Is_, long Pos_);
    VarLengthFileInfo::VarLengthFilePosInfo* SeekGet(istream& Is_, long Pos_, int Dir_);
    VarLengthFileInfo::VarLengthFilePosInfo* Seekg(istream& Is_, long Pos_);
    VarLengthFileInfo::VarLengthFilePosInfo* Seekg(istream& Is_, long Pos_, int Dir_);
    VarLengthFileInfo::VarLengthFilePosInfo* TellGet();

    VarLengthFileInfo::VarLengthFilePosInfo* SeekPut(ostream& Os_, long Pos_);
    VarLengthFileInfo::VarLengthFilePosInfo* SeekPut(ostream& Os_, long Pos_, int Dir_);
    VarLengthFileInfo::VarLengthFilePosInfo* Seekp(ostream& Os_, long Pos_);
    VarLengthFileInfo::VarLengthFilePosInfo* Seekp(ostream& Os_, long Pos_, int Dir_);
    VarLengthFileInfo::VarLengthFilePosInfo* TellPut();

    long Read(istream& Is_, char* Buffer_, long Max_);
    long Write(ostream& Os_, const char* Buffer_, long Max_);
    long ReadRec(istream& Is_, char* Buffer_, long Max_);
    long WriteRec(ostream& Os_, const char* Buffer_, long Max_);
    VarLengthFileInfo::VarLengthFilePosInfo* InputPos()
         { return _InputPos; }
    VarLengthFileInfo::VarLengthFilePosInfo* OutputPos()
         { return _OutputPos; }

    long FileSize(istream& Is_);
    long FileSize(ostream& Os_);
    long UpdateFileSize(ostream& Os_);

    // File position offset methods
    long GiveFileOffset() const
         { return (_FileBase + _FileOffset); }
    long** MinusFileOffset(long** PosArray_);
    static long** DeletePosArray(long** PosArray_);

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
class VarLengthFileBrowser : public StreamBrowser
{
  public:
    VarLengthFileBrowser(ByteStream* Stream_);

    // stream methods modified to work using custom
    // defined records as basic units instead of bytes
    virtual ByteStream& ReadRecord();
    virtual ByteStream& PeekRecord();
    virtual ByteStream& WriteRecord();
    virtual Boolean Verified() const;

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser();
    virtual void SetByteStreamBrowser(ByteStream* Browser_);
    virtual ByteStream* GetByteStreamBrowser() const;

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator();
};

/****************************************************************************/
class VarLengthFile : public File
{
  friend class VarLengthFileAcceptor;

  friend Object& RootObject(VarLengthFile& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const VarLengthFile& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:  
    Boolean InitializeHeader();
    inline void ClearStatus(Byte_t StatBit_)
	{ _Status &= ~StatBit_; }

    enum
    {
      VARLENGTHFILE_READY		= 0x00,
      VARLENGTHFILE_HEADER_NOTREAD	= 0x01,
      VARLENGTHFILE_INFO_NOTREAD	= 0x02,
      VARLENGTHFILE_NOT_OPENED	        = 0x04,
      VARLENGTHFILE_NOTREADY		= 7
    };

    VarLengthFileInfo* _Info;
    Byte_t _Status;
    Boolean _StoreToHeader;

    // ----- Saving and restoration in binary form ----
    Boolean ReadFileInfo();
    Boolean WriteFileInfo() const;

    // VarLength file check method
    virtual Boolean TestForVarLengthFile();

    // file section offsets
    long HeaderSection() const;
    long RecordSection() const;
    long EndOfFileOffset() const;

    virtual Boolean IsValidSeek(long RecNum_, int SeekDir_, int IOop_);
    virtual Boolean IsValidSlot(long SlotNum_, short IncludeEof_) const;
    virtual long NextRecordID(long CurSlot_, ObjectData& ObjData_);

    void AppendIDToHeader(long IDvalue_);
    void ModifyIDInHeader(long SlotNum_, long IDvalue_);

  public:
    VarLengthFile(VarLengthFileInfo* Info_);
    VarLengthFile(const File& Finfo_, VarLengthFileInfo* Info_);
    VarLengthFile(const VarLengthFile& Finfo_);
    virtual ~VarLengthFile();

    // Stream Methods
    virtual ByteStream& AppendRecord();
    
    // overriding binary input stream methods
    virtual ByteStream& SeekGet(long RecNum_);
    virtual ByteStream& SeekGet(long RecNum_, int SeekDir_);
    virtual long TellGet();
    virtual long SizeGet();
    virtual istream& iread(char* Bytes_, Ulong Max_);
    virtual istream& iseekg(long Pos_);
    virtual istream& iseekg(long Pos_, int Dir_);
    virtual long itellg();

    // overriding binary output stream methods
    virtual ByteStream& SeekPut(long RecNum_);
    virtual ByteStream& SeekPut(long RecNum_, int SeekDir_);
    virtual long TellPut();
    virtual long SizePut();
    virtual ostream& owrite(const char* Bytes_, Ulong Max_);
    virtual ostream& oseekp(long Pos_);
    virtual ostream& oseekp(long Pos_, int Dir_);
    virtual long otellp();

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser();
    virtual void SetByteStreamBrowser(ByteStream* Browser_);
    virtual ByteStream* GetByteStreamBrowser() const;

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator();

    // Virtual constructor and assignment methods
    static VarLengthFile* Make(VarLengthFileInfo* Info_);
    static VarLengthFile* Make(const File& Finfo_, VarLengthFileInfo* Info_);
    static VarLengthFile* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static VarLengthFile& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::VARLENGTHFILE_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // VarLength file data mutator method
    virtual VarLengthFile& SetVarLengthFileProperties(VarLengthFileInfo* Info_);

    // assignment operator
    VarLengthFile& operator = (const VarLengthFile& Sinfo_);

    // file open procedures
    virtual Boolean Open(const char* Path_=NULL, int Modes_=0);

    // file close procedures
    virtual Boolean Close();

    // VarLength file check method
    Boolean IsVarLengthFile(const char* Path_=NULL) const;

    // Record and vlen manipulation methods
    Boolean IsValidSlot(long SlotNum_) const;
    long AllocateSlot(const Object* Record_);
    long SlotSize() const;
    long SlotsAllocated() const;

    virtual long AddRecord(const Object& Record_);
    virtual Boolean DeleteRecord(long SlotNum_);
    virtual long FirstRecord(Object& Record_);
    virtual Boolean ModifyRecord(long SlotNum_, const Object& Record_);
    virtual long NextRecord(long CurSlot_, Object& Record_);
    virtual long NthRecord(long NthRecNum_, Object& Record_);
    virtual long NthRecordSlot(long NthRecNum_);
    virtual Boolean RetrieveRecord(long SlotNum_, Object& Record_);
    virtual const Object& NullRecord() const;
    virtual int IdentifyRecord(long NthRecNum_);

    // File header methods
    long HeaderSize() const;
    virtual Boolean ReadHeader(Byte_t* Header_, long Size_=0) const;
    virtual Boolean WriteHeader(const Byte_t* Header_, long Size_=0);
    Boolean StoreIDToHeader(Boolean Flag_=TRUE);
    Boolean ReadIDFromHeader(long* HeaderData_) const;

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





