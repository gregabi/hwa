
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 */

#include "../hwa/atmel/avr/2.h"
#include "../classes/atmel/avr/coreb_2.h"
#include "../classes/atmel/avr/io_2.h"
#include "../classes/atmel/avr/pin1a_2.h"
#include "../classes/atmel/avr/pcicb_2.h"
#include "../classes/atmel/avr/wdogb_2.h"
#include "../classes/atmel/avr/icua_2.h"
#include "../classes/atmel/avr/ocua_2.h"
#include "../classes/atmel/avr/c8a_2.h"
#include "../classes/atmel/avr/c8b_2.h"
#include "../classes/atmel/avr/c16a_2.h"
#include "../classes/atmel/avr/psca_2.h"
#include "../classes/atmel/avr/uarta_2.h"
#include "../classes/atmel/avr/admuxb_2.h"
#include "../classes/atmel/avr/acmpb_2.h"
#include "../classes/atmel/avr/ad10b_2.h"
#include "../classes/atmel/avr/eeproma_2.h"
#include "../classes/atmel/avr/flasha_2.h"
#include "../classes/atmel/avr/swuarta_2.h"


HW_INLINE void _hwa_begin_all( hwa_t *hwa )
{
  _hwa_begin_p( hw_core0 );
  _hwa_begin_p( hw_portb );
  _hwa_begin_p( hw_portc );
  _hwa_begin_p( hw_portd );
  _hwa_begin( hw_pcic0 );
  _hwa_begin_p( hw_wdog0 );
  _hwa_begin_p( hw_counter0 );
  _hwa_begin_p( hw_counter1 );
  _hwa_begin_p( hw_counter2 );
  _hwa_begin_p( hw_uart0 );
  _hwa_begin_p( hw_adc0 );
  _hwa_begin_p( hw_acmp0 );
}


HW_INLINE void _hwa_init_all( hwa_t *hwa )
{
  _hwa_init_p( hw_core0 );
  _hwa_init_p( hw_portb );
  _hwa_init_p( hw_portc );
  _hwa_init_p( hw_portd );
  _hwa_init( hw_pcic0 );
  _hwa_init_p( hw_wdog0 );
  _hwa_init_p( hw_counter0 );
  _hwa_init_p( hw_counter1 );
  _hwa_init_p( hw_counter2 );
  _hwa_init_p( hw_uart0 );
  _hwa_init_p( hw_adc0 );
  _hwa_init_p( hw_acmp0 );
}


HW_INLINE void _hwa_commit_all( hwa_t *hwa )
{
  /*  Solve the configuration.
   *
   *  The configuration values are written here since the solve function only
   *  has the address of the counter and does not know its name.
   *
   *  _reg_p(...) could be used as well though it can not access registers of
   *  another object.
   */
  _hwa_solve_c8a( &hwa->hw_counter0 );
  if ( hwa->hw_counter0.solved.cs != 0xFF )
    _hwa_write_reg( hw_counter0, cs, hwa->hw_counter0.solved.cs );
  if ( hwa->hw_counter0.solved.wgm != 0xFF )
    _hwa_write_reg( hw_counter0, wgm, hwa->hw_counter0.solved.wgm );
  if ( hwa->hw_counter0.solved.coma != 0xFF )
    _hwa_write_reg( hw_counter0, compare0_mode, hwa->hw_counter0.solved.coma );
  if ( hwa->hw_counter0.solved.comb != 0xFF )
    _hwa_write_reg( hw_counter0, compare1_mode, hwa->hw_counter0.solved.comb );

  _hwa_solve( hw_counter1 );
  if ( hwa->hw_counter1.solved.cs != 0xFF )
    _hwa_write_reg( hw_counter1, cs, hwa->hw_counter1.solved.cs );
  if ( hwa->hw_counter1.solved.wgm != 0xFF )
    _hwa_write_reg( hw_counter1, wgm, hwa->hw_counter1.solved.wgm );
  if ( hwa->hw_counter1.solved.coma != 0xFF )
    _hwa_write_reg( hw_counter1, compare0_mode, hwa->hw_counter1.solved.coma );
  if ( hwa->hw_counter1.solved.comb != 0xFF )
    _hwa_write_reg( hw_counter1, compare1_mode, hwa->hw_counter1.solved.comb );
  if ( hwa->hw_counter1.solved.acic != 0xFF )
    _hwa_write_reg( hw_counter1, acic, hwa->hw_counter1.solved.acic );
  if ( hwa->hw_counter1.solved.ices != 0xFF )
    _hwa_write_reg( hw_counter1, ices, hwa->hw_counter1.solved.ices );
  if ( hwa->hw_counter1.solved.icnc != 0xFF )
    _hwa_write_reg( hw_counter1, icnc, hwa->hw_counter1.solved.icnc );

  _hwa_solve_c8b( &hwa->hw_counter2 );
  if ( hwa->hw_counter2.solved.cs != 0xFF )
    _hwa_write_reg( hw_counter2, cs, hwa->hw_counter2.solved.cs );
  if ( hwa->hw_counter2.solved.wgm != 0xFF )
    _hwa_write_reg( hw_counter2, wgm, hwa->hw_counter2.solved.wgm );
  if ( hwa->hw_counter2.solved.coma != 0xFF )
    _hwa_write_reg( hw_counter2, compare0_mode, hwa->hw_counter2.solved.coma );
  if ( hwa->hw_counter2.solved.comb != 0xFF )
    _hwa_write_reg( hw_counter2, compare1_mode, hwa->hw_counter2.solved.comb );

  /*  If used, compare outputs must be configured as i/o outputs
   */
  if ( hwa->hw_counter0.config.compare0.output != 0xFF
       && hwa->hw_counter0.config.compare0.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc0a, direction, output );
  if ( hwa->hw_counter0.config.compare1.output != 0xFF
       && hwa->hw_counter0.config.compare1.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc0b, direction, output );

  if ( hwa->hw_counter1.config.compare0.output != 0xFF
       && hwa->hw_counter1.config.compare0.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc1a, direction, output );
  if ( hwa->hw_counter1.config.compare1.output != 0xFF
       && hwa->hw_counter1.config.compare1.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc1b, direction, output );

  if ( hwa->hw_counter2.config.compare0.output != 0xFF
       && hwa->hw_counter2.config.compare0.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc2a, direction, output );
  if ( hwa->hw_counter2.config.compare1.output != 0xFF
       && hwa->hw_counter2.config.compare1.output != HW_A1(hw_ocu_output_disconnected) )
    hwa_config( hw_pin_oc2b, direction, output );

  _hwa_commit_p( hw_core0 );
  _hwa_commit_p( hw_wdog0 );
  _hwa_commit_p( hw_portb );
  _hwa_commit_p( hw_portc );
  _hwa_commit_p( hw_portd );
  _hwa_commit( hw_pcic0 );
  _hwa_commit_p( hw_counter0 );
  _hwa_commit_p( hw_counter1 );
  _hwa_commit_p( hw_counter2 );
  _hwa_commit_p( hw_uart0 );
  _hwa_commit_p( hw_adc0 );
  _hwa_commit_p( hw_acmp0 );
}