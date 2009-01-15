#ifndef OBJECTENVELOPE_CPP
#define OBJECTENVELOPE_CPP
#ifndef BINDINGSOBJECTENVELOPE_H
  #include "bindobj.h"
#endif

/****************************************************************************/
Object* BindingsObjectEnvelope::Dispatch()
{
  NotImplemented("Object* Dispatch()");
  exit(-1); return this;
}

/****************************************************************************/
const Object* BindingsObjectEnvelope::Dispatch() const
{
  NotImplemented("const Object* DispatchConst()");
  exit(-1); return this;
}

/****************************************************************************/
Boolean BindingsObjectEnvelope::Destroy()
{
  if (Validate())
  {
    Dispatch()->Destroy();
  
    if (IsNewed())
    {
      delete this;
      return TRUE;
    }
  }

  return FALSE;
}

/****************************************************************************/
Object& BindingsObjectEnvelope::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (Validate())
    Dispatch()->Assign(Obj_, ObjComp_);

  return *this;
}

/****************************************************************************/
Object& BindingsObjectEnvelope::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (Validate())
    Dispatch()->ShallowAssign(Obj_, ObjComp_);

  return *this;
}

/****************************************************************************/
istream& BindingsObjectEnvelope::BinaryRead(istream& Is_, Boolean* Ok_)
{
  if (Validate())
    Dispatch()->BinaryRead(Is_, Ok_);

  return Is_;
}

/****************************************************************************/
ByteStream& BindingsObjectEnvelope::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  if (Validate())
    Dispatch()->BinaryRead(Is_, Ok_);

  return Is_;
}

/****************************************************************************/
istream& BindingsObjectEnvelope::TextRead(istream& Is_, Boolean* Ok_)
{
  if (Validate())
    Dispatch()->TextRead(Is_, Ok_);

  return Is_;
}

/****************************************************************************/
ByteStream& BindingsObjectEnvelope::ReadObjectWith(ByteStream& Bstrm_)
{
  if (Validate())
    Dispatch()->ReadObjectWith(Bstrm_);

  return Bstrm_;
}

/****************************************************************************/
#endif





