#include <iostream.h>
//#include "video.h"
#include "btree.cpp"

const int MAX_STRING = 12;

class LString
{
   protected:
     char str[MAX_STRING];

   public:
     LString()
       { memset(str, 0, MAX_STRING); }
     LString(const char* s);
     LString(const LString& s)
       { strcpy(str, s.str); }
     char* getstr()
       { return str; }
     unsigned getLen()
       { return strlen(str); }

     LString& operator =(LString& s);
     char& operator[](unsigned index)
      { return str[index]; }

     int operator <(LString& s)
       { return strcmp(str, s.str) < 0; }
     int operator <=(LString& s)
       { return strcmp(str, s.str) <= 0; }
     int operator >(LString& s)
       { return strcmp(str, s.str) > 0; }
     int operator >=(LString& s)
       { return strcmp(str, s.str) >= 0; }
     int operator ==(LString& s)
       { return strcmp(str, s.str) == 0; }
     int operator !=(LString& s)
       { return strcmp(str, s.str) != 0; }

    friend ostream& operator <<(ostream& os, LString& s);
};

LString::LString(const char* s)
{
  if (strlen(s) < MAX_STRING)
    strcpy(str, s);
  else {
    memcpy(str, s, MAX_STRING - 1);
    str[MAX_STRING] = '\0';
  }
}

LString& LString::operator =(LString& s)
{
  strcpy(str, s.str);
  return *this;
}

ostream& operator <<(ostream& os, LString& s)
{
  os << s.str;
  return os;
}


void showTree(Btree<LString>& t, const char* msg)
{
  LString str;
  Boolean ok = t.getFirstNode(str);

//  clrscr();
  cout << msg
       << " (tree has " << t.getNumNodes() << " nodes)"
       << "\n\n";
  while (ok) {
    cout << str << '\n';
    ok = t.getNextNode(str);
  }
//  gotoxy(1, 23);
  cout << "Press any key ... ";
//  getch();
}

void searchNode(Btree<LString>& t, LString& s)
{
//  gotoxy(1, 23);
//  clreol();
//  gotoxy(1, 22);
  cout << "Press any key to search for " << s;
//  getch();
//  gotoxy(1, 22);
//  clreol();
  if (t.search(s))
    cout << "Found " << s << " in tree\n";
  else
    cout << "No match for " << s << '\n';
  cout << "Press any key to continue ... ";
//  getch();
}

void displayLink(unsigned n)
{
  if (n != BTREE_NIL)
    cout << '(' << n << ") ";
  else
    cout << "(NIL) ";
}

void showPages(Btree<LString>& t, const char* msg, LString& s)
// displays the tree nodes for a B-tree with no deleted nodes
{
   Bstruct<LString> page;
//   clrscr();
   cout << msg << s << '\n';
   for (unsigned i = 0; i < t.getNumNodes(); i++) {
     t.getPage(&page, i);
     cout << "Page " << i << " : ";
     for (unsigned j = 0; j <= page.count; j++) {
       if (j > 0)
         cout << page.data[j] << " ";
       displayLink(page.nodeLink[j]);
     }
     cout << '\n';
   }
   cout << "\n\nPress any key to continue";
//   getch();
}

main()
{
  Btree<LString> tree("tsbtree.dat");
  LString country[] = { "France",
                        "Sweden",
                        "Spain",
                        "Portugal",
                        "Switzerland",
                        "Belgium",
                        "Ireland",
                        "Italy",
                        "Monaco",
                        "Andora",
                        "Luxembourg",
                        "Germany",
                        "England",
                        "Greece",   // not inserted in tree
                        "Denmark"   // not inserted in tree
                       };
  unsigned count = sizeof(country) / sizeof(LString);
  unsigned testSize = count - 2;
  unsigned numDelete = count / 2;
  unsigned i;

  // insert the names of the countries
  for (i = 0; i < testSize; i++) {
    tree.insert(country[i]);
    showPages(tree, "After inserting ", country[i]);
  }

  // display B-tree
  showTree(tree, "Complete B-tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);

  for (i = 0; i < numDelete; i++) {
    tree.remove(country[i]);
    showPages(tree, "After deleting ", country[i]);
  }

   // display AVL-tree
  showTree(tree, "Trimmed B-tree is:");

  for (i = 0; i < count; i++)
    searchNode(tree, country[i]);

//  gotoxy(1, 22); clreol();
//  gotoxy(1, 23); clreol();
  tree.TextWrite(cout);

  return 0;
}
