
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

/** \file
 *  \brief	Analog comparator
 *
 */

/*	Initialize a acmp instance
 */
#define _hwa_begin_acmps( hwa )			\
  HWA_INIT(hw_acmp0, csr);


HW_INLINE void _hwa_reset_acmps ( hwa_t *hwa )
{
  _hwa_reset_r8( &hwa->acmp0.csr );
}


HW_INLINE void _hwa_commit_acmps ( hwa_t *hwa )
{
  _hwa_commit_r8( hwa->commit, &hwa->acmp0.csr,  -1 );
}
