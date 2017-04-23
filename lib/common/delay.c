#include "delay.h"

uint32_t counter_prescaler_us = 0;
uint32_t counter_prescaler_ms = 0;

#ifndef WIN32
#include "system.h"

static uint32_t dumb_prescaler_us = 0;
static uint32_t dumb_prescaler_ms = 0;

#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC

#ifdef __GNUC__

void delay_ms(uint32_t delay_ms)
{
	uint32_t nb_loop;
	nb_loop = (dumb_prescaler_ms * delay_ms) + 1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
	asm volatile(
		"1: " "\n\t"
		" nop " "\n\t"
		" subs.w %0, %0, #1 " "\n\t"
		" bne 1b " "\n\t"
		: "=r" (nb_loop)
		: "0"(nb_loop)
		: "r3"
	);
}

void delay_us(uint32_t delay_us)
{
	uint32_t nb_loop;
	nb_loop = (dumb_prescaler_us * delay_us) + 1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
	asm volatile(
		"1: " "\n\t"
		" nop " "\n\t"
		" subs.w %0, %0, #1 " "\n\t"
		" bne 1b " "\n\t"
		: "=r" (nb_loop)
		: "0"(nb_loop)
		: "r3"
	);
}

#else

void delay_ms(uint32_t delay_ms)
{
	int r3 = (dumb_prescaler_ms * delay_ms) + 1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
	__asm {		
		loop:
		nop
		subs.w r3, r3, #1
		bne loop		
	}
}

void delay_us(uint32_t delay_us)
{
	int r3 = (dumb_prescaler_us * delay_us) + 1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
	__asm {		
		loop:
		nop
		subs.w r3, r3, #1
		bne loop		
	}
}

#endif

void delay_init() {
	counter_prescaler_ms = SystemCoreClock / 1000;
	counter_prescaler_us = SystemCoreClock / 1000000;
	dumb_prescaler_us =  ((SystemCoreClock >> 2) / 1000000);
	dumb_prescaler_ms =  ((SystemCoreClock >> 2) / 1000);

	if (!(DWT_CONTROL & 1))
	{
	    SCB_DEMCR  |= 0x01000000;
	    DWT_CYCCNT  = 0;
	    DWT_CONTROL|= 1; // enable the counter
	}
}

void reset_counter() {
	DWT_CYCCNT = 0;
}

uint32_t counter() {
	return DWT_CYCCNT;
}
/*
void delay_us(uint32_t t) {
	DWT_CYCCNT = 0;
    int32_t tp = DWT_CYCCNT + t * counter_prescaler_us;
    while (((int32_t)DWT_CYCCNT - tp) < 0);
}

void delay_ms(uint32_t t) {
	DWT_CYCCNT = 0;
    int32_t tp = DWT_CYCCNT + t * counter_prescaler_ms;
    while (((int32_t)DWT_CYCCNT - tp) < 0);
}
*/
#else
#include <Windows.h>

int32_t counter_value = 0;
void dumb_delay_ms(uint32_t delay_ms)
{
	Sleep(delay_ms);
}

void dumb_delay_us(uint32_t delay_us)
{
	Sleep(ceil(delay_us/1000));
}

void delay_init() {
	counter_prescaler_ms = 1;
	counter_prescaler_us = 1;
}

void reset_counter() {
	counter_value = GetTickCount();
}

volatile uint32_t counter() {
	return GetTickCount() - counter_value;
}

void delay_us(uint32_t t) {
    int32_t tp = GetTickCount() + t * counter_prescaler_us;
    while (((int32_t)GetTickCount() - tp) < 0);
}

void delay_ms(uint32_t t) {
    int32_t tp = GetTickCount() + t * counter_prescaler_ms;
    while (((int32_t)GetTickCount() - tp) < 0);
}
#endif

