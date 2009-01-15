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
Regexpr class member functions.

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
#include "exprclas.h"

#define ELEM_POINT  0
#define ELEM_BRACKET 1
#define ELEM_PARENTHESIS 2
#define NULLPTR ((void*)0)

// Regexpr a = s;
Regexpr::Regexpr(const Regexpr& s)
{
    pattern = s.pattern;
    start_ptr = end_ptr = line = s.line;
    eol = s.eol;
}

// Regexpr a(...,...)
Regexpr::Regexpr(char *new_pattern, char *new_line, unsigned char new_eol)
{
    pattern = new_pattern;
    start_ptr = end_ptr = line = new_line;
    set_eol(new_eol);       // set eol, if $ff search for 0,$d or $a
}

// sets a new pattern (restore start/end_ptr)
void Regexpr::set_pattern(char *new_pattern)
{
    pattern = new_pattern;
    start_ptr = end_ptr = line;
}

// sets a new line to search in (restore start/end_ptr)
void Regexpr::set_line(char *new_line, unsigned char new_eol)
{
    line = new_line;
    start_ptr = end_ptr = line;
    set_eol(new_eol);
}

// set eol instance variable (if $ff then search for 0,$a or $d)
void Regexpr::set_eol(unsigned char new_eol)
{
  char *ptr;
  char c;

    if (new_eol == 0xff) {  // search
        ptr = line;
        while ( ((c = *ptr++) != 0)  && (c != '\n') && (c != '\r') )
            ;
        eol = c;
    }
    else
        eol = new_eol;
}

// find first occurence of pattern in line OR NULLPTR if none
int Regexpr::find_first(char **start, char **end)
{
    start_ptr = end_ptr = line;
    return(search(start,end));
}

// find next occurence OR NULLPTR
int Regexpr::find_next(char **start,char **end)
{
    if (*start_ptr == eol)
        return 0;
    end_ptr = ++start_ptr;  // position start/end_ptr to next character
    return(search(start,end));
}

// find next occurence but begin search at end of the previous occurence
int Regexpr::find_next_skipcurrent(char **start,char **end)
{
    if (*end_ptr == eol)
        return 0;
    end_ptr = start_ptr = end_ptr+1;    // end of previous occurence
    return(search(start,end));
}


// find first occurence of pattern in line OR NULLPTR if none
int Regexpr::find_first(char *start)
{
  int err;

    start_ptr = end_ptr = line;
    if ((err = search()) != 0)
        last_occurence(start);
    return err;
}

// find next occurence OR NULLPTR
int Regexpr::find_next(char *start)
{
  int err;

    if (*start_ptr == eol)
        return 0;
    end_ptr = ++start_ptr;  // position start/end_ptr to next character
    if ((err = search()) != 0)
        last_occurence(start);
    return err;
}

// find next occurence but begin search at end of the previous occurence
int Regexpr::find_next_skipcurrent(char *start)
{
  int err;

    if (*end_ptr == eol)
        return 0;
    end_ptr = start_ptr = end_ptr+1;    // end of previous occurence
    if ((err = search()) != 0)
        last_occurence(start);
    return err;
}

void Regexpr::last_occurence(char *start)
{
  size_t size;

    size = end_ptr-start_ptr+1;
    strncpy(start,start_ptr,size);
    start[size] = eol;                  // eol may be other than '\0'
}


// main function for searching the pattern
// handles the '|' meta symbol
// returns 0 or 1 and sets pointers
int Regexpr::search(char **start,char **end)
{
  char *cur_startptr;

    patptr = pattern;   // init. patptr
    ignore_case = 0;
    lineptr = start_ptr;

    if (_search(&cur_startptr) <= 0)
        return 0;

    start_ptr = cur_startptr;
    end_ptr = lineptr-1;    // points on last character matching pattern
    if (start && end)
        last_occurence(start,end);  // set up start and end
    return 1;
}

