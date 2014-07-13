
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

/** \file
 *  \brief	HWA layer 1: Atmel AVR 16-bits timer-counters model 'a'
 *
 *	Used in:	ATtinyX4 Timer 1
 *			ATmegaX8 Timer 1
 */


/*	Generic functions
 */
#define hw_class_c16a

#define hw_def_hw_bn_c16a		, _hw_bn_c16a
#define _hw_bn_c16a(c,n,i,a)		16

//#define hw_def_hw_trigger_c


/*	16-bit counter class 'c16a'
 */
#define hw_c16a_countmode_loop_up		, 1
#define hw_c16a_countmode_loop_updown		, 2

#define hw_c16a_top_fixed_0xFF			, 1
#define hw_c16a_top_fixed_0x1FF			, 2
#define hw_c16a_top_fixed_0x3FF			, 3
#define hw_c16a_top_fixed_0xFFFF		, 4
#define hw_c16a_top_capture			, 5
#define hw_c16a_top_register_capture		, 5
#define hw_c16a_top_compare_a			, 6
#define hw_c16a_top_register_compare_a		, 6

#define hw_c16a_clock_none			, 0
#define hw_c16a_clock_syshz			, 1
#define hw_c16a_clock_syshz_div_1		, 1	/* Useful for concat */
#define hw_c16a_clock_syshz_div_8		, 2
#define hw_c16a_clock_syshz_div_64		, 3
#define hw_c16a_clock_syshz_div_256		, 4
#define hw_c16a_clock_syshz_div_1024		, 5
#define hw_c16a_clock_ext_rising		, 6
#define hw_c16a_clock_ext_falling		, 7


/*	Registers for c16a controller
 */
#ifndef __ASSEMBLER__

typedef struct {
  hwa_r8_t	ccra ;
  hwa_r8_t	ccrb ;
  hwa_r8_t	ccrc ;
  hwa_r16_t	count ;
  union {
    hwa_r16_t	ocra ;
    hwa_r16_t	compare_a ;
  };
  union {
    hwa_r16_t	ocrb ;
    hwa_r16_t	compare_b ;
  };
  union {
    hwa_r16_t	icr ;
    hwa_r16_t	capture ;
  };
  hwa_r8_t	imsk ;
  hwa_r8_t	ifr ;

  /*  Registers for high-level configuration
   */
  uint8_t	clock, countmode, top, update, overflow ;
  uint8_t	ocra_mode ;
  uint8_t	ocrb_mode ;
  uint8_t	icr_input, icr_edge, icr_filter ;

} hwa_c16a_t ;

#endif
