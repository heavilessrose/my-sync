#ifndef BINDINGS_H
#define BINDINGS_H
#ifndef LANGSUPP_H
  #include "langsupp.h"
#endif

/****************************************************************************/
struct BindingType
{
  enum
  {  
    BINDING0 = 0,       // Method prototype: Letter Client()
    BINDING1,           // Method prototype: Letter Client(const Sponsor&)
    BINDING2,           // Method prototype: Letter Client(const Sponsor&, long)
    BINDING0R,          // Method prototype: Letter& Client()
    BINDING1R,          // Method prototype: Letter& Client(const Sponsor&)
    BINDING2R,          // Method prototype: Letter& Client(const Sponsor&, long)
    BINDING0P,          // Method prototype: Letter* Client()
    BINDING1P,          // Method prototype: Letter* Client(const Sponsor&)
    BINDING2P,          // Method prototype: Letter* Client(const Sponsor&, long)
    
    CONSTBINDING0,      // Same as above, but with const Letter
    CONSTBINDING1,
    CONSTBINDING2,
    CONSTBINDING0R,                             // const Letter&
    CONSTBINDING1R,
    CONSTBINDING2R,
    CONSTBINDING0P,                             // const Letter*
    CONSTBINDING1P,
    CONSTBINDING2P,

    NOBINDINGS
  };
};

/****************************************************************************/
// This particular binding has a signature Method() accepting no arguments
// Whenever the SPONSOR changes the CLIENT object will be notified via this
// method
//
template <class CLIENT, class LETTER>
class ProxyBinding0
{
  public:
    typedef LETTER(CLIENT::*Type)();
    typedef LETTER&(CLIENT::*RefTo_Type)();
    typedef LETTER*(CLIENT::*PtrTo_Type)();
    
    typedef const LETTER(CLIENT::*ConstType)() const;
    typedef const LETTER&(CLIENT::*RefTo_ConstType)() const;
    typedef const LETTER*(CLIENT::*PtrTo_ConstType)() const;

    // union of (pointer to the class methods) of the CLIENT class object
    union
    {
      LETTER(CLIENT::*Ptr)();
      LETTER&(CLIENT::*RefTo_Ptr)();
      LETTER*(CLIENT::*PtrTo_Ptr)();
      
      const LETTER(CLIENT::*ConstPtr)() const;
      const LETTER&(CLIENT::*RefTo_ConstPtr)() const;
      const LETTER*(CLIENT::*PtrTo_ConstPtr)() const;
    };
};

// This particular binding has a signature Method(const SPONSOR&)
// accepting an object of the SPONSOR
// Whenever the SPONSOR changes the CLIENT object will be notified via this
// method. In practice this class will only be instantiated with type long
// as the sponsor meaning the binding will accept a single long parameter.
// This is done since bindings which must accept a SPONSOR object can always
// use the ProxyBinding2 class with a dummy 2nd argument.
//
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyBinding1
{
  public:
    typedef LETTER(CLIENT::*Type)(const SPONSOR&);
    typedef LETTER&(CLIENT::*RefTo_Type)(const SPONSOR&);
    typedef LETTER*(CLIENT::*PtrTo_Type)(const SPONSOR&);
    
    typedef const LETTER(CLIENT::*ConstType)(const SPONSOR&) const;
    typedef const LETTER&(CLIENT::*RefTo_ConstType)(const SPONSOR&) const;
    typedef const LETTER*(CLIENT::*PtrTo_ConstType)(const SPONSOR&) const;
    
    union
    {
      LETTER(CLIENT::*Ptr)(const SPONSOR&);
      LETTER&(CLIENT::*RefTo_Ptr)(const SPONSOR&);
      LETTER*(CLIENT::*PtrTo_Ptr)(const SPONSOR&);
      
      const LETTER(CLIENT::*ConstPtr)(const SPONSOR&) const;
      const LETTER&(CLIENT::*RefTo_ConstPtr)(const SPONSOR&) const;
      const LETTER*(CLIENT::*PtrTo_ConstPtr)(const SPONSOR&) const;
    };
};

