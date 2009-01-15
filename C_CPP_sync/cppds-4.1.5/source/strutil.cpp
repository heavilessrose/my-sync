/*****************************************************************************
* FILE		: STRUTIL.H
* PURPOSE	: String utilities class source file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: March 1 1997
*****************************************************************************/
#ifndef STRUTIL_CPP
#define STRUTIL_CPP
#ifndef STRUTIL_H
  #include "strutil.h"
#endif

/************************ Argument Checking Procedures **********************/
/****************************************************************************/
// PURPOSE:
//	   ณ The first few characters of s
//	   ณ determine the base of the value
//	   ณ being converted:
//	   ณ   1st  ณ 2nd  ณ String s
//	   ณ   char ณ char ณ interpreted as
//	   ณ  ออออออุออออออุอออออออออออออออออออ
//	   ณ    0   ณ 1-7  ณ Octal
//	   ณ    0   ณx or Xณ Hexadecimal
//	   ณ   1-9  ณ      ณ Decimal
//
//	The characters in s must match this generic format:
//	  þ strtol:   [ws] [sn] [0] [x] [ddd]
//	  þ strtoul:  [ws] [sn] [0] [x] [ddd]
//
//	where
//
//	  þ [ws]   =  optional whitespace
//	  þ [sn]   =  optional sign (+ or -)
//	  þ [ddd]  =  optional digits
//	  þ [fmt]  =  optional e or E
//	  þ [.]    =  optional decimal point
//	  þ [0]    =  optional zero (0)
//	  þ [x]    =  optional x or X
//
// PRE:
//   const char* Number_	: The string containing the number to be
//				  converted.
//   int Radix_			: The radix argument to be used in conversion
//			          If the radix argument is equal to 0 then
//				  the procedure is executed to find the
//				  appropriate radix for the given string,
//				  otherwise the original radix is returned.
//
// POST:
//   Returns the appropriate radix for the numeric string given a radix of
//   0, if the radix is non-zero then the radix is return unchanged.
//
int FindRadix(const char* Number_, int Radix_)
{
  if (Radix_ != 0)
    return Radix_;

  int x;
  for (x = 0; Number_[x] && isspace(Number_[x]); x++);

  if (Number_[x] == '+' || Number_[x] == '-')
    ++x;

  Number_ += x;

  return
  (
    IsOctPrefix(Number_) && IsOctc(Number_[1]) ? 8:
    IsHexPrefix(Number_) ? 16:
    IsDecc(*Number_) ? 10:
    (*Number_ == '.' && IsDecc(Number_[1])) ? 10:0
  );
}

/****************************************************************************/
int FindRadix(const char* Number_)
{
  return ::FindRadix(Number_, 0);
}

/****************************************************************************/
// PURPOSE:
//   Procedure to test if a string is a valid number
//
// PRE:
//   char* number : the number string to be tested
//   int signedval : signed value flag, indicates signed values are valid
//   int floatval : the float value flag, indicates float values are valid
//   int scinote : scientific notation for floating point values is valid
//
// POST:
//   Returns 1 if the string represents a valid number otherwise
//   returns 0.
//
int IsNumber(const char* number, int signedval, int floatval, int scinote, int radix)
{
  int valid = 1,
      numsign = 0,
      numpoint = 0,
      numfmt = 0,
      digitdone = 0,
      pointdone = 0,
      basedone = 0,
      allzeros = 0,
      scinotedone = 0,
      nonzerodone = radix != 10 || floatval || scinote,
      octsign, hexsign, fmt, digit, point, sign, incr;
  const char* numberp;
  const char* np;

  if (radix < 1 || radix > 16 || !number)
    return 0;

  if (scinote)
    floatval = 1;    

  for (numberp = number; *numberp && isspace(*numberp); numberp++);
  number = numberp;
  if (!strlen(numberp))
    return 0;

  hexsign = (radix == 16 && !floatval) ?
        (IsHexPrefix(number) || (IsHexPrefix(&number[1]) &&
                                    (number[0] == '+' || number[0] == '-'))):0;

  octsign = (radix == 8 && !floatval) ?
        (IsOctPrefix(number) || (IsOctPrefix(&number[1]) &&
                                    (number[0] == '+' || number[0] == '-'))):0;

  if (!floatval)
  {
    valid = (radix == 16) ? hexsign:
        (radix == 8) ? octsign:
            valid;

    if (valid && number[0] == '0')
    {
      if (hexsign)
        number += 2;
      else if (octsign)
        number++;
      basedone = 1;
    }
  }

  for (incr = 1; *number && valid; number += incr)
  {
    incr = 1;

    if (sign = ((*number == '-' && signedval || *number == '+') &&
                (numsign < 2)))
    {
      sign = ++numsign < 2;
      if (sign && (hexsign || octsign))
        if (basedone)
          valid = 0;
        else
        {
          incr = hexsign ? 3:2;
          basedone = 1;
        }
    }

    if (point = ((*number == '.' && floatval) &&
                 (numpoint < 2)))
      point = ++numpoint < 2;

    if (fmt = ((toupper(*number) == 'E' && floatval && scinote) &&
               (numfmt < 2)))
    {
      fmt = ++numfmt < 2;
      if (IsSign(number[1]))
        incr = 2;
    }

    digit = octsign ? IsOctc(*number):
        hexsign ? IsHexc(*number):
        IsBasec(*number, radix);

    if (digit && !nonzerodone)
    {
      np = number;
      for (allzeros = 0; *np && (allzeros = (*np == '0')); np++);
      nonzerodone = *number != '0' || allzeros;
    }

    if (floatval && scinote && digit && numfmt == 1 && !scinotedone)
      scinotedone = 1;
    
    valid =
      valid &&
      ((sign && !digitdone && !pointdone) ||
       (fmt && digitdone) ||
       (pointdone = point && !numfmt) ||
       (digitdone = (digit && nonzerodone)));
  }

  fmt = (floatval && scinote) ? ((numfmt == 1) ? scinotedone:(!numfmt)):1;
  valid = valid && digitdone && fmt && (strlen(numberp) != 0);
  return valid;
}

/****************************************************************************/
char* ExtractAnyNumber(const char* number, int signedval, int floatval, int scinote, int anynum, int& ptrdiff_, int alloc_, int radix)
{
  static char* nulstr = NULL;
  int valid = 1,
      numsign = 0,
      numpoint = 0,
      numfmt = 0,
      digitdone = 0,
      pointdone = 0,
      basedone = 0,
      allzeros = 0,
      scinotedone = 0,      
      negsignfound = 0,
      pointfound = 0,
      fmtfound = 0,
      nonzerodone = radix != 10 || floatval || scinote,
      octsign, hexsign, fmt, digit, point, sign, incr;      
  const char* startp;
  const char* numberp;
  const char* np;
  char* ns = NULL;
  char* rs;
  nulstr = NULL;

  if (radix < 1 || radix > 16 || !number)
    return nulstr;

  if (scinote)
    floatval = 1;    

  for (startp = numberp = number; *numberp && isspace(*numberp); numberp++);
  number = numberp;
  if (!strlen(numberp))
    return nulstr;

  hexsign = (radix == 16 && !floatval) ?
        (IsHexPrefix(number) || (IsHexPrefix(&number[1]) &&
                                    (number[0] == '+' || number[0] == '-'))):0;

  if (!hexsign && anynum)
  {
    octsign = ((radix == 8 || radix == 16) && !floatval) ?
          (IsOctPrefix(number) || (IsOctPrefix(&number[1]) &&
                                      (number[0] == '+' || number[0] == '-'))):0;

    if (octsign)
    {
      radix = 8;
      hexsign = 0;
    }
    else if (radix == 16 && !floatval)
      return ExtractAnyNumber(startp,1,1,1,anynum,ptrdiff_,alloc_,10);
  }
  else
    octsign = 0;

  if (!anynum)
    octsign = (radix == 8 && !floatval) ?
          (IsOctPrefix(number) || (IsOctPrefix(&number[1]) &&
                                      (number[0] == '+' || number[0] == '-'))):0;

  if (!floatval)
  {
    valid = (radix == 16) ? hexsign:
        (radix == 8) ? octsign:
            valid;

    if (valid && number[0] == '0')
    {
      if (hexsign)
        number += 2;
      else if (octsign)
        number++;
      basedone = 1;
    }
  }

  if (valid)
  for (incr = 1; *number; number += incr)
  {
    incr = 1;

    negsignfound = *number == '-';
    if (sign = ((*number == '-' && signedval || *number == '+') &&
                (numsign < 2)))
    {
      sign = ++numsign < 2;
      if (sign && (hexsign || octsign))
        if (basedone)
          valid = 0;
        else
        {
          incr = hexsign ? 3:2;
          basedone = 1;
        }
    }

    pointfound = *number == '.';
    if (point = ((pointfound && floatval) &&
                 (numpoint < 2)))
      point = ++numpoint < 2;

    fmtfound = toupper(*number) == 'E';
    if (fmt = ((fmtfound && floatval && scinote) &&
               (numfmt < 2)))
    {
      fmt = ++numfmt < 2;
      if (IsSign(number[1]))
        incr = 2;
    }

    digit = octsign ? IsOctc(*number):
        hexsign ? IsHexc(*number):
        IsBasec(*number, radix);

    if (digit && !nonzerodone)
    {
      np = number;
      for (allzeros = 0; (*np && IsBasec(*np, radix)) && (allzeros = (*np == '0')); np++);
      nonzerodone = *number != '0' || allzeros;
    }

    if (floatval && scinote && digit && numfmt == 1 && !scinotedone)
      scinotedone = 1;

    valid =
      valid &&
      ((sign && !digitdone && !pointdone) ||
       (fmt && digitdone) ||
       (pointdone = point && !numfmt) ||
       (digitdone = (digit && nonzerodone)));

    if (!valid)
      break;
  }

  negsignfound = negsignfound && !digitdone && !pointdone;
  fmtfound = fmtfound && radix != 16;
  fmt = (floatval && scinote) ? ((numfmt == 1) ? scinotedone:(!numfmt)):1;
  
  int wrong = (!signedval && negsignfound) ||
              (!floatval && pointfound) ||
              (!scinote && fmtfound) || !fmt ||
              (!digit && octsign && IsBasec(*number, 10));
              
  if (wrong && anynum && ((octsign && radix == 8 && !floatval) || (hexsign && radix == 16 && !floatval)))
    return ExtractAnyNumber(numberp,1,1,1,anynum,ptrdiff_,alloc_,10);

  if (!fmt && ((numfmt == 1 && !scinotedone) || numfmt > 0) && (anynum || radix == 10))
  {
    if (!(*number) && number > numberp)
      number--;

    for (;;)
    {
      if (*number == '.' && number > numberp)
        number--;
    
      if ((*number == '-' || *number == '+') && number > numberp)
        number--;

      if (toupper(*number) == 'E' && number > numberp)
        number--;

      digit = octsign ? IsOctc(*number):
        hexsign ? IsHexc(*number):
        IsBasec(*number, radix);        

      if (number == numberp || digit)
      {
        number++;
        break;
      }
    }
  }
    
  ptrdiff_ = number - startp;
  if (ptrdiff_ && nonzerodone && alloc_)
  {
  #if OVERLOAD_NEW
    rs = (ns = (char*)RawAllocateWith(MEMMATRIX, sizeof(char) * (ptrdiff_ + 1))) ? strncpy(ns, numberp, ptrdiff_):NULL;
    if (rs) rs[ptrdiff_] = 0;
    return rs;
  #else
    rs = (ns = (char*)RawAllocateWith(STD_NEW, sizeof(char) * (ptrdiff_ + 1))) ? strncpy(ns, numberp, ptrdiff_):NULL;
    if (rs) rs[ptrdiff_] = 0;
    return rs;
  #endif
  }
  
  return nulstr;
}

