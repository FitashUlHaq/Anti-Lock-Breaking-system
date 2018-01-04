#include <mc9s12dg128.h> / * derivative information * /
#include "main.h"
#include "ECT.h"
#include "AD.h"
   
extern word AD0_wData [4];
   
   
void AD0_Init (void) // AD is initialized
{
     // control register 2: power on, flag bit quickly cleared, open interrupt
     ATD0CTL2 = (ATD0CTL2_ADPU_MASK | ATD0CTL2_AFFC_MASK | ATD0CTL2_ASCIE_MASK);
     // control register 3: conversion sequence length is 3, FIFO mode
     ATD0CTL3 = (ATD0CTL3_S4C_MASK | ATD0CTL3_FIFO_MASK);
     // control register 4: 10-bit sampling accuracy, A / D Clock can not be greater than 2 MHz, less than 0.5MHz
     ATD0CTL4 = (ATD0CTL4_PRS1_MASK | ATD0CTL4_PRS0_MASK);
     // control register 5: right-aligned unsigned, multi-channel sampling, channel 0
     ATD0CTL5 = 0x90;
       
     ATD0DIEN = 0x00; // disable digital input buffer
   
}
   
   
#pragma CODE_SEG NON_BANKED // interrupt service routine
interrupt void AD0_ISR (void)
{
  word data
  if (ATD0STAT1_CCF0 == 1)
  {
    data = ATD0DR0;
    if ((data <AD0_VALUE_MAX) && (data> AD0_VALUE_MIN)) // check whether the sample value is within the specified range
    {
      AD0_wData [0] = data; // The values ​​in the result register are stored in the array
    }
  } // End of channel 0 conversion
     
  if (ATD0STAT1_CCF1 == 1)
  {
    data = ATD0DR1;
    if ((data <AD0_VALUE_MAX) && (data> AD0_VALUE_MIN)) // check whether the sample value is within the specified range
    {
      AD0_wData [1] = data; // The values ​​in the result register are stored in the array
    }
  } // End of channel 1 conversion
     
  if (ATD0STAT1_CCF2 == 1)
  {
    data = ATD0DR2;
    if ((data <AD0_VALUE_MAX) && (data> AD0_VALUE_MIN)) // check whether the sample value is within the specified range
    {
      AD0_wData [2] = data; // The values ​​in the result register are stored in the array
    }
  } // End of channel 2 conversion
     
  if (ATD0STAT1_CCF3 == 1)
  {
    data = ATD0DR3;
    if ((data <AD0_VALUE_MAX) && (data> AD0_VALUE_MIN)) // check whether the sample value is within the specified range
    {
      AD0_wData [3] = data; // The values ​​in the result register are stored in the array
    }
  } // End of channel 3 conversion
     
}
#pragma CODE_SEG DEFAULT
