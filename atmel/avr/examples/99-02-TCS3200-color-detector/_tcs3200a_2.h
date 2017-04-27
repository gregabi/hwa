#ifndef _HWA_TCS3200A_2_H_
#define _HWA_TCS3200A_2_H_

#define _hw_class__tcs3200a

#define _hw_mtd_hwa_configure__tcs3200a	, _hwa_cft3k2a
#define _hw_mtd_hw_read_channel__tcs3200a	, _hw_rdt3k2a

/* TODO
 *  * Verify that the capture/compare have the same counting unit
 *  * Verify that the counting unit is 16-bit
 */

/*  Configure pin S0 if defined
 */
#define _hwa_cft3k2a(o,i,a,...)		HW_X(_hwa_cft3k2a1,_hw_rel_##o##_s0)(o)
#define _hwa_cft3k2a1_0(o)				\
  _hwa(configure,_hw_rel_##o##_s0,direction,output);	\
  _hwa(write,_hw_rel_##o##_s0,0);			\
  _hwa_cft3k2a1_1(o)

/*  Configure pin S1 if defined
 */
#define _hwa_cft3k2a1_1(o)		HW_X(_hwa_cft3k2a2,_hw_rel_##o##_s1)(o)
#define _hwa_cft3k2a2_0(o)				\
  _hwa(configure,_hw_rel_##o##_s1,direction,output);	\
  _hwa(write,_hw_rel_##o##_s1,0);			\
  _hwa_cft3k2a2_1(o)

/*  Configure pin S2, pin S3, capture unit
 */
#define _hwa_cft3k2a2_1(o)				\
  _hwa(configure,_hw_rel_##o##_s2,direction,output);	\
  _hwa(write,_hw_rel_##o##_s2,0);			\
  _hwa(configure,_hw_rel_##o##_s3,direction,output);	\
  _hwa(write,_hw_rel_##o##_s3,0);			\
  _hwa(configure,_hw_rel_##o##_capture,			\
       input, pin_icp,					\
       edge,  rising )


/*  Measure the output signal period of the sensor on channel S3,S2.
 */
#define _hw_tcs3200_channel_red		, 0, 0	/* S3, S2 */
#define _hw_tcs3200_channel_clear	, 0, 1	/* S3, S2 */
#define _hw_tcs3200_channel_blue	, 1, 0	/* S3, S2 */
#define _hw_tcs3200_channel_green	, 1, 1	/* S3, S2 */

#define _hw_rdt3k2a(o,i,a,c,...)	HW_X(_hw_rdt3k2a1,_hw_tcs3200_channel_##c)(o,c)
#define _hw_rdt3k2a1_1(o,c)		_hw_##o##_read_channel(HW_TL(_hw_tcs3200_channel_##c))


#define HW_IMPLEMENT(...)		hw(implement,__VA_ARGS__)

#define _hw_mtd_hw_implement__tcs3200a		, _hw_imt3k2a

#define _hw_imt3k2a(o,i,a,...)		HW_X(_hw_imt3k2a,__VA_ARGS__)(o,__VA_ARGS__)
#define _hw_imt3k2a_0(o,g,...)		HW_E_G(g)
#define _hw_imt3k2a_1(o,...)					\
  _hw_imt3k2a_2(o,_hw_rel_##o##_s2, _hw_rel_##o##_s3,		\
		_HW_REL(_hw_rel_##o##_capture,counter),		\
		_hw_rel_##o##_capture,_hw_rel_##o##_compare)

#define _hw_imt3k2a_2(o,pin_s2,pin_s3,counter,capture,compare)		\
  hw_uint_t(HW_BITS(counter)) _hw_##o##_read_channel(uint8_t s3, uint8_t s2) { \
    hw_uint_t(HW_BITS(counter)) t;					\
    _hw( write, pin_s3, s3!=0 );					\
    _hw( write, pin_s2, s2!=0 );					\
    _hw( configure, capture, edge, rising );				\
    _hw( clear, HW_IRQFLAG(capture) );					\
    _hw( write, compare, _hw(read, counter) );				\
    _hw( clear, HW_IRQFLAG(compare) );					\
    for (;;) {								\
      if ( _hw( read, HW_IRQFLAG(capture) ) ) {				\
	t = _hw( read, capture );					\
	break ;								\
      }									\
      if ( _hw( read, HW_IRQFLAG(compare) ) )				\
	return 0xFFFF ;							\
    }									\
    _hw( configure, capture, edge, falling );				\
    _hw( clear, HW_IRQFLAG(capture) );					\
    _hw( write, compare, t );						\
    _hw( clear, HW_IRQFLAG(compare) );					\
    for (;;) {								\
      if ( _hw( read, HW_IRQFLAG(capture) ) )				\
	return _hw( read, capture ) - t ;				\
      if ( _hw( read, HW_IRQFLAG(compare) ) )				\
	return 0xFFFF ;							\
    }									\
  }

#endif
