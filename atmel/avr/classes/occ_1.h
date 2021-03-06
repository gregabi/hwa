
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief 16-bit compare unit
 */

/**
 * @page atmelavr_occ Class _occ: counter compare unit with waveform generator
 *
 * Class `_occ` objects are 16-bit compare units with waveform generator. They
 * are named using the notation (COUNTER,COMPARE),
 * e.g. (counter0,compare0). They have the following relative objects:
 *
 *  * one counter: `((counter0,compare0), counter)`
 *  * one output pin: `((counter0,compare0), pin)`
 *
 * They are used in:
 *
 * * @ref attinyx4 : `(counter1,compare0)`, `(counter1,compare1)`
 * * @ref atmegax8 : `(counter1,compare0)`, `(counter1,compare1)`
 *
 */
#define hw_class__occ


/**
 * @page atmelavr_occ
 * @par Instructions that do not produce C code
 *
 * The `HW_BITS()` instruction returns the number of bits of the compare
 * register:
 *
 * @code
 * #if HW_BITS( compare0 ) != 16
 * #  error A 16-bit compare unit is required!
 * #endif
 * @endcode
 */
#define HW_BITS__occ			, _hw_bits_occ
#define _hw_bits_occ(...)		16


/*  Return the compare register of a _occ
 *    HW_CODR is blued.
 */
#define hw__occ_reg			, _hw_occ_reg
#define _hw_occ_reg(oo,ct,cp)		HW_XOR(ct,ocr##cp)


/**
 * @page atmelavr_occ
 * @par Interrupts
 *
 * The compare unit can trigger an @ref using_interrupts IRQ when the
 * compare-match flag is set, one counter clock cycle after the compare register
 * matches the counting register.
 *
 * @code
 * HW_ISR(counter0,compare0)	   // "compare0" IRQ of counter 0
 * {
 *    // Process compare event
 * }
 * @endcode
 *
 * @code
 * HW_ISR( (counter0,compare0) )   // IRQ of (counter0,compare0)
 * {
 *    // Process compare event
 * }
 * @endcode
 */


#if !defined __ASSEMBLER__

/*  HWA context
 */
typedef struct {

  /*  Registers used for high-level configuration
   */
  struct {
    uint8_t	update ;
    uint8_t	output ;
  } config ;

  /*  Registers used for configuration resolution
   */
  struct {
    uint8_t	com ;
  } solved ;

} hwa_occ_t ;

#endif
