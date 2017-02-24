
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief EEPROM
 */

/**
 * @page atmelavr_eeproma _eeproma
 * @section atmelavr_eeproma_rw Read and write
 *
 * The `hw_read()` instruction reads one byte at given memory address:
 *
 * @code
 * uint8_t byte = hw_read( eeprom0, 0x42 );  // read byte at address 0x42
 * @endcode
 */
#define _hw_mthd_hw_read__eeproma		, _hw_read_eeproma

#define _hw_read_eeproma(o,i,a,addr,...)	\
  HW_TX( _hw_rdeeproma( _hw_ra(o,ar),		\
			_hw_ra(o,re),		\
			_hw_rbp(o,re),		\
			_hw_ra(o,dr),		\
			addr),			\
	 __VA_ARGS__)

HW_INLINE uint8_t _hw_rdeeproma( intptr_t ar,
				 intptr_t cr, uint8_t cb,
				 intptr_t dr,
				 uint16_t a )
{
  *(volatile uint16_t*)ar = a ;
  *(volatile uint8_t*)cr |= (1<<cb) ;
  return *(volatile uint8_t*) dr ;
}


/**
 * @page atmelavr_eeproma _eeproma
 *
 * The `hw_read_bytes()` instruction reads multiple bytes from given memory
 * address:
 *
 * @code
 * uint16_t address = 0x00F8 ;
 * uint8_t bytes[8] ;
 * hw_read_bytes( eeprom0, &bytes, address, sizeof(bytes) );
 * @endcode
 */
#define _hw_mthd_hw_read_bytes__eeproma		, _hw_eeproma_read_bytes

#define _hw_eeproma_read_bytes(o,i,a,dst,src,n,...)		\
  HW_TX( _hw_eeproma_rdn( (uint8_t*)dst, (intptr_t)src, n,	\
			  _hw_ra(o,ar),				\
			  _hw_ra(o,re),				\
			  _hw_rbp(o,re),			\
			  _hw_ra(o,dr) ), __VA_ARGS__)


HW_INLINE void _hw_eeproma_rdn( uint8_t *dst, intptr_t src, uint8_t n,
				intptr_t ar,
				intptr_t cr, uint8_t cb,
				intptr_t dr)
{
  while ( n-- ) {
    *(volatile uint16_t*)ar = src++ ;
    *(volatile uint8_t*)cr |= (1<<cb) ;
    *dst++ = *(volatile uint8_t*) dr ;
  }
}


/**
 * @page atmelavr_eeproma _eeproma
 *
 * The `hw_write()` instruction writes one byte at given memory address:
 *
 * @code
 * hw_write( eeprom0, 0x42, 42 );  // Write 42 at address 0x42
 * @endcode
 */
#define _hw_mthd_hw_write__eeproma		, _hw_write_eeproma

#define _hw_write_eeproma(o,i,a,addr,v,...)		\
  HW_TX( _hw_wreeproma( addr, v,			\
			_hw_ra(o,ar),			\
			_hw_ra(o,dr),			\
			_hw_ra(o,mpe), _hw_rbp(o,mpe),	\
			_hw_ra(o,pe), _hw_rbp(o,pe) ),	\
	 __VA_ARGS__)

HW_INLINE void _hw_wreeproma( uint16_t a, uint8_t v,
			      intptr_t ar,
			      intptr_t dr,
			      intptr_t mpe_ra, uint8_t mpe_bp,
			      intptr_t pe_ra, uint8_t pe_bp )
{
  /*  Assume EECR is 0, programming mode is 'Erase & write'
   */
  *(volatile uint16_t*)ar = a ;
  *(volatile uint8_t*)dr = v ;
  *(volatile uint8_t*)mpe_ra |= (1U<<mpe_bp) ;
  *(volatile uint8_t*)pe_ra |= (1U<<pe_bp) ;
}


/**
 * @page atmelavr_eeproma _eeproma
 *
 * The `hw_write_bytes()` instruction writes multiple bytes at given
 * memory address:
 *
 * @code
 * extern uint8_t bytes[42] ;
 * hw_write_bytes( eeprom0, 0x0100, bytes, sizeof(bytes) );  // Store 42 bytes at address 0x0100
 * @endcode
 */
#define _hw_mthd_hw_write_bytes__eeproma		, _hw_eeproma_write_bytes

#define _hw_eeproma_write_bytes(o,i,a,dst,src,n,...)		\
  HW_TX( _hw_eeproma_wrn( (intptr_t)(dst), (uint8_t*)(src), n,	\
			  _hw_ra(o,ar),				\
			  _hw_ra(o,dr),				\
			  _hw_ra(o,mpe), _hw_rbp(o,mpe),	\
			  _hw_ra(o,pe), _hw_rbp(o,pe) ),	\
	 __VA_ARGS__)


HW_INLINE void _hw_eeproma_wrn ( intptr_t dst,
				 uint8_t *src, uint8_t n,
				 intptr_t ar,
				 intptr_t dr,
				 intptr_t mpe_ra, uint8_t mpe_bp,
				 intptr_t pe_ra, uint8_t pe_bp )
{
  /*  Assume EECR is 0, programming mode is 'Erase & write'   */

  while ( n-- ) {
    /*
     *  Wait for completion of previous write
     */
    while ( *(volatile uint8_t*)pe_ra & (1U<<pe_bp) ) {}
    /*
     *  Write byte
     */
    *(volatile uint8_t*)dr = *src++ ;
    *(volatile uint16_t*)ar = dst++ ;
    *(volatile uint8_t*)mpe_ra |= (1U<<mpe_bp) ;
    *(volatile uint8_t*)pe_ra |= (1U<<pe_bp) ;
  }
}
