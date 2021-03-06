
/* This file is part of the HWA project.
 * Copyright (c) 2017 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Clocks
 */

/**
 * @page stm32_rcca Class _rcca: Reset and Clock Control (RCC)
 */
#define hw_class__rcca

/*	Class registers
 */
#define hw__rcca_cr			_r32, 0x00, 0x010D00F9, 0
#define hw__rcca_cfgr			_r32, 0x04, 0x077FFFF3, 0
#define hw__rcca_cir			_r32, 0x08, 0x00FFFFFF, 0
#define hw__rcca_apb2enr		_r32, 0x18, 0x0038FFFD, 0
#define hw__rcca_apb1enr		_r32, 0x1C, 0x3AFED3FF, 0

#define hw__rcca_pllrdy			_cb1, cr, 1, 25
#define hw__rcca_pllon			_cb1, cr, 1, 24
#define hw__rcca_csson			_cb1, cr, 1, 19
#define hw__rcca_hsebyp			_cb1, cr, 1, 18
#define hw__rcca_hserdy			_cb1, cr, 1, 17
#define hw__rcca_hseon			_cb1, cr, 1, 16
#define hw__rcca_hsical			_cb1, cr, 8,  8
#define hw__rcca_hsitrim		_cb1, cr, 5,  3
#define hw__rcca_hsirdy			_cb1, cr, 1,  1
#define hw__rcca_hsion			_cb1, cr, 1,  0

#define hw__rcca_mco			_cb1, cfgr, 3, 24
#define hw__rcca_usbpre			_cb1, cfgr, 1, 22
#define hw__rcca_pllmul			_cb1, cfgr, 4, 18
#define hw__rcca_pllxtpre		_cb1, cfgr, 1, 17
#define hw__rcca_pllsrc			_cb1, cfgr, 1, 16
#define hw__rcca_pllxtpresrc		_cb1, cfgr, 2, 16	/* convenient */
#define hw__rcca_adcpre			_cb1, cfgr, 2, 14
#define hw__rcca_ppre2			_cb1, cfgr, 3, 11
#define hw__rcca_apb2			_cb1, cfgr, 3, 11	/* convenient */
#define hw__rcca_apb2prescaler		_cb1, cfgr, 3, 11	/* convenient */
#define hw__rcca_ppre1			_cb1, cfgr, 3,	8
#define hw__rcca_apb1			_cb1, cfgr, 3,	8	/* convenient */
#define hw__rcca_apb1prescaler		_cb1, cfgr, 3,	8	/* convenient */
#define hw__rcca_hpre			_cb1, cfgr, 4,	4
#define hw__rcca_sws			_cb1, cfgr, 2,	2
#define hw__rcca_sw			_cb1, cfgr, 2,	0

#define hw__rcca_tim11en		_cb1, apb2enr, 1, 21
#define hw__rcca_tim10en		_cb1, apb2enr, 1, 20
#define hw__rcca_tim9en			_cb1, apb2enr, 1, 19
#define hw__rcca_adc3en			_cb1, apb2enr, 1, 15
#define hw__rcca_usart1en		_cb1, apb2enr, 1, 14
#define hw__rcca_tim8en			_cb1, apb2enr, 1, 13
#define hw__rcca_spi1en			_cb1, apb2enr, 1, 12
#define hw__rcca_tim1en			_cb1, apb2enr, 1, 11
#define hw__rcca_adc2en			_cb1, apb2enr, 1, 10
#define hw__rcca_adc1en			_cb1, apb2enr, 1,  9
#define hw__rcca_port6en		_cb1, apb2enr, 1,  8
#define hw__rcca_port5en		_cb1, apb2enr, 1,  7
#define hw__rcca_port4en		_cb1, apb2enr, 1,  6
#define hw__rcca_port3en		_cb1, apb2enr, 1,  5
#define hw__rcca_port2en		_cb1, apb2enr, 1,  4
#define hw__rcca_port1en		_cb1, apb2enr, 1,  3
#define hw__rcca_port0en		_cb1, apb2enr, 1,  2
#define hw__rcca_afioen			_cb1, apb2enr, 1,  0

