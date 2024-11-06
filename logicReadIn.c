/*Process:
  Regex for c does not have a way to return the location of the found delimiter, at least that I can find.
  As such, after turning the entire inputted char array to uppercase, strstr is used to identify the location of an operator.
  Then, strncpy is used to copy the two halves of the string separated by the delimiter.
  
  Code is not yet tested, there are a few spots where I am pretty certain there are errors.*/

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include "logicReadIn.h"
#define OPCOUNT 4

enum operator {
    NONE = -1,
    OR,
    AND,
    NOT,
    SUBTREE
};

void setBlankNode(treeNode* inputNode) {
    inputNode->nodeType = NONE;
    inputNode->leftChild = NULL;
    inputNode->rightChild = NULL;
    inputNode->subtreeChild = NULL;
}

treeNode* treeMake(char* inputString) {
    treeNode rootNode = {NONE, strupr(inputString), NULL, NULL};
    processString(&rootNode);
}

void processString(treeNode* inputNode) {
    char* opPtr;
    int beginOp = 0;
    int endOp = 0;
    char* operators[OPCOUNT] = {"(", " OR ", " AND ", "NOT "}; 
    int i = 0;
    
    for(i = 0; i < OPCOUNT; ++i) {
        opPtr = strstr(inputNode->contents, operators[i]);
        
        if(opPtr != NULL) {
            beginOp = strlen(inputNode->contents)-strlen(opPtr);
            endOp = beginOp + strlen(operators[i]);

            if(i == SUBTREE) {
                inputNode->nodeType = i;
                //To Be Developed.
                //Parentheses split functions into 3 separate parts, A(B)C, which is why there is now a subtree ptr in the treeNode struct
                //Steps:
                //Create stack of open and close parentheses
                //Takes first opened set of parentheses, puts all text inside into subtree ptr
                //Puts text to the left and right into left and right ptrs
                //Processes all 3 strings
                break;
            } else if((i == OR || i == AND) && !strlen(opPtr)) {
                inputNode->nodeType = i;

                inputNode->leftChild = (treeNode*)malloc(sizeof(treeNode*));
                strncpy(inputNode->leftChild->contents, inputNode->contents, beginOp);
                setBlankNode(inputNode->leftChild);
                processString(inputNode->leftChild);

                inputNode->rightChild = (treeNode*)malloc(sizeof(treeNode*));
                strncpy(inputNode->rightChild->contents, opPtr, strlen(inputNode->contents)-endOp);
                setBlankNode(inputNode->rightChild);
                processString(inputNode->rightChild);

                break;
            } else if(i == NOT) {
                inputNode->nodeType = i;

                inputNode->rightChild = (treeNode*)malloc(sizeof(treeNode*));
                strncpy(inputNode->rightChild->contents, inputNode->contents, beginOp);
                processString(inputNode->rightChild);
                setBlankNode(inputNode->rightChild);

                break;
            }
        }
    }
}