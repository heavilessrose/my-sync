// Slotted File: Fixed length record file layout
//
// 1.	Class ID Byte (long)
// 2.   SlottedFile object Object ID (long)
// 3.   Slottedfile object null status (byte)
// 4.   FileInfo Record
// 5.   File Header (dependent on 4.4)
// 6.   Fixed length record of "Object" objects
//      in standard Object data block format
//
// 4.1  Slot size (long)
// 4.2  Null Object (indefinite)
// 4.3  Slots allocated (long)
// 4.4  Header size (long)
//
// 6.1  Class ID (long)
// 6.2  Object ID (long)
// 6.3  Null object status (byte)
// 6.4  Object data (indefinite)
//
// Slotted file format:
//
// [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ]
//
// FileInfo record format:
//
// [ 4.1 ][ 4.2 ][ 4.3 ][ 4.4 ]
//
// Standard object data block format:
//
// [ 6.1 ][ 6.2 ][ 6.3 ][ 6.4 ]
//
#ifndef SLOTTEDFILE_H
#define SLOTTEDFILE_H
#ifndef FILE_H
  #include "file.h"
#endif

#define SLOTTEDFILE_DEBUG		      0
#define SLOTTEDFILE_RUNTIMECHECK	   0
#define SLOTTEDFILE_TRACE		      0

#if SLOTTEDFILE_TRACE
#  define SLFTRACE(x)	TRACE(x)
#  define SLFMARKER(x)	MARKER(x)
#else
#  define SLFTRACE(x)
#  define SLFMARKER(x)
#endif

/****************************************************************************/
class SlottedFileAcceptor : public ObjectAcceptor
{
  friend class SlottedFile;

  protected:
    union
    {
      const SlottedFile* _ObjPtr;
      SlottedFile* _VarPtr;
    };

    SlottedFileAcceptor(const SlottedFile* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // SlottedFile class specific acceptor methods
    virtual Boolean IsFile() const;
    virtual Boolean IsSlottedFile() const;
    virtual void* CreateFromSlottedFile(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSlottedFile(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class SlottedFileInfo
{
  friend class SlottedFile;

  protected:
    long _SlotSize;
    long _SlotsAllocated;
    long _HeaderSize;
    Object* _ObjectRecord;

    SlottedFileInfo(const SlottedFileInfo& Finfo_);
    SlottedFileInfo(const Object& Record_,
		    long Allocated_, long HeaderSize_);

    static SlottedFileInfo* Make(const SlottedFileInfo& Finfo_);

    istream& ReadFileInfo(istream& Is_);
    ostream& WriteFileInfo(ostream& Os_) const;

  public:
    virtual ~SlottedFileInfo();

    static SlottedFileInfo* Make(const Object& Record_,
				 long Allocated_ = 0, long HeaderSize_ = 1024);

    SlottedFileInfo& operator = (const SlottedFileInfo& Finfo_);

    SlottedFileInfo* AllocateSlot()
	{ return AllocateSlots(1); }
    SlottedFileInfo* AllocateSlots(long NewSlots_);

    SlottedFileInfo* SetHeaderSize(long Size_);

    long FormWidth() const
	{ return long(sizeof(long) * 3 + SlotSize()); }
    long DataSpan() const
	{ return long(FormWidth() + _HeaderSize + _SlotSize * _SlotsAllocated); }

    const Object* NullRecord() const
	{ return _ObjectRecord; }
    long HeaderSize() const
	{ return _HeaderSize; }
    long SlotSize() const
	{ return _SlotSize; }
    long AllocatedSlots() const
	{ return _SlotsAllocated; }

    Boolean operator == (SlottedFileInfo& Info_);
    Boolean operator != (SlottedFileInfo& Info_)
	{ return !(*this == Info_); }

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
class SlottedFileBrowser : public StreamBrowser
{
  public:
    SlottedFileBrowser(ByteStream* Stream_);

    // stream methods modified to work using custom
    // defined records as basic units instead of bytes
    virtual ByteStream& SeekGet(long RecNum_);
    virtual ByteStream& SeekPut(long RecNum_);
    virtual ByteStream& SeekGet(long Incr_, int SeekDir_);
    virtual ByteStream& SeekPut(long Incr_, int SeekDir_);
    virtual long TellGet();
    virtual long TellPut();
    virtual long SizeGet();
    virtual long SizePut();
    virtual ByteStream& ReadRecord();
    virtual ByteStream& PeekRecord();
    virtual ByteStream& WriteRecord();
    virtual ByteStream& AppendRecord();
    virtual Boolean Verified() const;

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser();
    virtual void SetByteStreamBrowser(ByteStream* Browser_);
    virtual ByteStream* GetByteStreamBrowser() const;

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator();
};

/****************************************************************************/
class SlottedFile : public File
{
  friend class SlottedFileAcceptor;

  friend Object& RootObject(SlottedFile& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const SlottedFile& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:  
    Boolean InitializeHeader();
    inline void ClearStatus(Byte_t StatBit_)
	{ _Status &= ~StatBit_; }

    enum
    {
      SLOTTEDFILE_READY			= 0x00,
      SLOTTEDFILE_HEADER_NOTREAD	= 0x01,
      SLOTTEDFILE_INFO_NOTREAD		= 0x02,
      SLOTTEDFILE_NOT_OPENED		= 0x04,
      SLOTTEDFILE_NOTREADY		= 7
    };

    SlottedFileInfo* _Info;
    Byte_t _Status;
    Boolean _StoreToHeader;

    // ----- Saving and restoration in binary form ----
    Boolean ReadFileInfo();
    Boolean WriteFileInfo() const;

    // Slotted file check method
    virtual Boolean TestForSlottedFile();

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
    SlottedFile(SlottedFileInfo* Info_);
    SlottedFile(const File& Finfo_, SlottedFileInfo* Info_);
    SlottedFile(const SlottedFile& Finfo_);
    virtual ~SlottedFile();

    // Stream Methods
    // overriding binary input stream methods
    virtual ByteStream& SeekGet(long RecNum_);
    virtual ByteStream& SeekGet(long RecNum_, int SeekDir_);
    virtual istream& iseekg(long Pos_);
    virtual istream& iseekg(long Pos_, int Dir_);
    virtual long itellg();

    // overriding binary output stream methods
    virtual ByteStream& SeekPut(long RecNum_);
    virtual ByteStream& SeekPut(long RecNum_, int SeekDir_);
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
    static SlottedFile* Make(SlottedFileInfo* Info_);
    static SlottedFile* Make(const File& Finfo_, SlottedFileInfo* Info_);
    static SlottedFile* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static SlottedFile& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SLOTTEDFILE_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Slotted file data mutator method
    virtual SlottedFile& SetSlottedFileProperties(SlottedFileInfo* Info_);

    // assignment operator
    SlottedFile& operator = (const SlottedFile& Sinfo_);

    // file open procedures
    virtual Boolean Open(const char* Path_=NULL, int Modes_=0);

    // file close procedures
    virtual Boolean Close();

    // Slotted file check method
    Boolean IsSlottedFile(const char* Path_=NULL) const;

    // Record and slot manipulation methods
    Boolean IsValidSlot(long SlotNum_) const;
    long AllocateSlot();
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
    virtual Boolean ReadHeader(char* Header_, long Size_=0) const;
    virtual Boolean WriteHeader(const char* Header_, long Size_=0);
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





