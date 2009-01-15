#ifndef RECBROWSER_H
#define RECBROWSER_H
#ifndef OBJIMP_H
  #include "objimp.h"
#endif

/****************************************************************************/
class RecordBrowser
{
  protected:
    union
    {
      Object* _DataObject;
      const Object* _ConstDataObject;
    };

    Boolean _IOdone;
    int _Mode;

  public:
    RecordBrowser();
    RecordBrowser(const Object* ConstData_, int Mode_);

    virtual const Object* GetObject() const;
    virtual void SetObject(Object* DataPtr_, int Mode_);
    virtual long StorableFormWidth() const;

    virtual ByteStream& Ignore(ByteStream& Is_);
    virtual ByteStream& Read(ByteStream& Is_);
    virtual ByteStream& Write(ByteStream& Os_);

    virtual int GetMode() const;
    virtual Boolean Verified() const;
};

/****************************************************************************/
#endif





