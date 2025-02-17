#include "timer.h"
#include "interrupt.h"
#include "cpu.h"

static unsigned int prev_time;
static unsigned int elapsed;
static unsigned int ticks;

static unsigned char tac;
static unsigned int started;
static unsigned int speed;
static unsigned int counter;
static unsigned int divider;
static unsigned int modulo;

void timer_set_div(unsigned char v) {
	(void) v;
	divider = 0;
}

unsigned char timer_get_div() {
	return divider;
}

void timer_set_counter(unsigned char v) {
	counter = v;
}

unsigned char timer_get_counter() {
	return counter;
}

void timer_set_modulo(unsigned char v) {
	modulo = v;
}

unsigned char timer_get_modulo() {
	return modulo;
}

void timer_set_tac(unsigned char v) {
	int speeds[] = {64, 1, 4, 16};

	tac = v;
	started = v & 4;
	speed = speeds[v & 3];
}

unsigned char timer_get_tac() {
	return tac;
}

static void timer_tick() {
	ticks++;

	if(ticks == 16) {
		divider++;
		ticks = 0;
	}

	if(!started)
		return;

	if(ticks == speed){
		counter++;
		ticks = 0;
	}

	if(counter == 0x100) {
		interrupt(INTR_TIMER);
		counter = modulo;
	}
}

void timer_cycle() {
	unsigned int delta = cpu_get_cycles() - prev_time;

	prev_time = cpu_get_cycles();
	elapsed += delta * 4;

	if(elapsed >= 16) {
		timer_tick();
		elapsed -= 16;
	}
}