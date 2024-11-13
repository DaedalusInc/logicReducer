#include "exec.h"
#include "logicReadIn.h"
#include "minterm_reducer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printInstructions() {
    printf("Instructions:\n1. Enter your input variables\n  - Variable names can not begin or end with parenthesis");
    printf("\n  - Variable names can not be boolean operators\n2. Enter the number of function you have");
    printf("\n  - Optional naming, defaults to F1, F2, ... Fn\n3. Select your input method");
    printf("\n3a. Enter a boolean equation using the input variables entered, for each function");
    printf("\n  - Allowed operators: not, or, and, xor, nor, nand, and parenthesis");
    printf("\n  - Separate all input variable names and operators with a space (exept parenthesis)");
    printf("\n  - There must be a space between parenthesis and operators, but it's not required for variables");
    printf("\n  - Order of operations: (), not, and, or, xor, nor, nand");
    printf("\n3b. Enter the minterm values of the function\n3c. Enter the maxterm values of the function");
    printf("\n  - The first input variable is the most significant bit and the last is the least significant bit");
    printf("\n  - Enter a list of integers, for the terms, seperated by non-number characters\n\n");
}

bool validName(char *name) {
    return strcasecmp(name, "not") && strcasecmp(name, "or") && strcasecmp(name, "and") && strcasecmp(name, "xor") && strcasecmp(name, "nor") &&
           strcasecmp(name, "nand") && name[0] != '(' && name[strlen(name) - 1] != ')';
}

int main() {
    int numIns, numFuncs;
    char textInput[1001];

    printInstructions();

    printf("How many input variables do you have? ");
    while (!scanf("%d", &numIns) || numIns < 1 || numIns > 32) {
        printf("You must enter an integer greater than 0 and less than or equal to 32.\nHow many input variables do you have? ");
        while (getchar() != '\n');
    }

    for (int inputIndex = 0; inputIndex < numIns; inputIndex++) {
        printf("Enter a name for input variable %d: ", inputIndex + 1);
        scanf("%s", textInput);
        printf("'%s'\n", textInput);
        textInput[1000] = '\n';
        while (!validName(textInput)) {
            printf("Names of boolean operators are not allowed.");
            printf("\nEnter a valid name for input variable %d: ", inputIndex + 1);
            scanf("%s", textInput);
            textInput[1000] = '\n';
        }
        variables[inputIndex] = malloc(sizeof(char) * (strlen(textInput)));
        strcpy(variables[inputIndex], textInput);
        num_vars++;
    }

    printf("How many functions do you have? ");
    while (!scanf("%d", &numFuncs) || numFuncs < 1) {
        printf("You must enter an integer greater than 0.\nHow many functions do you have? ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    for (int funcNum = 0; funcNum < numFuncs; funcNum++) {
        printf("Enter a logic equation: ");
        char equation[512];
        fgets(equation, 512, stdin);
        equation[strlen(equation) - 1] = 0;
        treeNode *n = treeMake(equation);
        printf("Parsed equation tree:\n");
        printTree(n, 'k', 0);
        size_t num_minterms = 0;
        uint32_t *minterms = execTree(n, &num_minterms);
        if (num_minterms == 0) {
            printf("Reduced equation:\n0\n");
        } else {
            size_t num_reduced = 0;
            term *reduced = reduce_minterms(minterms, num_minterms, &num_reduced);
            printf("Reduced equation:\n");
            char *eqn = minterms_to_equation(reduced, num_reduced);
            printf("%s\n", eqn);
            free(eqn);
            free(reduced);
        }
        freeTree(n);
        free(minterms);
    }
    for (int inputIndex = 0; inputIndex < numIns; inputIndex++) {
        free(variables[inputIndex]);
    }

    return 0;
}
