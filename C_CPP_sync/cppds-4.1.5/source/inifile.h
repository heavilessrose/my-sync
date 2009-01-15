// Custom ini file class: for the creation and manipulation of windows type
// .ini files
//
#ifndef INIFILE_H
#define INIFILE_H
#ifndef FILE_H
  #include "file.h"
#endif

#define INIFILE_DEBUG		0

/****************************************************************************/
class IniEntry : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, IniEntry)
  
  protected:
    char* _Key;
    char* _Value;
    Byte_t _Deleted;
    IniEntry* _Next;

  public:
    IniEntry();
    IniEntry(const char* Key_, const char* Val_);
    IniEntry(const IniEntry& Obj_);
    virtual ~IniEntry();

    static IniEntry* Make();
    static IniEntry* Make(const char* Key_, const char* Val_);
    static IniEntry* Make(const IniEntry& Obj_);

    Boolean WriteTo(FileReader* FileRead_);
    IniEntry& ReplaceEntry(const IniEntry& Obj_);
    IniEntry& operator = (const IniEntry& Obj_);

    IniEntry& SetDeleted(Boolean Flag_=TRUE);
    Boolean IsDeleted() const
        { return _Deleted; }

    IniEntry& SetKey(const char* Key_);
    IniEntry& SetValue(const char* Val_);
    IniEntry* SetNext(IniEntry* Next_);
    IniEntry* Append(IniEntry* Next_);

    const char* Key() const;
    const char* Value() const;
    const IniEntry* Next(long Num_=1) const;
    IniEntry* Next(long Num_=1);

    const IniEntry* Last() const;
    IniEntry* Last();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif    
};

/****************************************************************************/
class IniRecord : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, IniRecord)

  protected:
    char* _SectionName;
    long _NumEntries;
    
    Byte_t _Deleted;
    IniEntry* _Entry;
    IniRecord* _Next;    

    IniEntry* ReallyDeleteEntry(long SlotNum_, Boolean& Done_);
  
  public:
    IniRecord();
    IniRecord(const IniRecord& Obj_);
    IniRecord(const char* Name_);
    virtual ~IniRecord();

    static IniRecord* Make();
    static IniRecord* Make(const IniRecord& Obj_);
    static IniRecord* Make(const char* Name_);

    virtual Boolean UpdateList();

    Boolean WriteTo(FileReader* FileRead_);
    IniRecord& ReplaceRecord(const IniRecord& Obj_);
    IniRecord& operator = (const IniRecord& Obj_);

    void SetName(char* Name_);
    const char* GetName() const;

    virtual void AddEntry(IniEntry* Entry_);
    virtual Boolean DeleteEntry(long SlotNum_);
    virtual long FirstEntry(IniEntry*& Entry_);
    virtual Boolean ModifyEntry(long SlotNum_, const IniEntry& Entry_);
    virtual long NextEntry(long CurSlot_, IniEntry*& Entry_);
    virtual const IniEntry* GetEntry(long Num_) const;
    virtual IniEntry* GetEntry(long Num_);
    long NumEntries() const;

    IniRecord& SetDeleted(Boolean Flag_=TRUE);
    Boolean IsDeleted() const
        { return _Deleted; }
    
    IniRecord* SetNext(IniRecord* Next_);
    IniRecord* Append(IniRecord* Next_);

    const IniRecord* Next(long Num_=1) const;
    IniRecord* Next(long Num_=1);

    const IniRecord* Last() const;
    IniRecord* Last();    

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif    
};

/****************************************************************************/
class IniFile : public File
{
  ROOTOBJECTFNCS_DEFN(Object, IniFile)

  protected:
    IniRecord* _ListTail;
    IniRecord* _RecordList;
    long _RecordsAllocated;
    Byte_t _Status;

    enum
    {
      INIFILE_READY		= 0x00,
      INIFILE_NOT_OPENED	= 0x02,
      INIFILE_NOTREADY		= 0x04
    };

    inline void ClearStatus(Byte_t StatBit_)
	{ _Status &= ~StatBit_; }    
  
    // Slotted file check method
    virtual Boolean TestForIniFile();
    Boolean OpenForUpdate(int Modes_);

    IniRecord* ReallyDeleteRecord(long SlotNum_, Boolean& Done_);

  public:
    IniFile();
    IniFile(const char* Fname_, int Mode_);
    IniFile(const IniFile& Finfo_);
    virtual ~IniFile();

    // Virtual constructor and assignment methods
    static IniFile* Make(char* FileName_, int Mode_);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static IniFile& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::INIFILE_TYPE; }
    virtual int ClassID() const;

    // assignment operator
    IniFile& operator = (const IniFile& Sinfo_);

    // file open procedures
    virtual Boolean Open(const char* Path_=NULL, int Modes_=0);

    // file close procedures
    virtual Boolean Close();

    // file update procedures
    virtual Boolean UpdateFile();
    virtual Boolean UpdateList();    
    virtual Boolean ReadIntoMemory();

    // Ini file check method
    Boolean IsIniFile(const char* Path_=NULL) const;

    // Record and slot manipulation methods
    long RecordsAllocated() const;

    virtual long AddRecord(const IniRecord& Record_);
    virtual Boolean DeleteRecord(long SlotNum_);
    virtual long FirstRecord(IniRecord& Record_);
    virtual Boolean ModifyRecord(long SlotNum_, const IniRecord& Record_);
    virtual long NextRecord(long CurSlot_, IniRecord& Record_);
    virtual Boolean RetrieveRecord(long SlotNum_, IniRecord& Record_);

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





