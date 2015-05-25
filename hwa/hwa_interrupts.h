
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Interrupts
 *
 * This file contains all that is relative to interrupts.
 */

/*  Interrupt requests are objects of class _irq
 */
#define _hw_class__irq
#define _hw_is__irq__irq			, 1


/*  Object related to an IRQ
 */
#define _hw_mthd_hw_sup__irq			, _hw_sup_irq
#define _hw_sup_irq(vec, p, ie, if, ...)	HW_TX(p, __VA_ARGS__)

/**
 * @ingroup public
 * @brief Get an IRQ object
 *
 * Syntax: `hw_irq( object [, irq_name] )`
 * @hideinitializer
 */
#define hw_irq(...)		_hw_irq2(__VA_ARGS__,,)
#define _hw_irq2(p,c,...)	HW_G2(_hw_irq3,				\
				      HW_IS(_irq,_hw_irq_##p##_##c))(p,c,__VA_ARGS__)
#define _hw_irq3_1(p,c,...)	_hw_irq_##p##_##c
#define _hw_irq3_0(p,...)	HW_G2(_hw_irq4, HW_IS(_irq,_hw_irq_##p))(p,__VA_ARGS__)
#define _hw_irq4_1(p,...)	_hw_irq_##p, __VA_ARGS__
#define _hw_irq4_0(p,...)	HW_G2(_hw_irq5,HW_ISOBJ(p))(p,__VA_ARGS__)
#define _hw_irq5_0(p,...)	HW_ERR("`"#p"` is not a HWA object.")
#define _hw_irq5_1(p,...)	HW_ERR("`"#p"` has no IRQ `` or `" \
				       HW_QUOTE(__VA_ARGS__)"`.")

/*	Definition of an interrupt, extra arguments allowed and returned
 */
