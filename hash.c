/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "memcached.h"
#include "jenkins_hash.h"
#include "murmur3_hash.h"
#define XXH_INLINE_ALL // modifier for xxh3's include below
#include "xxhash.h"

#include <stdio.h>
#include "timing.h"

hash_func hash;

static uint32_t XXH3_hash(const void *key, size_t length) {
    return (uint32_t)XXH3_64bits(key, length);
}

static uint32_t dummy_hash(const void *key, size_t length) {
    uint32_t x = 0;
    if (length < 4) {
        return 0;
    }
    x = *(uint32_t *)key;
    return x;
}


static uint32_t sample_size = 0;
static uint64_t *samples;
#define REPORT_THRESHOLD 10000L
/* A wrapper function for measuring the time spent performing hash function
 * */
static uint32_t measure_hash_time(const void *key, size_t length) {
    uint64_t begin, duration;
    uint32_t h;
    /* uint32_t s_index = __atomic_fetch_add(&sample_size, 1, __ATOMIC_RELAXED); */
    begin = get_ns();
    h = MurmurHash3_x86_32(key, length);
    duration = get_ns() - begin;
    printf("%lu\n", duration);
    /* samples[s_index] = duration; */
    /* if (s_index == REPORT_THRESHOLD) { */
    /*     for (uint32_t i = 0; i < REPORT_THRESHOLD; i++) { */
    /*         printf("%lu\n", samples[i]); */
    /*     } */
    /*     sample_size = 0; */
    /* } */
    return h;
}

int hash_init(enum hashfunc_type type) {
    sample_size = 0;
    samples = malloc(1024L * 1024L * 1024L * sizeof(uint64_t));
    switch(type) {
        case JENKINS_HASH:
            hash = jenkins_hash;
            settings.hash_algorithm = "jenkins";
            break;
        case MURMUR3_HASH:
            hash = MurmurHash3_x86_32;
            /* printf("annotate the hash function\n"); */
            /* hash = measure_hash_time; */
            settings.hash_algorithm = "murmur3";
            break;
        case XXH3_HASH:
            hash = XXH3_hash;
            settings.hash_algorithm = "xxh3";
            break;
        case DUMMY_HASH:
            printf("Dummy hash selected\n");
            hash = dummy_hash;
            settings.hash_algorithm = "dummy";
            break;
        default:
            return -1;
    }
    return 0;
}
