#include "logicReadIn.h"
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool evalTree(treeNode *, uint32_t);

bool tor(treeNode *l, treeNode *r, uint32_t input) {
    return evalTree(l, input) || evalTree(r, input);
}

bool tnor(treeNode *l, treeNode *r, uint32_t input) {
    return !(evalTree(l, input) || evalTree(r, input));
}

bool tnot(treeNode *r, uint32_t input) {
    return !evalTree(r, input);
}

bool tand(treeNode *l, treeNode *r, uint32_t input) {
    return evalTree(l, input) && evalTree(r, input);
}

bool tnand(treeNode *l, treeNode *r, uint32_t input) {
    return !(evalTree(l, input) && evalTree(r, input));
}

bool txor(treeNode *l, treeNode *r, uint32_t input) {
    return evalTree(l, input) ^ evalTree(r, input);
}

uint32_t *execTree(treeNode *root, size_t *term_cnt) {
    size_t num_terms = 1 << num_vars;
    uint32_t *ret = (uint32_t *)malloc(sizeof(uint32_t) * num_terms);
    size_t ret_cnt = 0;
    for (size_t i = 0; i < num_terms; i++) {
        if (evalTree(root, i)) {
            ret[ret_cnt] = i;
            ret_cnt++;
        }
    }
    *term_cnt = ret_cnt;
    return ret;
}

bool evalTree(treeNode *root, uint32_t input) {
    switch (root->nodeType) {
        case OR:
            return tor(root->leftChild, root->rightChild, input);
        case NOR:
            return tnor(root->leftChild, root->rightChild, input);
        case NOT:
            return tnot(root->rightChild, input);
        case AND:
            return tand(root->leftChild, root->rightChild, input);
        case NAND:
            return tnand(root->leftChild, root->rightChild, input);
        case XOR:
            return txor(root->leftChild, root->rightChild, input);
        case NONE:
            return input & (1 << root->variable);
        case SUBTREE:
            if (root->leftChild != NULL) {
                return evalTree(root->leftChild, input);
            } else if (root->subtreeChild != NULL) {
                return evalTree(root->subtreeChild, input);
            } else if (root->rightChild != NULL) {
                return evalTree(root->rightChild, input);
            } else {
                return false;
            }
    }
}
