/****************************************************************************/
/*         		   Time / Date Library Module
*
* Library module contains time and date functions, date conversion,
* time conversion and global time and date variables and strings
*/
/****************************************************************************/
#ifndef TIMEDATE_H
#define TIMEDATE_H
/****************************************************************************/
/* library files */
#ifndef CGIDEFS_H
  #include "cgidefs.h"
#endif

/****************************************************************************/
typedef struct{
  int saved;

  char datestr[DATESTRLEN];

  char gmtimestr[TIMESTRLEN];          /* Greenwich Mean Time string  */
  char loctimestr[TIMESTRLEN];         /* local time string           */

  struct tm* local_time;               /* static local time structure */
  struct tm* mean_time; 	       /* static mean time structure  */

  char* zone;
} timedate_t;

/****************************************************************************/
/* object allocation procedure */
timedate_t* newtimedate(void);
timedate_t* resettimedate(timedate_t* tdp);
timedate_t* thistimedate(timedate_t* tdp);
timedate_t* givetimedate(void);
void freetimedate(void);

/* time procedures */
/* time and date retrieval procedures */
char* getmeantime(void);
char* getlocaltime(void);
char* getlocaltset(void);

/* time zone and daylight setting procedures */
void setzonestring(char* zone);
void settimezone(int hours);
void setdaylight(int dlflag);

/****************************************************************************/
#endif