/****************************************************************************/
char* ExtractNumber(const char* number, int signedval, int floatval, int scinote, int& ptrdiff_, int alloc_, int radix)
{
  return ExtractAnyNumber(number, signedval, floatval, scinote, 0, ptrdiff_, alloc_, radix);
}

/****************************************************************************/
char* ExtractAnyNum(const char* value, int signedval, int& ptrdiff_, int alloc_, int radix)
{
  radix = FindRadix(value, radix);
  return ExtractAnyNumber(value, signedval, radix == 10, radix == 10, 1, ptrdiff_, alloc_, radix);
}

/****************************************************************************/
char* ExtractSignedNum(const char* value, int& ptrdiff_, int alloc_, int radix)
{
  return ExtractAnyNumber(value, 1, 0, 0, 0, ptrdiff_, alloc_, FindRadix(value, radix));
}

/****************************************************************************/
char* ExtractUnsignedNum(const char* value, int& ptrdiff_, int alloc_, int radix)
{
  return ExtractAnyNumber(value, 0, 0, 0, 0, ptrdiff_, alloc_, FindRadix(value, radix));
}

/****************************************************************************/
char* ExtractFloat(const char* value, int& ptrdiff_, int alloc_, int radix)
{
  return ExtractAnyNumber(value, 1, 1, 1, 0, ptrdiff_, alloc_, FindRadix(value, radix));
}

/****************************************************************************/
int IsNumType(const char* value, const char* minstr, const char* maxstr, int radix)
{
  int index, len;
  const char* numptr;

  for (; *value && isspace(*value); value++);
  if (minstr ? !IsSignedNum(value, radix):!IsUnsignedNum(value, radix))
    return 0;

  if (*value == '-')
    numptr = minstr;
  else
  {
    numptr = maxstr;
    if (*value == '+')
      value++;
  }

  if (strlen(value) > strlen(numptr))
    return 0;
  else if (strlen(value) == strlen(numptr))
    for (index = 0, len = strlen(numptr); index < len; index++)
      if (toupper(value[index]) > toupper(numptr[index]))
        return 0;
      else if (toupper(value[index]) < toupper(numptr[index]))
        break;

  return 1;
}

/****************************************************************************/
int IsSignedNum(const char* value, int radix)
{
  return IsNumber(value, 1, 0, 0, FindRadix(value, radix));
}

/****************************************************************************/
int IsUnsignedNum(const char* value, int radix)
{
  return IsNumber(value, 0, 0, 0, FindRadix(value, radix));
}

/****************************************************************************/
int IsFloat(const char* value, int radix)
{
  return IsNumber(value, 1, 1, 1, FindRadix(value, radix));
}

/****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid integer value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid integer value
  , otherwise returns false
*/
int IsInt(const char* value, int radix)
{
  char maxint[50];
  char minint[50];

  radix = FindRadix(value, radix);
  if (radix < 1 || radix > 16)
    return 0;

  IntToStr(INT_MAX, maxint, radix);
  IntToStr(INT_MIN, minint, radix);
  return IsNumType(value, minint, maxint, radix);
}

/*****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid unsigned value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid unsigned value
  , otherwise returns false
*/
int IsUint(const char* value, int radix)
{
  char maxuint[50];

  radix = FindRadix(value, radix);
  if (radix < 1 || radix > 16)
    return 0;

  UintToStr(UINT_MAX, maxuint, radix);
  return IsNumType(value, NULL, maxuint, radix);
}

/*****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid short integer value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid integer value
  , otherwise returns false
*/
int IsShort(const char* value, int radix)
{
  char maxshort[50];
  char minshort[50];

  radix = FindRadix(value, radix);
  if (radix < 1 || radix > 16)
    return 0;

  ShortToStr(SHRT_MAX, maxshort, radix);
  ShortToStr(SHRT_MIN, minshort, radix);
  return IsNumType(value, minshort, maxshort, radix);
}

/*****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid unsigned long value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid unsigned value
  , otherwise returns false
*/
int IsUshort(const char* value, int radix)
{
  char maxushort[50];

  radix = FindRadix(value, radix);
  if (radix < 1 || radix > 16)
    return 0;

  UshortToStr(USHRT_MAX, maxushort, radix);
  return IsNumType(value, NULL, maxushort, radix);
}

/*****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid long integer value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid integer value
  , otherwise returns false
*/
int IsLong(const char* value, int radix)
{
  char maxlong[50];
  char minlong[50];

  radix = FindRadix(value, radix);
  if (radix < 1 || radix > 16)
    return 0;

  LongToStr(LONG_MAX, maxlong, radix);
  LongToStr(LONG_MIN, minlong, radix);
  return IsNumType(value, minlong, maxlong, radix);
}

/*****************************************************************************
PURPOSE:
  Procedure to test if string value passed is a valid unsigned long value

PRE:
  char* value : the string value passed

POST:
  returns true if the string represents a valid unsigned value
  , otherwise returns false
*/
int IsUlong(const char* value, int radix)
{
  char maxulong[50];

  radix = FindRadix(value, radix);  
  if (radix < 1 || radix > 16)
    return 0;

  UlongToStr(ULONG_MAX, maxulong, radix);
  return IsNumType(value, NULL, maxulong, radix);
}

/****************************************************************************/
/****************************************************************************/
char* AddPosSign(char* result)
{
  if (!IsSign(result[0]))
  {
    memmove(&result[1], result, strlen(result)+1);
    result[0] = '+';
  }

  return result;
}

/****************************************************************************/
char* AddRadixPrefix(char* result, int radix)
{
  int incr, done;
  char* original;

  original = result;
  for (; *result && isspace(*result); result++);
  if (IsSign(*result))
    result++;

  if (radix == 16 || radix == 8)
  {
    if (radix == 16)
    {
      incr = 2;
      done = !strcmpi(result, "0x");
    }
    else
    {
      incr = 1;
      done = result[0] == '0';
    }

    if (!done)
    {
      memmove(&result[incr], result, strlen(result)+1);
      memmove(result, "0x", incr);
    }
  }

  return original;
}

/****************************************************************************/
char* LowIntToStrHelper(int value, char* result, int radix)
{
  int digit;

  if (value < 0)
  {
    result = LowIntToStrHelper(value / radix, result, radix);
    digit = (value % radix) * -1;
    *result++ = ObjRelation<int>::InRange(digit, 0xA, 0xF, SortUsingBuiltInOperators<int>::Trait()) ?
			(digit - 0xA + 'A'):
			(digit + '0');
  }

  return result;
}

