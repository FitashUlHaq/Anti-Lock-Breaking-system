#include <hidef.h> / * common definitions and macros * /
#include <mc9s12dg128.h> / * derivative information * /
#include "main.h"
#include "ECT.h"
#include "AD.h"
#include "RTI.h"
#include "IO.h"
   
byte TimerCount; // Used to count the number of 1 ms interrupts
byte Timer1ms_Flag; // 1 millisecond flag
byte Timer10ms_Flag; // 10 milliseconds flag
word AD0_wData [4]; // save channel 0 sample value
word IOC0_wData; // interval between two falling edges of the input capture channel - pulse period (us)
word IOC1_wData;
word IOC2_wData;
word IOC3_wData;
byte wheelSpeed_FL; // left front wheel speed
byte wheelSpeed_FR; // right front wheel speed
byte wheelSpeed_RL; // left rear wheel speed
byte wheelSpeed_RR; // left rear wheel speed
   
word pressure_FL; // left front wheel pressure
word pressure_FR; // right front wheel pressure
word pressure_RL; // left rear wheel pressure
word pressure_RR; // left rear wheel pressure
   
#pragma LINK_INFO DERIVATIVE "mc9s12dg128b"
   
void main (void)
{
  byte i
  ECT_Init ();
  AD0_Init ();
  RTI_Init ();
  IO_Init ();
     
  TimerCount = 1;
  Timer1ms_Flag = 0;
  Timer10ms_Flag = 0;
  for (i = 0; i <4; i ++)
  {
    AD0_wData [i] = 0x00;
  }
  WheelSpeed_FL = 0;
  WheelSpeed_FR = 0;
  WheelSpeed_RL = 0;
  WheelSpeed_RR = 0;
       
  EnableInterrupts;
  for (;;)
  {
    if (Timer1ms_Flag == 1) // 1ms interrupt occurs, the following program is executed every 1ms
    {
       Timer1ms_Flag = 0; // flag is cleared
                
       if (TimerCount == 10) // count 10 times
       {
           
         Timer10ms_Flag = 1;
         TimerCount = 1;
       }
       else
       {
         TimerCount ++;
       }
    } // 1 ms task
    if (Timer10ms_Flag == 1)
    {
         ATD0CTL5 = 0x90; // Start a new conversion sequence and start sampling
         WheelSpeed_FL = DISTANCE_FOR_ONE_PULSE / IOC0_wData / 1000; // left front wheel speed meter / s
       WheelSpeed_FR = DISTANCE_FOR_ONE_PULSE / IOC1_wData / 1000; // right front wheel speed meter / s
       WheelSpeed_RL = DISTANCE_FOR_ONE_PULSE / IOC2_wData / 1000; // left rear wheel speed meter / s
       WheelSpeed_RR = DISTANCE_FOR_ONE_PULSE / IOC3_wData / 1000; // Right rear wheel speed meter / s
       
       Pressure_FL = AD0_wData [0]; // left front wheel pressure
       Pressure_FR = AD0_wData [1]; // right front wheel pressure
       Pressure_RL = AD0_wData [2]; // left rear wheel pressure
       Pressure_RR = AD0_wData [3]; // left rear wheel pressure
          
          
          
    } // 10 ms task
  } // for the main loop
}