// sub routine for searching current pattern
// is recursive
// tests for pattern by testing one meta char after the other
// o if firstmatch == 0 then if first char of lineptr doesnt match pattern
//   return failure
// o  returns pointer to first match in firstmatch and
//    -1 (failure) or 2 (success)
int Regexpr::_search(char **firstmatch)
{
  char *oldlineptr,*oldpatptr,*prevpatptr;
  int err,index;

    index = 0;
    oldpatptr = patptr;
    oldlineptr = lineptr;
    if (firstmatch)
        *firstmatch = lineptr;

    while (1) {
        prevpatptr = patptr;
        if ((err = check_next()) == 0) {    // pattern element not found
            if (!firstmatch || (*(lineptr-1) == eol)) {
                // fixed_mode or end of line reached -> failed (forever)
test_next:
                patptr = prevpatptr;
                if ((oldpatptr = scan_behind('|')) == NULLPTR)
                    return -1;
                index = -1;
                oldpatptr++;    // skip |
            }
            patptr = oldpatptr;
            lineptr = oldlineptr+(++index); // -> try with next character
            if (firstmatch)
                *firstmatch = lineptr;
            continue;
        }
        // -1 is returned a) when syntax error (patptr-1 == 0) occurred
        // b) pattern impossible
        if (err == -1) {    // if for some reason -1 came back
            if (*(patptr-1) == 0)   // try next pattern if possible
                return -1;  // not possible because illegal end
            goto test_next;
        }
        if (err == 2)       // return success
            return 2;
    }
}


// checks if the next pattern element is fulfilled (and positions pointers)
// return:
// -1 - not fulfilled in whole line, so dont try again!
//      used when reached eol or syntax error
//  0 - not fulfilled, line doens't contains current pattern element
//      in this case lineptr must point to the last_read_char+1
//      because it will be tested for eol: if (*(lineptr-1) == eol) return-1;
//  1 - fulfilled
//  2 - pattern end reached -> search posivtive!
int Regexpr::check_next(void)
{
  char pc,c;

    switch (pc = next_pat_elem()) {
        case '|':
        case 0:     // end of pattern
            return 2;

        case '$':   // end of line?
            if (*lineptr == eol)
                return 1;
            lineptr++;  // ret 0 -> position to last_read+1
            return 0;

        case '^':   // start of line?
            if (lineptr == line)
                return 1;
            return -1;  // pattern doesnt match line!

        case '\\':
            switch ((pc = next_pat_elem())) {

                case 'O':   // O stands for o and O because
                            // next_pat_elem executes toupper!!!
                    if (*(patptr-1) == 'O') // ignore
                        ignore_case = 1;
                    else
                        ignore_case = 0;
                    return 1;
                case 'o':
                    ignore_case = 0;
                    return 1;

                case '<':   // at the beginning of a word?
                    // if current character is alnum and beginning of line or
                    // previous is a space or a punctation char
                    if ( isalnum(*lineptr) &&
                        ( (lineptr == line) || isspace(*(lineptr-1)) ||
                            ispunct(*(lineptr-1)) ))
                        return 1;
                    lineptr++;  // ret 0 -> position to last_read+1
                    return 0;

                case '>':   // at the end of a word?
                    // if current character is eol or space or punctation and
                    // previous was alnum
                    if ( (lineptr != line) && isalnum(*(lineptr-1)) &&
                        ( (*lineptr == eol) || isspace(*lineptr) ||
                          ispunct(*lineptr) ))
                        return 1;
                    lineptr++;  // ret 0 -> position to last_read+1
                    return 0;

                default:
                    if (pc >= '0' && pc <= '7') {   // octal (3 digits)
                        pc = (pc-'0')*64+
                            (patptr[0]-'0')*8+
                            (patptr[1]-'0');
                        patptr += 2;
                    }
                    else
                        switch (toupper(pc)) {
                            case 'X':   // next two digit hex code
                                pc = (translate_hex(*patptr)<<4)+
                                        translate_hex(patptr[1]);
                                patptr += 2;
                                break;
                            case 'B':   // backspace
                                pc = '\b';
                                break;
                            case 'F':   // form feed
                                pc = '\f';
                                break;
                            case 'N':   // new line
                                pc = '\n';
                                break;
                            case 'R':   // carriage return
                                pc = '\r';
                                break;
                            case 'V':   // vertical tab
                                pc = '\v';
                                break;
                            case 'A':   // bell
                                pc = '\a';
                                break;
                            case 'T':   // tabulator
                                pc = '\t';
                                break;
                        } // eo switch (toupper(pc))
                    goto do_default;
            } // eo switch (pc)
            break;


        case '\'':  // take sequence literal
            while (1) {
                if (!(pc = next_pat_elem()))    // illegal end of pattern
                    return -1;  // failed forever
                if (pc == '\'') // literal sequence matched!
                    return 1;
                if ((c = next_line_elem()) != pc)   // char doesnt match
                    return 0;
            }
            break;

        case '(':
            return handle_postfix(ELEM_PARENTHESIS);
        case '[':
            return handle_postfix(ELEM_BRACKET);
        case '.':
            return handle_postfix(ELEM_POINT);

        default:
do_default:
            if ( ((c = *patptr) == '?') || (c == '+') || (c == '*') )
                return handle_postfix(pc);
            if ((c = next_line_elem()) != pc)   // doesnt match
                return 0;
            return 1;
    }

    return 1;
}