/****************************************************************************/
char* LowIntToStr(int value, char* result, int radix)
{
  char* ptr;
  Assert(1 <= radix && radix <= 16, ERRMSG_INVALIDRADIX);

  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(32);  

  *result = '-';
  if (radix == 1)
  {
    if (value < 0)
      for (ptr = result; value; value++)
	*result++ = '1';

    *result = '\0';
    return ptr;
  }

  *LowIntToStrHelper(value, result + 1, radix) = '\0';
  return AddRadixPrefix(result, radix);
}

/****************************************************************************/
char* LowShortToStrHelper(short value, char* result, int radix)
{
  int digit;

  if (value < 0)
  {
    result = LowShortToStrHelper(value / radix, result, radix);
    digit = (value % radix) * -1;
    *result++ = ObjRelation<int>::InRange(digit, 0xA, 0xF, SortUsingBuiltInOperators<int>::Trait()) ?
			(digit - 0xA + 'A'):
			(digit + '0');
  }

  return result;
}

/****************************************************************************/
char* LowShortToStr(short value, char* result, int radix)
{
  char* ptr;
  Assert(1 <= radix && radix <= 16, ERRMSG_INVALIDRADIX);

  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(32);  

  *result = '-';
  if (radix == 1)
  {
    if (value < 0)
      for (ptr = result; value; value++)
	*result++ = '1';

    *result = '\0';
    return ptr;
  }

  *LowShortToStrHelper(value, result + 1, radix) = '\0';
  return AddRadixPrefix(result, radix);
}

/****************************************************************************/
char* LowLongToStrHelper(long value, char* result, int radix)
{
  int digit;

  if (value < 0)
  {
    result = LowLongToStrHelper(value / radix, result, radix);
    digit = (value % radix) * -1;
    *result++ = ObjRelation<int>::InRange(digit, 0xA, 0xF, SortUsingBuiltInOperators<int>::Trait()) ?
			(digit - 0xA + 'A'):
			(digit + '0');
  }

  return result;
}

/****************************************************************************/
char* LowLongToStr(long value, char* result, int radix)
{
  char* ptr;
  Assert(1 <= radix && radix <= 16, ERRMSG_INVALIDRADIX);

  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(32);

  *result = '-';
  if (radix == 1)
  {
    if (value < 0)
      for (ptr = result; value; value++)
	*result++ = '1';

    *result = '\0';
    return ptr;
  }

  *LowLongToStrHelper(value, result + 1, radix) = '\0';
  return AddRadixPrefix(result, radix);
}

/****************************************************************************/
char* NumToStrHelper(unsigned long value, char* result, int radix)
{
  int digit;

  if (value > 0)
  {
    result = NumToStrHelper(value / radix, result, radix);
    digit = value % radix;
    *result++ = ObjRelation<int>::InRange(digit, 0xA, 0xF, SortUsingBuiltInOperators<int>::Trait()) ?
			(digit - 0xA + 'A'):
			(digit + '0');
  }

  return result;
}

/****************************************************************************/
char* NumToStr(unsigned long value, int sign, char* result, int radix)
{
  char* ptr;
  Assert(1 <= radix && radix <= 16, ERRMSG_INVALIDRADIX);

  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(32);  

  if (sign < 0)
    *result = '-';

  if (radix == 1)
  {
    for (ptr = result; value; value--)
      *result++ = '1';
    *result = '\0';
    return ptr;
  }

  if (sign > 0)
    *NumToStrHelper(value, result, radix) = '\0';
  else if (sign < 0)
    *NumToStrHelper(value, result + 1, radix) = '\0';
  else
    strcpy(result, "0");

  return AddRadixPrefix(result, radix);
}

/****************************************************************************/
char* LongFloatToStr(Ldouble value, char* result, int width, int prec, char fmt)
{
  #if HAS_LONGDOUBLE_STDFNCS  
    if (!result)
      result = (char*)MemMatrix::Matrix().Allocate(32);

    char fmtstr[10];
    strcpy(fmtstr, "%-*.*L");
    fmtstr[6] = fmt;
    fmtstr[7] = 0;
    sprintf(result, fmtstr, width, prec, value);
    return result;
  #else
    return FloatToStr(value, result, width, prec);
  #endif
}

/****************************************************************************/
char* FloatToStr(double value, char* result, int width, int prec, char fmt)
{
  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(32);

  char fmtstr[10];
  strcpy(fmtstr, "%-*.*");
  fmtstr[5] = fmt;
  fmtstr[6] = 0;
  sprintf(result, fmtstr, width, prec, value);
  return result;
}

/****************************************************************************/
// PURPOSE:
//   Short to string conversion procedure. Converts a short value into its
//   string representation.
//
// PRE:
//   short value	: the short value to be converted to a string
//   char* result	: the string to store the result
//   int radix		: the radix of the passed short value
//
// POST:
//   Returns the short value converted to a string
//
char* ShortToStr(short value, char* result, int radix)
{
  return
  (
    (value == SHRT_MIN) ?
      LowShortToStr(value, result, radix):
      NumToStr(abs(value),
	       (value > 0) ? 1:
	       (value < 0) ? -1:0,
	       result, radix)
  );
}

/****************************************************************************/
char* UshortToStr(unsigned short value, char* result, int radix)
{
  return NumToStr(value, (value > 0U), result, radix);
}

/****************************************************************************/
// PURPOSE:
//   Long to string conversion procedure. Converts a long value into its
//   string representation.
//
// PRE:
//   long value		: the long value to be converted to a string
//   char* result	: the string to store the result
//   int radix		: the radix of the passed long value
//
// POST:
//   Returns the long value converted to a string
//
char* LongToStr(long value, char* result, int radix)
{
  return
  (
    (value == LONG_MIN) ?
      LowLongToStr(value, result, radix):
      NumToStr(labs(value),
	       (value > 0) ? 1:
	       (value < 0) ? -1:0,
	       result, radix)
  );
}

/****************************************************************************/
char* UlongToStr(unsigned long value, char* result, int radix)
{
  return NumToStr(value, (value > 0UL), result, radix);
}

/****************************************************************************/
// PURPOSE:
//   Integer to string conversion procedure. Converts a long value into its
//   string representation.
//
// PRE:
//   int value		: the integer value to be converted to a string
//   char* result	: the string to store the result
//   int radix		: the radix of the passed integer value
//
// POST:
//   Returns the integer value converted to a string
//
char* IntToStr(int value, char* result, int radix)
{
  return
  (
    (value == INT_MIN) ?
      LowIntToStr(value, result, radix):
      NumToStr(abs(value),
	       (value > 0) ? 1:
	       (value < 0) ? -1:0,
	       result, radix)
  );
}

/****************************************************************************/
char* UintToStr(unsigned int value, char* result, int radix)
{
  return NumToStr(value, (value > 0U), result, radix);
}

/****************************************************************************/
int IsIntBin(const char* value)
{
  long len = ::SafeStrLen(value);
  if (!len)
    return 0;

  if (toupper(value[len - 1]) == 'B')
  {
    size_t x = 0;
    if (value[x] == '-')
      ++x;
    
    --len;    
    for (;x < len; x++)
      if (value[x] != '0' && value[x] != '1')
        return FALSE;

    return TRUE;
  }

  return IsNumber(value, 1, 0, 0, 2);
}

/****************************************************************************/
int IsIntOct(const char* value)
{
  long len = ::SafeStrLen(value);
  if (!len)
    return 0;

  return IsNumber(value, 1, 0, 0, 8);
}

/****************************************************************************/
int IsIntDec(const char* value)
{
  long len = ::SafeStrLen(value);
  if (!len)
    return 0;

  return IsNumber(value, 1, 0, 0, 10);
}

/****************************************************************************/
int IsIntHex(const char* value)
{
  long len = ::SafeStrLen(value);
  if (!len)
    return 0;

  return IsNumber(value, 1, 0, 0, 16);
}

/****************************************************************************/
char* BinToHexHelper(char* str_)
{
  long x, y, top, val, temp;
  long len = ::SafeStrLen(str_);

  val = 0;
  top = len - 1;
    
  for (x = top; x >= 0; x--)
    if (str_[x] == '1')
    {
      temp = 1;
      for (y = 0; y < (top - x); y++)
        temp *= 2;
      val += temp;
    }

  str_[0] = ('0' + val);
  str_[1] = 0;
  return str_;
}

