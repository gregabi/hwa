
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */


/*	Class methods
 */
#define hw_def_hw_config_core		, _hw_config_core
#define hw_def_hwa_config_core		, _hwa_config_core
#define hw_def_hw_stat_core		, _hw_stat_core


/*	Configure the MCU sleep mode
 *
 *	hwa_config( hw_core,
 *
 *		    sleep,        enabled
 *                              | disabled,
 *
 *		    sleep_mode,   idle
 *			        | adc_noise_reduction
 *			        | power_down );
 *
 *	TODO: use a list of wake-up sources instead (or in addition)?
 */

#define hw_is_sleep_sleep			, 1
#define hw_is_sleep_mode_sleep_mode		, 1
#define hw_sleepmode_idle			, 0
#define hw_sleepmode_adc_noise_reduction	, 1
#define hw_sleepmode_power_down			, 2
#define hw_sleepmode_standby			, 3

/* #define hw_is_awaker_awaker		, 1 */
/* #define hw_awaker_io			, 0 */
/* #define hw_awaker_int0_edge		, 0 */
/* #define hw_awaker_adc			, 1 */
/* #define hw_awaker_flash			, 1 */
/* #define hw_awaker_eeprom		, 1 */
/* #define hw_awaker_int0_level		, 2 */
/* #define hw_awaker_pinchange		, 2 */
/* #define hw_awaker_usi_start_condition	, 2 */
/* #define hw_awaker_watchdog		, 2 */


#define _hwa_config_core(c,n,i,a,...)					\
  do {									\
    uint8_t sleep = 0xFF ;						\
    uint8_t sleep_mode = 0xFF ;						\
    HW_G2(_hwa_cfcore_xsleep, HW_IS(sleep,__VA_ARGS__))(__VA_ARGS__);	\
  } while(0)

#define _hwa_cfcore_xsleep_0(...)					\
  HW_ERR("expected `sleep`, got `" HW_QUOTE(__VA_ARGS__) "` instead.")

#define _hwa_cfcore_xsleep_1(kw,...)					\
  HW_G2(_hwa_cfcore_vsleep, HW_IS(,hw_state_##__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_vsleep_0(...)					\
  HW_ERR("expected `enabled` or `disabled`, got `" HW_QUOTE(__VA_ARGS__) "` instead.")

#define _hwa_cfcore_vsleep_1(v,...)					\
  sleep = HW_A1(hw_state_##v);						\
  _hwa_write_reg(hw_core0, se, sleep );					\
  HW_G2(_hwa_cfcore_xsleepmode, HW_IS(sleep_mode,__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_xsleepmode_0(...)	_hwa_cfcore_end(__VA_ARGS__)

#define _hwa_cfcore_xsleepmode_1(kw,...)				\
  HW_G2(_hwa_cfcore_vsleepmode, HW_IS(,hw_sleepmode_##__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_vsleepmode_0(v,...)					\
  HW_ERR("`sleep_mode` can be `idle`, `adc_noise_reduction`, or "	\
	 "`power_down`, but not `" HW_QUOTE(v) "`.")

#define _hwa_cfcore_vsleepmode_1(v,...)		\
  sleep_mode = HW_A1(hw_sleepmode_##v);		\
  _hwa_cfcore_end(__VA_ARGS__)

/*   HW_G2(_hwa_cfcore_xawaker1, HW_IS(awaker,__VA_ARGS__))(__VA_ARGS__) */

/* #define _hwa_cfcore_xawaker1_0(...)	_hwa_cfcore_end(__VA_ARGS__) */

/* #define _hwa_cfcore_xawaker1_1(kw,...)					\ */
/*   HW_G2(_hwa_cfcore_vawaker1, HW_IS(,hw_awaker_##__VA_ARGS__))(__VA_ARGS__) */

/* #define _hwa_cfcore_vawaker1_0(v,...)				\ */
/*   HW_ERR("`awaker` can be `int0`, `pinchange`, `adc`, or "	\ */
/* 	 "`watchdog`, but not `" HW_QUOTE(v) "`.") */

/* #define _hwa_cfcore_vawaker1_1(v,...)		\ */
/*   level = HW_A1(hw_awaker_##v);			\ */
/*   if ( level < awaker_level )			\ */
/*     awaker_level = level ;			\ */
/*   _hwa_cfcore_end(__VA_ARGS__) */

#define _hwa_cfcore_end(...)			\
  if ( sleep == 1 )				\
    _hwa_write_reg(hw_core0, sm, sleep_mode);	\
  HW_TX(,__VA_ARGS__)


/*	Core status
 *
 *	hw_stat_t(...) declares the structure that holds the status
 *	hw_stat(...) reads and returns the status
 *
 *	The only flag that is available is the irq flag.
 */
typedef union {
  uint8_t         byte ;
  struct {
    unsigned int porf   : 1 ;
    unsigned int extrf  : 1 ;
    unsigned int borf   : 1 ;
    unsigned int wdrf   : 1 ;
    unsigned int __4to7 : 4 ;
  };
  struct {
    unsigned int reset_by_power_on  : 1 ;
    unsigned int reset_by_reset_pin : 1 ;
    unsigned int reset_by_brown_out : 1 ;
    unsigned int reset_by_watchdog  : 1 ;
    unsigned int __4to7_2           : 4 ;
  };
} _hw_core_stat_t ;


#define hw_def_hw_stat_t_core		, _hw_core_stat_t
#define _hw_core_stat_t(c,n,i,a,...)	HW_TX(_hw_core_stat_t, __VA_ARGS__)


#define _hw_stat_core(c,n,i,a)		_hw_core_stat(_hw_read_reg(c,n,i,a, mcusr))

HW_INLINE _hw_core_stat_t _hw_core_stat( uint8_t byte )
{
  _hw_core_stat_t	st ;
  st.byte = byte ;
  return st ;
}