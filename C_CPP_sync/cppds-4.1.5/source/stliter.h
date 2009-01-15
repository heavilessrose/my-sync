#ifndef STL_ITERATOR_H
#define STL_ITERATOR_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

// The base classes input_iterator, output_iterator, forward_iterator,
// bidirectional_iterator, and random_access_iterator are not part of
// the C++ standard.  (they have been replaced by struct iterator.)
// They are included for backward compatibility with the HP STL.

template <class _Tp, class _Distance> struct Input_Iterator {
  typedef _Tp                value_type;
  typedef _Distance          difference_type;
  typedef _Tp*               pointer;
  typedef _Tp&               reference;
};

struct Output_Iterator {
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;
};

template <class _Tp, class _Distance> struct Forward_Iterator {
  typedef _Tp                  value_type;
  typedef _Distance            difference_type;
  typedef _Tp*                 pointer;           // = value_type* iterator
  typedef _Tp&                 reference;         // = value_type& reference
};


template <class _Tp, class _Distance> struct Bidirectional_Iterator {
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

template <class _Tp, class _Distance> struct Random_Access_Iterator {
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

#endif





