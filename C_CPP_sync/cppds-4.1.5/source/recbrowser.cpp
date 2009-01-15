#ifndef RECBROWSER_CPP
#define RECBROWSER_CPP
#ifndef RECBROWSER_H
  #include "recbrowser.h"
#endif

/****************************************************************************/
RecordBrowser::RecordBrowser():
_Mode(0),
_IOdone(FALSE),
_ConstDataObject(NULL)
{}

/****************************************************************************/
RecordBrowser::RecordBrowser(const Object* ConstData_, int Mode_):
_Mode(Mode_),
_IOdone(FALSE),
_ConstDataObject(ConstData_)
{}

/****************************************************************************/
int RecordBrowser::GetMode() const
{
  return _Mode;
}

/****************************************************************************/
const Object* RecordBrowser::GetObject() const
{
  return _ConstDataObject;
}

/****************************************************************************/
void RecordBrowser::SetObject(Object* DataPtr_, int Mode_)
{
  _Mode = Mode_;
  _IOdone = FALSE;
  _DataObject = DataPtr_;
}

/****************************************************************************/
long RecordBrowser::StorableFormWidth() const
{
  return ::FormWidthOfObject(*_ConstDataObject);
}

/****************************************************************************/
ByteStream& RecordBrowser::Ignore(ByteStream& Is_)
{
  if (_Mode & ios::in)
    return ::Ignore(Is_, *_DataObject, &_IOdone);

  return Is_;
}

/****************************************************************************/
ByteStream& RecordBrowser::Read(ByteStream& Is_)
{
  if (_Mode & ios::in)
    return ::Read(Is_, *_DataObject, &_IOdone);

  return Is_;
}

/****************************************************************************/
ByteStream& RecordBrowser::Write(ByteStream& Os_)
{
  return ::Write(Os_, *_ConstDataObject, &_IOdone);
}

/****************************************************************************/
Boolean RecordBrowser::Verified() const
{
  return _IOdone;
}

/****************************************************************************/
#endif