/****************************************************************************/
char* BinToHex(char* str_)
{
  static char* NullStr_ = NULL;
  int neg = 0;
  long x, y;
  long len = ::SafeStrLen(str_);
  
  if (!len)
    return NullStr_;

  if (toupper(str_[len - 1]) == 'B')
  {
    str_[len - 1] = 0;
    len = strlen(str_);

    if (!len)
      return NullStr_;
  }

  if (str_[0] == '+' || str_[0] == '-')
  {
    neg = (str_[0] == '-') ? 1:0;
    memmove(str_, &str_[1], strlen(str_));
    len = strlen(str_);

    if (!len)
      return NullStr_;
  }
  
  for (x = 0; x < len; x++)
    if (str_[x] != '0' && str_[x] != '1')
      return NullStr_;

  if (len < 4)
    return BinToHexHelper(str_);    

  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(len + neg + 1);

  y = 0;
  for (x = len - 4; x >= 0; x -= 4)
  {
    Buffer_[y++] = (strncmpi(&str_[x], "0000", 4) == 0) ? '0':
                   (strncmpi(&str_[x], "0001", 4) == 0) ? '1':
                   (strncmpi(&str_[x], "0010", 4) == 0) ? '2':
                   (strncmpi(&str_[x], "0011", 4) == 0) ? '3':
                   (strncmpi(&str_[x], "0100", 4) == 0) ? '4':
                   (strncmpi(&str_[x], "0101", 4) == 0) ? '5':
                   (strncmpi(&str_[x], "0110", 4) == 0) ? '6':
                   (strncmpi(&str_[x], "0111", 4) == 0) ? '7':
                   (strncmpi(&str_[x], "1000", 4) == 0) ? '8':
                   (strncmpi(&str_[x], "1001", 4) == 0) ? '9':
                   (strncmpi(&str_[x], "1010", 4) == 0) ? 'A':
                   (strncmpi(&str_[x], "1011", 4) == 0) ? 'B':
                   (strncmpi(&str_[x], "1100", 4) == 0) ? 'C':
                   (strncmpi(&str_[x], "1101", 4) == 0) ? 'D':
                   (strncmpi(&str_[x], "1110", 4) == 0) ? 'E':
                   (strncmpi(&str_[x], "1111", 4) == 0) ? 'F':0;
  }

  if (x > -4)
  {
    char Temp_[4];  
    x += 4;
    strncpy(Temp_, str_, x);
    Temp_[x] = 0;
    Buffer_[y++] = *(BinToHexHelper(Temp_));
    Buffer_[y] = 0;
  }
  else
    Buffer_[y] = 0;

  len = strlen(Buffer_);
  if (len > 1)
    StrReverse(Buffer_);

  if (neg)
    strcpy(str_, "-0x");
  else
    strcpy(str_, "0x");
        
  strcat(str_, Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);  
  return str_;
}

/****************************************************************************/
const char* ToBinStr(char ch)
{
  ch = toupper(ch);
  return
  (
    (ch == '0') ? "0000":
    (ch == '1') ? "0001":
    (ch == '2') ? "0010":
    (ch == '3') ? "0011":
    (ch == '4') ? "0100":
    (ch == '5') ? "0101":
    (ch == '6') ? "0110":
    (ch == '7') ? "0111":
    (ch == '8') ? "1000":
    (ch == '9') ? "1001":
    (ch == 'A') ? "1010":
    (ch == 'B') ? "1011":
    (ch == 'C') ? "1100":
    (ch == 'D') ? "1101":
    (ch == 'E') ? "1110":
    (ch == 'F') ? "1111":0
  );
}

/****************************************************************************/
int BytesInCmp2Bin(const char* str_)
{
  int neg = 0;
  long len = ::SafeStrLen(str_);
  int leadch;

  if (!len)
    return 0;

  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(len + 1);
  strcpy(Buffer_, str_);

  if (toupper(Buffer_[len - 1]) == 'B')
  {
    Buffer_[len - 1] = 0;
    len = strlen(Buffer_);

    if (!len)
    {
      MemMatrix::Matrix().Deallocate(Buffer_);
      return 0;
    }
  }

  leadch = Buffer_[0];
  if (leadch == '+' || leadch == '-')
  {
    neg = (leadch == '-');
    memmove(Buffer_, &Buffer_[1], strlen(Buffer_));
    len = strlen(Buffer_);
    leadch = Buffer_[0];

    if (!len)
    {
      MemMatrix::Matrix().Deallocate(Buffer_);
      return 0;
    }
  }

  MemMatrix::Matrix().Deallocate(Buffer_);
  int Bytes_ = len / 8;
  if (len % 8 || (neg && leadch == '1'))
    Bytes_++;

  return Bytes_;
}

/****************************************************************************/
char* BinToCmp2Bin(char* str_, int NumBytes_)
{
  static char* NullStr_ = NULL;
  int neg = 0;
  long x, carry;
  long len = ::SafeStrLen(str_);

  if (!len)
    return NullStr_;

  if (toupper(str_[len - 1]) == 'B')
  {
    str_[len - 1] = 0;
    len = strlen(str_);

    if (!len)
      return NullStr_;
  }

  if (str_[0] == '+' || str_[0] == '-')
  {
    neg = (str_[0] == '-');
    memmove(str_, &str_[1], strlen(str_));
    len = strlen(str_);

    if (!len)
      return NullStr_;
  }

  long Bytes_ = len / 8;
  if (len % 8)
    Bytes_++;

  if (NumBytes_ < Bytes_)
    return NullStr_;
  else if (NumBytes_ == Bytes_ && (len / 8) == Bytes_)
  {
    if (neg)
    {
      if (str_[0] == '1')
        return NullStr_;
    }
    else
      return str_;
  }

  StrReverse(str_);
  x = NumBytes_ * 8;
  
  while (len < x)
    str_[len++] = '0';
    
  str_[len] = 0;
  
  if (neg)
  {
    for (x = 0; x < len; x++)
      str_[x] = (str_[x] == '0') ? '1':
                (str_[x] == '1') ? '0':0;

    carry = 1;
    for (x = 0; x < len && carry; x++)
    {
      carry += (str_[x] - '0');
      carry = (carry == 2) ? 1:0;
      str_[x] = carry ? '0':'1';
    }

    if (carry)
    {
      str_[len++] = '1';
      str_[len] = 0;
    }
  }

  StrReverse(str_);
  return str_;
}

/****************************************************************************/
char* Cmp2BinToBin(char* str_)
{
  static char* NullStr_ = NULL;
  int neg = 0;
  long x, carry;
  long len = ::SafeStrLen(str_);

  if (!len)
    return NullStr_;

  neg = str_[0] == '1';
  if (!neg)
  {
    if (str_[0] == '0')
    {
      StrReverse(str_);
      while (len && str_[len - 1] == '0')
        len--;

      if (len)      
        str_[len] = 0;
      else      
        str_[len=1] = 0;

      StrReverse(str_);      
    }
    
    return str_;
  }

  StrReverse(str_);
  for (x = 0; x < len; x++)
    str_[x] = (str_[x] == '0') ? '1':
              (str_[x] == '1') ? '0':0;

  carry = 1;
  for (x = 0; x < len && carry; x++)
  {
    carry += (str_[x] - '0');
    carry = (carry == 2) ? 1:0;
    str_[x] = carry ? '0':'1';
  }

  if (carry)
  {
    str_[len++] = '1';
    str_[len] = 0;
  }

  while (len && str_[len - 1] == '0')
    len--;

  if (len)
    str_[len] = 0;
  else
    str_[len=1] = 0;

  StrReverse(str_);
  memmove(&str_[1], str_, strlen(str_) + 1);
  str_[0] = '-';  
  
  return str_;
}

/****************************************************************************/
char* CharToStr(unsigned char ch, char* result)
{
  if (!result)
    result = (char*)MemMatrix::Matrix().Allocate(2);

  return strcpy(result, CharToStr(ch));
}

/****************************************************************************/
char* CharToStr(unsigned char ch)
{
  static char result[2];

  result[0] = ch;
  result[1] = '\0';

  return result;
}

