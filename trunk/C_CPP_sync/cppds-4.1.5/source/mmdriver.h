#ifndef OBJECT_H
  #include "object.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
class FooMem_t
{
  private:
    Ulong _SomeX; // To give it some size

  public:
    // Constructor, destructor, test function for breakpointing
    FooMem_t();
    virtual ~FooMem_t();

    // new & delete operators
    void* operator new(size_t Bytes_);
    void operator delete(void* Space_);

    // Demo method
    Ulong& Getx()
	{ return _SomeX; }
};

void main()
{
  unsigned char array[1000];
  Pool apool_;
  Foo13a* afoop_;
  Foo13a* afoop2_;

  afoop_ = (Foo13a*)Foo13a::operator new(sizeof(Foo13a), &apool_);
  afoop2_ = (Foo13a*)Foo13a::operator new(sizeof(Foo13a), &array[0]);

  afoop_->X() = 13;
  afoop2_->X() = 17;

  cout <<afoop_->X() <<" " <<afoop2_->X() <<endl;
}
