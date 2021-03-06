
/*
 * This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Analog Comparator
 */

/**
 * @page atmelavr_acb
 * @section atmelavr_acb_act Actions
 *
 * <br>
 * `configure`:
 *
 * __Note__ When the analog comparator shares the analog multiplexer output
 * with the ADC, the ADC must be turned off for the analog comparator to be able
 * to use the analog multiplexer output.
 *
 * @code
 * hwa( configure, acmp0,
 * 
 *    [ edge,		 falling
 *		       | rising
 *		       | both, ]
 *
 *    [ positive_input,	 (pin,ain0)
 *		       | bandgap,    ]
 *
 *    [ negative_input,	 (pin,ain1)
 *		       | (pin,adc0..3) ] );
 * @endcode
 */
/*
 *  NOTE: another solution could be the creation of an object `admux0` for
 *  the analog multiplexer that's shared between the ADC and the ACMP but the
 *  ADC and ACMP can not use the analog multiplexer at the same time. Then, it
 *  seems acceptable to make the configuration instructions of the ADC or the
 *  ACMP drive the analog multiplexer transparently.
 */
#define hwa_configure__acb		, _hwa_cfacb

/*  Optionnal parameter `edge`
 */
#define _hw_acb_edge_falling		, 2	/* ACIS */
#define _hw_acb_edge_rising		, 3
#define _hw_acb_edge_both		, 0

#define _hwa_cfacb(o,a,k,...)					\
  do {									\
    HW_B(_hwa_cfacb_xedge_,_hw_is_edge_##k)(o,k,__VA_ARGS__,,);	\
  } while(0)

#define _hwa_cfacb_xedge_1(o,k,v,...)					\
  HW_B(_hwa_cfacb_vedge_,_hw_acb_edge_##v)(o,v,__VA_ARGS__)

#define _hwa_cfacb_vedge_0(o,v,...)					\
  HW_E_AVL(edge, v, falling | rising | both)

#define _hwa_cfacb_vedge_1(o,v,k,...)			\
  _hwa_write(o,acis, HW_A1(_hw_acb_edge_##v));	\
  HW_G2(_hwa_cfacb_xposin,HW_IS(positive_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacb_xedge_0(o,k,...)					\
  HW_G2(_hwa_cfacb_xposin,HW_IS(positive_input,k))(o,k,__VA_ARGS__)

/*  Optionnal parameter `positive_input`
 */
#define _hwa_cfacb_xposin_1(o,k,v,...)				\
  HW_B(_hwa_cfacb_vposin_bandgap_,_hw_is_bandgap_##v)(o,v,__VA_ARGS__)

#define _hwa_cfacb_vposin_bandgap_1(o,v,k,...)			\
  _hwa_write(o,acbg,1);						\
  HW_G2(_hwa_cfacb_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacb_vposin_bandgap_0(o,v,k,...)			\
  if ( HW_ADDRESS(v)==HW_ADDRESS((pin,ain0)) )					\
    _hwa_write(o,acbg,0);						\
  else									\
    HWA_ERR("`positive_input` can be `(pin,ain0) | bandgap`, but not `"#v"`."); \
  HW_G2(_hwa_cfacb_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacb_xposin_0(o,k,...)					\
  HW_G2(_hwa_cfacb_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

/*  Optionnal parameter `negative_input`
 */
#define _hwa_cfacb_xnegin_0(o,...)		HW_EOL(__VA_ARGS__)

#define _hwa_cfacb_xnegin_1(o,k,v,...)				\
  if ( HW_ADDRESS(v)==HW_ADDRESS((pin,ain1)) ) {			\
    _hwa_write(o,acme,0);						\
  } else {								\
    _hwa_write(o,acme,1);						\
    _hwa_write(o,aden,0);						\
    if ( HW_ADDRESS(v) == HW_ADDRESS( (pin,adc0) ) )			\
      _hwa_write(o,admux, 0);						\
    else if ( HW_ADDRESS(v) == HW_ADDRESS( (pin,adc1) ) )		\
      _hwa_write(o,admux, 1);						\
    else if ( HW_ADDRESS(v) == HW_ADDRESS( (pin,adc2) ) )		\
      _hwa_write(o,admux, 2);						\
    else if ( HW_ADDRESS(v) == HW_ADDRESS( (pin,adc3) ) )		\
      _hwa_write(o,admux, 3);						\
    else								\
      HWA_ERR("`negative_input` can be `(pin,ain1)`, or any analog input pin, but not `"#v"`."); \
  }									\
  HW_EOL(__VA_ARGS__)


#define hw_power__acb			, _hw_power
#define hwa_power__acb		, _hwa_power


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__acb(o,a)	_hwa_setup_r( o, csr )
#define _hwa_init__acb(o,a)		_hwa_init_r( o, csr, 0x00 )
#define _hwa_commit__acb(o,a)	_hwa_commit_r( o, csr )

/**
 * @page atmelavr_acb
 * @section atmelavr_acb_internals Internals
 *
 * Class `_acb` objects hold the following hardware registers:
 *
 *  * `csr`: control/status register
 *
 * that hold the following logical registers:
 *
 *  * `acme`: analog comparator multiplexer enabled
 *  * `aden`: A/D converter enable
 *  * `admux`: analog multiplexer input select
 *  * `acd` : analog comparator disable (power management)
 *  * `acbg` : analog comparator bandgap select
 *  * `aco` : analog comparator output
 *  * `if` : analog comparator interrupt flag
 *  * `ie` : analog comparator interrupt enable
 *  * `acic` : analog comparator input capture enable
 *  * `acis` : analog comparator interrupt mode select
 */
