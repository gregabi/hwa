
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

/** \file
 *  \brief	HWA layer 1, common declarations for Atmel AVR counter-timers
 */


/*	Compare units
 */
#define hw_class_ocu
#define _hw_pop_ocu(c,n,i,cn,chn,ion,...)	__VA_ARGS__


/*	Capture units
 */
#define hw_class_icu
#define _hw_pop_icu(c,n,i,cn,chn,ion,...)	__VA_ARGS__
