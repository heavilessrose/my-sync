//#include "video.h"
#include "charset.h"

main()
{
   CharSet CS1('A','Z'), CS2(0, 25);
   ChrString s("^-");
   CharSet CS3(s);

//   clrscr();
   cout << "Set CS1 is " << CS1 << '\n';
   cout << "Set CS2 is " << CS2 << '\n';
   CS1.clear();
   cout << "Set CS1 is " << CS1 << '\n';
   CS1.mkalphanumeric();
   cout << "Set CS1 is " << CS1 << '\n';
   CS1.remove('a', 'z').add('y', 'z');
   cout << "Set CS1 is " << CS1 << '\n';
   CS1 += CS2;
   cout << "Set CS1 is " << CS1 << '\n';
   CS2 = CS1;
   cout << "Set CS2 is " << CS2 << '\n';
   CS2.remove(1, 4);
   CS2 += CS3;
   cout << "Set CS2 is " << CS2 << '\n';
   cout << "Press any key to end the program ...";
//   getch();
   return 0;
}
