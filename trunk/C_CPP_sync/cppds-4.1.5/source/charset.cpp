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
#ifndef CHARSET_H
  #include "charset.h"
#endif

///////////////////////////////////////////////////////////////////
//
void CharSet::initbitVal()
//
// Purpose: assigns power of 2 values to the elements of array bitVal:
//
//              bitVal[i] = 2 ^ i, for i = 0, 1, ..., 7
//
{
   bitVal[0] = 1;
   for (unsigned char i = 1; i < CHAR_SIZE; i++)
      bitVal[i] = 2 * bitVal[i-1];
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::clear()
//
// Purpose: clears the CHAR_ARRAY_SIZE bytes of array cset and
// assign zero to the setSize data member.
//
{
   for (unsigned char i = 0; i < CHAR_ARRAY_SIZE; i++)
      cset[i] = 0;
   setSize = 0;
   return *this;
}

void CharSet::setbit(unsigned char bitnum)
//
// Purpose: sets bit number bitnum.
//
// Parameters:
//
//    input: bitnum - the bit to set.
//
{
    cset[bitnum / CHAR_SIZE] |= bitVal[bitnum % CHAR_SIZE];
}

///////////////////////////////////////////////////////////////////
//
void CharSet::clearbit(unsigned char bitnum)
//
// Purpose: clears bit number bitnum.
//
// Parameters:
//
//    input: bitnum - the bit to clear.
//
{
    cset[bitnum / CHAR_SIZE] ^= bitVal[bitnum % CHAR_SIZE];
}

///////////////////////////////////////////////////////////////////
//
Boolean CharSet::testbit(unsigned char bitnum)
//
// Purpose: test if bit number "bitnum" is set or clear.  The
// function returns true if bit is set, or false if bit is clear.
//
// Parameters:
//
//    input: bitnum - the number of the tested bit.
//
{
    return ((cset[bitnum / CHAR_SIZE] &
             bitVal[bitnum % CHAR_SIZE]) > 0) ? true : false;
}

//------------------ class constructors ----------------

///////////////////////////////////////////////////////////////////
//
CharSet::CharSet()
//
//  Purpose: constructs an empty character set.
//
{
    clear();
    initbitVal();
}

///////////////////////////////////////////////////////////////////
//
CharSet::CharSet(const CharSet& cs)
//
//  Purpose: constructs a character set that duplicates the set cs.
//
// Parameters:
//
//    input: cs - the source character set.
//
{
    setSize = cs.setSize;
    memcpy(cset, cs.cset, CHAR_ARRAY_SIZE);
    initbitVal();
}

///////////////////////////////////////////////////////////////////
//
CharSet::CharSet(unsigned char first, unsigned char last)
//
//  Purpose: constructs a character set by using a range of
//  characters first and last.  The first parameter may be
//  equal to, greater than, or less than parameter last.
//  The function makes swaps the values of first and last
// if first is greater than last.
//
// Parameters:
//
//    input: first - the lower limit of the specified character
//             range.
//           last -  the higher limit of the specified character
//             range.
//
{
    unsigned char c;
    initbitVal();
    clear();
    // need to swap parameters first and last?
    if (first > last) {
       c = first;
       first = last;
       last = c;
    }
    setSize = last - first + 1;
    for (c = first; c <= last; c++)
        setbit(c);
}

///////////////////////////////////////////////////////////////////
//
CharSet::CharSet(const ChrString& s)
//
//  Purpose: constructs a character set by using the characters
//  of the parameter s.  The function is able to detect duplicate
//  characters in string s.  For example, if s is "  ", a single
//  character set, containing the space character, is created.
//
// Parameters:
//
//    input: s - the source string.
//
{
   char c;

   clear();
   initbitVal();
   setSize = 0;
   for (unsigned i = 0; i < s.strlen(); i++) {
      // get the next character of string s
 	  #if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
        c = (char&)(s[i]);
	  #else
        c = s[i];
	  #endif

      // is c not already in the character set?
      if (!testbit(c)) {
          // set the bit for character c
          setbit(c);
          // increment the set size
          setSize++;
      }
   }
}

//////////////////////////////////////////////////////////////////////
Object* CharSet::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new CharSet(*this));

  return NULL;
}

//////////////////////////////////////////////////////////////////////
CharSet* CharSet::CloneArrayBinTree() const
{
  return (new CharSet(*this));
}

