#ifndef BST_CPP
  #include "bst.cpp"
#endif
#ifndef LONESTRING_H
  #include "lonestr.h"
#endif

#include <iostream.h>

//=================================================================================
//
//-------------------------------------------------------------------------
//
//    TESTBST.CPP  --  Test driver for bst binary search tree class.
//
//    Written July 96   - N. Dadoun
//
//    Modification History:
//
//
//
//-------------------------------------------------------------------------
//
//
//=================================================================================

template <class KEY, class VALUE>
void applytest(BstNode<KEY, VALUE>*, ostream&);

int main()
{
   char pause;
   LoneString* StrPtr_;

   Bst<LoneString, LoneString>::SetComparisonFunction(CompLoneString);   
   Bst<LoneString, LoneString> dict;
   Bst<LoneString, LoneString> another;

   dict.Insert("715", "Nou");
   dict.Insert("720", "Vince");
   dict.Insert("30",  "Habib");
   dict.Insert("216", "Lorene");
   dict.Insert("316", "Pari");
   dict.Insert("740", "Michael");
   dict.Insert("730", "Pierre");
   dict.Insert("615", "Ardeshir");
   dict.Insert("615", "Ardeshir2");

   cout << "Finished Inserting:\n";

   dict.DumpNode(dict.Root(), cout);
   cout << "Type any character to continue:";
   cin >> pause;

   another = dict;
   another = another;
   another = dict;

   cout << "Finished assignment:\n";

   another.DumpNode(another.Root(), cout);
   cout << "Type any character to continue:";
   cin >> pause;

   dict.Del("216");
   dict.Del("316");
   dict.Del("740");
   dict.Del("999");
   dict.Del("715");
   dict.Del("740");

   cout << "Finished deleting:\n";

   dict.DumpNode(dict.Root(), cout);
   cout << "Type any character to continue:";
   cin >> pause;

   //  Note this does searching on the (another) tree without deletions.
   cout << "Key 715 - Value: " << ((StrPtr_ = another.SearchNode("715", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key  30 - Value: " << ((StrPtr_ = another.SearchNode("30", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 720 - Value: " << ((StrPtr_ = another.SearchNode("720", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 615 - Value: " << ((StrPtr_ = another.SearchNode("615", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 740 - Value: " << ((StrPtr_ = another.SearchNode("740", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 730 - Value: " << ((StrPtr_ = another.SearchNode("730", another.Root())) ? *StrPtr_:"(null)") << '\n';
   cout << "Key 999 - Value: " << ((StrPtr_ = another.SearchNode("999", another.Root())) ? *StrPtr_:"(null)") << "\n\n";

   cout << "Finished searching\n\n";

   cout << "Type any character to continue:";
   cin >> pause;

   another.InOrder(another.Root(), cout, applytest);

   cout << "Finished applying\n\n";

   cout << "Type any character to continue:";
   cin >> pause;

   cout << "Test complete (for now)\n\n";

   return 0;
}

//=================================================================================
//  This function is passed as a function pointer to the Bst function
//  doFunc and is recursively applied to each node in an inorder traversal
//  of the Bst tree.  Note that applytest could be redefined as any
//  function with the same argument types without altering the Bst structure
//  at all.
template <class KEY, class VALUE>
void applytest(BstNode<KEY, VALUE>* node, ostream& Os_)
{
   Os_ << "Key: " << node->key << "  Value: " << *node->value << endl;
}