/****************************************************************************/
long double StrToLd(const char* nptr, char** endptr)
{
  int negative;
  long double i;
  register const char *s;
  register unsigned char c;
  const char *save;
  int powinc, expval;

  s = nptr;

  /* Skip white space.  */
  if (s)
    while (isspace(*s))
      ++s;

  if (!s || *s == '\0')
  {
    /* There was no number to convert.  */
    if (endptr != NULL)
      *endptr = (char *) nptr;
    return 0L;
  }

  /* Check for a sign.  */
  if (*s == '-')
  {
    negative = 1;
    ++s;
  }
  else
  {
    negative = 0;
    if (*s == '+')
      ++s;
  }

  /* Save the pointer so we can check later if anything happened.  */
  save = s;
  i = 0;
  powinc = expval = 0;

  for (c = *s; c != '\0' && toupper(c) != 'E'; c = *++s)
  {
    if (isdigit (c))
      c -= '0';
    else
      if (c == '.' && !powinc)
      {
	powinc = 1;
	continue;
      }
      else
	break;

    i *= 10UL;
    i += c;
    expval -= powinc;
  }

  expval += (toupper(c) == 'E') ? atoi(s + 1):0;
  for (powinc = (expval < 0) ? 1:-1; expval; expval += powinc)
    if (expval < 0)
      i /= 10;
    else
      i *= 10;

  /* Check if anything actually happened.  */
  if (s == save)
  {
    /* There was no number to convert.  */
    if (endptr != NULL)
      *endptr = (char *) nptr;
    return 0L;
  }

  /* Store in ENDPTR the address of one character
     past the last character we converted.  */
  if (endptr != NULL)
    *endptr = (char *) s;

  /* Return the result of the appropriate sign.  */
  return (negative ? -i : i);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Function to make a character set from either a string or character filter
//   function. The character set is represented a standard C string of ASCII
//   characters put into a null terminated string with special formatting
//   for the null or 0th character. Any non-null characters is set as a
//   member in the set if it is set with its own ASCII character code,
//   otherwise it is set to 0 if not a member in the set. the 0th or null
//   character is a special case in which its membership in the set is
//   determined by the first character in the ASCII string. It is explicitly
//   set as a member by setting the first character in the string to the
//   character '0'. It can also be explicitly set the non-membership by
//   setting the first character of the string to the character '-'. If
//   neither of these characters are present in the first character of the
//   the set string then non-membership of the null character is implied.
//
//   for example:
//
//   "0helo" 	: specifies that the null character and 'e','l','h','o' are
//	          members of the character set
//   "-0urb"    : specifies that the characters '0','b','r','u' are present
//                in the set (the null character is explicitly set as
//		  non-member)
//   "1st"	: specifies that the characters '1','s','t' are members of
//		  of the character set (the null character is implied as
//		  non-member)
//   "lesthan0" : specifies that the characters 'a','e','h','l','n','s','t',
//		  '0' are members of the set (the null character is implied
//		  as non-member)
//
char* MakeCharSet(int(*iscsetfn)(int), const char* src, size_t RunLen_)
{
#if OVERLOAD_NEW
  char* charset = (char*)RawAllocateWith(MEMMATRIX, sizeof(char) * 257);
#else
  char* charset = (char*)RawAllocateWith(STD_NEW, sizeof(char) * 257);
#endif

  int charcnt, index;
  charcnt = 0;

  if (src && RunLen_)
  {
    for (charcnt = index = 0; index < 256; index++)
      charset[index] = memchr(src, index, RunLen_) ? 1:0;
  }
  else if (iscsetfn)
  {
    if (iscsetfn == isspace)
    {
      strcpy(charset, "- \f\n\r\t\v");
      return charset;
    }
    else if (iscsetfn == isdigit)
    {
      strcpy(charset, "-0123456789");
      return charset;
    }
    else if (iscsetfn == isxdigit)
    {
      strcpy(charset, "-0123456789ABCDEF");
      return charset;
    }
    else if (iscsetfn == islower)
    {
      strcpy(charset, "-abcdefghijklmnopqrstuvwxyz");
      return charset;    
    }
    else if (iscsetfn == isupper)
    {
      strcpy(charset, "-ABCDEFGHIJKLMNOPQRSTUVWXYZ");
      return charset;        
    }
    else if (iscsetfn == isalpha)
    {
      strcpy(charset, "-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
      return charset;            
    }
    else if (iscsetfn == isgraph)
    {
      memset(charset, 0, 33);
      memset(charset + 33, 1, 256 - 33);
    }
    else if (iscsetfn == isprint)
    {
      memset(charset, 0, 32);
      memset(charset + 32, 1, 256 - 32);
    }
    else if (iscsetfn == iscntrl)
    {
      memset(charset, 1, 32);
      memset(charset + 32, 0, 256 - 32);
    }    
    else    
      for (charcnt = index = 0; index < 256; index++)
        charset[index] = (*iscsetfn)(index);
  }

  charset[charcnt++] = charset[0] ? '0':'-';

  for (index = 1; index < 256; index++)
    if (charset[index])
    {
      charset[charcnt] = index;
      charcnt++;
    }

  charset[charcnt] = 0;
  return charset;
}

/****************************************************************************/
char* MakeCharSet(const char* src, size_t RunLen_)
{
  if (!RunLen_)
    RunLen_ = SafeStrLen(src);

  return MakeCharSet(NULL, src, RunLen_);
}

/****************************************************************************/
Boolean InCharSet(char ch, const char* chset)
{
  if (chset == NULL || *chset == 0)
    return FALSE;

  if (*chset == '0' && ch == 0)
    return TRUE;

  if (*chset == '-' || *chset == '0')
    ++chset;

  for (;*chset; chset++)
    if (ch == *chset)
      return TRUE;

  return FALSE;
}

/****************************************************************************/
void CopyCharSet(char* dest, const char* src)
{
  if (!dest || !src)
    return;

  strcpy(dest, src);
}

/****************************************************************************/
size_t CharSetLen(const char* chset)
{
  if (!chset)
    return 0;

  return strlen(chset);
}

/****************************************************************************/
/*
PURPOSE:
  Procedure to strip all specified characters from a string

PRE:
  char* str : the string which has the specified characters to be stripped

POST:
  Returns the string with all specified characters in the string removed.
*/
char* RemoveChar(char* str, const char* chset)
{
  size_t i, x, Max_;

  Max_ = strlen(str);
  for (i = x = 0; i < Max_; i++)
  {
    for (;i < Max_ && InCharSet(str[i], chset); i++);
    str[x++] = str[i];
  }
  str[x] = '\0';

  return str;
}

/****************************************************************************/
char* RemoveByte(char* str, size_t& RunLen_, const char* chset)
{
  size_t i, x;

  for (i = x = 0; i < RunLen_; i++)
  {
    for (;i < RunLen_ && InCharSet(str[i], chset); i++);
    str[x++] = str[i];
  }
  RunLen_ = x;

  return str;
}

/****************************************************************************/
char* RemoveLeading(char* str, const char* chset)
{
  size_t x, l;

  for (l = 0; str[l] && InCharSet(str[l], chset); l++);
  for (x = 0; (str[x] = str[l]); x++, l++);

  return str;
}

/****************************************************************************/
char* RemoveLeadingBytes(char* str, size_t& RunLen_, const char* chset)
{
  size_t x, l;

  for (l = 0; l < RunLen_ && InCharSet(str[l], chset); l++);
  for (x = 0; l < RunLen_; x++, l++)
    str[x] = str[l];

  RunLen_ = x;
  return str;
}

/****************************************************************************/
char* RemoveTrailing(char* str, const char* chset)
{
  for (size_t l = strlen(str); l;)
    if (InCharSet(str[--l], chset))
      str[l] = '\0';
    else
      break;

  return str;
}

/****************************************************************************/
char* RemoveTrailingBytes(char* str, size_t& RunLen_, const char* chset)
{
  int InSet_;
  size_t l;
  for (l = RunLen_ - 1; (InSet_ = InCharSet(str[l], chset)) && l; --l);
  RunLen_ = InSet_ ? 0:(l + 1);

  return str;
}

/****************************************************************************/
char* RemovePadding(char* str, const char* chset)
{
  RemoveLeading(str, chset);
  RemoveTrailing(str, chset);
  return str;
}

/****************************************************************************/
char* RemovePaddingBytes(char* str, size_t& RunLen_, const char* chset)
{
  RemoveLeadingBytes(str, RunLen_, chset);
  RemoveTrailingBytes(str, RunLen_, chset);
  return str;
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   substitute the NULL character in a string with another character
//
// PRE:
//   char* s		: the string
//   size_t RunLen_	: the length of the string. This must be specified
//			  explicitly since cannot use strlen function to find
//			  length of string if null character is included as
//			  part of string and is not treated as the special
//			  null terminator.
//   char pad		: the new character to substitute
//   char nullchar 	: The specified null character for the string.
//		     	  default to '\0' for standard C style strings.
//
// POST:
//   The new string substituted with the specified character is returned
//
char* SubstNull(char* s, size_t RunLen_, char pad, char nullchar)
{
  for (size_t index = 0; index < RunLen_; index++)
    if (s[index] == nullchar)
      s[index] = pad;

  return s;
}

/****************************************************************************/
// PURPOSE:
//   substitute specified character in a string with another character
//
// PRE:
//   char* s		: the string
//   size_t RunLen_	: the length of the string
//   char ch		: the character to find
//   char pad		: the new character to substitute
//
// POST:
//   The new string substituted with the specified character is returned
//
char* SubstByte(char* s, size_t RunLen_, const char* chset, char pad)
{
  for (size_t x = 0; x < RunLen_; x++)
    if (InCharSet(s[x], chset))
      s[x] = pad;

  return s;
}

/****************************************************************************/
char* SubstnByte(char* s, size_t RunLen_, const char* chset, char pad, size_t n, size_t index)
{
  size_t lim = RunLen_;
  int dir = 1;

  if (index == RunLen_)
  {
    --index;
    dir = -1;
    lim = 0;
    --lim;
  }
  else if (index < RunLen_)
  {
    dir = 1;
    lim = RunLen_;
  }
  else
    return s;

  for (;lim - index && n; index += dir)
    if (InCharSet(s[index], chset))
    {
      s[index] = pad;
      --n;
    }

  return s;
}

/****************************************************************************/
char* StrByteFill(char* s, size_t RunLen_, char pad, size_t len)
{
  if (!len)
    len = RunLen_;

  if (len)
    for (size_t index = 0; index < len; index++)
      s[index] = pad;

  return s;
}

/****************************************************************************/
size_t CountByte(const char* s, size_t RunLen_, const char* chset)
{
  size_t count, x;

  for (count = x = 0; x < RunLen_; x++)
    if (InCharSet(s[x], chset))
      count++;

  return count;
}

/****************************************************************************/
size_t FindNextByteStr(const char* Str_, size_t RunLen_, const char* Delims_, size_t& Index_, size_t& NextPos_)
{
  if (!Str_)
    return 0;

  for (;Index_ < RunLen_ && InCharSet(Str_[Index_], Delims_); Index_++);
  NextPos_ = Index_;

  for (;NextPos_ < RunLen_; NextPos_++)
    if (InCharSet(Str_[NextPos_], Delims_))
      break;

  return (NextPos_ - Index_);
}

/****************************************************************************/
char* ByteStrReverse(char* str, size_t RunLen_)
{
  size_t len = RunLen_;
  size_t lim, index;
  char temp;

  if (!len)
    return str;

  lim = len / 2;
  --len;

  for (index = 0; index < lim; index++)
  {
    temp = str[index];
    str[index] = str[len - index];
    str[len - index] = temp;
  }

  return str;
}

/****************************************************************************/
char* NewByteString(const char* src, size_t RunLen_, MemAllocMethod Method_)
{
#if OVERLOAD_NEW
  char* ns = NULL;
  return ((src && (ns = (char*)RawAllocateWith(Method_, sizeof(char) * RunLen_))) ? (char*)memmove(ns, src, RunLen_):NULL);
#else
  char* ns = NULL;
  return ((src && (ns = (char*)RawAllocateWith(STD_NEW, sizeof(char) * RunLen_))) ? (char*)memmove(ns, src, RunLen_):NULL);
#endif
}

/****************************************************************************/
char* ReplaceByteString(char* trg, const char* src, size_t RunLen_, MemAllocMethod Method_)
{
  trg = EraseString(trg);
  trg = NewByteString(src, RunLen_, Method_);
  return trg;
}

/****************************************************************************/
char* ResizeByteString(char* src, size_t& RunLen_, size_t NewSize_, MemAllocMethod Method_)
{
  char* Temp_ = src;

#if OVERLOAD_NEW
  if (MemMatrix::Matrix().HasThis(src))
    src = (char*)RawReallocateWith(Method_, src, sizeof(char) * NewSize_);
  else
  {
    src = (char*)RawAllocateWith(Method_, sizeof(char) * NewSize_);

    if (src)
    {
      memmove(src, Temp_, RunLen_);
      delete[] Temp_;
    }
  }
#else
  src = (char*)RawAllocateWith(STD_NEW, sizeof(char) * NewSize_);

  if (src)
  {
    memmove(src, Temp_, RunLen_);
    delete[] Temp_;
  }
#endif

  if (src == NULL)
    return NULL;

  RunLen_ = NewSize_;
  return src;
}

/****************************************************************************/
char* NewString(const char* src, MemAllocMethod Method_)
{
#if OVERLOAD_NEW
  char* ns = NULL;
  return ((src && (ns = (char*)RawAllocateWith(Method_, sizeof(char) * (strlen(src) + 1)))) ? strcpy(ns, src):NULL);
#else
  char* ns = NULL;
  return ((src && (ns = (char*)RawAllocateWith(STD_NEW, sizeof(char) * (strlen(src) + 1)))) ? strcpy(ns, src):NULL);
#endif
}

/****************************************************************************/
char* ReplaceString(char* trg, const char* src, MemAllocMethod Method_)
{
  trg = EraseString(trg);
  trg = NewString(src, Method_);
  return trg;
}

/****************************************************************************/
char* ResizeString(char* src, size_t NewSize_, MemAllocMethod Method_)
{
  char* Temp_ = src;

#if OVERLOAD_NEW
  if (MemMatrix::Matrix().HasThis(src))
    src = (char*)RawReallocateWith(Method_, src, sizeof(char) * (NewSize_ + 1));
  else
  {
    src = (char*)RawAllocateWith(Method_, sizeof(char) * (NewSize_ + 1));

    if (src)
    {
      strcpy(src, Temp_);
      delete[] Temp_;
    }
  }
#else
  src = (char*)RawAllocateWith(STD_NEW, sizeof(char) * (NewSize_ + 1));

  if (src)
  {
    strcpy(src, Temp_);
    delete[] Temp_;
  }
#endif

  if (src == NULL)
    return NULL;

  src[NewSize_] = 0;
  return src;
}

/****************************************************************************/
char* EraseString(char* str)
{
  if (str)
    RawDeleteArray(str);

  str = NULL;
  return str;
}

/*****************************************************************************/
int FindFunctionDelimiters(const char* list, size_t len, int** DelimArrayPtr_, size_t ArraySize_,
                           const char* LeftBrackets_, const char* RightBrackets_, char SepChar_,
                           char QuoteChar_, MemAllocMethod Method_)
{
  if (!list || !DelimArrayPtr_)
    return 0;

  size_t i, x, y;
  size_t bal = 0;
  size_t OldSize_;
  int* DelimArray_ = *DelimArrayPtr_;
  int* OldArray_;
  int InQuote_ = 0;
  
  x = 1;
  DelimArray_[0] = 0;
  
  for (i = 0; i < len; i++)
  {
    if (list[i] == QuoteChar_)
      InQuote_ = !InQuote_;

    if (InQuote_)
      continue;
    
    if (InCharSet(list[i], LeftBrackets_))
    {
      ++bal;
      if (bal == 1)
      {
        DelimArray_[x++] = i;
        ++DelimArray_[0];
      }
    }
    else if (InCharSet(list[i], RightBrackets_))
    {
      --bal;
      if (bal == 0)
      {
        DelimArray_[x++] = i;
        ++DelimArray_[0];
        break;
      }
    }
    else if (list[i] == SepChar_ && bal == 1)
    {
      DelimArray_[x++] = i;
      ++DelimArray_[0];      
    }

    if (x >= ArraySize_)
    {
      OldSize_ = ArraySize_;      
      OldArray_ = DelimArray_;
      ArraySize_ += 20;
      
      if (MemMatrix::Matrix().HasThis(OldArray_))
        DelimArray_ = (int*)RawAllocateWith(Method_, sizeof(int) * ArraySize_);      

      for (y = 0; y < OldSize_; y++)
        DelimArray_[y] = OldArray_[y];

      for (;y < ArraySize_; y++)
        DelimArray_[y] = 0;

      RawDeleteArray(OldArray_);
    }    
  }

  *DelimArrayPtr_ = DelimArray_;
  return DelimArray_[0];
}

/****************************************************************************/
char* RetrieveFunctionArguments(const char* SubList_, int* DelimArray_)
{
  size_t len = DelimArray_[DelimArray_[0]] + 1;
  char* Buffer_;
  int x;
  
  Buffer_ = (char*)MemMatrix::Matrix().Callocate(len + 1);
  strncpy(Buffer_, SubList_, len);
  Buffer_[len] = 0;
  
  for (x = 1; x <= DelimArray_[0]; x++)
    Buffer_[DelimArray_[x]] = 0;

  return Buffer_;
}

/****************************************************************************/
size_t ByteStrHasChar(const char* Str_, size_t RunLen_, const char* chset)
{
  if (Str_ && chset)
  {
    size_t x;
    for (x = 0; x < RunLen_; x++)
      if (InCharSet(Str_[x], chset))
	break;

    return x;
  }

  return (Str_ ? Str_[::strlen(Str_)]:0);
}

/****************************************************************************/
char* UpCase(char* Str_)
{
  size_t Len_ = SafeStrLen(Str_);
  size_t i;
  
  if (Len_)  
    for (i = 0; i < Len_; i++)
      Str_[i] = toupper(Str_[i]);

  return Str_;
}

/****************************************************************************/
char* DownCase(char* Str_)
{
  size_t Len_ = SafeStrLen(Str_);
  size_t i;
  
  if (Len_)  
    for (i = 0; i < Len_; i++)
      Str_[i] = tolower(Str_[i]);

  return Str_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to compare two strings for equality. If both strings are
//   NULL then procedure returns 0, otherwise the strings are compared
//   character by character up to max characters same as strncmp, but
//   without case sensitivity.
//
// PRE:
//   char* s1 : the first string
//   char* s2 : the second string
//   int max : the maximum number of characters to compare in each string
//
// POST:
//   Return Value:
//   These routines return an int value based on the result of comparing
//   s1 (or part of it) to s2 (or part of it):
//   þ <  0  if s1 <  s2
//   þ == 0  if s1 == s2
//   þ >  0  if s1 >  s2
//
int StrniComp(const char* s1, const char* s2, size_t max)
{
  size_t index;
  int diff;

  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  for (index = 0, diff = 0;
       index < max &&
       !(diff = tolower(s1[index]) - tolower(s2[index])) &&
       s1[index] && s2[index];
       index++);

  return diff;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to compare two strings for equality. If both strings are
//   NULL then procedure returns 0, otherwise the strings are compared
//   character by character until a NULL characters is found in either string
//   same as strcmp, but without case sensitivity.
//
// PRE:
//   char* s1 : the first string
//   char* s2 : the second string
//   int max : the maximum number of characters to compare in each string
//
// POST:
//   Return Value:
//   These routines return an int value based on the result of comparing
//   s1 (or part of it) to s2 (or part of it):
//   þ <  0  if s1 <  s2
//   þ == 0  if s1 == s2
//   þ >  0  if s1 >  s2
//
int StriComp(const char* s1, const char* s2)
{
  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  int diff;
  for (diff = 0;
       !(diff = tolower(*s1) - tolower(*s2)) && *s1 && *s2;
       s1++, s2++);

  return diff;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to compare two strings for equality. If both strings are
//   NULL then procedure returns 0, otherwise the strings are compared
//   character by character up to max characters same as strncmp, but
//   without case sensitivity.
//
//   NOTE: strings are not null terminated in this case
//
// PRE:
//   char* s1 : the first string (not null-terminated)
//   char* s2 : the second string (not null-terminated)
//   int max : the maximum number of characters to compare in each string
//
// POST:
//   Return Value:
//   These routines return an int value based on the result of comparing
//   s1 (or part of it) to s2 (or part of it):
//   þ <  0  if s1 <  s2
//   þ == 0  if s1 == s2
//   þ >  0  if s1 >  s2
//
int MemiComp(const char* s1, const char* s2, size_t max)
{
  size_t index;
  int diff;

  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  for (index = 0, diff = 0;
       index < max &&
       !(diff = tolower(s1[index]) - tolower(s2[index]));
       index++);

  return diff;
}

/****************************************************************************/
int ReadStringPart(istream& In_, char* Dest_, int Size_, char Delimiter_, int RdStatus_)
{
  if (RdStatus_ > 0)
    return RdStatus_;

  if (!RdStatus_)
    In_.clear();

  Boolean RetVal_ = ::ReadLine(In_, Dest_, Size_, Delimiter_);
  return (RetVal_ ? 1:0);
}

/****************************************************************************/
char* ReadStringUntil(istream& In_, char* Dest_, int& Size_, char Delimiter_, int Max_)
{
  Boolean RetVal_ = FALSE;
  int Incr_ = Size_;
  size_t Iter_ = 0;
  char* Start_ = Dest_;

  while (!RetVal_)
  {
    RetVal_ = ::ReadLine(In_, Dest_, Incr_, Delimiter_);

    if (!RetVal_)
    {
      if (Max_ - Size_ >= Incr_)
        Size_ += Incr_;
      else
        break;
      
      Dest_ = ResizeString(Start_, Size_);
      
      if (!Dest_)
        break;
      
      Start_ = Dest_;
      Iter_++;
      Dest_ += (Iter_ * (Incr_ - 1));
      In_.clear();
    }
  }

  if (!RetVal_)
    In_.clear();
    
  return Start_;
}

/****************************************************************************/
/****************************************************************************/
#if STRUTIL_DEBUG
#if !defined(INCL_CONIO_H) & defined(__TURBOC__)
  #include <conio.h>
  #define INCL_CONIO_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
int main()
{
  long double ldval;
  char buffer[21];
  system("clear");

#if STRUTIL_DEBUG_BASECONV
  char Buffer_[35];
  
  LongToStr(42, buffer, 2);
  cout <<buffer <<endl;
  strcpy(Buffer_, buffer);
  assert(!SafeStrCmp(buffer, "101010"));
  BinToHex(buffer);
  cout <<buffer <<endl;
  assert(!SafeStrCmp(buffer, "0x2A"));
  BinToCmp2Bin(Buffer_, 4);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "00000000000000000000000000101010"));
  Cmp2BinToBin(Buffer_);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "101010"));

  LongToStr(147, buffer, 2);
  cout <<buffer <<endl;
  strcpy(Buffer_, buffer);  
  assert(!SafeStrCmp(buffer, "10010011"));
  BinToHex(buffer);
  cout <<buffer <<endl;
  assert(!SafeStrCmp(buffer, "0x93"));
  BinToCmp2Bin(Buffer_, 4);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "00000000000000000000000010010011"));
  Cmp2BinToBin(Buffer_);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "10010011"));

  LongToStr(255, buffer, 2);
  cout <<buffer <<endl;
  strcpy(Buffer_, buffer);  
  assert(!SafeStrCmp(buffer, "11111111"));
  BinToHex(buffer);
  cout <<buffer <<endl;
  assert(!SafeStrCmp(buffer, "0xFF"));
  BinToCmp2Bin(Buffer_, 4);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "00000000000000000000000011111111"));
  Cmp2BinToBin(Buffer_);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "11111111"));

  LongToStr(-152, buffer, 2);
  cout <<buffer <<endl;
  strcpy(Buffer_, buffer);  
  assert(!SafeStrCmp(buffer, "-10011000"));
  BinToHex(buffer);
  cout <<buffer <<endl;
  assert(!SafeStrCmp(buffer, "-0x98"));
  BinToCmp2Bin(Buffer_, 4);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "11111111111111111111111101101000"));
  Cmp2BinToBin(Buffer_);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "-10011000"));

  LongToStr(-255, buffer, 2);
  cout <<buffer <<endl;
  strcpy(Buffer_, buffer);  
  assert(!SafeStrCmp(buffer, "-11111111"));
  BinToHex(buffer);
  cout <<buffer <<endl;
  assert(!SafeStrCmp(buffer, "-0xFF"));
  BinToCmp2Bin(Buffer_, 4);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "11111111111111111111111100000001"));
  Cmp2BinToBin(Buffer_);
  cout <<Buffer_ <<endl;
  assert(!SafeStrCmp(Buffer_, "-11111111"));
  
