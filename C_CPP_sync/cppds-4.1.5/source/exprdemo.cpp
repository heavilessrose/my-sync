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
Demonstration program for the Regexpr class.  This program implements a
version of the UNIX grep program.  It asks for a filename to search and
a regular expression to search for, then prints all lines in the file that
match the given expression.

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
    This is a demonstration for using the Regexpr class in your own code.
    It asks for a file and a pattern and lists all occurrances of the
    pattern within the file. This is done in a line loop.
---------------------------------------------------------------------------*/

#include "exprclas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE    256     // max line width

main()
{
  Regexpr look;         // instance creation (line,pattern are default)
  FILE *h;
  char filename[40];
  char pattern[40];
  char line[LINE],space[LINE];
  char *s,*e;
  int lineno = 1,occurrences = 0;
  int size;

    // ask user for file and pattern
    printf("\nFile to search in: ");
    gets(filename);
    if ((h = fopen(filename,"r")) == NULL) {
        printf("\nCannot open %s.",filename);
        exit(-1);
    }

    printf("Pattern to search for: ");
    gets(pattern);
    if (*pattern)   // if pattern is empty, keep default pattern (.*)
        look.set_pattern(pattern);      // set the pattern to Regexpr instance look

    // now do grep!
    while (fgets(line,LINE,h)) {    // read in line from file!
        look.set_line(line);    // find eol-symbol automatically (here: $d)
        size = strlen(line);

        if (look.find_first(&s,&e)) // now find first occurrances in line
            do {
                occurrences++;
                memset(space,' ',(size_t)size);
                if (s == e) // same position
                    space[s-line] = '^';
                else {
                    space[s-line] = '<';
                    space[e-line] = '>';
                }
                space[size] = 0;
                printf("\n----- Line %d ----\n%s%s",
                        lineno,line,space);
            } while (look.find_next(&s,&e));    // search next occurrance in line
        lineno++;
    }

    fclose(h);
    printf("\n%d occurrences found.",occurrences);
}
