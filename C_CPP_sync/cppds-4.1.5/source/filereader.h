// File reader ADT class
//
#ifndef FILEREADER_H
#define FILEREADER_H
#ifndef STREAMREADER_H
  #include "streamreader.h"
#endif

#define FILEREADER_DEBUG	0

/****************************************************************************/
class FileReaderAcceptor : public ObjectAcceptor
{
  friend class FileReader;

  protected:
    union
    {
      const FileReader* _ObjPtr;
      FileReader* _VarPtr;
    };

    FileReaderAcceptor(const FileReader* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // FileReader class specific acceptor methods
    virtual Boolean IsFileReader() const;
    virtual Boolean IsStreamReader() const;
    virtual Boolean Isostream() const;
    virtual Boolean Isistream() const;
    virtual Boolean Isifstream() const;
    virtual Boolean Isofstream() const;
    virtual Boolean Isios() const;

    virtual void* CreateFromFileReader(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFileReader(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
// NOTE: Do not try to implement dynamic allocation of stream buffers
// ie. streambuf or filebuf since this has been attempted many times and
// the result is nothing but grief. Deallocation of the respective stream
// buffers will cause the program to behave strangely or crash. Because of
// this problem the stream buffer passed to the reader classes will not be
// allocated, deep copied or deallocated in any manner.
//

class FileReader : public StreamReader
{
  friend class FileReaderAcceptor;

  friend Object& RootObject(FileReader& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const FileReader& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    ifstream* _Fin;
    ofstream* _Fout;

    // file open procedures
    Boolean OpenIn(const char* Fname_, int Mode_);
    Boolean OpenOut(const char* Fname_, int Mode_);

  public:
	#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
      typedef std::_Ios_Openmode IOS_OPENMODE;
	#else
      typedef int IOS_OPENMODE;
	#endif

  public:
    // Constructor / Destructor
    FileReader();
    FileReader(filebuf* Buffer_);
    FileReader(ifstream* Infsp_, ofstream* Outfsp_);
    FileReader(const StreamReader& Strmrd_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      FileReader(const FileReader& Strmrd_);
    #endif
    virtual ~FileReader();

    // Virtual constructor and assignment methods
    static FileReader* Make();
    static FileReader* Make(filebuf* Buffer_);
    static FileReader* Make(ifstream* Infsp_, ofstream* Outfsp_);
    static FileReader* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static FileReader& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::FILEREADER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // virtual assignment operator
    virtual StreamReader& operator = (const StreamReader& Sr_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual FileReader& operator = (const FileReader& Sr_);
    #endif

    // Stream Buffer Accessors
    filebuf* GetInBuffer();
    filebuf* GetOutBuffer();
    filebuf* GetBuffer(int Mode_);

    // Stream member mutators
    virtual istream& operator () (istream* Insp_);
    virtual ostream& operator () (ostream* Outsp_);

    // file open procedure
    virtual Boolean Open(const char* Fname_, int Mode_);

    // file close procedure
    virtual Boolean Close();

    // Method to return the size of the file in bytes
    long FileSize(int Mode_);

    // Method to change the size of the file to the given number of bytes
    Boolean ChangeFileSize(int Mode_, long Size_);

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





