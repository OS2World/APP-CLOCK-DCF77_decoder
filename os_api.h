#ifndef _OS_API_H_
#define _OS_API_H_

#define INCL_DOSFILEMGR
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSDATETIME
#include <os2.h>

#define INCL_DOSDEVICES
#include <bsedos.h>        //DosDevIOCtl()
#include <bsedev.h>        //constants for DevIOCtl()

#include <stdlib.h>  //exit()
#include <iostream.h>   //cout

#include "zeit.h"

class t_serial_api
  {
  public:
    t_serial_api(char* device);     //contructor
    ~t_serial_api(void);    //destructor
    unsigned char receive(void);  //receive one byte from com-port
  protected:
    // option for DosOpen()
    HFILE    hcom;  //handle fuer den COM-Port
  }; //t_serial_api


class t_system_clock_api
  {
  public:
  t_time actual_time;
  t_system_clock_api(void);
  void get_time(void);    //puts actual system time in this->actual_time
  void set_time(void);  //setzt actual system time from this->actual_time
  };
#endif
