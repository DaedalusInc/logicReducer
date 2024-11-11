#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logicReadIn.h"

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

bool validName(char* name) {
   return strcmp(name, "not") && strcmp(name, "or") && strcmp(name, "and") && strcmp(name, "xor") && strcmp(name, "nor")
   && strcmp(name, "nand") && strcmp(name, "xnor") && name[0] != '(' && name[strlen(name)-1] != ')';
}

int main() {
   int numIns, numFuncs;
   char** inputNames;
   char** functionNames;
   char textInput[1001];

   printInstructions();

   printf("How many input variables do you have? ");
   while (!scanf("%d", &numIns) || numIns < 1) {
      printf("You must enter an integer greater than 0.\nHow many input variables do you have? ");
      while (getchar() != '\n');
   }
   inputNames = malloc(sizeof(char*)*numIns);
   
   for (int inputIndex = 0; inputIndex < numIns; inputIndex++) {
      printf("Enter a name for input variable %d: ", inputIndex+1);
      scanf("%s", textInput);
      textInput[1000] = '\n';
      while (!validName(textInput)) {
         printf("Names of boolean operators are not allowed.");
         printf("\nEnter a valid name for input variable %d: ", inputIndex+1);
         scanf("%s", textInput);
         textInput[1000] = '\n';
      }
      inputNames[inputIndex] = malloc(sizeof(char)*(strlen(textInput)));
      strcpy(inputNames[inputIndex], textInput);
   }

   printf("How many functions do you have? ");
   while (!scanf("%d", &numFuncs) || numFuncs < 1) {
      printf("You must enter an integer greater than 0.\nHow many functions do you have? ");
      while (getchar() != '\n');
   }
   functionNames = malloc(sizeof(char*)*numFuncs);
   
   printf("Would you like to name your functions (yes or no)? ");
   scanf("%s", textInput);
   if (textInput[0] == 'y') {
      for (int functionIndex = 0; functionIndex < numIns; functionIndex++) {
         printf("Enter a name for function %d: ", functionIndex+1);
         scanf("%s", textInput);
         while (!validName(textInput)) {
            printf("Names of boolean operators are not allowed.");
            printf("\nEnter a valid name for function %d: ", functionIndex+1);
            scanf("%s", textInput);
         }
         functionNames[functionIndex] = malloc(sizeof(char)*(strlen(textInput)));
         strcpy(functionNames[functionIndex], textInput);
      }
   }
   else {
      for (int functionIndex = 0; functionIndex < numIns; functionIndex++) {
         functionNames[functionIndex] = malloc(sizeof(char)*3);
         strcpy(functionNames[functionIndex], "F");
      }
   }
   while (getchar() != '\n');

   printf("Enter a logic equation: ");
   char equation[512];
   fgets(equation, 512, stdin);
   equation[strlen(equation) - 1] = 0;
   treeNode *n = treeMake(equation);
   printTree(n);
   for (int inputIndex = 0; inputIndex < numIns; inputIndex++) {
      free(inputNames[inputIndex]);
   }
   free(inputNames);
   for (int functionIndex = 0; functionIndex < numFuncs; functionIndex++) {
      free(functionNames[functionIndex]);
   }
   free(functionNames);

   return 0;
}
