#ifndef TIMING_H
#define TIMING_H
static inline unsigned long int get_ns(void) {
	struct timespec spec = {};
	clock_gettime(CLOCK_MONOTONIC, &spec);
	unsigned long int rprt_ts = spec.tv_sec * 1000000000LL + spec.tv_nsec;
	return rprt_ts;
}
#endif
