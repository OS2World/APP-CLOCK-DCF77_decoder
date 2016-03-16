/*          Time Datastructur      */
#ifndef _ZEIT_H_
#define _ZEIT_H_
class t_time
  {
  public:
  int summertime; // bool
  int wintertime; // bool
  int minute;
  int hour;
  int calenderday;
  int weekday;
  int calendermonth;
  int calenderyear;
  int correct;    //bool  True if Time is correct
  int minutes_of_week(void);
  int operator == (t_time right);  //compare operator
  int operator < (t_time right);  //compare operator
  t_time operator + (int i);      //add integer
  int allowed_difference;          //allowed_difference of the compare operator in seconds
  t_time(void);
  };

#endif
