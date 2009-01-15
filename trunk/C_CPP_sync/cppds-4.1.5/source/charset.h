/////////////////////////////////////////////////////////////////////
//                                                                 //
//                        Character Set Library                    //
//                                                                 //
//              Copyright (c) 1992  Namir Clement Shammas          //
//                                                                 //
// Version 1.0.0                                           3/25/91 //
//                                                                 //
// Implements a class for character set that stores each member in //
// a bit.  Each instances uses 32 bytes of storage for the         //
// character set.                                                  //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#ifndef CHARSET_HPP
#define CHARSET_HPP

#ifndef COMNDATA_H
  #include "comndata.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif

#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

const unsigned CHAR_SIZE = 8;
const unsigned CHAR_ARRAY_SIZE = 256 / CHAR_SIZE;

class CharSet : public ObjectSkeleton
{
   private:
     unsigned char bitVal[CHAR_SIZE];
     void initbitVal();
     void setbit(unsigned char bitnum);
     void clearbit(unsigned char bitnum);
     Boolean testbit(unsigned char bitnum);

   protected:
     unsigned char cset[CHAR_ARRAY_SIZE];
     unsigned char setSize;
     unsigned char xmitstr[3];

     char* xlat(unsigned char c);

   public:
     CharSet();
     CharSet(unsigned char first, unsigned char last);
     CharSet(const ChrString& s);
     CharSet(const CharSet& cs);

     // object cloning method
     virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
     virtual CharSet* CloneArrayBinTree() const;

     // Static null object accessor methods
     static CharSet& GiveNullObject();

     // Null object accessors
     virtual const Object& NULLOBJECT() const;
     virtual Object& NULLOBJECT();

     // Class Identification methods
     static int StaticID()
 	 { return TypeInfo::CHARSET_TYPE; }
     virtual int ClassID() const;

     // Text input and output
     virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	 { return Is_; }
     virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

     unsigned char getSetSize() const { return setSize; }
     CharSet& clear();
     Boolean ismember(unsigned char c) { return testbit(c); }
     Boolean ismember(unsigned char first, unsigned char last);
     Boolean ismember(ChrString& s);
     Boolean ismember(CharSet& cs);
     CharSet& add(unsigned char c);
     CharSet& add(unsigned char first, unsigned char last);
     CharSet& add(ChrString& s);
     CharSet& add(CharSet& cs);
     CharSet& remove(unsigned char c);
     CharSet& remove(unsigned char first, unsigned char last);
     CharSet& remove(ChrString& s);
     CharSet& remove(CharSet& cs);
     CharSet& intersect(CharSet& cs);
     CharSet& reverse();

     // special sets
     CharSet& mkupcase();
     CharSet& mklocase();
     CharSet& mkdigit();
     CharSet& mkletter();
     CharSet& mkalphanumeric();

     // operators and friend operators
     CharSet& operator =(CharSet& cs);
     CharSet& operator +=(CharSet& cs);
     CharSet& operator -=(CharSet& cs);
     CharSet& operator /=(CharSet& cs);
     friend CharSet operator +(CharSet& cs1, CharSet& cs2);
     friend CharSet operator -(CharSet& cs1, CharSet& cs2);
     friend CharSet operator /(CharSet& cs1, CharSet& cs2);
     friend int operator ==(CharSet& cs1, CharSet& cs2);
     friend ostream& operator <<(ostream& o, CharSet& cs);
};

#endif




