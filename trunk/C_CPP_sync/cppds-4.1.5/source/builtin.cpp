#ifndef BUILTIN_CPP
#define BUILTIN_CPP
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

/****************************************************************************/
unsigned* new_unsigned_int(unsigned Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned));
#else
  void* RawMem_ = new unsigned;
#endif
  *((unsigned*)RawMem_) = Val_;
  return ((unsigned*)RawMem_);
}

/****************************************************************************/
int* new_int(int Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(int));
#else
  void* RawMem_ = new int;
#endif
  *((int*)RawMem_) = Val_;
  return ((int*)RawMem_);
}

/****************************************************************************/
unsigned char* new_unsigned_char(unsigned char Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned char));
#else
  void* RawMem_ = new Uchar;
#endif
  *((unsigned char*)RawMem_) = Val_;
  return ((unsigned char*)RawMem_);
}

/****************************************************************************/
#if HAS_BOOL
bool* new_bool(bool Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(bool));
#else
  void* RawMem_ = new bool;
#endif
  *((bool*)RawMem_) = Val_;
  return ((bool*)RawMem_);
}
#endif
/****************************************************************************/
char* new_char(char Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(char));
#else
  void* RawMem_ = new char;
#endif
  *((char*)RawMem_) = Val_;
  return ((char*)RawMem_);
}

/****************************************************************************/
short* new_short(short Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(short));
#else
  void* RawMem_ = new short;
#endif
  *((short*)RawMem_) = Val_;
  return ((short*)RawMem_);
}

/****************************************************************************/
unsigned short* new_unsigned_short(unsigned short Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned short));
#else
  void* RawMem_ = new Ushort;
#endif
  *((unsigned short*)RawMem_) = Val_;
  return ((unsigned short*)RawMem_);
}

/****************************************************************************/
long* new_long(long Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(long));
#else
  void* RawMem_ = new long;
#endif
  *((long*)RawMem_) = Val_;
  return ((long*)RawMem_);
}

/****************************************************************************/
unsigned long* new_unsigned_long(unsigned long Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned long));
#else
  void* RawMem_ = new Ulong;
#endif
  *((unsigned long*)RawMem_) = Val_;
  return ((unsigned long*)RawMem_);
}

/****************************************************************************/
unsigned char* new_unsigned_chars(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned char) * Max_);
#else
  void* RawMem_ = new Uchar[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
  {
    unsigned Val_ = va_arg(ap, unsigned);
    ((unsigned char*)RawMem_)[Index_] = (unsigned char)Val_;
  }

  va_end(ap);
  return ((unsigned char*)RawMem_);
}

/****************************************************************************/
unsigned* new_unsigned_ints(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned) * Max_);
#else
  void* RawMem_ = new unsigned[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((unsigned*)RawMem_)[Index_] = va_arg(ap, unsigned);

  va_end(ap);
  return ((unsigned*)RawMem_);
}

/****************************************************************************/
unsigned short* new_unsigned_shorts(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned short) * Max_);
#else
  void* RawMem_ = new Ushort[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((unsigned short*)RawMem_)[Index_] = (unsigned short)(va_arg(ap, int));

  va_end(ap);
  return ((unsigned short*)RawMem_);
}

/****************************************************************************/
unsigned long* new_unsigned_longs(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(unsigned long) * Max_);
#else
  void* RawMem_ = new Ulong[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((unsigned long*)RawMem_)[Index_] = va_arg(ap, unsigned long);

  va_end(ap);
  return ((unsigned long*)RawMem_);
}

/****************************************************************************/
#if HAS_BOOL
bool* new_bools(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLAOD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(bool) * Max_);
#else
  void* RawMem_ = new bool[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
  {
    unsigned short Val_ = (unsigned short)(va_arg(ap, int));
    ((bool*)RawMem_)[Index_] = bool(Val_);
  }

  va_end(ap);
  return ((bool*)RawMem_);
}
#endif
/****************************************************************************/
char* new_chars(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLAOD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(char) * Max_);
#else
  void* RawMem_ = new char[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
  {
    int Val_ = va_arg(ap, int);
    ((char*)RawMem_)[Index_] = char(Val_);
  }

  va_end(ap);
  return ((char*)RawMem_);
}

/****************************************************************************/
int* new_ints(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(int) * Max_);
#else
  void* RawMem_ = new int[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((int*)RawMem_)[Index_] = va_arg(ap, int);

  va_end(ap);
  return ((int*)RawMem_);
}

/****************************************************************************/
short* new_shorts(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(short) * Max_);
#else
  void* RawMem_ = new short[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((short*)RawMem_)[Index_] = (short)(va_arg(ap, int));

  va_end(ap);
  return ((short*)RawMem_);
}

