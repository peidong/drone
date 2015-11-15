/** timer.h
 *  Microsecond timer in C, using `gettimeofday()`.
 *
 *  Note: `gettimeofday()` is Unix, GNU/Linux and
 *        Mac OS X system-specific.
 *
 *        The only portable function is time.h's `clock()`,
 *        but it isn't very precise.
 *
 *        See: http://www.songho.ca/misc/timer/timer.html
 *
 *  Copyright (c) 2011,2013 Alexandre Dantas <eu@alexdantas.net>
 */

#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <stdio.h>
#include <sys/time.h>			/* gettimeofday() */
#include <stdbool.h>			/* bool */

typedef struct custom_timer_t
{
	suseconds_t start_mark; 	/* Timer start point */
	suseconds_t pause_mark; 	/* In case we pause the timer */
	bool running;				/* Is it running? */
	bool paused;				/* Is it paused? */

} custom_timer_t;

/** Starts the timer.
 *
 *  @note If called multiple times, restarts the timer.
 */
void timer_start(custom_timer_t* t);

/** Pauses the timer.
 *
 *  @note If called multiple times, it does nothing.
 */
void timer_pause(custom_timer_t* t);

/** Unpauses the timer.
 *
 *  @note If the timer's not paused or this is called
 *        multiple times, it does nothing.
 */
void timer_unpause(custom_timer_t* t);

/** Returns the time difference in microseconds
 *
 * @note (1/1000000 seconds)
 */
suseconds_t timer_delta_us(custom_timer_t* t);

/** Returns the time difference in miliseconds.
 *
 * @note (1/1000 seconds)
 */
long timer_delta_ms(custom_timer_t* t);

/** Returns the time difference in seconds. */
long timer_delta_s(custom_timer_t* t);

/** Returns the time difference in minutes (60 seconds). */
long timer_delta_m(custom_timer_t* t);

/** Returns the time difference in hours (3600 seconds). */
long timer_delta_h(custom_timer_t* t);

#endif


//.c

#define MICROSSECONDS_IN_SECONDS 1000000

/** Local function that returns the ticks
 *	(number of microsseconds) since the Epoch.
 */
static suseconds_t get_ticks()
{
	struct timeval tmp;
	gettimeofday(&(tmp), NULL);

	return (tmp.tv_usec) + (tmp.tv_sec * MICROSSECONDS_IN_SECONDS);
}

void timer_start(custom_timer_t* t)
{
	t->start_mark = get_ticks();
	t->pause_mark = 0;
	t->running	  = true;
	t->paused	  = false;
}

void timer_pause(custom_timer_t* t)
{
	if (!(t->running) || (t->paused)) return;

	t->pause_mark = get_ticks() - (t->start_mark);
	t->running	  = false;
	t->paused	  = true;
}

void timer_unpause(custom_timer_t* t)
{
	if (t->running || !(t->paused)) return;

	t->start_mark = get_ticks() - (t->pause_mark);
	t->running	  = true;
	t->paused	  = false;
}

suseconds_t timer_delta_us (custom_timer_t* t)
{
	if (t->running)
		return get_ticks() - (t->start_mark);

	if (t->paused)
		return t->pause_mark;

	// Will never actually get here
	return (t->pause_mark) - (t->start_mark);
}

long timer_delta_ms(custom_timer_t* t)
{
	return (timer_delta_us(t) / 1000);
}

long timer_delta_s(custom_timer_t* t)
{
	return (timer_delta_us(t) / 1000000);
}

long timer_delta_m(custom_timer_t* t)
{
	return (timer_delta_s(t) / 60);
}

long timer_delta_h(custom_timer_t* t)
{
	return (timer_delta_m(t) / 60);
}
