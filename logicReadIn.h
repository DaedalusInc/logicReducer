enum operator{ NONE = -1, OR, AND, NOT, SUBTREE };

typedef struct treeNode {
    enum operator nodeType;
    char *contents;
    struct treeNode *leftChild;
    struct treeNode *subtreeChild;
    struct treeNode *rightChild;
} treeNode;

treeNode *treeMake(char *);
