#include <iostream.h>
#include "loneerr.h"

ostream& operator << (ostream& s, const LoneErr& e)
{
    return s << e.message();
}

LoneErr::~LoneErr() {}




