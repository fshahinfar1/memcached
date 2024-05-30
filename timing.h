#ifndef TIMING_H
#define TIMING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MEMBARRIER() asm volatile("": : :"memory")

/* NOTE: Turn on to record the time spent in each step of processing the
 * request
 * */
#ifndef MEASURE_REQ_PROCESSING_TIME
#define MEASURE_REQ_PROCESSING_TIME 1
struct op_timestamp {
	uint64_t ts_recv_req;
	uint64_t ts_parse_cmd;
	uint64_t ts_lookup_begin;
	uint64_t ts_lookup_end;
	uint64_t ts_send;
};
#endif

static inline unsigned long int get_ns(void) {
	MEMBARRIER();
	struct timespec spec;
	memset(&spec, 0, sizeof(struct timespec));
	clock_gettime(CLOCK_MONOTONIC, &spec);
	MEMBARRIER();
	unsigned long int rprt_ts = spec.tv_sec * 1000000000LL + spec.tv_nsec;
	return rprt_ts;
}
#endif
