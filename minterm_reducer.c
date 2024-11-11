#include "minterm_reducer.h"
#include "logicReadIn.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Implementation of the Quine-McCluskey boolean simplification algorithm

#define BINFMT "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define BINIZEU32(n)                                                                                                                                           \
    (n & 0x80000000) ? '1' : '0', (n & 0x40000000) ? '1' : '0', (n & 0x20000000) ? '1' : '0', (n & 0x10000000) ? '1' : '0', (n & 0x08000000) ? '1' : '0',      \
        (n & 0x04000000) ? '1' : '0', (n & 0x02000000) ? '1' : '0', (n & 0x01000000) ? '1' : '0', (n & 0x00800000) ? '1' : '0', (n & 0x00400000) ? '1' : '0',  \
        (n & 0x00200000) ? '1' : '0', (n & 0x00100000) ? '1' : '0', (n & 0x00080000) ? '1' : '0', (n & 0x00040000) ? '1' : '0', (n & 0x00020000) ? '1' : '0',  \
        (n & 0x00010000) ? '1' : '0', (n & 0x00008000) ? '1' : '0', (n & 0x00004000) ? '1' : '0', (n & 0x00002000) ? '1' : '0', (n & 0x00001000) ? '1' : '0',  \
        (n & 0x00000800) ? '1' : '0', (n & 0x00000400) ? '1' : '0', (n & 0x00000200) ? '1' : '0', (n & 0x00000100) ? '1' : '0', (n & 0x00000080) ? '1' : '0',  \
        (n & 0x00000040) ? '1' : '0', (n & 0x00000020) ? '1' : '0', (n & 0x00000010) ? '1' : '0', (n & 0x00000008) ? '1' : '0', (n & 0x00000004) ? '1' : '0',  \
        (n & 0x00000002) ? '1' : '0', (n & 0x00000001) ? '1' : '0'

uint8_t count_ones_u32(uint32_t in) {
    uint8_t ones = 0;
    for (int i = 0; i < 32; i++) {
        if (in & (1 << i)) {
            ones++;
        }
    }
    return ones;
}

// compare the number of ones in each of the input values
int cmp_ones_u32(uint32_t a, uint32_t b) {
    return count_ones_u32(a) - count_ones_u32(b);
}

// comparison function for the qsort() stdlib function to compare minterms
// by the number of ones in their value.
int cmp_ones_minterm(const void *a, const void *b) {
    term *real_a = (term *)a;
    term *real_b = (term *)b;
    return cmp_ones_u32(real_a->val, real_b->val);
}

// Check if the terms are valid to merge
// This will be true if the values differ by one bit and the masks are the same.
bool check_term_diff(term *a, term *b) {
    uint32_t diff = a->val ^ b->val;
    return a->mask == b->mask && count_ones_u32(diff) == 1;
}

bool terms_eq(term *a, term *b) {
    return a->val == b->val && a->mask == b->mask;
}

// check if the specified term is in the list of terms
bool term_in_arr(term *a, term *arr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (terms_eq(a, &arr[i])) {
            return true;
        }
    }
    return false;
}

// Combine minterms and update the mask
// Assumes that it has already been verified that the terms can be combined
// Construct the output value out of the first input's bits and the mask out
// of whether the bit in that position matches on the two sources.
// Combined minterm is added to the end of the primes parameter if it's not already in that list.
// Otherwise, the function does nothing.
// Returns true if the term was added to the list and false otherwise
bool merge_terms(term *a, term *b, term *primes, size_t cnt) {
    // assume the masks should be equal
    term out = {.mask = a->mask};
    /*    printf("Merging:\n");
        printf("v" BINFMT "\n", BINIZEU32(a->val));
        printf("v" BINFMT "\n", BINIZEU32(b->val));
        printf("m" BINFMT "\n", BINIZEU32(a->mask));
        printf("m" BINFMT "\n", BINIZEU32(b->mask));
    */
    for (size_t i = 0; i < 32; i++) {
        if ((a->val & (1 << i)) == (b->val & (1 << i))) {
            // printf(" s,");
            out.val |= (a->val & (1 << i));
        } else {
            // printf(" d,");
            out.mask |= (1 << i);
        }
    }
    if (term_in_arr(&out, primes, cnt)) {
        return false;
        // puts("");
    }
    // printf("\nGot:\n");
    // printf("v" BINFMT "\n", BINIZEU32(out.val));
    // printf("m" BINFMT "\n", BINIZEU32(out.mask));
    // puts("");
    primes[cnt] = out;
    return true;
}

