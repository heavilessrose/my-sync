// String input/output library
//
#ifndef STRIO_CPP
#define STRIO_CPP
#ifndef STRIO_H
  #include "strio.h"
#endif

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Calls istream::getline and extracts the specified numbers of characters
//   from the input stream. Returns a boolean value to indicate whether the
//   extraction was terminated upon retrieving the delimiter character or
//   end of file character or if the extraction was terminated because the
//   input buffer was filled.
//
// PRE:
//   istream& In_	: The input stream
//   char* Dest_	: The input buffer
//   int Size_		: The size of the input buffer
//   char Delimiter_	: The delimiter character
//
// POST:
//   Returns TRUE if the extraction was terminated because a delimiter
//   character was retrieved. If the extraction was terminated because the
//   input buffer is full then ReadLine will return TRUE if end of file is
//   detected, otherwise FALSE is returned.
//
Boolean ReadLine(istream& In_, char* Dest_, int Size_, char Delimiter_)
{
  In_.getline(Dest_, Size_, Delimiter_);
  if (strlen(Dest_) == size_t(Size_ - 1))
    return In_.eof();

  return TRUE;
}

/****************************************************************************/
// PURPOSE:
//   Read a single character from the input stream, skipping over any
//   specified delimiter characters. If no delimiters are specified
//   (Delimiter_ == 0) then returns read character without any processing
//
// PRE:
//   istream& In_    : Input stream
//   char& Dest_     : The character to be stored
//   char Delimiter_ : The delimiter character to skip over.
//
// POST:
//   Single character is extracted and stored in Dest_. Any delimiter
//   characters encountered in reading from stream are ignored. Returns
//   the input stream In_.
//
istream& ReadChar(istream& In_, char& Dest_, char Delimiter_)
{
  if (Delimiter_)
  {
    do In_.get(Dest_);
    while (Dest_ == Delimiter_);
  }
  else
    In_.get(Dest_);

  return In_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to extract character from the input stream until either the
//   delimiter character is detected or end of file is detected. If the
//   input buffer is filled and end of file or the delimiter character has
//   not been retrieved then the function will read and throw away characters
//   until either the delimiter is read or end of file is detected.
//
// PRE:
//   istream& In_	: The input stream
//   char* Dest_	: The input buffer
//   int Size_		: The size of the input buffer
//   char Delimiter_	: The delimiter character
//
// POST:
//   Characters are extracted and placed into the input buffer until either
//   the delimiter character is read or end of file is detected. If the
//   input buffer is filled then character will be read and thrown away.
//   returns the number of character read not including the delimiter
//   character or end of file character.
//
size_t TruncLine(istream& In_, char* Dest_, int Size_, char Delimiter_)
{
  if (!ReadLine(In_, Dest_, Size_, Delimiter_))
    return (strlen(Dest_) + Ignore(In_, Delimiter_));

  return strlen(Dest_);
}

/****************************************************************************/
// PURPOSE:
//   Procedure to read and ignore characters from the specified input stream
//   until the delimiter character is retrieved or end of file is reached.
//
// PRE:
//   istream& In_	: The input stream
//   char Delimiter_	: The delimiter character
//
// POST:
//   Returns the number of characters read and ignored not including the
//   delimiter character or end of file character.
//
size_t Ignore(istream& In_, char Delimiter_)
{
  size_t Count_;
  for (Count_ = 0; !In_.eof() && In_.get() != Delimiter_; Count_++);
  return (Count_ + 1);
}

/****************************************************************************/
// PURPOSE:
//   Procedure to Center the given string in the given output stream.
//
// PRE:
//   ostream& Os_	: The given output stream
//   char* Str_		: The string to be centered
//   int LineLen_	: The length of the line
//
// POST:
//   The string is centered on the given output stream
//
ostream& Center(ostream& Os_, char* Str_, int LineLen_)
{
  int Fil_ = (LineLen_ - strlen(Str_));
  int Mid_ = Fil_ / 2;

  if (Mid_ > 0)
  {
    char* temps_ = new char[LineLen_ + 1];
    memset(temps_, ' ', Mid_);
    strcpy(&temps_[Mid_], Str_);
    size_t Len_ = strlen(temps_);
    memset(&temps_[Len_], ' ', Mid_);
    Len_ += Mid_;
    
    if (Fil_ % 2)
      temps_[Len_++] = ' ';

    temps_[Len_] = 0;
    Os_ <<temps_;
    delete[] temps_;
  }
  else
    Os_ <<Str_;

  return Os_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure for displaying the title of a program.
//   The title of a program consists of:
//
//		- The title string of the program centered on the line
//		- An underline of '-' characters of the same length as the
//		  the title string shown centered at the line below the title
//		- A blank line following the underline to separate from the
//		  rest of the program
//
// PRE:
//   ostream& Os_	: The output stream
//   char* Title_	: The title of the program
//   int LineLen_	: The length of the line
//
// POST:
//   The title of the program is displayed in the given output stream
//   centered on the line.
//
void ShowTitle(ostream& Os_, char* Title_, int LineLen_)
{
  char* Buf_ = new char[LineLen_];
  int Len_ = strlen(Title_);
  int Index_;

  for (Index_ = 0; Index_ < Len_; Index_++)
    Buf_[Index_] = '-';
  Buf_[Index_] = '\0';

  Center(Os_, Title_, LineLen_);
  Center(Os_, Buf_, LineLen_);

  delete[] Buf_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to print a message to a standard device, flushes standart output
//   stream and specified output stream.
//
// PRE:
//   msg : the message string
//   reference : reference string (Procedure name)
//
// POST:
//   The standard output stream is flushed then the given message is printed
//   in the given output stream then the given output stream is flushed.
//
void PrintMsg(ostream& Fp_, char* Msg_)
{
  flush(cout);
  Fp_ <<((Msg_ == NULL) ? "":Msg_);
  flush(Fp_);
}

/****************************************************************************/
// PURPOSE:
//   Procedure to print a message to the standard error device and
//   terminate the program with an error status.
//
// PRE:
//   msg : the message string
//   reference : reference string (Procedure name)
//
// POST:
//   The given message is printed in the standard error stream via PrintMsg
//   and the program is terminated with the given error status.
//
void ExitMsg(char* Msg_, int ExitCode_)
{
  PrintMsg(cerr, Msg_);
  exit(ExitCode_);
}

/****************************************************************************/
// PURPOSE:
//   Procedure to output into variable argument string given the
//   format specifier string. (Non-threadsafe)
//
// PRE:
//   char* Fmt_		: The format specifier string
//
// POST:
//   The arguments for the string is printed onto the static message buffer
//   and returned to the program.
//
char* MakeVstr(char *Fmt_, ...)
{
  va_list Args_;
  static char Msg_[200];

  va_start(Args_, Fmt_);
  vsprintf(Msg_, Fmt_, Args_);
  va_end(Args_);

  return Msg_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to output into variable argument string given the
//   format specifier string. (Non-threadsafe)
//
// PRE:
//   char* Fmt_		: The format specifier string
//
// POST:
//   The arguments for the string is printed onto the static message buffer
//   and returned to the program.
//
char* MakeVstrInBuf(char* Buf_, char *Fmt_, ...)
{
  va_list Args_;

  va_start(Args_, Fmt_);
  vsprintf(Buf_, Fmt_, Args_);
  va_end(Args_);

  return Buf_;
}

/****************************************************************************/
#if STRIO_DEBUG
int main()
{
  char Buffer_[5];

//  CLRSCR();
  
  for (;;)
  {
    cout <<ReadLine(cin, Buffer_, 5) <<"\t";
    cout <<Buffer_ <<endl;

    if (!strcmp(Buffer_, "000"))
      break;
  }

  ShowTitle(cout, "Hello World");
  ShowTitle(cout, "GoodbyeWorld");
  return 0;
}
#endif
/****************************************************************************/
#endif




