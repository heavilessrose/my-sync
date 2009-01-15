#include <ctype.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

const char* FileNames[92][2] =
{
  {"\"advforlp.h\"", "\"advforloop.h\""},
  {"\"arbntree.h\"", "\"arraybintree.h\""},
  {"\"arrclass.h\"", "\"arrayclass.h\""},
  {"\"bsercmp.h\"", "\"bltinseriescmp.h\""},
  {"\"darrbase.h\"", "\"dynarraybase.h\""},
  {"\"datastor.h\"", "\"datastore.h\""},
  {"\"datestuf.h\"", "\"datestuff.h\""},
  {"\"fileread.h\"", "\"filereader.h\""},
  {"\"hashtab.h\"", "\"hashtable.h\""},
  {"\"imgstack.h\"", "\"imagestack.h\""},
  {"\"objenv.h\"", "\"objenvelope.h\""},
  {"\"pdynarra.h\"", "\"ptrdynarray.h\""},
  {"\"polynomi.h\"", "\"polynomial.h\""},
  {"\"recbrows.h\"", "\"recbrowser.h\""},
  {"\"sercmp.h\"", "\"seriescmp.h\""},
  {"\"simparray.h\"", "\"simplearray.h\""},
  {"\"simplist.h\"", "\"simplelist.h\""},
  {"\"strbrows.h\"", "\"strbrowser.h\""},
  {"\"streamre.h\"", "\"streamreader.h\""},
  {"\"string.h\"", "\"chrstring.h\""},
  {"\"tempstrm.h\"", "\"tempstream.h\""},
  {"\"ushrtint.h\"", "\"ushortint.h\""},
  {"\"vlenfile.h\"", "\"varlengthfile.h\""},
  {"\"advforlp.cpp\"", "\"advforloop.cpp\""},
  {"\"arbntree.cpp\"", "\"arraybintree.cpp\""},
  {"\"arrclass.cpp\"", "\"arrayclass.cpp\""},
  {"\"bsercmp.cpp\"", "\"bltinseriescmp.cpp\""},
  {"\"darrbase.cpp\"", "\"dynarraybase.cpp\""},
  {"\"datastor.cpp\"", "\"datastore.cpp\""},
  {"\"datestuf.cpp\"", "\"datestuff.cpp\""},
  {"\"fileread.cpp\"", "\"filereader.cpp\""},
  {"\"hashtab.cpp\"", "\"hashtable.cpp\""},
  {"\"imgstack.cpp\"", "\"imagestack.cpp\""},
  {"\"objenv.cpp\"", "\"objenvelope.cpp\""},
  {"\"pdynarra.cpp\"", "\"ptrdynarray.cpp\""},
  {"\"polynomi.cpp\"", "\"polynomial.cpp\""},
  {"\"recbrows.cpp\"", "\"recbrowser.cpp\""},
  {"\"sercmp.cpp\"", "\"seriescmp.cpp\""},
  {"\"simparray.cpp\"", "\"simplearray.cpp\""},
  {"\"simplist.cpp\"", "\"simplelist.cpp\""},
  {"\"strbrows.cpp\"", "\"strbrowser.cpp\""},
  {"\"streamre.cpp\"", "\"streamreader.cpp\""},
  {"\"string.cpp\"", "\"chrstring.cpp\""},
  {"\"tempstrm.cpp\"", "\"tempstream.cpp\""},
  {"\"ushrtint.cpp\"", "\"ushortint.cpp\""},
  {"\"vlenfile.cpp\"", "\"varlengthfile.cpp\""},
  {"ADVFORLP_H", "ADVFORLOOP_H"},
  {"ARBNTREE_H", "ARRAYBINTREE_H"},
  {"ARRCLASS_H", "ARRAYCLASS_H"},
  {"BSERCMP_H", "BLTINSERIESCMP_H"},
  {"DARRBASE_H", "DYNARRAYBASE_H"},
  {"DATASTOR_H", "DATASTORE_H"},
  {"DATESTUF_H", "DATESTUFF_H"},
  {"FILEREAD_H", "FILEREADER_H"},
  {"HASHTAB_H", "HASHTABLE_H"},
  {"IMGSTACK_H", "IMAGESTACK_H"},
  {"OBJENV_H", "OBJENVELOPE_H"},
  {"PDYNARRA_H", "PTRDYNARRAY_H"},
  {"POLYNOMI_H", "POLYNOMIAL_H"},
  {"RECBROWS_H", "RECBROWSER_H"},
  {"SERCMP_H", "SERIESCMP_H"},
  {"SIMPARRAY_H", "SIMPLEARRAY_H"},
  {"SIMPLIST_H", "SIMPLELIST_H"},
  {"STRBROWS_H", "STRBROWSER_H"},
  {"STREAMRE_H", "STREAMREADER_H"},
  {"STRING_H", "CHRSTRING_H"},
  {"TEMPSTRM_H", "TEMPSTREAM_H"},
  {"USHRTINT_H", "USHORTINT_H"},
  {"VLENFILE_H", "VARLENGTHFILE_H"},  
  {"ADVFORLP_CPP", "ADVFORLOOP_CPP"},
  {"ARBNTREE_CPP", "ARRAYBINTREE_CPP"},
  {"ARRCLASS_CPP", "ARRAYCLASS_CPP"},
  {"BSERCMP_CPP", "BLTINSERIESCMP_CPP"},
  {"DARRBASE_CPP", "DYNARRAYBASE_CPP"},
  {"DATASTOR_CPP", "DATASTORE_CPP"},
  {"DATESTUF_CPP", "DATESTUFF_CPP"},
  {"FILEREAD_CPP", "FILEREADER_CPP"},
  {"HASHTAB_CPP", "HASHTABLE_CPP"},
  {"IMGSTACK_CPP", "IMAGESTACK_CPP"},
  {"OBJENV_CPP", "OBJENVELOPE_CPP"},
  {"PDYNARRA_CPP", "PTRDYNARRAY_CPP"},
  {"POLYNOMI_CPP", "POLYNOMIAL_CPP"},
  {"RECBROWS_CPP", "RECBROWSER_CPP"},
  {"SERCMP_CPP", "SERIESCMP_CPP"},
  {"SIMPARRAY_CPP", "SIMPLEARRAY_CPP"},
  {"SIMPLIST_CPP", "SIMPLELIST_CPP"},
  {"STRBROWS_CPP", "STRBROWSER_CPP"},
  {"STREAMRE_CPP", "STREAMREADER_CPP"},
  {"STRING_CPP", "CHRSTRING_CPP"},
  {"TEMPSTRM_CPP", "TEMPSTREAM_CPP"},
  {"USHRTINT_CPP", "USHORTINT_CPP"},
  {"VLENFILE_CPP", "VARLENGTHFILE_CPP"}
};

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
int ReadLine(istream& In_, char* Dest_, int Size_, char Delimiter_)
{
  In_.getline(Dest_, Size_, Delimiter_);
  if (strlen(Dest_) == size_t(Size_ - 1))
    return In_.eof();

  return 1;
}

