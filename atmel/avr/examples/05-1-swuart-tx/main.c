
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 * Send a '.' on TXD every 0.1s. Can use hardware UART as well as software
 * UART.
 *
 * Test application:
 *
 *     ./main.py -b <BAUDRATE>
 *
 * Note: this can fail if the system frequency is not known precisely
 * enough (if using an internal RC oscillator for example).
 *
 * @par config.h
 * @include 05-1-swuart-tx//config.h
 *
 * @par main.c
 */

#include "config.h"

int
main ( )
{
  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa_begin_from_reset();

  /*  Configure the UART
   */
  hwa_config( UART,
	      bps,	BPS,
	      databits, 8,
	      parity,	none,
	      stopbits, 1,
	      );

  /*  Write this configuration into the hardware
   */
  hwa_commit();

  hw_enable_interrupts();

  for(;;) {

    /*	Wait for 0.1s
     */
    hw_delay_cycles( 0.1 * hw_syshz );

    /*	Send a '.'
     */
    hw_write( UART, '.');
  }
}