//////////////////////////////////////////////////////////////////////
CharSet& CharSet::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static CharSet _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//////////////////////////////////////////////////////////////////////
const Object& CharSet::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return CharSet::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
Object& CharSet::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return CharSet::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
int CharSet::ClassID() const
{
  return TypeInfo::CHARSET_TYPE;
}

//////////////////////////////////////////////////////////////////////
ostream& CharSet::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<(*this);
  return Os_;
}

///////////////////////////////////////////////////////////////////
//
Boolean CharSet::ismember(unsigned char first, unsigned char last)
//
//  Purpose: tests whether the character set contains a range of
//  characters defined by first and last.  The function returns
//  true if the character range is in the set, and returns false if
//  otherwise.
//
// Parameters:
//
//    input: first - the lower limit for the tested character range.
//           last  - the upper limit for the tested character range.
//
{
    unsigned c;

    // need to swap parameters first and last?
    if (first > last) {
       c = first;
       first = last;
       last = c;
    }

    c = first;
    // loop to test more characters
    while (c <= last)
       // is c not a member of this character set?
       if (!testbit(c++))
          return false; // exit
    // all tested characters are in the set
    return true;
}

///////////////////////////////////////////////////////////////////
//
Boolean CharSet::ismember(ChrString& s)
//
//  Purpose: tests if all of the characters in string s are members
//  in this character set.  The function returns true if all of the
//  characters of s are members, and returns false otherwise.
//
// Parameters:
//
//    input: s - the string that contains the set of characters to
//             be tested for their membership in this character set.
//

{
   unsigned len = s.strlen();
   unsigned i = 0;

   // return 0 if the string s is empty
   if (len == 0)
      return false;

   // loop to test the characters of string s
   while (i < len)
      // is the next character not a set member?
      if(!testbit(s[i++]))
         // exit: at least one character in string s is not a member
         return false;
   // all characters of s are members in this set
   return true;
}

