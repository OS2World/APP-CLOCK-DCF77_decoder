#ifndef _CLOCK_H_
#define _CLOCK_H_


#include "zeit.h"
#include "os_api.h"

/*Ringbuffer-Class*/

#define BUFFER_MINUTES 5             //Achtung bei Aenderung muss das Programm auch angepasst werden (suchen nach "59")
#define BUFFER_SIZE (BUFFER_MINUTES*59)

class t_ringbuffer
  {
  public:
    t_ringbuffer(void);
    void put(unsigned char value);
    unsigned char get(int position);          //position is relative to begin_last_sequence;
    int get_write_pointer(void);
    void inc_read_pointer(void);    // +1
    void inc_read_pointer(int n);   // +n
    void dump(void);                //only debug
    void dump_relative(void);       //only debug
    int buffer_full;
  private:
    unsigned char buffer[BUFFER_SIZE];
    int write_pointer;     // this is the cell, where the next write takes place
    int read_pointer;      // points at the beginning of a sequence, if programm is synchron with signal
  };

/* Mouse Clock decoding class*/

class t_expert_mouse_clock
   {
   public:
     t_expert_mouse_clock(char* device);  //constructor

     t_time actual_time;
     t_time last_time;

     unsigned char signal_quality;
     void get_time(void);
   private:
     t_ringbuffer ring_buffer;    // Buffer to store the data from the clock
     t_serial_api * p_serial_api;   //interface to operating system serial interface
     int synchron(void);   //true if einsynchronisiert
     unsigned char translate(unsigned char value);
     int check_minutes(void);
     int check_parity(int offset);
     void parse_time(t_time* t, int offset);
     void read_clock(void);
   };

#endif