// This particular binding has a signature Method(const SPONSOR&, long)
// accepting an object of the SPONSOR and a notification code of type long
// Whenever the SPONSOR changes the CLIENT object will be notified via this
// method
//
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyBinding2
{
  public:
    typedef LETTER(CLIENT::*Type)(const SPONSOR&, long);
    typedef LETTER&(CLIENT::*RefTo_Type)(const SPONSOR&, long);
    typedef LETTER*(CLIENT::*PtrTo_Type)(const SPONSOR&, long);
    
    typedef const LETTER(CLIENT::*ConstType)(const SPONSOR&, long) const;
    typedef const LETTER&(CLIENT::*RefTo_ConstType)(const SPONSOR&, long) const;
    typedef const LETTER*(CLIENT::*PtrTo_ConstType)(const SPONSOR&, long) const;
    
    union
    {
      LETTER(CLIENT::*Ptr)(const SPONSOR&, long);
      LETTER&(CLIENT::*RefTo_Ptr)(const SPONSOR&, long);
      LETTER*(CLIENT::*PtrTo_Ptr)(const SPONSOR&, long);
      
      const LETTER(CLIENT::*ConstPtr)(const SPONSOR&, long) const;
      const LETTER&(CLIENT::*RefTo_ConstPtr)(const SPONSOR&, long) const;
      const LETTER*(CLIENT::*PtrTo_ConstPtr)(const SPONSOR&, long) const;
    };
};

/****************************************************************************/
// Class bindings as inherited from the proxy binding classes. The return
// value of all bindings is hard coded as Boolean.
//
template <class CLIENT>
class ClassBinding0 : public ProxyBinding0<CLIENT, Boolean>
{};

template <class CLIENT, class SPONSOR>
class ClassBinding1 : public ProxyBinding1<CLIENT, SPONSOR, Boolean>
{};

template <class CLIENT, class SPONSOR>
class ClassBinding2 : public ProxyBinding2<CLIENT, SPONSOR, Boolean>
{};

/****************************************************************************/
/****************************************************************************/
// Bindings union class which can accept all different types of bindings
// but store only one instance as a memory efficient representation.
//
template <class CLIENT, class SPONSOR, class LETTER>
union BindingsUnion
{
  BindingsUnion()
      { _Bind2.Ptr = (TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::Type)NULL; }
      
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::Type Ptr_)
      { _Bind0.Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::Type Ptr_)
      { _Bind1.Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::Type Ptr_)
      { _Bind2.Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Ptr_)
      { _Bind0.RefTo_Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Ptr_)
      { _Bind1.RefTo_Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Ptr_)
      { _Bind2.RefTo_Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Ptr_)
      { _Bind0.PtrTo_Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Ptr_)
      { _Bind1.PtrTo_Ptr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Ptr_)
      { _Bind2.PtrTo_Ptr = Ptr_; }
            
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::ConstType Ptr_)
      { _Bind0.ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::ConstType Ptr_)
      { _Bind1.ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::ConstType Ptr_)
      { _Bind2.ConstPtr = Ptr_; }      
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_ConstType Ptr_)
      { _Bind0.RefTo_ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_ConstType Ptr_)
      { _Bind1.RefTo_ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_ConstType Ptr_)
      { _Bind2.RefTo_ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_ConstType Ptr_)
      { _Bind0.PtrTo_ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_ConstType Ptr_)
      { _Bind1.PtrTo_ConstPtr = Ptr_; }
  BindingsUnion(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_ConstType Ptr_)
      { _Bind2.PtrTo_ConstPtr = Ptr_; }
            
  BindingsUnion(const BindingsUnion<CLIENT, SPONSOR, LETTER>& Bindings_, int Type_)
      { if (Type_ != BindingType::NOBINDINGS && (int(Type_) % 3) == BindingType::BINDING0)
          _Bind0 = Bindings_._Bind0;
        else if (Type_ != BindingType::NOBINDINGS && (int(Type_) % 3) == BindingType::BINDING1)
          _Bind1 = Bindings_._Bind1;
        else if (Type_ != BindingType::NOBINDINGS && (int(Type_) % 3) == BindingType::BINDING2)
          _Bind2 = Bindings_._Bind2; }

  BindingsUnion<CLIENT, SPONSOR, LETTER>&
  operator = (const BindingsUnion<CLIENT, SPONSOR, LETTER>& Obj_)
      { if (this != &Obj_)
        {
          _Bind0 = Obj_._Bind0;
          _Bind1 = Obj_._Bind1;
          _Bind2 = Obj_._Bind2;
        }
        return *this; }

  ProxyBinding0<CLIENT, LETTER> _Bind0;          // LETTER Binding()
  ProxyBinding1<CLIENT, long, LETTER> _Bind1;    // LETTER Binding(long)
  ProxyBinding2<CLIENT, SPONSOR, LETTER> _Bind2; // LETTER Binding(const SPONSOR&, long)
};

/****************************************************************************/
#endif




