
/*	Fade a LED using an output-compare pin configured for PWM
 */

#include <hwa.h>

#define	LED		hw_counter0_compare_b


HW_ISR( hw_irq(hw_ctr(LED), overflow) )
{
  static uint8_t	duty ;
  static uint8_t	phase ;

  if ( phase == 0 )
    hw_write( LED, duty );
  else if ( phase == 1 )
    hw_write( LED, ~duty );

  duty++ ;

  if ( duty==0 )
    phase = (phase + 1) & 3 ;
}


HW_INLINE void config_counter( hwa_t *hwa )
{
  hwa_config( hw_ctr(LED),
	      clock,		syshz_div_64,
	      //	      countmode,	loop_updown,
	      countmode,	loop_up,
	      bottom,		0,
	      //	      top,		fixed_0xFF	
	      top,		register_compare_a
	      );

  hwa_write_bits( hw_ctr(LED), compare_a, 0xFF );
  //  hwa_write( hw_ctr(LED), icr, 0xFF );

  hwa_turn( hw_irq(hw_ctr(LED), overflow), on );
  //  hwa_turn_irq( hw_counter(LED), capture, on );
  //  hwa_turn_irq( hw_counter(LED), match_a, on );
}


HW_INLINE void config_led( hwa_t *hwa )
{
  //  hwa_config( LED, clear_on_match_up_set_on_match_down );
  hwa_config( LED, set_at_bottom_clear_on_match );
}


int main ( )
{
  hwa_begin_from_reset();
  config_led(hwa);
  config_counter(hwa);
  hwa_commit();

  hw_enable_interrupts();

  for(;;)
    hw_sleep_until_irq();

  return 0 ;
}
