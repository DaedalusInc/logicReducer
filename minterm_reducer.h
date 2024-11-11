#ifndef MINTERM_REDUCER_H_
#define MINTERM_REDUCER_H_
#include <stddef.h>
#include <stdint.h>

// a minterm/prime implicant
// val is the raw value and mask is 1 in every "combined" position
typedef struct term {
    uint32_t val;
    uint32_t mask;
} term;

term *reduce_minterms(uint32_t *minterms, size_t len, size_t *retlen);
char *minterms_to_equation(term *, size_t);

#endif // MINTERM_REDUCER_H_