#define hw_irqx(...)		HW_G2(_hw_irqx2,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_irqx2_1(...)	__VA_ARGS__
#define _hw_irqx2_0(p,...)						\
  HW_G2(_hw_irqx3, HW_IS(_irq,_hw_irq_##p##_##__VA_ARGS__))(p,__VA_ARGS__)
#define _hw_irqx3_1(p,...)	_hw_irq_##p##_##__VA_ARGS__
#define _hw_irqx3_0(p,...)					\
  HW_G2(_hw_irqx4, HW_IS(_irq,_hw_irq_##p))(p,__VA_ARGS__)
#define _hw_irqx4_1(p,...)	_hw_irq_##p, __VA_ARGS__
#define _hw_irqx4_0(p,...)	HW_G2(_hw_irqx5,HW_ISOBJ(p))(p,__VA_ARGS__)
#define _hw_irqx5_0(p,...)	HW_ERR("`"#p"` is not a HWA object.")
#define _hw_irqx5_1(p,...)	HW_ERR("`"#p"` has no IRQ `` or `" \
				       HW_QUOTE(__VA_ARGS__)"`.")


/**
 * @ingroup public
 * @brief Definition of the enable bit of an IRQ
 *
 * Syntax 1: `hw_irqe( object [, irq_name] )`
 *
 * Syntax 2: `hw_irqe( hw_irq(object [, irq_name]) )`
 * @hideinitializer
 */
#define hw_irqe(...)		_hw_irqe_2(hw_irqx(__VA_ARGS__))
#define _hw_irqe_2(...)		HW_G2(_hw_irqe,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_irqe_0(...)		__VA_ARGS__
#define _hw_irqe_1(t,v,p,e,f)	_hw_reg(p,e)


/**
 * @ingroup public
 * @brief Definition of the flag bit of an IRQ
 *
 * Syntax 1: `hw_irqf( object [, irq_name] )`
 *
 * Syntax 2: `hw_irqf( hw_irq(object [, irq_name]) )`
 * @hideinitializer
 */
#define hw_irqf(...)		_hw_irqf_2(hw_irqx(__VA_ARGS__))
#define _hw_irqf_2(...)		HW_G2(_hw_irqf,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_irqf_0(...)		__VA_ARGS__
#define _hw_irqf_1(t,v,p,e,f)	_hw_reg(p,f)


/**
 * @ingroup public
 * @brief Declaration of an ISR
 *
 * The `HW_ISR(...)` instruction accepts 2 optionnal arguments after the
 * definition of the IRQ:
 *
 * @li `isr_naked`: ask the compiler to not initialize any register. You have to
 *   put the `reti` instruction yourself.
 * @li `isr_interruptible`: ask the compiler to enable the interrupts as soon as
 *   possible.
 * @li `isr_non_interruptible`: can be use to assert that the interrupts are not
 *   enabled.
 *
 * Syntax 1:
 * @code
 * HW_ISR( object [, irq_name] [, isr_naked ] [, isr_interruptible | isr_non_interruptible] )
 * {
 *    ... // Service the ISR
 *
 *    [ hw_reti(); ]
 * }
 * @endcode
 *
 * Syntax 2:
 * @code
 * HW_ISR( hw_irq(object [, irq_name]) [, isr_naked ] [, isr_interruptible | isr_non_interruptible] )
 * {
 *    ... // Service the ISR
 *
 *    [ hw_reti(); ]
 * }
 * @endcode
 * @hideinitializer
 */
#define HW_ISR(...)			_hw_isr_2(hw_irqx(__VA_ARGS__,))
#define _hw_isr_2(...)			HW_G2(_hw_isr2,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_isr2_0(...)			HW_ERRFN(__VA_ARGS__)
#define _hw_isr2_1(irq,v,n,e,f,...)	HW_G2(_hw_isr_a1, HW_IS(,__VA_ARGS__))(__VA_ARGS__, v)

#define _hw_isr_a1_1(_,...)		_hw_isr_(__VA_ARGS__,)
#define _hw_isr_a1_0(x,...)		HW_G2(_hw_isr_a1_0, HW_IS(,hw_israttr_##x))(x,__VA_ARGS__)
#define _hw_isr_a1_0_0(x,...)		HW_ERRFN(HW_ERR("`"#x"` is not a valid ISR attribute."))
#define _hw_isr_a1_0_1(x,...)		\
  HW_G2(_hw_isr_a2,HW_IS(,__VA_ARGS__))(__VA_ARGS__, HW_A1(hw_israttr_##x))

#define _hw_isr_a2_1(_,...)		_hw_isr_(__VA_ARGS__)
#define _hw_isr_a2_0(x,...)		HW_G2(_hw_isr_a2_0, HW_IS(,hw_israttr_##x))(x,__VA_ARGS__)
#define _hw_isr_a2_0_0(x,...)		HW_ERRFN(HW_ERR("`"#x"` is not a valid ISR attribute."))
#define _hw_isr_a2_0_1(x,...)		\
  HW_G2(_hw_isr_a3,HW_IS(,__VA_ARGS__))(__VA_ARGS__ HW_A1(hw_israttr_##x))

#define _hw_isr_a3_1(_,...)		_hw_isr_(__VA_ARGS__)
#define _hw_isr_a3_0(x,...)		HW_ERRFN(HW_ERR("garbage starting with `" #x "...`"))


#if !defined __ASSEMBLER__


#define _hw_mthd_hw_turn__irq		, _hw_turn_irq
/**
 * @ingroup private
 * @brief  Turn an IRQ on/off
 * @hideinitializer
 */
#define _hw_turn_irq(v,p,e,f, ...)					\
  HW_G2(_hw_turn_irq_vstate, HW_IS(,hw_state_##__VA_ARGS__))(p,e, __VA_ARGS__,)
#define _hw_turn_irq_vstate_0(p,e, ...)					\
  HW_ERR("expected `on` or `off`, got `" HW_QUOTE(__VA_ARGS__) "` instead.")
#define _hw_turn_irq_vstate_1(p,e,v, ...)				\
    HW_TX(_hw_write_reg( p,e, HW_A1(hw_state_##v)), __VA_ARGS__)


#define _hw_mthd_hwa_turn__irq		, _hwa_turn_irq
/**
 * @ingroup private
 * @brief  Turn an IRQ on/off
 * @hideinitializer
 */
#define _hwa_turn_irq(v,p,e,f, ...)					\
  HW_G2(_hwa_turn_irq_vstate, HW_IS(,hw_state_##__VA_ARGS__))(p,e, __VA_ARGS__,)
#define _hwa_turn_irq_vstate_0(p,e, ...)				\
  HW_ERR("expected `on` or `off`, got `" HW_QUOTE(__VA_ARGS__) "` instead.")
#define _hwa_turn_irq_vstate_1(p,e,v, ...)			\
  HW_TX(_hwa_write_reg(p,e, HW_A1(hw_state_##v)), __VA_ARGS__)


/**
 * @ingroup public
 * @brief  Read an IRQ enable bit
 *
 * Syntax: `hw_read_irqe( object [, irq_name] )`
 * @hideinitializer
 */
#define hw_read_irqe(...)		_hw_readirqe_2(hw_irq(__VA_ARGS__))
#define _hw_readirqe_2(...)		HW_G2(_hw_readirqe,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_readirqe_0(...)		__VA_ARGS__
#define _hw_readirqe_1(t,v,o,e,f,...)	_hw_read_reg(o,e)


/**
 * @ingroup public
 * @brief  Read an IRQ flag bit
 *
 * Syntax: `hw_read_irqf( object [, irq_name] )`
 * @hideinitializer
 */
#define hw_read_irqf(...)		_hw_readirqf_2(hw_irq(__VA_ARGS__))
#define _hw_readirqf_2(...)		HW_G2(_hw_readirqf,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_readirqf_0(...)		__VA_ARGS__
#define _hw_readirqf_1(t,v,o,e,f,...)	_hw_read_reg(o,f)


/**
 * @ingroup public
 * @brief  Turn an IRQ on/off
 * @hideinitializer
 *
 * Short for `hw_turn( hw_irq(...), ... )`
 */
#define hw_turn_irq(...)		_hw_turnirq_2(hw_irqx(__VA_ARGS__,))
#define _hw_turnirq_2(...)		HW_G2(_hw_turnirq,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_turnirq_0(...)		__VA_ARGS__
#define _hw_turnirq_1(t,...)		_hw_turn_irq(__VA_ARGS__)


/**
 * @ingroup public
 * @brief  Turn an IRQ on/off
 * @hideinitializer
 *
 * Short for `hwa_turn( hw_irq(...), on | off )`
 *
 * Syntax: `hwa_turn_irq( object [, irq_name], on | off );`
 */
#define hwa_turn_irq(...)		_hwa_turnirq_2(hw_irqx(__VA_ARGS__,))
#define _hwa_turnirq_2(...)		HW_G2(_hwa_turnirq,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hwa_turnirq_0(...)		__VA_ARGS__
#define _hwa_turnirq_1(t,...)		_hwa_turn_irq(__VA_ARGS__)


/**
 * @ingroup private
 * @brief  Clear an IRQ flag
 * @hideinitializer
 */
#define _hw_mthd_hw_clear__irq		, _hw_clear_irq
#define _hw_clear_irq(v,p,e,f, ... )	HW_TX(_hw_write_reg(p,f, 1 ), __VA_ARGS__)


/**
 * @ingroup private
 * @brief  Clear irq flag
 * @hideinitializer
 */
#define _hw_mthd_hwa_clear__irq		, _hwa_clear_irq
#define _hwa_clear_irq(v,p,e,f, ... )	HW_TX(_hwa_write_reg(p,f, 1 ), __VA_ARGS__)


/**
 * @ingroup public
 * @brief	Clear an IRQ flag
 * @hideinitializer
 *
 * Syntax: `hw_clear_irqf( object [, irq_name] );`
 */
#define hw_clear_irqf(...)		_hw_clearirq_2(hw_irqx(__VA_ARGS__,))


/**
 * @ingroup public
 * @brief	Clear an IRQ flag
 * @hideinitializer
 *
 * Short for `hw_clear( hw_irq(...) )`
 *
 * Syntax: `hw_clear_irq( object [, irq_name] );`
 */
#define hw_clear_irq(...)		_hw_clearirq_2(hw_irqx(__VA_ARGS__,))
#define _hw_clearirq_2(...)		HW_G2(_hw_clearirq,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hw_clearirq_0(...)		__VA_ARGS__
#define _hw_clearirq_1(t,...)		_hw_clear_irq(__VA_ARGS__)


/**
 * @ingroup public
 * @brief	Clear an IRQ flag
 * @hideinitializer
 *
 * Syntax: `hwa_clear_irqf( object [, irq_name] );`
 */
#define hwa_clear_irqf(...)		_hwa_clearirq_2(hw_irqx(__VA_ARGS__,))

/**
 * @ingroup public
 * @brief	Clear an IRQ flag
 * @hideinitializer
 *
 * Short for `hwa_clear( hw_irq(...) )`
 *
 * Syntax: `hwa_clear_irq( object [, irq_name] );`
 */
#define hwa_clear_irq(...)		_hwa_clearirq_2(hw_irqx(__VA_ARGS__,))
#define _hwa_clearirq_2(...)		HW_G2(_hwa_clearirq,HW_IS(_irq,__VA_ARGS__))(__VA_ARGS__)
#define _hwa_clearirq_0(...)		__VA_ARGS__
#define _hwa_clearirq_1(t,...)		_hwa_clear_irq(__VA_ARGS__)


/**
 * @ingroup private
 * @brief	Stat irq flag
 * @hideinitializer
 */
#define _hw_mthd_hw_stat__irq		, _hw_stat_irq
#define _hw_stat_irq(v,o,e,f, ... )	HW_TX((_hw_read_reg(o,e)==1), __VA_ARGS__)

#endif /* !defined __ASSEMBLER__ */