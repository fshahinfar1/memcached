#ifndef HASH_H
#define    HASH_H

typedef uint32_t (*hash_func)(const void *key, size_t length);
extern hash_func hash;

enum hashfunc_type {
    JENKINS_HASH=0, MURMUR3_HASH, XXH3_HASH, DUMMY_HASH
};

int hash_init(enum hashfunc_type type);

#endif    /* HASH_H */