#define hw__rcca_dacen			_cb1, apb1enr, 1, 29
#define hw__rcca_pwren			_cb1, apb1enr, 1, 28
#define hw__rcca_bkpen			_cb1, apb1enr, 1, 27
#define hw__rcca_canen			_cb1, apb1enr, 1, 25
#define hw__rcca_usben			_cb1, apb1enr, 1, 23
#define hw__rcca_i2c2en			_cb1, apb1enr, 1, 22
#define hw__rcca_i2c1en			_cb1, apb1enr, 1, 21
#define hw__rcca_uart5en		_cb1, apb1enr, 1, 20
#define hw__rcca_uart4en		_cb1, apb1enr, 1, 19
#define hw__rcca_usart3en		_cb1, apb1enr, 1, 18
#define hw__rcca_usart2en		_cb1, apb1enr, 1, 17
#define hw__rcca_spi3en			_cb1, apb1enr, 1, 15
#define hw__rcca_spi2en			_cb1, apb1enr, 1, 14
#define hw__rcca_wwdgen			_cb1, apb1enr, 1, 11
#define hw__rcca_tim14en		_cb1, apb1enr, 1,  8
#define hw__rcca_tim13en		_cb1, apb1enr, 1,  7
#define hw__rcca_tim12en		_cb1, apb1enr, 1,  6
#define hw__rcca_tim7en			_cb1, apb1enr, 1,  5
#define hw__rcca_tim6en			_cb1, apb1enr, 1,  4
#define hw__rcca_tim5en			_cb1, apb1enr, 1,  3
#define hw__rcca_tim4en			_cb1, apb1enr, 1,  2
#define hw__rcca_tim3en			_cb1, apb1enr, 1,  1
#define hw__rcca_tim2en			_cb1, apb1enr, 1,  0

/*  HWA names
 */
#define hw__rcca_dac			_cb1, apb1enr, 1, 29
#define hw__rcca_power			_cb1, apb1enr, 1, 28
#define hw__rcca_backup			_cb1, apb1enr, 1, 27
#define hw__rcca_can			_cb1, apb1enr, 1, 25
#define hw__rcca_usb			_cb1, apb1enr, 1, 23
#define hw__rcca_i2c2			_cb1, apb1enr, 1, 22
#define hw__rcca_i2c1			_cb1, apb1enr, 1, 21
#define hw__rcca_uart5			_cb1, apb1enr, 1, 20
#define hw__rcca_uart4			_cb1, apb1enr, 1, 19
#define hw__rcca_usart3			_cb1, apb1enr, 1, 18
#define hw__rcca_usart2			_cb1, apb1enr, 1, 17
#define hw__rcca_spi3			_cb1, apb1enr, 1, 15
#define hw__rcca_spi2			_cb1, apb1enr, 1, 14
#define hw__rcca_windiwwdg		_cb1, apb1enr, 1, 11
#define hw__rcca_counter14		_cb1, apb1enr, 1,  8
#define hw__rcca_counter13		_cb1, apb1enr, 1,  7
#define hw__rcca_counter12		_cb1, apb1enr, 1,  6
#define hw__rcca_counter7		_cb1, apb1enr, 1,  5
#define hw__rcca_counter6		_cb1, apb1enr, 1,  4
#define hw__rcca_counter5		_cb1, apb1enr, 1,  3
#define hw__rcca_counter4		_cb1, apb1enr, 1,  2
#define hw__rcca_counter3		_cb1, apb1enr, 1,  1
#define hw__rcca_counter2		_cb1, apb1enr, 1,  0


/*	Convenient objects
 */
#define hw_class__hsea
#define hw_hse				_hsea, 0
#define hw_hse_cken			_xob1, rcc, cr, 1, 16	/* convenient */

#define hw_class__hsia
#define hw_hsi				_hsia, 0
#define hw_hsi_cken			_xob1, rcc, cr, 1,  0	/* convenient */

#define hw_usbprescaler			_obj, 0
#define hw_adcprescaler			_obj, 0

#define hw_apb1				_apba, 0
#define hw_apb2				_apba, 0


//#define hw_class__rccen


#if !defined __ASSEMBLER__

/*	HWA context
 */
typedef struct {
  hwa_r32_t cr ;
  hwa_r32_t cfgr ;
  hwa_r32_t apb2enr ;
  hwa_r32_t apb1enr ;
} hwa_rcca_t ;

#endif
