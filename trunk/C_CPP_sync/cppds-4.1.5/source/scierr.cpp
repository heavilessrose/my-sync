/*
modified
Example error class from the book Scientific and Engineering Programming
in C++: An Introduction with Advanced Techniques and Examples,
*/
#include <iostream.h>
#include "scierr.h"
#include "chrstring.h"


ostream& operator<<(ostream& s, const SciEngErr& e) { 
    return s << e.message();
}


SciEngErr::~SciEngErr() {}