///////////////////////////////////////////////////////////////////
//
Boolean CharSet::ismember(CharSet& cs)
//
//  Purpose: tests if set cs is equal to or is a subset of the
//  current set.  Function returns true if the above condition is
//  true, and returns false when otherwise.
//
// Parameters:
//
//    input: cs - the tested character set.
//
{
   int i;
   unsigned bit = 0;
   // loop to test the members of cs
   do {
      // is character ASCII "bit" a member of cs
      i = cs.testbit((unsigned char)bit);
      if (i) {
         // is member of cs not a member of this set?
         if (!testbit((unsigned char)bit))
            // cs is not equivalent or a subset of this set
            return false;
      }
      bit++;
   } while (bit < CHAR_ARRAY_SIZE);
   // Every member of cs is also a member of this set
   return true;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::add(unsigned char c)
//
//  Purpose: adds the character c to this set.  The function tests
//  if the argument is already a member.
//
// Parameters:
//
//    input: c - the character added to this set.
//
{
   // is c not a member of this set?
   if (!testbit(c)) {
      setbit(c); // add to set
      setSize++; // increment set size
   }
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::add(unsigned char first, unsigned char last)
//
// Purpose: adds a range of characters defined by the first and last
// parameters.  The functions swaps the arguments of these parameters
// if first is greater than last.
//
// Parameters:
//
//    input: first - the lower limit of the added character set.
//           last - the upper limit of the added character set.
//
{
    unsigned char c;

    // need to spwa first and last?
    if (first > last) {
       c = first;
       first = last;
       last = c;
    }

    // loop to process the range of characters defined
    // by the parameters first and last
    for (c = first; c <= last; c++)
       // is c not a member of this set?
       if (!testbit(c)) {
          setbit(c); // add c to this set
          setSize++;  // increment the set size
       }
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::add(ChrString& s)
//
// Purpose: adds the characters of string s to this set.
//
// Parameters:
//
//    input: s - the string that contains the added characters.
//
{
    unsigned len = s.strlen();

    // exit if string s is empty
    if (len == 0)
        return *this;
    // loop for each string character
    for (unsigned i = 0; i < len; i++)
       // is the current character not a set member?
       if (!testbit(s[i])) {
          setbit(s[i]); // add character to this set
          setSize++;    // incrememnt the set size
       }
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::add(CharSet& cs)
//
// Purpose: add the characters of set cs to this set.
//
// Parameters:
//
//    input: cs - the set whose characters are added to this set.
//
{
    unsigned i;
    
    // exit if set cs is empty
    if (cs.setSize == 0)
        return *this;
    // bitwize OR the cset members of this set and set cs
    for (i = 0; i < CHAR_ARRAY_SIZE; i++)
       cset[i] |= cs.cset[i];
    // reset the set size
    setSize = 0;
    // scan the members of this set to recalculate setSize
    for (i = 0; i <= CHAR_ARRAY_SIZE; i++)
       // is ASCII i a member of this set?
       if (testbit((unsigned char)i))
         setSize++; // increment the set size
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::remove(unsigned char c)
//
// Purpose: deletes the character c from this set.
//
// Parameters:
//
//    input: c - the deleted character.
//
{
   // is character c a member of this set?
   if (testbit(c)) {
      // delete the character from this set
      clearbit(c);
      // decrement the set size
      setSize--;
   }
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::remove(unsigned char first, unsigned char last)
//
// Purpose: deletes the characters, defined by the range first to
// last, from this set.  The function swaps the arguments of first
// and last if first is greater than last.
//
// Parameters:
//
//    input: first - the lower limited of the deleted character
//             range.
//           last - the lower limited of the deleted character
//             range.
//
{
    unsigned char c;

    // need to swap first and last?
    if (first > last) {
       c = first;
       first = last;
       last = c;
    }

    // loop for the range defined by first and last
    for (c = first; c <= last; c++)
       // is character c a member of this set?
       if (testbit(c)) {
          // delete the character from this set
          clearbit(c);
          // decrement the set size
          setSize--;
       }
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::remove(ChrString& s)
//
// Purpose: deletes the characters of string s that are members
// of this set.
//
// Parameters:
//
//    input: s - the string containing the characters to delete
//             from this set.
//
{
    unsigned len = s.strlen();

    // exit if string s is empty
    if (len==0)
        return *this;

    // examine the characters of string s
    for (unsigned i = 0; i < len; i++)
       // is character number i a member of this set?
       if (testbit(s[i])) {
          // delete the character from this set
          clearbit(s[i]);
          // decrement the set size
          setSize--;
       }
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::remove(CharSet& cs)
//
// Purpose: deletes the characters in set cs that are members of
// this set.
//
// Parameters:
//
//    input: cs - the set of characters to delete from this set.
//
{
    unsigned i;
    
    // exit if the set cs is empty
    if (cs.setSize == 0)
        return *this;

    // bitwize XOR to clear members of this set
    // that are also in set cs
    for (i = 0; i < CHAR_ARRAY_SIZE; i++)
       cset[i] ^= cs.cset[i];
    // reset the set size
    setSize = 0;
    // scan the bits of this set to recalculate the new set size
    for (i = 0; i <= CHAR_ARRAY_SIZE; i++)
       if (testbit((unsigned char)i))
         setSize++;
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::intersect(CharSet& cs)
//
// Purpose: intersect the set cs with this set.  This function adds
// the members of set cs that are already in this set.
//
// Parameters:
//
//    input: cs - the intersected set.
//
{
    unsigned i;
    
    // exit if set cs is empty
    if (cs.setSize == 0)
        return *this;

    // bitwize AND the members of set cs with this set
    for (i = 0; i < CHAR_ARRAY_SIZE; i++)
       cset[i] &= cs.cset[i];
    // reset the set size
    setSize = 0;
    // scan the bits of this set to recalculate the new set size
    for (i = 0; i <= CHAR_ARRAY_SIZE; i++)
       if (testbit((unsigned char)i))
         setSize++;
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::reverse()
//
// Purpose: toggles the membership of this set.  The characters that
// were not members of this set become members, and vice versa.
//
{
    // bitwize NOT the set members
    for (unsigned char i = 0; i < CHAR_ARRAY_SIZE; i++)
       cset[i] = ~cset[i];
    // update the set size
    setSize = ~setSize;
    return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::mkupcase()
//
// Purpose: make this set contain members that are uppercase
// characters.  The previous set members are cleared.
//
{
   clear();
   add('A', 'Z');
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::mklocase()
//
// Purpose: make this set contain members that are lowercase
// characters.  The previous set members are cleared.
//
{
   clear();
   add('a', 'z');
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::mkdigit()
//
// Purpose: make this set contain members that are digits. The
// previous set members are cleared.
//
{
   clear();
   add('0', '9');
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::mkletter()
//
// Purpose: make this set contain members that are letters.
// The previous set members are cleared.
//
{
   clear();
   add('A', 'Z');
   add('a', 'z');
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::mkalphanumeric()
//
// Purpose: make this set contain members that are letters and
// digits.  The previous set members are cleared.
//
{
   clear();
   add('0', '9');
   add('A', 'Z');
   add('a', 'z');
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::operator =(CharSet& cs)
//
// Purpose: assigns the character set cs to this set.
//
// Parameters:
//
//    input: cs - the source character set.
//
{
   setSize = cs.setSize;
   memcpy(cset, cs.cset, CHAR_ARRAY_SIZE);
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::operator +=(CharSet& cs)
//
// Purpose: adds the members of character set cs to this set.
//
// Parameters:
//
//    input: cs - the added character set.
//
{
   add(cs);
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::operator -=(CharSet& cs)
//
// Purpose: delete the members of character set cs from this set.
//
// Parameters:
//
//    input: cs - the set of characters to delete.
//
{
   remove(cs);
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet& CharSet::operator /=(CharSet& cs)
//
// Purpose: intersect the members of set cs with this set.
//
// Parameters:
//
//    input: cs - the intersected character set.
//
{
   intersect(cs);
   return *this;
}

///////////////////////////////////////////////////////////////////
//
CharSet operator +(CharSet& cs1, CharSet& cs2)
//
// Purpose: add character sets cs1 and cs2.
//
// Parameters:
//
//    input: cs1 - the first operand.
//           cs2 - the second operand.
//
{
   CharSet result(cs1);
   result.add(cs2);
   return result;
}

///////////////////////////////////////////////////////////////////
//
CharSet operator -(CharSet& cs1, CharSet& cs2)
//
// Purpose: deletes the set cs2 from set cs1.
//
// Parameters:
//
//    input: cs1 - the first operand.
//           cs2 - the second operand.
//
{
   CharSet result(cs1);
   result.remove(cs2);
   return result;
}

///////////////////////////////////////////////////////////////////
//
CharSet operator /(CharSet& cs1, CharSet& cs2)
//
// Purpose:  intersect sets cs1 and cs2.
//
// Parameters:
//
//    input: cs1 - the first operand.
//           cs2 - the second operand.
//
{
   CharSet result(cs1);
   result.intersect(cs2);
   return result;
}

///////////////////////////////////////////////////////////////////
//
int operator ==(CharSet& cs1, CharSet& cs2)
//
// Purpose: compares sets cs1 and cs2.  The function returns 1 if
// the two set match.  Otherwise, teh function yields 0.
//
// Parameters:
//
//    input: cs1 - the first operand.
//           cs2 - the second operand.
//
{
   for (unsigned i = 0; i < CHAR_ARRAY_SIZE; i++)
      // compare byte number i
      if (cs1.cset[i] != cs2.cset[i])
         return 0; // sets do not match
   // sets match
   return 1;
}

///////////////////////////////////////////////////////////////////
//
char* CharSet::xlat(unsigned char c)
//
// Purpose: translates the character c to a formatted form.
// Control characters are translated to ^char form.
//
// Parameters:
//
//    input: c - the input character.
//
{
   if (c < 26 || c == '[' || c == ']' || c == '^' || c == '-') {
     xmitstr[0] = '^';
     xmitstr[1] = (c < 26) ? ('A' + (int)c) : c;
     xmitstr[2] = '\0';
   }
   else {
     xmitstr[0] = c;
     xmitstr[1] = '\0';
   }
   return ((char*)xmitstr);
}

///////////////////////////////////////////////////////////////////
//
ostream& operator <<(ostream& os, CharSet& cs)
//
// Purpose: writes a formatted output of set cs to the stream os.
//
// Parameters:
//
//    input: cs - the output set.
//
//    in/out: os - the output stream.
//
{
   unsigned char qsize = 0;
   unsigned char c, lastc;

   os << '[';
   for (unsigned i = 0, count = 0;
        i < 256 && count <= cs.setSize;
        i++) {
     c = (unsigned char)i;
     if (cs.ismember(c)) {
       count++;
       if (qsize == 0) {
         lastc =c;
         qsize = 1;
         os << cs.xlat(c);
       }
       else {
         if ((c - lastc) == 1) {
           lastc = c;
           qsize++;
         }
         else {
           if (qsize == 1) {
             lastc = c;
             qsize = 1;
             os << cs.xlat(c);
           }
           else if (qsize == 2) {
             os << cs.xlat(lastc);
             lastc = c;
             qsize = 1;
             os << cs.xlat(c);
           }
           else {
             os << '-' << cs.xlat(lastc);
             lastc = c;
             qsize = 1;
             os << cs.xlat(c);
           }
         }
       }
     }
   }
   if (qsize > 1) {
     if (qsize > 2)
        os << '-';
     os << cs.xlat(lastc);
   }
   os << ']';
   return os;
}




