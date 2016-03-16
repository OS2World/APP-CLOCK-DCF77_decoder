#include "os_api.h"

/* Serial api implementation*/

t_serial_api::t_serial_api(char* device)
  {
  // ----  Open COM-Device ---------
   {
   cout << "Trying to open " << device << "...";
   ULONG   usaction;  //Action taken by DosOpen
   if (DosOpen(device, &hcom, &usaction, 0,
                FILE_NORMAL,
                FILE_OPEN, OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE,
                (PEAOP2) NULL )
              )
      {
        cout << device << " not available or COM0x.SYS not loaded" << endl;
        exit(1);
      }
   else cout << "ok" << endl;
   }
   int rc;
   //  ---------- Set Baud Rate ----------
   {
   USHORT usBPS = 50;
   ULONG  ulParmLen = sizeof(usBPS);
   rc= DosDevIOCtl(hcom,            // Device Handle
                   IOCTL_ASYNC,     // Serial-device control
                   ASYNC_SETBAUDRATE,  //Sets Bits Rate
                   &usBPS,            //points at bit Rate
                   sizeof(usBPS),     //length
                   &ulParmLen,        //Size Of parameter packet
                   NULL,              //No data packet
                   0,                 //Maximum size of data packet
                   NULL);             //Size of data packet

   cout << "SetBaudRate rc= " <<  rc << endl;
   }
   // --------- Set Line format -------------------
   {
   struct {
           unsigned char DataBits;
           unsigned char Parity;
           unsigned char StopBits;
          }  usLineCtrl = {
                 8,    // 8 Bits
                 0,    // no patity
                 0 };  // 1 stop bit
   ULONG ulParmLen = sizeof(usLineCtrl);
   rc= DosDevIOCtl(hcom,               // Device Handle
                   IOCTL_ASYNC,        // Serial-device control
                   ASYNC_SETLINECTRL,  // Sets data format of com-port (bits, parity, stop-bits)
                   &usLineCtrl,             // points at bit Rate
                   sizeof(usLineCtrl),      // length
                   &ulParmLen,         // Size Of parameter packet
                   NULL,               // No data packet
                   0,                  // Maximum size of data packet
                   NULL);              // Size of data packet
   cout << "SetLineCtrl rc= " << rc << endl;
   }
   // --------- Set SetModemctrl (RTS=1 and DTR=0) -------------------
   {
   struct _MODEMSTATUS usModemStatus;
   usModemStatus.fbModemOn=    0x01;    // set DTR
   usModemStatus.fbModemOff=   0xfd;    // clear RTS

   unsigned short usPacket;
   usPacket= 0;
   unsigned long usPacketLen;

   ULONG ulParmLen = sizeof(usModemStatus);
   rc= DosDevIOCtl(hcom,            // Device Handle
                   IOCTL_ASYNC,     // Serial-device control
                   ASYNC_SETMODEMCTRL,  //Set Modem Control Lines
                   &usModemStatus,            //points at struct
                   sizeof(usModemStatus),     //length
                   &ulParmLen,        //Size Of parameter packet
                   &usPacket,              //No data packet
                   sizeof(usPacket),                 //Maximum size of data packet
                   &usPacketLen);             //Size of data packet
   cout << "SetModemctrl rc= " << rc  << endl;
   }
   // --------- Set DCB Info (Timeouts) -------------------
   {
   struct _DCBINFO
          usDCBinfo = {
                 500,    // 5 seconds write timeout
                 500,    // 5 seconds read timeout
                 0x0,    // flags1: complete no handshake
                 0x0,    // flags2: complete no handshake
                 0x3,    // flags2: Bitmask: 0000 0011
                         // * normal Read timeout processing
                         // * extended hardware buffering not changed
                 0x50,   // receive 0x50 on error, but this ist disabled
                 0x51,   // receive 0x51 on abort, but this ist disabled
                 0x00,   // XON  char, but ignored
                 0x00    // XOFF char, but ignored
                };  // 1 stop bit
   ULONG ulParmLen = sizeof(usDCBinfo);
   rc= DosDevIOCtl(hcom,               // Device Handle
                   IOCTL_ASYNC,        // Serial-device control
                   ASYNC_SETDCBINFO,
                   &usDCBinfo,             // points at bit Rate
                   sizeof(usDCBinfo),      // length
                   &ulParmLen,         // Size Of parameter packet
                   NULL,               // No data packet
                   0,                  // Maximum size of data packet
                   NULL);              // Size of data packet
   cout << "SetDCBinfo rc= " << rc << endl;
   }
   // --------- Set SetModemctrl (RTS=1 and DTR=0) -------------------
   // 2nd Time because at receive of 1st byte DTR is set 0 by hardware
   {
   struct _MODEMSTATUS usModemStatus;
   usModemStatus.fbModemOn=    0x01;    // set DTR
   usModemStatus.fbModemOff=   0xfd;    // clear RTS

   unsigned short usPacket;
   usPacket= 0;
   unsigned long usPacketLen;

   ULONG ulParmLen = sizeof(usModemStatus);
   rc= DosDevIOCtl(hcom,            // Device Handle
                   IOCTL_ASYNC,     // Serial-device control
                   ASYNC_SETMODEMCTRL,  //Set Modem Control Lines
                   &usModemStatus,            //points at struct
                   sizeof(usModemStatus),     //length
                   &ulParmLen,        //Size Of parameter packet
                   &usPacket,              //No data packet
                   sizeof(usPacket),                 //Maximum size of data packet
                   &usPacketLen);             //Size of data packet
   cout << "SetModemctrl rc= " << rc << endl;
   }
  } //Contstruktor

