#ifndef DATASTOREATTRIB_H
#define DATASTOREATTRIB_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

/****************************************************************************/
class DataStorageAttributes
{
  protected:
    Byte_t _Attr;

  public:
    enum
    {
      ACTIVE = 1,
      DEEP_COPY = 2,
      DESTROY = 4
    };

    DataStorageAttributes();
    DataStorageAttributes(int Flags_);
    DataStorageAttributes(const DataStorageAttributes& Obj_);

    DataStorageAttributes& SetStorageAttributes(const DataStorageAttributes& Obj_);
    DataStorageAttributes& SetStorageAttributes(int Flags_);
    DataStorageAttributes& SetDeepCopy(Boolean Flag_=TRUE);
    DataStorageAttributes& SetDestroy(Boolean Flag_=TRUE);
    DataStorageAttributes& SetActive(Boolean Flag_=TRUE);

    Boolean IsActive() const;
    Boolean ShouldDeepCopy() const;
    Boolean ShouldDestroy() const;
    int GetStorageAttributes() const;
};

/****************************************************************************/
#endif