char Regexpr::next_line_elem(void)
{
  char c;

    c = *lineptr++;
    return (ignore_case ? toupper(c) : c);
}

char Regexpr::next_pat_elem(void)
{
  char pc;

  pc = *patptr++;
  return (ignore_case ? toupper(pc) : pc);
}

// test whether current [...] matches or not
// moves patptr and lineptr!
// returns -1 (line doesnt contain pattern!), 0 (failed) or 1 (success)
int Regexpr::test_field(void)
{
  char c,pc;
  int negate,from,to;

    if ((c = next_line_elem()) == eol)  // read next char from line
        return 0;

    if (*patptr == '^') {   // all but mentioned characters
        patptr++;
        negate = 1;
    }
    else
        negate = 0;

    while (1) {
        if (!(pc = next_pat_elem()))    // pattern end, is not allowed
            return -1;  // -> failed in whole line
        if (pc == ']')  // end of field
            return (negate ? 1 : 0);    // good for [^], bad for []
        if (*patptr == '-') {   // char region
            from = pc;  // region from
            patptr++;   // overread '-' symbol
            to = next_pat_elem();   // region to
            if ((c >= from) && (c <= to)) // test if inside region
                if (negate)
                    return 0;   // failed
                else
                    break;
        }
        else
            if (pc == c) {  // char matches
                if (negate) // if [^...] this situation is bad
                    return 0;   // ->failed
                else
                    break;
            }
            // dont react (go on to next char) if current char doesnt match
    }
    // a char matched, so position patptr to the end of the field [...]
    if ((patptr = strchr(patptr,']')) == (void*)0)
        return -1;  // no ], so stop immediately
    patptr++;   // position behind ]
    return 1;
}


// position on next <symbol> found
// handles \ and ' and [] and recursive use of ()
char *Regexpr::scan_behind(char symbol)
{
  char p;
  int literal,infield,count;

    literal = 0;
    infield = (symbol == ']') ? 1 : 0;
    count = (symbol == ')') ? 1 : 0;

    while (1)
        switch (p = *patptr++) {
            case 0:
                return ((char*)NULLPTR);
            case '\\':          // test for "\"
                if (!literal && !infield)
                    patptr++;
                break;
            case '\'':          // test for '
                if (!infield)
                    literal ^= 1;
                break;

            case '[':   // all inside [...] is taboo
                if (!literal)
                    infield = 1;
                break;
            case ']':
                if (literal)
                    break;
                infield = 0;
                goto test_found;

            case '(':
                if (!infield && !literal)
                    count++;
                break;

            case ')':
                if (infield  || literal)
                    break;
                count--;
                // goto test_found;

            default:
                if (!literal && !infield)
test_found:
                    if (p == symbol && !count)
                        return patptr-1;
        }
}

