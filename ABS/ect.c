#include <mc9s12dg128.h> / * derivative information * /
#include "ECT.h"
#include "AD.h"
#include "RTI.h"
   
extern word IOC0_wData; // the interval between the two falling edges of the input capture channel - the pulse period
extern word IOC1_wData;
extern word IOC2_wData;
extern word IOC3_wData;
   
// ************************************************ *******
// * Channel 0-3 for input capture
// * Frequency division coefficient 16, each count is equivalent to 1us
// * When the values ​​in the hold and capture registers are updated, an interrupt is generated
// * The period of the input pulse should be between 65.535ms and 2us (500KHz-15Hz)
// ************************************************ *******
void ECT_Init (void)
{
   TIOS = 0x00; // channel 0-7 for input capture
   TCTL4 = 0xAA; // channel 0-3 falling edge capture
   ICOVW = 0xFF; // prohibit overwriting the previous value
   ICSYS = 0xA0; // interrupt is generated when both the hold and capture registers are updated
   TSCR2 = 0x04; // Frequency division coefficient 16, each count is equivalent to 1us
   TFLG1 = 0xFF; // The interrupt flag is cleared
   TIE = 0x0F; // Enable capture interrupt
   TSCR1 = 0x80; // Enable the timer module
}
   
#pragma CODE_SEG NON_BANKED
interrupt void IOC0_ISR (void)
{
   word oldValue, newValue;
   TFLG1_C0F = 1;
   oldValue = TC0H; // Keep the value in the register as the first falling edge
   newValue = TC0; // Enter the value in the capture register as the second falling edge
   if (newValue> = oldValue) // if both falling edges occur between the counter from 0-0xFFFF
   {
     IOC0_wData = newValue - oldValue; // The interval between two falling edges (microseconds)
   }
   else // twice falling edge intermediate counter overflow
   {
     IOC0_wData = 0xFFFF - oldValue + newValue; // Interval of two falling edges (microseconds)
   }
}
interrupt void IOC1_ISR (void)
{
   word oldValue, newValue;
   TFLG1_C1F = 1;
   oldValue = TC1H; // Keep the value in the register as the first falling edge
   newValue = TC1; // Enter the value in the capture register as the second falling edge
   if (newValue> = oldValue) // if both falling edges occur between the counter from 0-0xFFFF
   {
     IOC1_wData = newValue - oldValue; // The interval between two falling edges (microseconds)
   }
   else // twice falling edge intermediate counter overflow
   {
     IOC1_wData = 0xFFFF - oldValue + newValue; // Interval of two falling edges (microseconds)
   }
}
interrupt void IOC2_ISR (void)
{
   word oldValue, newValue;
   TFLG1_C2F = 1;
   oldValue = TC2H; // Keep the value in the register as the first falling edge
   newValue = TC2; // Enter the value in the capture register as the second falling edge
   if (newValue> = oldValue) // if both falling edges occur between the counter from 0-0xFFFF
   {
     IOC2_wData = newValue - oldValue; // Interval of two falling edges (microseconds)
   }
   else // twice falling edge intermediate counter overflow
   {
     IOC2_wData = 0xFFFF - oldValue + newValue; // The interval between two falling edges (microseconds)
   }
}
interrupt void IOC3_ISR (void)
{
   word oldValue, newValue;
   TFLG1_C3F = 1;
   oldValue = TC3H; // Keep the value in the register at the first falling edge
   newValue = TC3; // Enter the value in the capture register as the second falling edge
   if (newValue> = oldValue) // if both falling edges occur between the counter from 0-0xFFFF
   {
     IOC3_wData = newValue - oldValue; // The interval between two falling edges (microseconds)
   }
   else // twice falling edge intermediate counter overflow
   {
     IOC3_wData = 0xFFFF - oldValue + newValue; // Interval of two falling edges (microseconds)
   }
}
#pragma CODE_SEG DEFAULT
