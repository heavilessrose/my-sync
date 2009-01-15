#ifndef FILE_H
#define FILE_H
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef FILEREADER_H
  #include "filereader.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

#define FILE_DEBUG	0

/****************************************************************************/
class FileAcceptor : public ObjectAcceptor
{
  friend class File;

  protected:
    union
    {
      const File* _ObjPtr;
      File* _VarPtr;
    };

    FileAcceptor(const File* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // File class specific acceptor methods
    virtual Boolean IsFile() const;
    virtual void* CreateFromFile(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFile(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class File : public ObjectImp, public ByteStream
{
  friend class FileAcceptor;

  friend Object& RootObject(File& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const File& Obj_)
	{ return *((const Object*)(&Obj_)); }

#if defined(WIN_DOS)
  enum { PATH_SEPARATOR = '\\' };
#else
  enum { PATH_SEPARATOR = '/' };
#endif

  protected:
    filebuf _FileBuffer;
    FileReader* _Reader;

    int _FileDesc;
    ChrString _Fname;

    int _Mode;
    Boolean _Opened;

    // This method is similar to the -> operator for the delegation idiom
    // but it isn't an operator and is private to the class
    // Defining it as the -> operator is worthless since this is called
    // only in the base dispatcher class and would use up the -> operator
    // that could be used for some other purpose.
    virtual const ByteStream* ByteStream_Dispatch() const;
    virtual ByteStream* ByteStream_Dispatch();    

    inline Boolean ValidFileName() const
	{ return (_Fname.c_str() && strlen(_Fname.c_str())); }

  public:
    File(int Mode_=ios::in);
    File(int FileDesc_, int Mode_);
    File(const char* Fname_, int Mode_);
    File(const File& Finfo_);
    virtual ~File();

    // Virtual constructor and assignment methods
    static File* Make(int Mode_=ios::in);
    static File* Make(int FileDesc_, int Mode_);
    static File* Make(const char* Fname_, int Mode_);
    static File* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static File& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::FILE_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // File attribute mutators
    virtual File& SetFileProperties(const char* Fname_, int Mode_);
    virtual File& SetFileDescriptor(int FileDesc_);
    virtual File& SetFileMode(int Mode_);
    #if HAS_NONSTD_FILEBUF
      virtual File& SetFileBuffer(char* Buffer_, int Size_);
    #endif

    // assignment operator
    File& operator = (const File& Finfo_);

    // Smart pointer to File Reader data member
    FileReader* operator -> () const;
    filebuf* GetBuffer() const;

    // file open procedures
    virtual Boolean Open(const char* Path_=NULL, int Modes_=0);

    // file close procedures
    virtual Boolean Close();

    // file truncate procedure
    virtual Boolean Truncate();

    // Returns TRUE if file opened in text mode
    Boolean InTextMode()
	{ return !(_Mode & ios::binary); }

    // Returns TRUE if file opened in binary mode
    Boolean InBinaryMode()
	{ return (_Mode & ios::binary); }

    // Method to return the file size in bytes
    long FileSize();

    // Method to change the size of the file to the given number of bytes
    Boolean ChangeFileSize(long Size_);

    // Data member mutators
    File& SetReader(FileReader* Reader_);

    // Data member accessors
    const char* GetPath(char* Result_=NULL, char PathSep_=PATH_SEPARATOR) const;
    inline FileReader* GetReader() const
	{ return _Reader; }
    inline const char* GetName() const
	{ return _Fname.c_str(); }
    inline int GetMode() const
	{ return _Mode; }
    inline Boolean Opened() const
	{ return _Opened; }

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





