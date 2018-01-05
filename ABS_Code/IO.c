#include <mc9s12dg128.h> / * derivative information * /
#include "main.h"
#include "ECT.h"
#include "AD.h"
#include "RTI.h"
#include "IO.h"
   
// PORTP initialization program
void IO_Init (void)
{
   DDRP = 0xFF; // PORTS is defined as output
   PTP = 0xFF; // initialize high, LED off
}