#endif

#if STRUTIL_DEBUG_CONVERT
  LongToStr(10, buffer, 1);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "1111111111"));
  ldval = strtold("-3515781642.5132501e+3", NULL);
  cout <<ldval <<endl;
  //assert(ldval == -3515781642.5132501e+3);
  LongToStr(23500, buffer);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "23500"));
  LongToStr(-57523, buffer);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "-57523"));
  LongToStr(LONG_MAX, buffer);
  cout <<buffer <<endl;
  LongToStr(LONG_MAX, buffer, 16);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MAX, buffer);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MAX, buffer, 16);
  cout <<buffer <<endl;
  LongToStr(LONG_MIN, buffer);
  cout <<buffer <<endl;
  LongToStr(LONG_MIN, buffer, 16);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MIN, buffer);
  cout <<buffer <<endl;
  ShortToStr(SHRT_MIN, buffer, 16);
  cout <<buffer <<endl;
  FloatToStr(-543.212394, buffer, 21, 10);
  cout <<buffer <<endl;
  FloatToStr(-543.212394e8, buffer, 21, 10);
  cout <<buffer <<endl;
#endif

#if STRUTIL_DEBUG_HEXINPUT
  UintToStr(0xCAFE, buffer, 16);
  cout <<AddPosSign(buffer) <<endl;
  assert(!strcmp(buffer, "+0xCAFE"));
  IntToStr(-0x535A, buffer, 16);
  cout <<buffer <<endl;
  assert(!strcmp(buffer, "-0x535A"));
