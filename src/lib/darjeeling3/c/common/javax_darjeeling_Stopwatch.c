#include "djtimer.h"

#ifdef AVRORA

void javax_darjeeling_Stopwatch_void_resetAndStart() {
	avroraStartTimer();
}

void javax_darjeeling_Stopwatch_void_measure() {
	avroraStopTimer();
}

#else // AVRORA

dj_time_t stopwatch_start_time;

void javax_darjeeling_Stopwatch_void_resetAndStart() {
	stopwatch_start_time = dj_timer_getTimeMillis();
}

void javax_darjeeling_Stopwatch_void_measure() {
	DARJEELING_PRINTF("%c[31mSTOPWATCH: %lld ms", 0x1b, dj_timer_getTimeMillis() - stopwatch_start_time);
	DARJEELING_PRINTF("%c[0m\n", 0x1b); // Not sure why this doesn't work in one printf, but it doesn't.
}

#endif // AVRORA