#include <stdio.h>
#include <iostream.h>
#include "clock.h"

int main(int argc, char** argv)
  {
  cout << "DCF77 decoder for Gude Expert Mouse Clock (OS/2)" << endl;
  cout << "Version 1.0 alpha (test & debug)" << endl;
  cout << "2002 by Markus Rosellen" << endl << endl;
  if (argc != 2)   //parameter count ok?
     {//parameter not ok
     cout << "Syntax: dcf77 {comX}" << endl;
     }//parameter not ok
  else
     { //parameter ok
     t_expert_mouse_clock uhr(argv[1]);
     t_system_clock_api   system_uhr;

     while(1)
       {
       uhr.get_time();
       system_uhr.get_time();
       if (uhr.actual_time == system_uhr.actual_time)
           {
           system_uhr.actual_time= uhr.actual_time;
           system_uhr.set_time();
           cout << "Uhrzeit gesetzt" << endl;
           }
       else cout << "!!!!!!!!!!Abweichung Systemzeit<->empfangene Zeit zu gross!!!!!!!!!!!!" << endl;
       cout << "Sommerzeit= " << (int) uhr.actual_time.summertime << endl;
       cout << "Winterzeit= " << (int) uhr.actual_time.wintertime << endl;
       cout << "weekday= " << (int) uhr.actual_time.weekday << endl;
       cout <<  (int) uhr.actual_time.calenderday << "-" <<
                (int) uhr.actual_time.calendermonth << "-" << (int) uhr.actual_time.calenderyear << "   ";
       cout <<  (int) uhr.actual_time.hour << ":" << (int) uhr.actual_time.minute << endl;

       //System Uhr einstellen
       }
    return 0;
    } //Parameter ok
  } //main
