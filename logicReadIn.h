enum operator{ NONE = -1, SUBTREE, OR, AND, NOT };

typedef struct treeNode {
    enum operator nodeType;
    char *contents;
    struct treeNode *leftChild;
    struct treeNode *subtreeChild;
    struct treeNode *rightChild;
} treeNode;

treeNode *treeMake(char *);
void printTree(treeNode *);
