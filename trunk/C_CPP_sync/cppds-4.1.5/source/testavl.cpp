#ifndef AVLTREE_CPP
  #include "avltree.cpp"
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
//=================================================================================

template <class KEY, class VALUE>
void applytest(BstNode<KEY, VALUE>*, ostream&);

int main()
{
   char pause;
   LoneString* StrPtr_;
   LoneString TestStr1_ = "Habib";
   LoneString TestStr2_ = "Pierre";
   LoneString* TestStr3_ = new LoneString("Lorene");
   LoneString* TestStr4_ = new LoneString("Ardeshir2");

   AvlTree<LoneString, LoneString>::SetComparisonFunction(CompLoneString);
   AvlTree<LoneString, LoneString> dict;
   AvlTree<LoneString, LoneString> another;

   dict.Insert("715", "Nou");
   dict.Insert("720", "Vince");
   dict.Insert("30",  &TestStr1_);
   dict.Insert("216", TestStr3_, DataStorageAttributes::DESTROY);
   dict.Insert("316", "Pari");
   dict.Insert("740", "Michael");
   dict.Insert("730", &TestStr2_);
   dict.Insert("615", "Ardeshir");
   dict.Insert("615", TestStr4_, DataStorageAttributes::DESTROY);

   cout << "Finished inserting:\n\n";
   dict.DumpNode(dict.Root(), cout);
   cout << "Type any character to continue:";
   cin >> pause;

   another = dict;
   another = another;
   another = dict;

   cout << "Finished assignment:\n\n";
   another.DumpNode(another.Root(), cout);
   cout << "Type any character to continue:";
   cin >> pause;

   dict.Del("216");
   dict.Del("316");
   dict.Del("740");
   dict.Del("999");
   dict.Del("715");
   dict.Del("740");

   cout << "Finished deleting:\n\n";
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
   assert(another.IsFound());   
   cout << "Key 999 - Value: " << ((StrPtr_ = another.SearchNode("999", another.Root())) ? *StrPtr_:"(null)") << "\n\n";
   assert(!another.IsFound());

   cout << "Finished searching\n\n";
   cout << "Type any character to continue:";
   cin >> pause;

   another.InOrder(another.Root(), cout, applytest);

   cout << "Finished applying\n\n";
   cout << "Type any character to continue:";
   cin >> pause;

   // Test binary IO methods
   AvlTree<LoneString, LoneString> destTree_;
   
   cout << "storing original tree to file\n\n";
   another.DumpNode(another.Root(), cout);   
   cout << "Type any character to continue:";
   cin >> pause;
   
   ofstream fout("testing.bin", ios::out|ios::binary);
   Write(fout, another);
   fout.close();
   ifstream fin("testing.bin", ios::in|ios::binary);
   Read(fin, destTree_);
   fin.close();

   cout << "Reading stored tree from file\n\n";
   destTree_.DumpNode(destTree_.Root(), cout);   
   cout << "Type any character to continue:";
   cin >> pause;

/// --- --- ---
   destTree_.PreOrder(destTree_.Root(), cout, applytest);
   cout << "PreOrder sequence of tree\n\n";
   cout << "Type any character to continue:";
   cin >> pause;

   PreOrderTreeWalker<LoneString, LoneString> Walker_(&destTree_);
   
   while (Walker_.Next())
   {
     applytest(Walker_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }

   Walker_.Reset();
   cout << "Resetting tree walker\n";
   cout << "Type any character to continue:";
   cin >> pause;

   while (Walker_.Next())
   {
     applytest(Walker_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }

/// --- --- ---
   destTree_.InOrder(destTree_.Root(), cout, applytest);
   cout << "InOrder sequence of tree\n\n";
   cout << "Type any character to continue:";
   cin >> pause;

   InOrderTreeWalker<LoneString, LoneString> Walker2_(&destTree_);
   
   while (Walker2_.Next())
   {
     applytest(Walker2_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }

   Walker2_.Reset();
   cout << "Resetting tree walker\n";
   cout << "Type any character to continue:";
   cin >> pause;

   while (Walker2_.Next())
   {
     applytest(Walker2_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }   
   
/// --- --- ---
   destTree_.PostOrder(destTree_.Root(), cout, applytest);
   cout << "PostOrder sequence of tree\n\n";
   cout << "Type any character to continue:";
   cin >> pause;

   PostOrderTreeWalker<LoneString, LoneString> Walker3_(&destTree_);
   
   while (Walker3_.Next())
   {
     applytest(Walker3_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }

   Walker3_.Reset();
   cout << "Resetting tree walker\n";
   cout << "Type any character to continue:";
   cin >> pause;

   while (Walker3_.Next())
   {
     applytest(Walker3_.Here(), cout);
     cout << "Type any character to go to next tree node:";
     cin >> pause;
   }   

   cout << "Test complete (for now)\n\n";

   return 0;
}

//=================================================================================
//  This function is passed as a function pointer to the bst function
//  doFunc and is recursively applied to each node in an inorder traversal
//  of the bst tree.  Note that applytest could be redefined as any
//  function with the same argument types without altering the bst structure
//  at all.
template <class KEY, class VALUE>
void applytest(BstNode<KEY, VALUE>* node, ostream& Os_)
{
   Os_ << "Key: " << node->key << "  Value: " << *node->value << '\n';
}

