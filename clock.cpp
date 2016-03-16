#include "clock.h"
#include <iostream.h>


/* Ringbuffer Member function implementation  */

t_ringbuffer::t_ringbuffer(void)
  {
  write_pointer= 0;         //zeigt immer auf das Element, auf das als naechstes geschrieben wird
                            //d.h. es ist auch das erste lesbare Element
  read_pointer= 0;          //unbeutzt
  buffer_full= 0;           //buffer is at start empty
  } //construktor

void t_ringbuffer::put(unsigned char value)
  {
  buffer[write_pointer]= value;
  if (write_pointer >= (BUFFER_SIZE - 1))
         {
         write_pointer= 0;
         buffer_full= 1;
         }
  else write_pointer++;
  } //put

unsigned char t_ringbuffer::get(int position)
  {
  position= position + write_pointer;
  position= position % (BUFFER_SIZE);   //truncate to buffer_length
  return buffer[position];
  } //get

int t_ringbuffer::get_write_pointer()
  { // just to debug
  return(write_pointer);
  } //get_write_pointer

void t_ringbuffer::inc_read_pointer(int n)
  {
  read_pointer= (read_pointer + n) % BUFFER_SIZE;
  }

void t_ringbuffer::inc_read_pointer(void)
  {
  inc_read_pointer(1);
  }

void t_ringbuffer::dump(void)
  {
  //just for debug
  cout << endl << "=================================" << endl;
  for (int i= 0; i <= (BUFFER_SIZE-1); i++)
     {
     if (write_pointer != i)     cout << (int) buffer[i];
     else                        cout << "->" << (int) buffer[i] << "<-";
     }
  }//dump
void t_ringbuffer::dump_relative(void)
  {
  //just for debug
  cout << endl << "=====relative====================" << endl;
  for (int i= 0; i <= (BUFFER_SIZE-1); i++)
     {
     cout << (int) this->get(i);
     }
  } //dump_relative

/* clock decode Member function implementation */

t_expert_mouse_clock::t_expert_mouse_clock(char* device)
  {
  p_serial_api= new t_serial_api(device);   //create instance of t_serial_api
  //synchron= 0;
  } //constructor

unsigned char t_expert_mouse_clock::translate(unsigned char value)
  { // translates the values from the Clock in 0 and 1 values
    if (value <= 0xef)  return 1;
    else                return 0;
  } // translate

int t_expert_mouse_clock::synchron(void)
  {
    int found_good= 0;
    t_time last, other;
    int i= BUFFER_SIZE - 59;  //auf letzte Minute

    // letzte Minute ok
    if (check_parity(i))
         {
         // Ein Kandidat, da Zeit Parity schonmal stimmt
         found_good++;
         parse_time(&last, i);
         cout << "K" << (int) last.hour << ":" << (int) last.minute << flush;

         //letzte Zeit mit den anderen im Puffer vergleichen
         i-= 59;
         do {
            while ( !check_parity(i) && (i >= 0) ) { i--; cout << "-" << flush; }  // vor bis zur naechsten Minute bzw. Ringpufferende
            if (i >= 0)  // dann war eben parity ok
                { //ein naechster Kandidat
                parse_time(&other, i);
                cout << "k" << "[" << i << "]" << (int) other.hour << ":" << (int) other.minute << flush;
                //Kandidat mit Referenz (last) vergleichen
                if ( (other+(found_good-1)) < last) { found_good++; // sind die neu gefundenen kleiner als der originale
                                   cout << "+" << flush; } // aber Unterschied auch nicht zu gross
                i-= 59;
                }
             } while (i >= 0);  //bis am ende angekommen
         cout << "g" << found_good << flush;
         if (found_good >= (BUFFER_MINUTES/2 + 1)) return 1;  // mehr als die Haelfte waren ok
         else                return 0;
       } // Ende Parity war beim ersten ok
    else //Parity d. letzten Minute nicht ok -> nicht synchron
       return 0;
  }//einsysnchronisieren

