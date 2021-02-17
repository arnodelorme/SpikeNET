/////////////////////////////////////////////////////////
//                                                     //
//    Cubpack++                                        //
//                                                     //
//        A Package For Automatic Cubature             //
//                                                     //
//        Authors : Ronald Cools                       //
//                  Dirk Laurie                        //
//                  Luc Pluym                          //
//                                                     //
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// File : chrono.h
// History:
//   (date)          (version)
//   19 Aug 1994     V0.1 (first limited distribution)
//   28 Mar 1996     V0.1h(long instead of int)
//    1 Aug 1996     V1.0 (paper accepted by ACM TOMS)
//   19 Feb 1997     V1.1 (TIMES added)
/////////////////////////////////////////////////////////
// DEFINITION OF CLASS Chrono
// --------------------------
//
// BASECLASSES:
//   Counter
//
// PURPOSE:
//   Chrono implements a simple stop-watch; it can be used for
//   all sorts of timings. At the moment it will only give
//   useful results when compiled with an appropriate flag.
//   If no such flag is provided, Read() will always return 0.
//
//   On a system that has a getrusage() system call, one must
//   compile using -DGETRUSAGE.
//
//   On a system that has a times() system call, one must compile
//   using -DTIMES.
//
// TEMPLATES:
//   None
//
// METHODS:
//   CONSTRUCTORS:
//     1) Chrono()
//     -----------
//     Default constructor. The stop-watch isn't running after
//     construction.
//   SELECTORS:
//     1) unsigned long Read()
//     ----------------
//     Read reads out the time in milliseconds. Currently it
//     is accurate up to one millisecond.
//   MODIFIERS:
//     1) void Start()
//     ---------------
//     Start() makes the stop-watch running. Successive calls
//     of Start() have no effect.
//     2) void Stop()
//     ---------------
//     Stops the stop-watch. After Stop(), Read() will always
//     read out the same value, until the stop-watch is started
//     again. Stopping a stopped stop-watch has no effect.
//     3) void Reset()
//     ---------------
//     After a call of Reset(), Read() will read out zero,
//     until the stop-watch is started again. A running
//     stop-watch should not be Reset().
//
//   OPERATORS:
//     None
//   SPECIAL:
//     None
//////////////////////////////////////////////////////////

#define GETRUSAGE

/////////////////////////////////////////////////////////
// File : real.h
// History:
//   (date)          (version)
//   19 Aug 1994     V0.1 (first limited distribution)
//    1 Aug 1996     V1.0 (paper accepted by ACM TOMS)
/////////////////////////////////////////////////////////
#ifndef REAL_H
#define REAL_H
/////////////////////////////////////////////////////////
#include <float.h>

#ifdef FLOAT
  typedef float real;

#define REAL_MAX  FLT_MAX
#define REAL_MIN  FLT_MIN
#define REAL_EPSILON  FLT_EPSILON
#define REAL_MAX_EXP  FLT_MAX_EPS
#define DEFAULT_REL_ERR_REQ  (1.0e-4)

#else
  typedef double real;

#define REAL_MAX  DBL_MAX
#define REAL_MIN  DBL_MIN
#define REAL_EPSILON  DBL_EPSILON
#define REAL_MAX_EXP  DBL_MAX_EXP
#define DEFAULT_REL_ERR_REQ  (1.0e-6)

#endif
/////////////////////////////////////////////////////////
#endif

#ifndef CHRONO_H
#define CHRONO_H
//#define TIMES

//////////////////////////////////////////////////////////

#ifndef COUNTER_H
#define COUNTER_H

//////////////////////////////////////////////
class Counter
  {
  public:

  virtual void Start()=0;
  virtual void Stop()=0;
  virtual void Reset()=0;
  virtual unsigned long  Read()=0;
  };
//////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////
#ifdef GETRUSAGE
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifdef TIMES
#include <sys/times.h>
#include <sys/types.h>
#ifdef __ultrix
#define _POSIX_SOURCE
#endif // __ultrix
#include <time.h>
#ifdef __ultrix
#undef _POSIX_SOURCE
#endif // __ultrix
#endif

class Chrono :public Counter
  {
  public:

  Chrono()
  {
  Reset();
  }
  void Start()
  {
  if ( !Running)
    {
    Time= OldTime = times_();
    Running = 1;
    };
  }

  void Stop()
  {
  if (Running)
    {
    Time = times_();
    Running = 0;
    };
  }

  void Reset()
  {
  Time=0;
  OldTime=0;
  Running = 0;
  }

  unsigned long Read()
  {
  if (Running)
    {
    return (times_() - OldTime);
    }
  else
    {
    return (Time -OldTime);
    };
  }

  private:

  long Time;
  long OldTime;
  long Running;
  long times_()
  {
  long t=0;
#ifdef GETRUSAGE
   struct rusage  info ;
   struct timeval *user_t = &(info.ru_utime), *sys_t  = &(info.ru_stime) ;

   getrusage (0, &info) ;

   // convert (seconds,microseconds) to milliseconds
   t += user_t->tv_sec * 1000 + user_t->tv_usec / 1000 ;
   t += sys_t->tv_sec  * 1000 + sys_t->tv_usec  / 1000 ;
#endif
#ifdef TIMES
  struct tms tms; 
  times(&tms); 

  t = tms.tms_utime + tms.tms_stime;   // this is in  clock_ticks 
  // conversion of clock_ticks to milliseconds
  t = long(real(t)*real(1000)/real(CLK_TCK));
#endif

  return t;
  }

  };
/////////////////////////////////////////////////////////
#endif
