#ifndef TCONSOLE_H
#define TCONSOLE_H

// Common console functions translated from
// DOS to UNIX, UNIX to DOS
//
#if defined(__unix__) | defined(__linux__)
#  include <curses.h>

// DOS to UNIX
#  define INITCONSOLE initscr(); cbreak();
#  define CLEANUPCONSOLE endwin();
#  define CLRSCR clear
#  define GOTOXY(x, y) move(y - 1, x - 1)
#  define CLREOL clrtoeol
#  define GETCH getch
#  define PUTCH addch
#  define CPUTS printw
#  define WHEREX() getcurx(stdscr)
#  define WHEREY() getcury(stdscr)
#  define REFRESH refresh

// UNIX curses
#  define CLEAR clear
#  define MOVE(y, x) move(y, x)
#  define CLRTOEOL clrtoeol
#  define ADDCH addch
#  define GETCURX() getcurx(stdscr)
#  define GETCURY() getcury(stdscr)
#else
#  if defined(__DJGPP__) | defined(__TURBOC__)
#    include <conio.h>

// DOS conio
#    define INITCONSOLE
#    define CLEANUPCONSOLE
#    define CLRSCR clrscr
#    define GOTOXY(x, y) gotoxy(x, y)
#    define CLREOL clreol
#    define GETCH getch
#    define PUTCH putch
#    define CPUTS cputs
#    define WHEREX() wherex()
#    define WHEREY() wherey()
#    define REFRESH

// UNIX to DOS
#    define CLEAR clrscr
#    define MOVE(y, x) gotoxy(x + 1, y + 1)
#    define CLRTOEOL clreol
#    define ADDCH putch
#    define GETCURX() wherex()
#    define GETCURY() wherey()
#  endif
#endif

#endif