t_serial_api::~t_serial_api(void)
  {
  int rc;
  rc= DosClose(hcom);
  } //destructor

unsigned char t_serial_api::receive(void)
  {
  int rc;
  ULONG numread;
  unsigned char buff;
  rc= DosRead(hcom, &buff, sizeof(buff), &numread);
  return buff;
  } //receive


/* Ringbuffer Member function implementation  */

// member function   t_system_clock_api

t_system_clock_api::t_system_clock_api(void)
  {
  actual_time.correct= 0;  //time of internal data is not correct because not set
  } //t_system_clock_api

void t_system_clock_api::get_time(void)
  {
  int rc;
  DATETIME dt = {0};

  rc= DosGetDateTime(&dt);
  cout << "DosGetTime rc= " << rc <<endl;
  actual_time.minute= dt.minutes;
  actual_time.hour= dt.hours;
  actual_time.calenderday= dt.day;
  // Weekday definition in OS/2:  Sunday= 0 .... Saturday= 6
  // Weekday definition in DCF77: Sunday= 7 .... Saturday= 6
  if (dt.weekday == 0) actual_time.weekday= 7;
  else  actual_time.weekday= dt.weekday;
  actual_time.calendermonth= dt.month;
  actual_time.calenderyear= dt.year;
  actual_time.correct= 1;    //bool  True if Time is correct
  }//get_time

void t_system_clock_api::set_time(void)
  {
  int rc;
  DATETIME dt = {0};
  if (actual_time.correct)
     {
     if (actual_time.summertime) dt.timezone= 2*60;  //+0200 Hours (german summertime)
     if (actual_time.wintertime) dt.timezone= 1*60;  //+0100 Hours (german wintertime)
     else dt.timezone= 0;
     dt.hundredths= 0;
     dt.seconds= 0;
     dt.minutes= actual_time.minute;
     dt.hours= actual_time.hour;
     dt.day= actual_time.calenderday;
  // Weekday definition in OS/2:  Sunday= 0 .... Saturday= 6
  // Weekday definition in DCF77: Sunday= 7 .... Saturday= 6
     if (actual_time.weekday == 7) dt.weekday= 0;
     else dt.weekday= actual_time.weekday;
     dt.month= actual_time.calendermonth;
     dt.year= 2000 + actual_time.calenderyear;  // Nicht Jahr 3000 sicher!!
     rc= DosSetDateTime(&dt);
     cout << "DosSetTime rc= " << rc << endl;
     }
  }//set_time