// Recursive function to combine prime implicants
// This is based on the first part of the quine-mccluskey algorithm.
term *get_primes(term *minterms, size_t len, size_t *outlen) {
    qsort(minterms, len, sizeof(term), cmp_ones_minterm);
    term *primes = malloc(sizeof(term) * len);
    size_t primeslen = len;
    memset(primes, 0, len * sizeof(term));
    bool *merges = malloc(sizeof(bool) * len);
    memset(merges, 0, len * sizeof(bool));
    size_t num_merges = 0;
    /*puts("\n\n\nnew iter inputs:");
    for (size_t i = 0; i < len; i++) {
        printf("v" BINFMT "\n", BINIZEU32(minterms[i].val));
        printf("m" BINFMT "\n", BINIZEU32(minterms[i].mask));
    }*/
    for (size_t i = 0; i < len; i++) {
        for (size_t c = i + 1; c < len; c++) {
            if (check_term_diff(&minterms[i], &minterms[c])) {
                merges[i] = merges[c] = true;
                if (num_merges >= primeslen) {
                    primeslen *= 2;
                    primes = realloc(primes, primeslen * sizeof(term));
                }
                if (merge_terms(&minterms[i], &minterms[c], primes, num_merges)) {
                    num_merges++;
                }
            }
        }
    }
    size_t newidx = num_merges;
    for (size_t i = 0; i < len; i++) {
        if (!merges[i] && !term_in_arr(&minterms[i], primes, newidx)) {
            if (newidx >= primeslen) {
                primeslen *= 2;
                primes = realloc(primes, primeslen * sizeof(term));
            }
            primes[newidx] = minterms[i];
            newidx += 1;
        }
    }
    /*printf("\nnumber merges: %zu\n", num_merges);
    puts("output primes:");
    for (size_t i = 0; i < newidx; i++) {
        printf("v" BINFMT "\n", BINIZEU32(primes[i].val));
        printf("m" BINFMT "\n", BINIZEU32(primes[i].mask));
    }*/
    free(merges);
    if (num_merges != 0) {
        term *out = get_primes(primes, newidx, outlen);
        free(primes);
        return out;
    } else {
        puts("Finished getting prime implicants");
        *outlen = newidx;
        return primes;
    }
}

// Create the prime implicant table and determine the essential prime implicants.
term *tablize(term *primes, size_t plen, term *minterms, size_t mlen, size_t *outlen) {
    uint32_t *vals = malloc(plen * sizeof(uint32_t));
    memset(vals, 0, plen * sizeof(uint32_t));
    for (size_t i = 0; i < plen; i++) {
        uint32_t tester = primes[i].val | primes[i].mask;
        for (size_t c = 0; i < mlen; i++) {
            // Check if the minterm maches with the prime implicant
            if ((minterms[c].val & tester) == minterms[c].val) {
                // each val is associated with one input prime implicant
                vals[i] += 1;
            }
        }
    }
    term *outputs = malloc(plen * sizeof(term));
    memset(outputs, 0, plen * sizeof(term));
    size_t j = 0;
    // any prime implicants that match 1 minterm are essential
    for (size_t i = 0; i < plen; i++) {
        if (vals[i] == 1) {
            outputs[j] = primes[i];
            j++;
        }
    }
    *outlen = j;
    free(vals);
    return outputs;
}

// Return the essential prime implicants from a list of minterms.
// Minterms should be in basic numerical format, e.g. binary 1010 for minterm 10
term *reduce_minterms(uint32_t *minterms, size_t len, size_t *retlen) {
    term *m = malloc(len * sizeof(term));
    memset(m, 0, len * sizeof(term));
    for (size_t i = 0; i < len; i++) {
        m[i].val = minterms[i];
    }
    size_t outlen = 0;
    term *primes = get_primes(m, len, &outlen);
    size_t esslen = 0;
    term *ess = tablize(primes, outlen, m, len, &esslen);
    /*puts("Final outputs:");
    for (size_t i = 0; i < esslen; i++) {
        printf("v" BINFMT "\n", BINIZEU32(ess[i].val));
        printf("m" BINFMT "\n", BINIZEU32(ess[i].mask));
    }*/
    *retlen = esslen;
    return ess;
}

// O(n^2)
char *minterms_to_equation(term *terms, size_t len) {
    char *output = malloc(1000);
    memset(output, 0, 1000);
    char *ptr = output;
    ptr[0] = '(';
    ptr++;
    for (size_t i = 0; i < len; i++) {
        for (int j = 0; j < num_vars; j++) {
            if (!(terms[i].mask & (1 << j))) {
                ptr += sprintf(ptr, "%s%s AND ", terms[i].val & (1 << j) ? "" : "NOT ", variables[j]);
            }
        }
        ptr -= 5;
        ptr += sprintf(ptr, ") OR (");
    }
    ptr -= 6;
    ptr[1] = 0;
    return output;
}