#endif

#if STRUTIL_DEBUG_VALIDATE
  char* newbptr;
  int pdif;

  newbptr = ExtractSignedNum("-45231", pdif);
  cout <<"IsSignedNum(\"-45231\"): " <<IsSignedNum("-45231") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractSignedNum("45-231", pdif);
  cout <<"IsSignedNum(\"45-231\"): " <<IsSignedNum("45-231") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractSignedNum("-3515.51501", pdif);
  cout <<"IsSignedNum(\"-3515.51501\"): " <<IsSignedNum("-3515.51501") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractSignedNum("-0xdeadbeef", pdif);
  cout <<"IsSignedNum(\"-0xdeadbeef\"): " <<IsSignedNum("-0xdeadbeef") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("0x-deadbeef", pdif);
  cout <<"IsFloat(\"0x-deadbeef\"): " <<IsFloat("0x-deadbeef") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-3515.51501e15", pdif);
  cout <<"IsFloat(\"-3515.51501e15\"): " <<IsFloat("-3515.51501e15") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-3515.51501e-15", pdif);
  cout <<"IsFloat(\"-3515.51501e-15\"): " <<IsFloat("-3515.51501e-15") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-3515.51501e+015", pdif);
  cout <<"IsFloat(\"-3515.51501e+015\"): " <<IsFloat("-3515.51501e+015") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-3515.51501e-015", pdif);
  cout <<"IsFloat(\"-3515.51501e-015\"): " <<IsFloat("-3515.51501e-015") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);  

  newbptr = ExtractFloat("-351551501e+1.5", pdif);
  cout <<"IsFloat(\"-351551501e+1.5\"): " <<IsFloat("-351551501e+1.5") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-3515.51501e", pdif);
  cout <<"IsFloat(\"-3515.51501e\"): " <<IsFloat("-3515.51501e") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("0.5150", pdif);
  cout <<"IsFloat(\"0.5150\"): " <<IsFloat("0.5150") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat(".5150", pdif);
  cout <<"IsFloat(\".5150\"): " <<IsFloat(".5150") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-0xdeadbeefe+12", pdif);
  cout <<"IsFloat(\"-0xdeadbeefe+12\"): " <<IsFloat("-0xdeadbeefe+12") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-0xdead.beefe-12", pdif);
  cout <<"IsFloat(\"-0xdead.beefe-12\"): " <<IsFloat("-0xdead.beefe-12") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-.e+5", pdif);
  cout <<"IsFloat(\"-.e+5\"): " <<IsFloat("-.e+5") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat(".e+5", pdif);
  cout <<"IsFloat(\".e+5\"): " <<IsFloat(".e+5") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("-e+5", pdif);
  cout <<"IsFloat(\"-e+5\"): " <<IsFloat("-e+5") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat("e+5", pdif);
  cout <<"IsFloat(\"e+5\"): " <<IsFloat("e+5") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractFloat(".", pdif);
  cout <<"IsFloat(\".\"): " <<IsFloat(".") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractUnsignedNum("-65535", pdif);
  cout <<"IsUnsignedNum(\"-65535\"): " <<IsUnsignedNum("-65535") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractUnsignedNum("65535", pdif);
  cout <<"IsUnsignedNum(\"65535\"): " <<IsUnsignedNum("65535") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractUnsignedNum("-0xfeedcad", pdif);
  cout <<"IsUnsignedNum(\"-0xfeedcad\"): " <<IsUnsignedNum("-0xfeedcad") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractUnsignedNum("0xfeedcad", pdif);
  cout <<"IsUnsignedNum(\"0xfeedcad\"): " <<IsUnsignedNum("0xfeedcad") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractSignedNum("-", pdif);
  cout <<"IsSignedNum(\"-\"): " <<IsSignedNum("-") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  newbptr = ExtractSignedNum("000", pdif);
  cout <<"IsSignedNum(\"000\"): " <<IsSignedNum("000") <<"; " <<(newbptr ? newbptr:" ") <<endl;
  ::DeleteArray(newbptr);
  
  cout <<"IsInt(\"32768\"): " <<IsInt("32768") <<endl;
  cout <<"IsInt(\"42767\"): " <<IsInt("42767") <<endl;
  cout <<"IsInt(\"33767\"): " <<IsInt("33767") <<endl;
  cout <<"IsInt(\"32767\"): " <<IsInt("32767") <<endl;

  cout <<"IsInt(\"-32768\"): " <<IsInt("-32768") <<endl;
  cout <<"IsInt(\"-42767\"): " <<IsInt("-42767") <<endl;
  cout <<"IsInt(\"-33767\"): " <<IsInt("-33767") <<endl;
  cout <<"IsInt(\"-32767\"): " <<IsInt("-32767") <<endl;
  cout <<"IsInt(\"0xFFFF\"): " <<IsInt("0xFFFF") <<endl;
  cout <<"IsInt(\"0x7FFF\"): " <<IsInt("0x7FFF") <<endl;
  cout <<"IsInt(\"-0x8000\"): " <<IsInt("-0x8000") <<endl;

  cout <<"IsUint(\"65536\"): " <<IsUint("65536") <<endl;
  cout <<"IsUint(\"65535\"): " <<IsUint("65535") <<endl;
  cout <<"IsUint(\"75535\"): " <<IsUint("75535") <<endl;
  cout <<"IsUint(\"66535\"): " <<IsUint("66535") <<endl;
  cout <<"IsUint(\"-0xFFFF\"): " <<IsUint("-0xFFFF") <<endl;
  cout <<"IsUint(\"0xFFFF\"): " <<IsUint("0xFFFF") <<endl;
  cout <<"IsUint(\"-1\"): " <<IsUint("-1") <<endl;
  cout <<"IsLong(\"2147483648\"): " <<IsLong("2147483648") <<endl;
  cout <<"IsLong(\"-2147483648\"): " <<IsLong("-2147483648") <<endl;
  cout <<"IsUlong(\"4294967295\"): " <<IsUlong("4294967295") <<endl;
