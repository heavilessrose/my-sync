#ifndef OBJECTCOUNTER_H
#define OBJECTCOUNTER_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif

class ObjectCounter
{
  private:
    static size_t _Count;

  public:
    ObjectCounter();
    ObjectCounter(const ObjectCounter& Obj_);
    ~ObjectCounter();

    static size_t GiveCount();
};

#endif