/****************************************************************************/
long* new_longs(ptrdiff_t Max_, ...)
{
  va_list ap;
  va_start(ap, Max_);

#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(long) * Max_);
#else
  void* RawMem_ = new long[Max_];
#endif

  for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
    ((long*)RawMem_)[Index_] = va_arg(ap, long);

  va_end(ap);
  return ((long*)RawMem_);
}

/****************************************************************************/
float* new_float(float Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(float));
#else
  void* RawMem_ = new float;
#endif

  *((float*)RawMem_) = Val_;
  return ((float*)RawMem_);
}

/****************************************************************************/
double* new_double(double Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(double));
#else
  void* RawMem_ = new double;
#endif

  *((double*)RawMem_) = Val_;
  return ((double*)RawMem_);
}

/****************************************************************************/
long double* new_long_double(long double Val_)
{
#if OVERLOAD_NEW
  void* RawMem_ = MemMatrix::Matrix().Callocate(sizeof(long double));
#else
  void* RawMem_ = new Ldouble;
#endif

  *((long double*)RawMem_) = Val_;
  return ((long double*)RawMem_);
}

/****************************************************************************/
float* new_floats(ptrdiff_t Max_, float* Args_, MemAllocMethod Method_)
{
#if OVERLOAD_NEW
  void* RawMem_ = RawAllocateWith(Method_, sizeof(float) * Max_);
#else
  void* RawMem_ = RawAllocateWith(STD_NEW, sizeof(float) * Max_);
#endif

  if (Args_)
    for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
      ((float*)RawMem_)[Index_] = Args_[Index_];

  return ((float*)RawMem_);
}

/****************************************************************************/
double* new_doubles(ptrdiff_t Max_, double* Args_, MemAllocMethod Method_)
{
#if OVERLOAD_NEW
  void* RawMem_ = RawAllocateWith(Method_, sizeof(double) * Max_);
#else
  void* RawMem_ = RawAllocateWith(STD_NEW, sizeof(double) * Max_);
#endif

  if (Args_)
    for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
      ((double*)RawMem_)[Index_] = Args_[Index_];

  return ((double*)RawMem_);
}

/****************************************************************************/
long double* new_long_doubles(ptrdiff_t Max_, long double* Args_, MemAllocMethod Method_)
{
#if OVERLOAD_NEW
  void* RawMem_ = RawAllocateWith(Method_, sizeof(long double) * Max_);
#else
  void* RawMem_ = RawAllocateWith(STD_NEW, sizeof(long double) * Max_);
#endif

  if (Args_)
    for (ptrdiff_t Index_ = 0; Index_ < Max_; Index_++)
      ((long double*)RawMem_)[Index_] = Args_[Index_];

  return ((long double*)RawMem_);
}

/****************************************************************************/
unsigned char* new_unsigned_char_string(const unsigned char* Arr_, MemAllocMethod Method_)
{
  char* ns = NULL;
#if OVERLOAD_NEW
  return ((unsigned char*)((Arr_ && (ns = (char*)RawAllocateWith(Method_, sizeof(unsigned char) * (strlen((const char*)Arr_) + 1)))) ? strcpy(ns, (const char*)Arr_):NULL));
#else
  return ((unsigned char*)((Arr_ && (ns = (char*)RawAllocateWith(STD_NEW, sizeof(unsigned char) * (strlen((const char*)Arr_) + 1)))) ? strcpy(ns, (const char*)Arr_):NULL));
#endif
}