/****************************************************************************/
char* StripLeadingSpaces(char* str)
{
  if (str)
    while (*str && isspace(*str))
      ++str;
  
  return str;
}

/****************************************************************************/
char* TrimSpaces(char* str)
{
  if (str)
  {
    str = StripLeadingSpaces(str);
  
    char* ptr = &str[strlen(str) - 1];
    while (ptr >= str && isspace(*ptr))
      *ptr-- = 0;
  }

  return str;  
}

/****************************************************************************/
const char* FindMatch(char* Ptr_)
{
  int i;
  for (i = 0; i < 92; i++)
    if (!strcmp(Ptr_, FileNames[i][0]))
      return FileNames[i][1];

  return NULL;
}

/****************************************************************************/
void CopyFile(const char* source, const char* target)
{
  int LineBufSize_ = 512;
  char* LineBuf_ = new char[LineBufSize_];

  ifstream Fin(source, ios::in);
  ofstream Fout(target, ios::out);

  if (Fin.good() && Fout.good())
    while (!Fin.eof())
    {
      ReadLine(Fin, LineBuf_, LineBufSize_, '\n');
      Fout <<LineBuf_ <<endl;
    }

  Fout.close();
  Fin.close();  
}

/****************************************************************************/
void ModifyFile(const char* target)
{
  int FileNameShown_;
  long linecount_ = 0;
  int LineBufSize_ = 512;
  char* Ptr_;
  char* Needle_;
  char* LineBuf_ = new char[LineBufSize_];
  const char* replacement;
  long diff_;
  
  ifstream Fin(target, ios::in);
  ofstream Fout("tempfile.txt", ios::out);

  if (Fin.good() && Fout.good())
  {
    FileNameShown_ = 0;
    
    while (!Fin.eof())
    {
      ReadLine(Fin, LineBuf_, LineBufSize_, '\n');
      Ptr_ = LineBuf_;
      ++linecount_;

      if (Needle_ = strstr(Ptr_, "#ifndef"))
      {
        Ptr_ = Needle_ + 8;
        if (replacement = FindMatch(Ptr_))
        {
          if (!FileNameShown_)
          {
            cout <<"in file: " <<target <<endl;
            FileNameShown_ = 1;
          }          
          diff_ = Ptr_ - LineBuf_;
          cout <<"line " <<linecount_ <<": " <<LineBuf_ <<endl;
          LineBuf_[diff_] = 0;
          Fout <<LineBuf_ <<replacement <<endl;
          cout <<"replaced with: " <<LineBuf_ <<replacement <<endl;
        }
        else
          Fout <<LineBuf_ <<endl;
      }
      else if (Needle_ = strstr(Ptr_, "#include"))
      {
        Ptr_ = Needle_ + 9;
        if (replacement = FindMatch(Ptr_))
        {
          if (!FileNameShown_)
          {
            cout <<"in file: " <<target <<endl;
            FileNameShown_ = 1;
          }                
          diff_ = Ptr_ - LineBuf_;
          cout <<"line " <<linecount_ <<": " <<LineBuf_ <<endl;          
          LineBuf_[diff_] = 0;
          Fout <<LineBuf_ <<replacement <<endl;
          cout <<"replaced with: " <<LineBuf_ <<replacement <<endl;          
        }
        else
          Fout <<LineBuf_ <<endl;        
      }
      else if (Needle_ = strstr(Ptr_, "#define"))
      {
        Ptr_ = Needle_ + 8;
        if (replacement = FindMatch(Ptr_))
        {
          if (!FileNameShown_)
          {
            cout <<"in file: " <<target <<endl;
            FileNameShown_ = 1;
          }                
          diff_ = Ptr_ - LineBuf_;
          cout <<"line " <<linecount_ <<": " <<LineBuf_ <<endl;
          LineBuf_[diff_] = 0;
          Fout <<LineBuf_ <<replacement <<endl;
          cout <<"replaced with: " <<LineBuf_ <<replacement <<endl;
        }
        else
          Fout <<LineBuf_ <<endl;        
      }      
      else
        Fout <<LineBuf_ <<endl;
    }
  }

  Fout.close();
  Fin.close();

  CopyFile("tempfile.txt", target);
}

/****************************************************************************/
void ReadFileList()
{
  int LineBufSize_ = 512;
  char *Ptr_;
  char* LineBuf_ = new char[LineBufSize_];
  ifstream Fin;

  Fin.open("filelist.txt", ios::in);

  if (Fin.good())
    while (!Fin.eof())
    {      
      ReadLine(Fin, LineBuf_, LineBufSize_, '\n');

      if (Fin.eof())
        break;
      
      Ptr_ = LineBuf_;
      Ptr_ = TrimSpaces(Ptr_);
      ModifyFile(Ptr_);
    }

  delete[] LineBuf_;
  Fin.close();
}

/****************************************************************************/
int main()
{
  ReadFileList();
  return 0;
}
