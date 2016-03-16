Serial Expert Mouse Clock decoder 1.0 alpha "dcf77"
                 for OS/2


/WARNING/
  This software is not well tested and under development.
  No warranty of any kind. Use on your own risk.
  This software is absolute independent from "Gude Analog- und Digitalsysteme 
  GmbH" (http://www.gudeads.com/) and not supported from them.
/WARNING/


DCF77 is a time signal and is broadcasted at 77.5 kHz from Frankfurt/Main 
Germany with a range of 2000 km.
With the received signal the program sets local clock.
Details see http://www.dcf77.de/



/FEATURES/
- supported hardware: Serial Gude Expert Mouse Clock
     connected to any serial is supported on OS/2
- sets system time and timezone
- multi stage statistic algorithm for decoding signal
- no need to detect missing second of DCF77 signal. So program can run in 
  regular and not in timecrital priority. Even on a heavy loaded system 
  decoding should work fine.
- clean I/O over standard api
- highly portable
- nativ OS/2



/LIMITATIONS/
- sets system clock only if difference between System and DCF77 is smaller 
  than 3 minutes. This is only for security in alpha stadium and will be 
  removed later.
- every time a good block is received the system clock is set
- currently only german timezone supported
- not year 3000 sure ;-)




/USAGE/
 
   dcf77 comx

comx --> com-Port where clock is attached




/CONTACT/
For questions, bugreports or remarks contact me: Markus.Rosellen@gmx.de
Full source code for porting is aviable on request.

Thank you for supporting OS/2!
Markus Rosellen