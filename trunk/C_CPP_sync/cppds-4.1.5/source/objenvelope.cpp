#ifndef OBJECTENVELOPE_CPP
#define OBJECTENVELOPE_CPP
#ifndef OBJECTENVELOPE_H
  #include "objenvelope.h"
#endif

/****************************************************************************/
Object* ObjectEnvelope::Dispatch()
{
  NotImplemented("Object* Dispatch()");
  exit(-1); return this;
}

/****************************************************************************/
const Object* ObjectEnvelope::Dispatch() const
{
  NotImplemented("const Object* DispatchConst()");
  exit(-1); return this;
}

/****************************************************************************/
Boolean ObjectEnvelope::Destroy()
{
  Dispatch()->Destroy();
  
  if (IsNewed())
  {
    delete this;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
#endif





