#include <stdio.h>
#include <time.h>
int main(){

  struct tm * timeinfo;
 
 
  timeinfo->tm_year = 2016 - 1900;
  timeinfo->tm_mon = 2 - 1;
  timeinfo->tm_mday = 19;
  
   mktime ( timeinfo );

//  printf ("That day is a %s.\n", weekday[timeinfo->tm_wday]);
return 0;
}
