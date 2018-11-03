
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief I/O port
 */


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__p8a(o,i,a)			\
  _hwa_setup_or( o, port );			\
  _hwa_setup_or( o, ddr  )

#define _hwa_init__p8a(o,i,a)			\
  _hwa_init_or( o, port, 0x00 );		\
  _hwa_init_or( o, ddr,  0x00 )

#define _hwa_commit__p8a(o,i,a)		\
  _hwa_commit_or( o, port );			\
  _hwa_commit_or( o, ddr  )


/**
 * @page atmelavr_p8a Class _p8a: 8-bit I/O port
 * <br>
 */
