/*TUG PDS CERT 1.01 (C++)

==========================================================================

                 TUG PUBLIC DOMAIN SOFTWARE CERTIFICATION

The Turbo User Group (TUG) is recognized by Borland International as the
official support organization for Turbo languages.  This file has been
compiled and verified by the TUG library staff.  We are reasonably certain
that the information contained in this file is public domain material, but
it is also subject to any restrictions applied by its author.

This diskette contains PROGRAMS and/or DATA determined to be in the PUBLIC
DOMAIN, provided as a service of TUG for the use of its members.  The
Turbo User Group will not be liable for any damages, including any lost
profits, lost savings or other incidental or consequential damages arising
out of the use of or inability to use the contents, even if TUG has been
advised of the possibility of such damages, or for any claim by any
other party.

To the best of our knowledge, the routines in this file compile and function
properly in accordance with the information described below.

If you discover an error in this file, we would appreciate it if you would
report it to us.  To report bugs, or to request information on membership
in TUG, please contact us at:

             Turbo User Group
             PO Box 1510
             Poulsbo, Washington USA  98370

--------------------------------------------------------------------------
                        F i l e    I n f o r m a t i o n

* DESCRIPTION
Definition of the Regexpr class.

* ASSOCIATED FILES
EXPRCLAS.DOC
EXPRCLAS.CPP
EXPRDEMO.CPP
EXPRCLAS.HPP
EXPRDEMO.MAK
EXPRDEMO.EXE

* CHECKED BY
TAG - 11/30/91

* KEYWORDS
PROGRAM C++ REGULAR EXPRESSION MATCH SEARCH CLASS
==========================================================================
}*/
/*---------------------------------------------------------------------------
                   Regular Expressions (Zortech) C++ Class V1.0b
                                       &
                                  Documentation

                                  (c) 1990 by
                                Martin Backschat
                                   Bergstr. 16
                                  8071 Hepberg
                                  West Germany

                                  Email Access:
                    Zortech BBS England: Martin Backschat
                        FIDO: 2:246/11 Martin Backschat
                                    BIX: maba
                    Maus M2,M3,K”ln,Starnberg: Martin Backschat


Annotations:
             o  Developped & testet with Zortech C++ 2.1
             o  Please excuse my strange english -- but I am only a German!



    Meta characters:

    postfix characters (comes after a comparable pattern)
    ------------------
    matches n >= 0                              *
    matches n > 0                               +
    matches n = 0,1                             ?

    comparable patterns:
    -------------------
    any single character                        .
    a character of the enumeration              [...]
    a character of the region...                [a-e]
    a character of the regions...               [a-eh-x]
    all characters but...                       [^...]
    matches literally taken char                \C
    matches hex value                           \xXX
    matches ocatal value                        \OOO
    macthes bell,backspc,ffeed,nl,tab,cr,vtab   \a,\b,\f,\n,\t,\r,\v
    no interpretation                           '...'
    pattern block (for *+?)                     (...)

    conditional expressions:
    ------------------------
    OR (give choice of several patterns)        ...|...

    status:
    ------
    string at the beginning of the line         ^...
    string at the end of the line               ...$
    string at the beginning of a word           \<...
    string at the end of a word                 ...\>

    flags:
    -----
    case sensitivity off                        \O
    case sensitivity on                         \o

-- Modified by:         Joseph Wong
-- Modification date:   August 8 1999
---------------------------------------------------------------------------*/
#ifndef REGEXPR
#define REGEXPR

#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif

#define DEFAULT_PATTERN ".*"    // default is all

class Regexpr
{
  private:
    // private instance variables
    char *pattern;              // current search pattern
    char *line;                 // line to search in
    unsigned char eol;          // eol character ($0 or $a)
                                // if $ff instance will search for 0,$d,$a
    char *patptr,*lineptr;      // temporary used for search
    char *start_ptr,*end_ptr;   // used for find_first/next
    int ignore_case;            // if set, ignore case

    // private instance functions

    int search(char **start=NULL,char **end=NULL);  // find pattern in line
    int _search(char **firstmatch);     // subroutine (recursive)
    inline int check_next(void);        // test next element of pattern
    int handle_postfix(char kind);      // handles the postfix character
                                        // recognition
    int test_field(void);               // tests if current [...] matches
    inline int test_parenthesis(char *eo_parenthesis);
                                        // test if block matches
    inline int test_parenthesis_fixed(char *eo_parenthesis);
                                        // test if block starts at lineptr
    char *scan_behind(char symbol);     // scan behind closesymbol
    char translate_hex(char c);         // returns 0-15
    inline char next_line_elem(void);   // as: return *lineptr++;
    inline char next_pat_elem(void);    // as: return *patptr++;

public:
    // constructors
    Regexpr(const Regexpr&);                  // Regexpr a = b;
    Regexpr(char *new_pattern = DEFAULT_PATTERN ,
            char *new_line = "",unsigned char new_eol = 0xff);
                                        // Regexpr a;
                                        // Regexpr a("test.*",chrarray,0);

    // instance methods

    void set_pattern(char *pattern);    // a.set_pattern("[Hh]allo.*");
    void set_line(char *line, unsigned char new_eol = 0xff);
                                            // a.set_line(new_chrarray);
    void set_eol(unsigned char new_eol);    // a.set_eol($ff) ->
                                            // search for 0,'\n','\c'

    inline const char *get_pattern(void) const  // printf("\nPattern: %s",
    {                                           // a.get_pattern());
        return pattern;
    }
    
    inline const char *get_line(void) const     // printf("\nLine: %s",a.get_line());
    {
        return line;
    }
    
    inline unsigned char get_eol(void) const    // printf("\nEol: %c",a.get_eol());
    {
        return eol;
    }

    // functions returning pointers to occurrences

    int find_first(char **start=NULL, char **end=NULL);
                                            // find first occurrence (1) OR 0
    int find_next(char **start=NULL, char **end=NULL);
                                            // find next occurrence OR 0
    int find_next_skipcurrent(char **start=NULL, char **end=NULL);
      // find next occurence but begin searching at the end of last occurence
    inline void last_occurence(char **start, char **end) {
                                            // returns current_ptr
        *start = start_ptr;
        *end = end_ptr;
    }

    // functions copying occurrences

    int find_first(char *start);    // find first occurence (1) OR 0
    int find_next(char *start); // find next occurence OR 0
    int find_next_skipcurrent(char *start);
      // find next occurence but begin searching at the end of last occurence
    void last_occurence(char *start);   // returns current_ptr

    // overloaded operators
    Regexpr& operator = (char *new_line);       // a = chrarray; (pattern =
    Regexpr& operator = (const Regexpr& st);    // a = b;
};

#endif




