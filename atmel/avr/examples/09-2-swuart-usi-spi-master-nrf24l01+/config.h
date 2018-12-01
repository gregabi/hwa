#ifndef CONFIG_H
#define CONFIG_H

/*
 *  NOTE: do not leave the RX pin floating. Fix-it to high level through 10 kohms.
 */
#define hw_swuart0_pin_txd		DIABOLO_PIN_TX
#define hw_swuart0_pin_rxd		DIABOLO_PIN_RX
#define hw_swuart0_starter		(DIABOLO_PIN_RX,pcic)
#define hw_swuart0_autosync		5_1
#define hw_swuart0_compare		(counter1,compare0)
#define hw_swuart0_clk_div		1

#define UART				swuart0

#include BOARD_H

#endif
