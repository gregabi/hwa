
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 *  Blink a LED using a counter overflow interrupt
 */

/*  Include the target board (and device) definitions
 */
#include BOARD_H


/*  The counter
 */
#define COUNTER			counter0
#define CLKDIV			64
#define COUNTMODE		up_loop
#define PERIOD			0.5

/*  Compare strings
 */
#define STRCMP(s1,s2)		__builtin_strcmp(s1,s2)


/*  Service the counter overflow IRQ
 */
HW_ISR( (COUNTER,irq,overflow) )
{
  static uint8_t n ;
  n++ ;
  if ( n >= (uint8_t)(PERIOD / 2.0 / 0.001) ) {
    n = 0 ;
    hw( toggle, PIN_LED );
  }
}


int main ( )
{
  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa( begin, reset );

  /*  Configure the LED pin
   */
  hwa( configure, PIN_LED, mode, digital_output );

  /*  Have the CPU enter idle mode when the 'sleep' instruction is executed.
   */
  hwa( configure,  core0,
       sleep,	   enabled,
       sleep_mode, idle );

  /*  Configure the counter to overflow every 0.001 s.
   *
   *  The compare unit `compare0` (OCRxA) is used to store the top value.
   *  Unless otherwise stated, the overflow will be automatically set to occur
   *  at top in `up_loop` counting mode, and at bottom in `updown_loop` counting
   *  mode.
   */
  hwa( configure, COUNTER,
       clock,	  ioclk / CLKDIV,
       direction, COUNTMODE,
       bottom,	  0,
       top,	  compare0
       );

  /*  FIXME: should subtract 1 from the compare value because the compare match
   *  flag is set one counting cycle after the match.
   */
  if ( !STRCMP(HW_Q(COUNTMODE),"updown_loop") )
    hwa( write, (COUNTER,compare0), 0.5 + 0.001 * HW_SYSHZ / CLKDIV / 2 );
  else
    hwa( write, (COUNTER,compare0), 0.5 + 0.001 * HW_SYSHZ / CLKDIV );

  hwa( enable, (COUNTER,irq,overflow) );

  /*  Write this configuration into the hardware
   */
  hwa( commit );

  hw( enable, interrupts );

  for(;;)
    hw( wait, irq );
}