int t_expert_mouse_clock::check_parity(int offset)
  { //boolean function
    /*checks the parity bits in the minute sequence*/
    //Operationen auf basis von: read_pointer
    if (
        (
          ((ring_buffer.get(21+offset) + ring_buffer.get(22+offset)
          + ring_buffer.get(23+offset) + ring_buffer.get(24+offset)
          + ring_buffer.get(24+offset) + ring_buffer.get(25+offset)
          + ring_buffer.get(26+offset) + ring_buffer.get(27+offset) )
          % 2) == ring_buffer.get(28+offset)
        ) &&
        (
          ((ring_buffer.get(29+offset) + ring_buffer.get(30+offset)
          + ring_buffer.get(31+offset) + ring_buffer.get(32+offset)
          + ring_buffer.get(33+offset) + ring_buffer.get(34+offset))
          % 2) == ring_buffer.get(35+offset)
        ) &&
        (
          ((ring_buffer.get(36+offset) + ring_buffer.get(37+offset)
           + ring_buffer.get(38+offset) + ring_buffer.get(39+offset)
           + ring_buffer.get(40+offset) + ring_buffer.get(41+offset)
           + ring_buffer.get(42+offset) + ring_buffer.get(43+offset)
           + ring_buffer.get(44+offset) + ring_buffer.get(45+offset)
           + ring_buffer.get(46+offset) + ring_buffer.get(47+offset)
           + ring_buffer.get(48+offset) + ring_buffer.get(49+offset)
           + ring_buffer.get(50+offset) + ring_buffer.get(51+offset)
           + ring_buffer.get(52+offset) + ring_buffer.get(53+offset)
           + ring_buffer.get(54+offset) + ring_buffer.get(55+offset)
           + ring_buffer.get(56+offset) + ring_buffer.get(57+offset) )
          % 2) == ring_buffer.get(58+offset)
        ) &&
        (
        ring_buffer.get(20+offset) == 1
        )
       )
         return 1;
    else return 0;
  }

int t_expert_mouse_clock::check_minutes(void)
  { //boolean function
    /*checks the last full sequence of a complete minute against the Data therefore*/
    t_time t1, t2;

    parse_time(&t1, 0);
    parse_time(&t2, 59);
    if (t1.minutes_of_week()+1 == t2.minutes_of_week() )  return(1);
    else                                                  return(0);
  } //check_minutes

void t_expert_mouse_clock::parse_time(t_time* t, int offset)
  {
  t->summertime= ring_buffer.get(17+offset);
  t->wintertime= ring_buffer.get(18+offset);

  t->minute= ring_buffer.get(21+offset)*1 + ring_buffer.get(22+offset)*2 + ring_buffer.get(23+offset)*4
                    + ring_buffer.get(24+offset)*8 + ring_buffer.get(25+offset)*10 + ring_buffer.get(26+offset)*20
                    + ring_buffer.get(27+offset)*40;
  t->hour= ring_buffer.get(29+offset)*1 + ring_buffer.get(30+offset)*2 + ring_buffer.get(31+offset)*4
                    + ring_buffer.get(32+offset)*8 + ring_buffer.get(33+offset)*10 + ring_buffer.get(34+offset)*20;
  t->calenderday= ring_buffer.get(36+offset)*1 + ring_buffer.get(37+offset)*2 + ring_buffer.get(38+offset)*4
                    + ring_buffer.get(39+offset)*8 + ring_buffer.get(40+offset)*10 + ring_buffer.get(41+offset)*20;
  t->weekday= ring_buffer.get(42+offset)*1 + ring_buffer.get(43+offset)*2 + ring_buffer.get(44+offset)*4;
  t->calendermonth= ring_buffer.get(45+offset)*1 + ring_buffer.get(46+offset)*2 + ring_buffer.get(47+offset)*4
                    + ring_buffer.get(48+offset)*8 + ring_buffer.get(49+offset)*10;
  t->calenderyear= ring_buffer.get(50+offset)*1 + ring_buffer.get(51+offset)*2 + ring_buffer.get(52+offset)*4
                    + ring_buffer.get(53+offset)*8 + ring_buffer.get(54+offset)*10 + ring_buffer.get(55+offset)*20
                    + ring_buffer.get(56+offset)*40 + ring_buffer.get(57+offset)*80;
  t->correct= 1;
  } //parse_time

void t_expert_mouse_clock::read_clock(void)
  {
  //reads a char from the clock and puts it translated into the ringbuffer
  unsigned char wert;
  wert= p_serial_api->receive();
//  signal_quality= ???             //Case Auswertung
  ring_buffer.put( translate( wert ) );
  } //read_clock

void t_expert_mouse_clock::get_time(void)
  { //get_time  (interface to extern)

  //---->> der Regelfall kann schneller abgehandelt werden

  //lesen bis puffer voll ist
  cout << "Fuelle Puffer" << flush;
  while(!ring_buffer.buffer_full)
     {
     read_clock();
     cout << "." << flush;
     }
  cout << "Puffer gefuellt" << endl;
  //ring_buffer.dump();
  cout << "Einsynchronisieren" << endl;
  while(!synchron())
     {
     //einsynchronisieren();
     read_clock();
     cout << "." << flush;
//     ring_buffer.dump_relative();
     }
  cout << "Synchron" << endl;
  last_time= actual_time;
  parse_time(&actual_time, BUFFER_SIZE-59);  //letzter Teil des Ringpuffers parsen, da dies die aktuelle Minute ist
                                 //kann spaeter rausoptimiert werden, denn das macht schon die zweite
                                 //Ueberpruefungsstufe
  read_clock();  //warten auf 0. Bit der naechsten Minute, dann ist die letzte Zeit gueltig
                 //damit wird der Ringpuffer auch aussynchroniert, was NOETIG ist
                 // (sonst bei !synchron() eine Endlosschleife)
  } //get_time