// handles postfix use by using recursive calls to _search
int Regexpr::handle_postfix(char kind)
{
  char *firstmatch,*oldlineptr,*oldpatptr,*lastlineptr;
  char *begin,*eo_parenthesis,*tmp;
  char mc,c;
  int n,i,err,old_ignore_case;

    if (kind == ELEM_BRACKET) {
        begin = patptr;
        if (!scan_behind(']'))  // go to next ] within pattern string
            return -1;
    }
    else if (kind == ELEM_PARENTHESIS) {
        begin = patptr;
        if ((eo_parenthesis = scan_behind(')')) == 0)
            return -1;
    }

    // Is postfix symbol (?+*) ?
    if ( ((mc = *patptr) == '?') || (mc == '+') || (mc == '*') ) {
        oldlineptr = lineptr;
        oldpatptr = ++patptr;   // skip postfix symbol
        lastlineptr = ((char*)NULLPTR);

        while (1) {
            old_ignore_case = ignore_case;
            err = _search(&firstmatch);
            ignore_case = old_ignore_case;
            if (err == 2) { // pattern element found

                n = firstmatch-oldlineptr;  // number of untested chars
                tmp = lineptr;

                switch (kind) { // for each joker do something special
                    case ELEM_POINT:
                        break;
                    case ELEM_BRACKET:
                        for (i = 0; i < n; i++) {
                            patptr = begin;
                            lineptr = oldlineptr+i;
                            if (test_field() <= 0) { // char doesnt match!
take_prev:
                                if (!lastlineptr) { // no prev match!!
                                    lineptr = oldlineptr;
                                    return 0;
                                }
                                else {
                                    lineptr = lastlineptr; // take prev match
                                    return 2;
                                }
                            }
                        }
                        break;
                    case ELEM_PARENTHESIS:
                        lineptr = oldlineptr;
                        n = 0;
                        while (lineptr < firstmatch) {
                            patptr = begin;
                            if (test_parenthesis_fixed(eo_parenthesis) == -1)
                                goto take_prev;
                            if (lineptr > firstmatch)
                                goto cont_search;
                            n++;
                        }
                        break;
                    default:    // character
                        for (lineptr = oldlineptr, i = 0; i < n; i++) {
                            if (kind != next_line_elem())
                                goto take_prev;  // char doesnt match
                        }
                        break;
                }
                lineptr = tmp;

                if (mc == '+' && !n) {  // n > 0 is ok
                    if (*firstmatch == eol) // we already reached eol
                        return -1;          // thus no success possible
                    goto cont_search;
                }
                else if (mc == '?' && n > 1)    // n = 0,1 is ok
                    goto take_prev;

                // *, + (n > 0), ? (n == 0,1)
                if (*firstmatch == eol) // test if eol reached
                    return 2;   // and if so indicate success

                // use current match as prev and continue searching
                lastlineptr = lineptr;
cont_search:
                lineptr = firstmatch+1; // we must look for biggest
                                        // possible match!!
                patptr = oldpatptr;
            }
            else // pattern didnt match
                if (!lastlineptr) { // no prev match!!
                    lineptr = oldlineptr;
                    return -1;
                }
                else {
                    lineptr = lastlineptr;  // take prev match
                    return 2;
                }
        } // end of while (1)

    } // end of if (is_joker
    else    // without postfix char
        switch (kind) {
            case ELEM_POINT:
                return ((c = next_line_elem()) == eol) ? 0 : 1;
            case ELEM_BRACKET:
                patptr = begin;
                return test_field();
            case ELEM_PARENTHESIS:
                patptr = begin;
                return test_parenthesis(eo_parenthesis);
        }

		return 0;
}

int Regexpr::test_parenthesis(char *eo_parenthesis)
{
  char *firstmatch,*oldlineptr;
  int err;

    oldlineptr = lineptr;
    *eo_parenthesis = 0;
    err = _search(&firstmatch); // patptr points behind $0 (former ) )
    *eo_parenthesis = ')';
    patptr = eo_parenthesis+1;
    if (err <= 0)
        return -1;
    if(oldlineptr != firstmatch)    // (...) must begin at former lineptr
        return 0;
    return 1;
}

// similiar to function above with the difference, that it tells _search
// to return -1 if first char of lineptr doesnt match pattern
int Regexpr::test_parenthesis_fixed(char *eo_parenthesis)
{
  char *oldlineptr;
  int err;

    oldlineptr = lineptr;
    *eo_parenthesis = 0;
    err = _search((char**)NULLPTR); // patptr points behind $0 (former ) )
    *eo_parenthesis = ')';
    patptr = eo_parenthesis+1;
    return err;
}


char Regexpr::translate_hex(char c)
{
    if (c >= '0' && c <= '9')
        return c-'0';
    if (c >= 'a' && c <= 'f')
        return c-'a'+10;
    if (c >= 'A' && c <= 'F')
        return c-'A'+10;
    return 0;
}

Regexpr& Regexpr::operator=(char *new_line) { // a = chrarray; (pattern =
                                                // default, eol = $ff)
  pattern = DEFAULT_PATTERN;
  start_ptr = end_ptr = line = new_line;
  set_eol(0xFF);       // set eol, if $ff search for 0,$d or $a
  
  return *this;
}
    
Regexpr& Regexpr::operator = (const Regexpr& s) { // a = b;
  if (this != &s)
  {
    pattern = s.pattern;
    start_ptr = end_ptr = line = s.line;
    eol = s.eol;
  }
    
  return *this;
}

#undef  ELEM_POINT
#undef  ELEM_BRACKET
#undef  ELEM_PARENTHESIS
#undef  NULLPTR





