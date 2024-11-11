/*Process:
  Regex for c does not have a way to return the location of the found delimiter, at least that I can find.
  As such, after turning the entire inputted char array to uppercase, strstr is used to identify the location of an operator.
  Then, strncpy is used to copy the two halves of the string separated by the delimiter.

  Code is not yet tested, there are a few spots where I am pretty certain there are errors.*/

#include "logicReadIn.h"
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPCOUNT 7

void processString(treeNode *inputNode);

void setBlankNode(treeNode *inputNode) {
    inputNode->leftChild = NULL;
    inputNode->rightChild = NULL;
    inputNode->subtreeChild = NULL;
}

treeNode *treeMake(char *inputString) {
    // strupr function is only available in some ancient microsoft C version
    for (int i = 0; i < strlen(inputString); i++) {
        inputString[i] = toupper(inputString[i]);
    }
    treeNode *rootNode = malloc(sizeof(treeNode));
    rootNode->nodeType = NONE;
    rootNode->contents = inputString;
    rootNode->leftChild = rootNode->rightChild = rootNode->subtreeChild = NULL;
    processString(rootNode);
    return rootNode;
}

void processString(treeNode *inputNode) {
    regex_t reegex;
    regcomp(&reegex, "\\([^()]+\\)", REG_EXTENDED);
    regmatch_t matchList;
    char *opPtr;
    int beginOp = 0;
    int endOp = 0;
    char *operators[OPCOUNT] = {"(", " NAND ", " NOR ", " XOR ", " OR ", " AND ", "NOT "};
    int i = 0;

    printf("Starting process of '%s'\n", inputNode->contents);
    for (i = 0; i < OPCOUNT; ++i) {
        opPtr = strstr(inputNode->contents, operators[i]);

        if (opPtr != NULL) {
            printf("Found op %s at %p\n", operators[i], opPtr);
            beginOp = strlen(inputNode->contents) - strlen(opPtr);
            endOp = beginOp + strlen(operators[i]);

            if (i == SUBTREE) {
                printf("Entering subtree\n");
                inputNode->nodeType = i;
                if (!regexec(&reegex, inputNode->contents, 1, &matchList, 0)) {
                    printf("Regex matched\n");
                    inputNode->subtreeChild = (treeNode *)malloc(sizeof(treeNode));
                    inputNode->subtreeChild->parent = inputNode;
                    inputNode->subtreeChild->contents = (char *)malloc(matchList.rm_eo - matchList.rm_so);
                    strncpy(inputNode->subtreeChild->contents, &inputNode->contents[matchList.rm_so + 1], matchList.rm_eo - matchList.rm_so - 2);
                    setBlankNode(inputNode->subtreeChild);
                    processString(inputNode->subtreeChild);
                    if (matchList.rm_so != 0) {
                        inputNode->leftChild = (treeNode *)malloc(sizeof(treeNode));
                        inputNode->leftChild->parent = inputNode;
                        inputNode->leftChild->contents = (char *)malloc(matchList.rm_so + 1);
                        memset(inputNode->leftChild->contents, 0, matchList.rm_so + 1);
                        strncpy(inputNode->leftChild->contents, inputNode->contents, matchList.rm_so);
                        setBlankNode(inputNode->leftChild);
                        processString(inputNode->leftChild);
                    }
                    if (matchList.rm_so != strlen(inputNode->contents)) {
                        inputNode->rightChild = (treeNode *)malloc(sizeof(treeNode));
                        inputNode->rightChild->parent = inputNode;
                        size_t len = strlen(inputNode->contents) - matchList.rm_eo;
                        inputNode->rightChild->contents = (char *)malloc(len + 1);
                        memset(inputNode->rightChild->contents, 0, len + 1);
                        strncpy(inputNode->rightChild->contents, &inputNode->contents[matchList.rm_eo], len);
                        setBlankNode(inputNode->rightChild);
                        processString(inputNode->rightChild);
                    }
                }
                break;
            } else if ((i == OR || i == AND || i == NAND || i == NOR || i == XOR)) {
                inputNode->nodeType = i;

                if (beginOp == 0) {
                    if (inputNode->parent != NULL) {
                        if (inputNode->parent->subtreeChild != NULL) {
                            inputNode->leftChild = inputNode->parent->subtreeChild;
                            inputNode->parent->subtreeChild = NULL;
                        } else if (inputNode->parent->leftChild != NULL) {
                            inputNode->leftChild = inputNode->parent->leftChild;
                            inputNode->parent->leftChild = NULL;
                        }
                    }
                } else {
                    inputNode->leftChild = (treeNode *)malloc(sizeof(treeNode));
                    inputNode->leftChild->parent = inputNode;
                    inputNode->leftChild->contents = (char *)malloc(beginOp + 1);
                    memset(inputNode->leftChild->contents, 0, beginOp + 1);
                    strncpy(inputNode->leftChild->contents, inputNode->contents, beginOp);
                    setBlankNode(inputNode->leftChild);
                    processString(inputNode->leftChild);
                }

                if (endOp == strlen(inputNode->contents)) {
                    if (inputNode->parent != NULL) {
                        if (inputNode->parent->subtreeChild != NULL) {
                            inputNode->rightChild = inputNode->parent->subtreeChild;
                            inputNode->parent->subtreeChild = NULL;
                        } else if (inputNode->parent->rightChild != NULL) {
                            inputNode->rightChild = inputNode->parent->rightChild;
                            inputNode->parent->rightChild = NULL;
                        }
                    }
                } else {
                    inputNode->rightChild = (treeNode *)malloc(sizeof(treeNode));
                    inputNode->rightChild->parent = inputNode;
                    inputNode->rightChild->contents = (char *)malloc(strlen(inputNode->contents) - endOp + 1);
                    memset(inputNode->rightChild->contents, 0, strlen(inputNode->contents) - endOp + 1);
                    strncpy(inputNode->rightChild->contents, &(inputNode->contents[endOp]), strlen(inputNode->contents) - endOp);
                    setBlankNode(inputNode->rightChild);
                    processString(inputNode->rightChild);
                }

                break;
            } else if (i == NOT) {
                inputNode->nodeType = i;
                if (endOp == strlen(inputNode->contents)) {
                    treeNode *new = inputNode->parent;
                    while (new != NULL) {
                        if (new->nodeType != NOT) {
                            if (new->parent != NULL) {
                                if (new->parent->subtreeChild != NULL) {
                                    inputNode->rightChild = new->parent->subtreeChild;
                                    new->parent->subtreeChild = NULL;
                                } else if (new->parent->rightChild != NULL) {
                                    inputNode->rightChild = new->parent->rightChild;
                                    new->parent->rightChild = NULL;
                                }
                            }
                            break;
                        }
                        new = new->parent;
                    }
                } else {
                    inputNode->rightChild = (treeNode *)malloc(sizeof(treeNode));
                    inputNode->rightChild->parent = inputNode;
                    inputNode->rightChild->contents = (char *)malloc(beginOp + 1);
                    memset(inputNode->rightChild->contents, 0, beginOp + 1);
                    strncpy(inputNode->rightChild->contents, inputNode->contents, beginOp);
                    setBlankNode(inputNode->rightChild);
                    processString(inputNode->rightChild);
                }

                break;
            }
        }
    }
}

void printTree(treeNode *root, char c) {
    if (root == NULL) {
        return;
    }
    printf("%c '%s'\n", c, root->contents);
    printTree(root->leftChild, 'l');
    printTree(root->subtreeChild, 's');
    printTree(root->rightChild, 'r');
}
