typedef struct treeNode {
    enum operator nodeType;
    char* contents;
    treeNode* leftChild;
    treeNode* subtreeChild;
    treeNode* rightChild;
} treeNode;

treeNode* treeMake(char*);