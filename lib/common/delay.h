#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>

extern uint32_t counter_prescaler_us;
extern uint32_t counter_prescaler_ms;

extern void delay_init(void);
extern void reset_counter(void);
extern uint32_t counter(void);
extern void delay_us(uint32_t delay_us);
extern void delay_ms(uint32_t delay_ms);

#define WAIT_COND_BEGIN(x,cond) { \
	uint32_t nwait = x;\
	reset_counter();\
	while (nwait && counter() < nwait && cond) {

#define WAIT_BEGIN(x) { \
	uint32_t nwait = x;\
	reset_counter();\
	while (x && counter() < nwait) {

#define WAIT_END } }

#endif// _DELAY_H_
