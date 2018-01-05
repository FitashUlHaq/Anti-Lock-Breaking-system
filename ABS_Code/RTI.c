#include <mc9s12dg128.h> / * derivative information * /
#include "ECT.h"
#include "AD.h"
#include "RTI.h"
   
extern byte Timer1ms_Flag;
   
// ************************************************ *****
// * Crystal frequency = OSCCLK; 16MHz
// * RTICTL RTR [6: 4] = 100 The prescaler is 2 times 13 times
// * RTICTL RTR [3: 0] = 0001 The counter is 2
// * overflow interrupt time is 2 * 8 * 1024/16000000 = 1.024ms
// *
// ************************************************ ******* /
void RTI_Init (void)
{
   RTICTL = 0x41; // Real-time interrupt overflow time 1.024ms (system crystal 16MHZ)
   CRGINT_RTIE = 1; // Real-time interrupt enable
}
   
#pragma CODE_SEG NON_BANKED
interrupt void RTI_ISR (void)
{
   CRGFLG_RTIF = 1; // The interrupt flag is cleared
   Timer1ms_Flag = 1;
}
