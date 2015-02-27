
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

#include "hwa_atmelavr__acmpa_1.h"

/*
 *	Analog comparator
 */

/*	Instance			class, name, id, address
 */				        
#define hw_acmp0			_acmpa, acmp0, 700, 0

/*	Class registers			class, rw, ra, rwm, rfm
 */				        
#define hw__acmpa_csr			crg, 8, 0x28, 0xDF, 0x10

/* #define hw__acmpa_acd			cb1, csr, 1, 7 */
/* #define hw__acmpa_acbg			cb1, csr, 1, 6 */
/* #define hw__acmpa_aco			cb1, csr, 1, 5 */
/* #define hw__acmpa_aci			cb1, csr, 1, 4 */
/* #define hw__acmpa_acie			cb1, csr, 1, 3 */
/* #define hw__acmpa_acic			cb1, csr, 1, 2 */
/* #define hw__acmpa_acis			cb1, csr, 2, 0 */

//#define HWA_DCL_ACMPS			hwa__acmpa acmp0