#endif

#if STRUTIL_DEBUG_SUBST
  size_t Count_;

  strcpy(buffer, "supercalifragalistic");
  Count_ = CountChar(buffer, "a");
  cout <<Count_ <<endl;
  assert(Count_ == 3);

  cout <<SubstnChar(buffer, "i", 'Y', 2, strlen(buffer)) <<endl;
  assert(!strcmp(buffer, "supercalifragalYstYc"));
  cout <<SubstnChar(buffer, "a", 'O', 2) <<endl;
  assert(!strcmp(buffer, "supercOlifrOgalYstYc"));
  cout <<SubstnChar(buffer, "c", 'K', 2, 5) <<endl;
  assert(!strcmp(buffer, "superKOlifrOgalYstYK"));
  cout <<SubstnChar(buffer, "u", 'T', 2, 2) <<endl;
  assert(!strcmp(buffer, "superKOlifrOgalYstYK"));
  cout <<StrReverse(buffer) <<endl;
  assert(!strcmp(buffer, "KYtsYlagOrfilOKrepus"));
#endif

#if STRUTIL_DEBUG_MISCFUNC
  char* StrPtr_;

  StrPtr_ = MakeCharSet(isspace);
  cout <<"strlen(StrPtr_) == " <<strlen(StrPtr_) <<endl;
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);
  StrPtr_ = MakeCharSet(iscntrl);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);
  StrPtr_ = MakeCharSet(isdigit);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);
  StrPtr_ = MakeCharSet(isxdigit);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);  
  StrPtr_ = MakeCharSet(islower);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);
  StrPtr_ = MakeCharSet(isupper);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);
  StrPtr_ = MakeCharSet(isalpha);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);    
  StrPtr_ = MakeCharSet(ispunct);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);  
  StrPtr_ = MakeCharSet(isgraph);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);  
  StrPtr_ = MakeCharSet(isprint);
  cout <<StrPtr_ <<endl;
  ::DeleteArray(StrPtr_);  

  StrPtr_ = MakeCharSet(isalnum);
  StrPtr_ = NewString(StrPtr_);
  cout <<StrPtr_ <<endl;
  StrReverse(StrPtr_);
  cout <<StrPtr_ <<endl;

  ResizeString(StrPtr_, strlen(StrPtr_) + 20);
  strcat(StrPtr_, " *** This is it *** ");
  cout <<StrPtr_ <<endl;
  EraseString(StrPtr_);

  StrPtr_ = NewString("10inchnail");
  StrFill(StrPtr_, 'o');
  cout <<StrPtr_ <<endl;
  EraseString(StrPtr_);
#endif

#if STRUTIL_DEBUG_REMOVAL
  char* TrailSpace_ = "Trailing Spaces17          ";
  char* LeadSpace_ =  "           Leading Spaces16";
  char* PadSpace_ =   "          Padding Spaces16           ";
  char* CharSpace_ =   " 1   2    3     4      5       6     7    8   9 ";

  RemoveTrailing(TrailSpace_, " ");
  RemoveLeading(LeadSpace_, " ");
  RemovePadding(PadSpace_, " ");
  RemoveChar(CharSpace_, " ");

  cout <<TrailSpace_ <<", " <<strlen(TrailSpace_) <<endl
       <<LeadSpace_ <<", " <<strlen(LeadSpace_) <<endl
       <<PadSpace_ <<", " <<strlen(PadSpace_) <<endl
       <<CharSpace_ <<", " <<strlen(CharSpace_) <<endl;
#endif

#if STRUTIL_DEBUG_STRCMP
  cout <<StriComp("greater", "LESSER") <<endl; // -5
  cout <<StriComp("lesser", "greater") <<endl; // 5
  cout <<StriComp("EQUAL", "equal") <<endl; // 0

  cout <<StrniComp("greater", "lesser", 4) <<endl; // -5
  cout <<StrniComp("lesser", "GREATER", 5) <<endl; // 5
  cout <<StrniComp("EQUAL", "equal", 6) <<endl; // 0
#endif

#if STRUTIL_DEBUG_READSTRING
//  system("clear");
  ofstream Fout("StrutilTest.txt", ios::out);
  Fout <<"testing 123 testing 456 testing 789\nNextLine 123 NextLine 456 NextLine 789;";
  Fout.close();
  ifstream Fin("StrutilTest.txt", ios::in);
  int Result_;
  char* Buffer = new_char_array(5, NULL);
  int Size_ = 5;
  char* Buffer2 = new_char_array(5, NULL);
  int Size2_ = 5;
  char* Buffer3 = new_char_array(5, NULL);
  int Size3_ = 5;
  
  Buffer = ReadStringUntil(Fin, Buffer, Size_, '\n');
  cout <<"buf = " <<Buffer <<endl;
  cout <<"size = " <<Size_ <<endl;
  assert(strcmp(Buffer, "testing 123 testing 456 testing 789") == 0);
  assert(!Fin.eof());  
  Buffer = ReadStringUntil(Fin, Buffer, Size_, '\n');
  cout <<"buf = " <<Buffer <<endl;
  cout <<"size = " <<Size_ <<endl;
  assert(strcmp(Buffer, "NextLine 123 NextLine 456 NextLine 789;") == 0);
  assert(Fin.eof());
  cout <<endl <<endl;
  Fin.clear();  
  Fin.close();

  Fin.open("StrutilTest.txt", ios::in);
  Buffer2 = ReadStringUntil(Fin, Buffer2, Size2_, ';');
  cout <<"buf2 = " <<Buffer2 <<endl;
  cout <<"size2 = " <<Size2_ <<endl;
  assert(strcmp(Buffer2, "testing 123 testing 456 testing 789\nNextLine 123 NextLine 456 NextLine 789") == 0);
  assert(!Fin.eof());
  Fin.close();

  Fin.open("StrutilTest.txt", ios::in);
  Result_ = FALSE;
  cout <<"buf3 = ";
  
  while (!Result_)
  {
    Result_ = ReadStringPart(Fin, Buffer3, Size3_, ';', Result_);
    cout <<Buffer3;
  }

  cout <<endl;
  Fin.close();
  Fin.open("StrutilTest.txt", ios::in);
  Result_ = FALSE;
  cout <<"buf3 = ";
  
  while (!Result_)
  {
    Result_ = ReadStringPart(Fin, Buffer3, Size3_, '\n', Result_);
    cout <<Buffer3;
  }

  cout <<endl;  
  ::DeleteArray(Buffer);
  ::DeleteArray(Buffer2);
  ::DeleteArray(Buffer3);
//  unlink("StrutilTest.txt");
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




