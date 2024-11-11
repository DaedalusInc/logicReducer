#ifndef LOGICREADIN_H_
#define LOGICREADIN_H_

enum operator{ NONE = -1, SUBTREE, NAND, NOR, XOR, OR, AND, NOT };

enum child { LEFT, RIGHT, SUB };

typedef struct treeNode {
    enum operator nodeType;
    char *contents;
    int variable;
    enum child side;
    struct treeNode *leftChild;
    struct treeNode *subtreeChild;
    struct treeNode *rightChild;
    struct treeNode *parent;
} treeNode;

treeNode *treeMake(char *);
void printTree(treeNode *, char, int);
void freeTree(treeNode *);

extern char *variables[32];
extern int num_vars;
#endif