/****************************************************************************/
unsigned char* new_unsigned_char_array(ptrdiff_t Max_, const unsigned char* Arr_, MemAllocMethod Method_)
{
  unsigned char* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((unsigned char*)RawAllocateWith(Method_, sizeof(unsigned char) * Max_));
  return ((unsigned char*)((Arr_ && (ns = (unsigned char*)RawAllocateWith(Method_, sizeof(unsigned char) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((unsigned char*)RawAllocateWith(STD_NEW, sizeof(unsigned char) * Max_));
  return ((unsigned char*)((Arr_ && (ns = (unsigned char*)RawAllocateWith(STD_NEW, sizeof(unsigned char) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
unsigned* new_unsigned_int_array(ptrdiff_t Max_, const unsigned* Arr_, MemAllocMethod Method_)
{
  unsigned* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((unsigned*)RawAllocateWith(Method_, sizeof(unsigned) * Max_));
  return ((unsigned*)((Arr_ && (ns = (unsigned*)RawAllocateWith(Method_, sizeof(unsigned) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((unsigned*)RawAllocateWith(STD_NEW, sizeof(unsigned) * Max_));
  return ((unsigned*)((Arr_ && (ns = (unsigned*)RawAllocateWith(STD_NEW, sizeof(unsigned) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
unsigned short* new_unsigned_short_array(ptrdiff_t Max_, const unsigned short* Arr_, MemAllocMethod Method_)
{
  unsigned short* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((unsigned short*)RawAllocateWith(Method_, sizeof(unsigned short) * Max_));
  return ((unsigned short*)((Arr_ && (ns = (unsigned short*)RawAllocateWith(Method_, sizeof(unsigned short) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((unsigned short*)RawAllocateWith(STD_NEW, sizeof(unsigned short) * Max_));
  return ((unsigned short*)((Arr_ && (ns = (unsigned short*)RawAllocateWith(STD_NEW, sizeof(unsigned short) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
unsigned long* new_unsigned_long_array(ptrdiff_t Max_, const unsigned long* Arr_, MemAllocMethod Method_)
{
  unsigned long* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((unsigned long*)RawAllocateWith(Method_, sizeof(unsigned long) * Max_));
  return ((unsigned long*)((Arr_ && (ns = (unsigned long*)RawAllocateWith(Method_, sizeof(unsigned long) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((unsigned long*)RawAllocateWith(STD_NEW, sizeof(unsigned long) * Max_));
  return ((unsigned long*)((Arr_ && (ns = (unsigned long*)RawAllocateWith(STD_NEW, sizeof(unsigned long) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
char* new_char_string(const char* Arr_, MemAllocMethod Method_)
{
  char* ns = NULL;
#if OVERLOAD_NEW
  return ((char*)((Arr_ && (ns = (char*)RawAllocateWith(Method_, sizeof(char) * (strlen(Arr_) + 1)))) ? strcpy(ns, Arr_):NULL));
#else
  return ((char*)((Arr_ && (ns = (char*)RawAllocateWith(STD_NEW, sizeof(char) * (strlen(Arr_) + 1)))) ? strcpy(ns, Arr_):NULL));
#endif
}

/****************************************************************************/
#if HAS_BOOL
bool* new_bool_array(ptrdiff_t Max_, const bool* Arr_, MemAllocMethod Method_)
{
  bool* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((bool*)RawAllocateWith(Method_, sizeof(bool) * Max_));
  return ((bool*)((Arr_ && (ns = (bool*)RawAllocateWith(Method_, sizeof(bool) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((bool*)RawAllocateWith(STD_NEW, sizeof(bool) * Max_));
  return ((bool*)((Arr_ && (ns = (bool*)RawAllocateWith(STD_NEW, sizeof(bool) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}
#endif
/****************************************************************************/
char* new_char_array(ptrdiff_t Max_, const char* Arr_, MemAllocMethod Method_)
{
  char* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((char*)RawAllocateWith(Method_, sizeof(char) * Max_));
  return ((char*)((Arr_ && (ns = (char*)RawAllocateWith(Method_, sizeof(char) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((char*)RawAllocateWith(STD_NEW, sizeof(char) * Max_));
  return ((char*)((Arr_ && (ns = (char*)RawAllocateWith(STD_NEW, sizeof(char) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
int* new_int_array(ptrdiff_t Max_, const int* Arr_, MemAllocMethod Method_)
{
  int* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((int*)RawAllocateWith(Method_, sizeof(int) * Max_));
  return ((int*)((Arr_ && (ns = (int*)RawAllocateWith(Method_, sizeof(int) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((int*)RawAllocateWith(STD_NEW, sizeof(int) * Max_));
  return ((int*)((Arr_ && (ns = (int*)RawAllocateWith(STD_NEW, sizeof(int) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
short* new_short_array(ptrdiff_t Max_, const short* Arr_, MemAllocMethod Method_)
{
  short* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((short*)RawAllocateWith(Method_, sizeof(short) * Max_));
  return ((short*)((Arr_ && (ns = (short*)RawAllocateWith(Method_, sizeof(short) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((short*)RawAllocateWith(STD_NEW, sizeof(short) * Max_));
  return ((short*)((Arr_ && (ns = (short*)RawAllocateWith(STD_NEW, sizeof(short) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
long* new_long_array(ptrdiff_t Max_, const long* Arr_, MemAllocMethod Method_)
{
  long* ns = NULL;
#if OVERLOAD_NEW
  if (!Arr_)
    return ((long*)RawAllocateWith(Method_, sizeof(long) * Max_));
  return ((long*)((Arr_ && (ns = (long*)RawAllocateWith(Method_, sizeof(long) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#else
  if (!Arr_)
    return ((long*)RawAllocateWith(STD_NEW, sizeof(long) * Max_));
  return ((long*)((Arr_ && (ns = (long*)RawAllocateWith(STD_NEW, sizeof(long) * Max_))) ? memmove(ns, Arr_, Max_):NULL));
#endif
}

/****************************************************************************/
void* BuiltIn::MakeObject(int TypeId_, void* Ptr_)
{
  if (Ptr_)
    return
    ((TypeId_ == TypeInfo::BUILTIN_CHAR) 		? (void*)new_char(*((char*)Ptr_)):
#if HAS_BOOL
     (TypeId_ == TypeInfo::BUILTIN_BOOL)           	? (void*)new_bool(*((bool*)Ptr_)):
#endif
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_CHAR)   	? (void*)new_unsigned_char(*((Uchar*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_INT) 		? (void*)new_int(*((int*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_INT) 	? (void*)new_unsigned_int(*((Uint*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_SHORT) 		? (void*)new_short(*((short*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_SHORT)  	? (void*)new_unsigned_short(*((Ushort*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_LONG) 		? (void*)new_long(*((long*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_LONG) 	? (void*)new_unsigned_long(*((Ulong*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_FLOAT) 		? (void*)new_float(*((float*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_DOUBLE) 		? (void*)new_double(*((double*)Ptr_)):
     (TypeId_ == TypeInfo::BUILTIN_LONG_DOUBLE)		? (void*)new_long_double(*((Ldouble*)Ptr_)):
     NULL);
  else
    return
    ((TypeId_ == TypeInfo::BUILTIN_CHAR) 		? (void*)new_char(0):
#if HAS_BOOL
     (TypeId_ == TypeInfo::BUILTIN_BOOL)           	? (void*)new_bool(0):
#endif
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_CHAR)   	? (void*)new_unsigned_char(0):
     (TypeId_ == TypeInfo::BUILTIN_INT) 		? (void*)new_int(0):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_INT) 	? (void*)new_unsigned_int(0):
     (TypeId_ == TypeInfo::BUILTIN_SHORT) 		? (void*)new_short(0):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_SHORT)  	? (void*)new_unsigned_short(0):
     (TypeId_ == TypeInfo::BUILTIN_LONG) 		? (void*)new_long(0):
     (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_LONG) 	? (void*)new_unsigned_long(0):
     (TypeId_ == TypeInfo::BUILTIN_FLOAT) 		? (void*)new_float(0):
     (TypeId_ == TypeInfo::BUILTIN_DOUBLE) 		? (void*)new_double(0):
     (TypeId_ == TypeInfo::BUILTIN_LONG_DOUBLE)		? (void*)new_long_double(0):
     NULL);
}

/****************************************************************************/
void* BuiltIn::MakeArray(int TypeId_, ptrdiff_t Qty_, void* Ptr_)
{
  return
  ((TypeId_ == TypeInfo::BUILTIN_CHAR) 			? (void*)new_char_array(Qty_, (char*)Ptr_):
#if HAS_BOOL
   (TypeId_ == TypeInfo::BUILTIN_BOOL)               	? (void*)new_bool_array(Qty_, (bool*)Ptr_):
#endif
   (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_CHAR)     	? (void*)new_unsigned_char_array(Qty_, (Uchar*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_INT) 			? (void*)new_int_array(Qty_, (int*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_INT) 		? (void*)new_unsigned_int_array(Qty_, (Uint*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_SHORT) 		? (void*)new_short_array(Qty_, (short*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_SHORT) 	? (void*)new_unsigned_short_array(Qty_, (Ushort*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_LONG) 			? (void*)new_long_array(Qty_, (long*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_UNSIGNED_LONG) 	? (void*)new_unsigned_long_array(Qty_, (Ulong*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_FLOAT) 		? (void*)new_floats(Qty_, (float*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_DOUBLE) 		? (void*)new_doubles(Qty_, (double*)Ptr_):
   (TypeId_ == TypeInfo::BUILTIN_LONG_DOUBLE) 		? (void*)new_long_doubles(Qty_, (Ldouble*)Ptr_):
   NULL);
}

/****************************************************************************/
/****************************************************************************/
#if BUILTIN_DEBUG
//#include <conio.h>
#include <assert.h>

int main()
{
//  clrscr();

  int* val = new_int(-5);
  int* valarr = new_ints(5, -1, -2, -3, -4, -5);

  unsigned* uval = new_unsigned_int(5);
  unsigned* uvalarr = new_unsigned_ints(5, 1, 2, 3, 4, 5);

  char* ch = new_char('A');
  char* str = new_char_string("New ChrString");

  double* dval = new_double(3.1415927);

  int index;

  cout <<*val <<endl;
  for (index = 0; index < 5; index++)
    cout <<valarr[index];
  cout <<endl;
  cout <<*uval <<endl;
  for (index = 0; index < 5; index++)
    cout <<uvalarr[index];
  cout <<endl;

  cout <<*ch <<" " <<str <<endl;
  cout <<*dval <<endl;

  index = 1;
  Increment(index);
  cout <<"inc(1) == " <<index <<endl;
  Decrement(index);
  cout <<"dec(2) == " <<index <<endl;

  Delete(val);
  DeleteArray(valarr);
  Delete(uval);
  DeleteArray(uvalarr);
  Delete(ch);
  DeleteArray(str);

  return 0;
}
#endif
/****************************************************************************/
#endif




