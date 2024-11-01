#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#define OPCOUNT 3

enum operator {
    OR,
    ADD,
    NOT,
    SUBTREE
};

typedef struct treeNode {
    enum operator;
    treeNode* leftChild;
    treeNode* rightChild;
} treeNode;

void addNode(int opType) {
    //(treeNode*)malloc(sizeof(treeNode*));
}

 treeMake(char* inputString) {
    treeNode* rootNode = NULL;
    int i = 0;
    char* operators[OPCOUNT] = {".{0,1} or .{0,1}", ".{0,1} and .{0,1}", ".{0,1}not .{0,1}"}; 

    regex_t regexIn;

    
    for(i = 0; i < OPCOUNT; ++i) {                                                                  //Finds root node
        regcomp(&regexIn, operators[i], REG_ICASE);
        if (!regexec(&regexIn, inputString, 0, NULL, 0)) {
            addNode(i);
            break;
        }
    }
    

    
}