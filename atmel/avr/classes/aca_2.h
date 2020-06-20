
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
 * @page atmelavr_aca
 * @section atmelavr_aca_act Actions
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
 *    [ edge,		  falling
 *			| rising
 *			| both, ]
 *
 *    [ positive_input,	  (pin,ain0)
 *			| bandgap, ]
 *
 *    [ negative_input,	  (pin,ain1)
 *			| (pin,adc0..7) ] );
 * @endcode
 */
/*
 *  FIXME
 *
 *  Another solution could be the creation of an object `admux0` for the analog
 *  multiplexer that's shared between the ADC and the ACMP but the ADC and ACMP
 *  can not use the analog multiplexer at the same time. Then, it seems
 *  acceptable to make the configuration instructions of the ADC or of the ACMP
 *  drive the analog multiplexer.
 */
#define hwa_configure__aca		, _hwa_cfaca

/*  Optionnal parameter `edge`
 */
#define _hwa_cfaca(o,a,k,...)		do { HW_YW(_hwa_cfaca_ed,edge,k)(o,k,__VA_ARGS__,,); } while(0)
#define _hwa_cfaca_ed0(o,k,...)		HW_YW(_hwa_cfaca_po,positive_input,k)(o,k,__VA_ARGS__)
#define _hwa_cfaca_ed1(o,k,v,...)	_hwa_cfaca_ed2(HW_VE(_hw_aca_edge_,v,0,HW_EM_XNIL(v,(falling,rising,both))),o,__VA_ARGS__)
#define _hwa_cfaca_ed2(...)		_hwa_cfaca_ed3(__VA_ARGS__)
#define _hwa_cfaca_ed3(v,o,k,...)	_hwa_write(o,acis,v); HW_YW(_hwa_cfaca_po,positive_input,k)(o,k,__VA_ARGS__)

#define _hw_aca_edge_falling		, 2	/* ACIS */
#define _hw_aca_edge_rising		, 3
#define _hw_aca_edge_both		, 0

/*  Optionnal parameter `positive_input`
 */
#define _hwa_cfaca_po0(o,k,...)		HW_YW(_hwa_cfaca_ne,negative_input,k)(o,k,__VA_ARGS__)
#define _hwa_cfaca_po1(o,k,v,...)	HW_YW(_hwa_cfaca_po1,bandgap,v)(o,v,__VA_ARGS__)
#define _hwa_cfaca_po11(o,v,k,...)	_hwa_write(o,acbg,1); HW_YW(_hwa_cfaca_ne,negative_input,k)(o,k,__VA_ARGS__)
#define _hwa_cfaca_po10(o,v,k,...)					\
  if ( HW_ADDRESS(v)==HW_ADDRESS((pin,ain0)) )	_hwa_write(o,acbg,0);	\
  else						HWA_ERR(HW_EM_XNIL(v,((pin,ain0),bandgap))); \
  HW_YW(_hwa_cfaca_ne,negative_input,k)(o,k,__VA_ARGS__)


/*  Optionnal parameter `negative_input`
 */
#define _hwa_cfaca_ne0(o,...)		HW_EOL(__VA_ARGS__)
#define _hwa_cfaca_ne1(o,k,v,...)					\
  uint32_t a = HW_ADDRESS(v);						\
 if ( a == HW_ADDRESS((pin,ain1)) )		_hwa_write(o,acme,0);	\
 else {									\
   _hwa_write(o,acme,1);						\
   _hwa_write(o,aden,0);						\
   if      ( a == HW_ADDRESS((pin,adc0)) )	_hwa_write(o,admux,0);	\
   else if ( a == HW_ADDRESS((pin,adc1)) )	_hwa_write(o,admux,1);	\
   else if ( a == HW_ADDRESS((pin,adc2)) )	_hwa_write(o,admux,2);	\
   else if ( a == HW_ADDRESS((pin,adc3)) )	_hwa_write(o,admux,3);	\
   else if ( a == HW_ADDRESS((pin,adc4)) )	_hwa_write(o,admux,4);	\
   else if ( a == HW_ADDRESS((pin,adc5)) )	_hwa_write(o,admux,5);	\
   else if ( a == HW_ADDRESS((pin,adc6)) )	_hwa_write(o,admux,6);	\
   else if ( a == HW_ADDRESS((pin,adc7)) )	_hwa_write(o,admux,7);	\
   else	HWA_ERR(HW_EM_XNIL(v,((pin,ain1), (pin,adc0..X))));		\
 }									\
 HW_EOL(__VA_ARGS__)


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__aca(o,a)	_hwa_setup_r( o, csr	);

#define _hwa_init__aca(o,a)		_hwa_init_r( o, csr, 0x00 );

#define _hwa_commit__aca(o,a)	_hwa_commit_r( o, csr );


/**
 * @page atmelavr_aca
 * @section atmelavr_aca_internals Internals
 *
 * Class `_aca` objects hold the following hardware registers:
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
