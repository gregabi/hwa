
/*	Fade a LED using an output-compare pin configured for PWM
 */

#define HW_DEVICE		attiny84
#define HW_DEVICE_PACKAGE	dil

#define LED_COUNTER		hw_counter0
#define LED_COUNTER_CLKDIV	64
#define	LED_PWM			hw_counter0_compare_b
//#define	LED_PWM			HW_G3(hw,hw_name(LED_COUNTER),compare_b) /* is ok too */
//#define	LED_PWM			LED_COUNTER ## _compare_b /* will not work */

#include <hwa.h>


HW_ISR( LED_COUNTER, overflow )
//HW_ISR( hw_irq(LED_COUNTER, overflow) )
//HW_ISR( LED_COUNTER, overflow, interruptible )
//HW_ISR( hw_irq(LED_COUNTER, overflow), interruptible )
{
  static uint8_t	duty ;
  static uint8_t	phase ;

  if ( phase == 0 )
    hw_write( LED_PWM, duty );
  else if ( phase == 1 )
    hw_write( LED_PWM, ~duty );

  duty++ ;

  if ( duty==0 )
    phase = (phase + 1) & 3 ;
}


HW_INLINE void config_led_counter( hwa_t *hwa )
{
  hwa_config( LED_COUNTER,
	      clock,       HW_G2(syshz_div, LED_COUNTER_CLKDIV),
	      countmode,   loop_up,
	      bottom,      0,
	      top,         register_compare_a
	      );
  hwa_write_reg( LED_COUNTER, compare_a, 0xFF );
  hwa_turn_irq( LED_COUNTER, overflow, on );
}


HW_INLINE void config_led_pin( hwa_t *hwa )
{
  hwa_config( LED_PWM, set_at_bottom_clear_on_match );
}


/* static void foo ( ) */
/* { */
/*   hwa_begin(); */
/*   hwa_write_reg( hw_usi0, count, 2 ); */
/*   hwa_write_reg( hw_usi0, sif, 1 ); */
/*   hwa_commit(); */
/* } */


int main ( )
{
  hwa_begin_from_reset();
  config_led_pin(hwa);
  config_led_counter(hwa);
  hwa_commit();

  /* foo(); */

  hw_enable_interrupts();

  for(;;)
    hw_sleep_until_irq();

  return 0 ;
}

/* hw_irq( hw_pcint8 ); */
/* hw_irq( hw_pin_pcint8 ); */
/* hw_irq( hw_pin_pcint8, overflow ); */
/* hw_irq( hw_pin_pcint8, overflow, on ); */
/* hw_irq( LED_COUNTER, on ); */
/* hw_irq( LED_COUNTER, overflow ); */
/* hw_irq( LED_COUNTER, overflow, on ); */

/* hwa_turn_irq( hw_pcint8 ); */
/* hwa_turn_irq( hw_pin_pcint8 ); */
/* hwa_turn_irq( hw_pin_pcint8, on ); */
/* hwa_turn_irq( hw_pin_pcint8, overflow ); */
/* hwa_turn_irq( hw_pin_pcint8, overflow, on ); */
/* hwa_turn_irq( LED_COUNTER, on ); */
/* hwa_turn_irq( LED_COUNTER, overflow ); */
/* hwa_turn_irq( LED_COUNTER, overflow, on ); */


/* hw_irqe( hw_pcint8 ); */
/* hw_irqe( hw_pin_pcint8 ); */
/* hw_irqe( hw_pin_pcint8, overflow ); */
/* hw_irqe( hw_pin_pcint8, overflow, on ); */
/* hw_irqe( LED_COUNTER, on ); */
/* hw_irqe( LED_COUNTER, overflow ); */
/* hw_irqe( LED_COUNTER, overflow, on ); */

/* HW_ISR( hw_pcint8 ); */
/* HW_ISR( hw_pin_pcint8 ); */
/* HW_ISR( hw_pin_pcint8, overflow ); */
/* HW_ISR( hw_pin_pcint8, overflow, on ); */
/* HW_ISR( LED_COUNTER, on ); */
/* HW_ISR( LED_COUNTER, overflow ); */
/* HW_ISR( LED_COUNTER, overflow, on ); */

/* hwa_turn_irq( LED_COUNTER, overflow, on ); */

/* hw_irq(LED_COUNTER, overflow); */
/* hwa_turn( hw_irq(LED_COUNTER, overflow), on ); */
/* hwa_turn_irq( hw_irq(LED_COUNTER, overflow), on ); */


/* hw_irq(LED_COUNTER, overflow); */

/* HW_ISR( LED_COUNTER, overflow ); */
/* HW_ISR( hw_irq(LED_COUNTER, overflow) ); */
/* HW_ISR( LED_COUNTER, overflow, naked ); */
/* HW_ISR( hw_irq(LED_COUNTER, overflow), naked ); */

/* hwa_turn( hw_irq(LED_COUNTER, overflow), on ); */
/* hwa_turn_irq( LED_COUNTER, overflow, on ); */

/* hw_turn( hw_irq(LED_COUNTER, overflow), on ); */
/* hw_turn_irq( LED_COUNTER, overflow, on ); */