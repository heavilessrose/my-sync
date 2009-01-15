#ifndef MACROCON_H
#define MACROCON_H

#include <iostream.h>

#define CONVERT_RATIONAL            1
#define CONVERT_DATE                0
#define CONVERT_DOUBLE              0
#define CONVERT_FLOAT               0
#define CONVERT_INTEGER             0
#define CONVERT_LDOUBLE             0
#define CONVERT_LONGINT             0
#define CONVERT_LONGNUM             0
#define CONVERT_SHORTINT            0
#define CONVERT_TIMEOFDAY           0
#define CONVERT_ULONGINT            0
#define CONVERT_UNSIGNED            0
#define CONVERT_USHRTINT            0
#define CONVERT_YEAR                0
#define CONVERT_YMONTH              0

/****************************************************************************/
class MacroConverter
{
  public:
    static void Convert(const char* MacroName, const char* Host);
    static void Convert(const char* MacroName, const char* Prefix,
                        const char* Host, const char* Target=NULL);
};

/****************************************************************************/
#endif
