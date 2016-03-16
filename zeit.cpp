#include "zeit.h"

t_time::t_time(void)
  {
  allowed_difference= 3;
  } //t_time

int t_time::minutes_of_week(void)
  { //returns minutes since monday 00:00
  // Weekday definition in DCF77: Sunday= 7 .... Saturday= 6    <--
  // (Weekday definition in OS/2:  Sunday= 0 .... Saturday= 6)
  int sw;
  if ((!wintertime) && summertime) sw = 1;  //  Sommerzeit beachten
  else                             sw = 0;  //  Winterzeit beachten
  return weekday*24*60 + hour*60 + minute + sw;
  } //minutes_in_week

int t_time::operator == (t_time right)
  {
  int l,r;

  l= minutes_of_week();  //left
  if (l < allowed_difference) l+= 6*24*60;  //Umbruch am Wochenanfang (Samstag->Sonntag) beruecksichtigen
  r= right.minutes_of_week();
  if (r < allowed_difference) r+= 6*24*60;  //Umbruch am Wochenanfang (Samstag->Sonntag) beruecksichtigen
  if (  (((l-r) < allowed_difference) && ((l-r) >= 0))
     || (((r-l) < allowed_difference) && ((r-l) >= 0)) ) return 1; //Abweichung im Rahmen
  else  return 0;  //Abweichung zu hoch
  } //overload ==

int t_time::operator < (t_time right)
  {
  int l,r;

  l= minutes_of_week();  //left
  if (l < allowed_difference) l+= 6*24*60;  //Umbruch am Wochenanfang (Samstag->Sonntag) beruecksichtigen
  r= right.minutes_of_week();
  if (r < allowed_difference) r+= 6*24*60;  //Umbruch am Wochenanfang (Samstag->Sonntag) beruecksichtigen
  if (  ((r-l) < allowed_difference) && ((r-l) > 0)) return 1; //Abweichung im Rahmen
  else  return 0;  //Abweichung zu hoch
  } //overload <

t_time t_time::operator + (int i)      //add integer
  {
  t_time uebergabe;
  uebergabe= (*this);
  uebergabe.minute+= i;
  return uebergabe;
  } //
