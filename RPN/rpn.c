#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

#define STACK_SIZE 20

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);




// his code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here ****************************/
    printf("Welcome to Ric's RPN calculator.\nEnter floats and + - / * in RPN format:\n");
    char buffer[62];
    struct Stack the_stack;
    float pop1, pop2, val;
    char *token;
    while (1) {
        StackInit(&the_stack);
        int clean = 1;
        fgets(buffer, sizeof (buffer) - 1, stdin);
        token = strtok(buffer, " ");
        while (token != NULL) {
            int gotval = 0;
            char *dummy;
            strtod(token, &dummy);
            if (*dummy != 0) {
                switch (token[0]) {
                case '+':
                    gotval = 1;
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        StackInit(&the_stack);
                        clean = 0;
                        break;
                    }

                    StackPop(&the_stack, &pop1);
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        StackInit(&the_stack);
                        clean = 0;
                        break;
                    }

                    StackPop(&the_stack, &pop2);
                    float pushVal = pop1 + pop2;
                    StackPush(&the_stack, pushVal);

                    break;

                case '-':
                    gotval = 1;
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        clean = 0;
                        StackInit(&the_stack);
                        break;
                    }
                    StackPop(&the_stack, &pop1);
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        StackInit(&the_stack);
                        clean = 0;
                        break;
                    }
                    StackPop(&the_stack, &pop2);
                    StackPush(&the_stack, pop2 - pop1);
                    break;

                case '/':
                    gotval = 1;
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        clean = 0;
                        StackInit(&the_stack);
                        break;
                    }
                    StackPop(&the_stack, &pop1);
                    if (pop1 == 0) {
                        printf("Error: divide by zero\n");
                        clean = 0;
                        StackInit(&the_stack);
                        break;
                    }
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        StackInit(&the_stack);
                        clean = 0;
                        break;
                    }
                    StackPop(&the_stack, &pop2);
                    StackPush(&the_stack, pop2 / pop1);
                    break;

                case '*':
                    gotval = 1;
                    if (the_stack.currentItemIndex < 0) {
                        printf("Error: stack underflow\n");
                        clean = 0;
                        StackInit(&the_stack);
                        break;
                    }
                    StackPop(&the_stack, &pop1);
                    if (the_stack.currentItemIndex < 0) {
                        printf("Not enough operands before operand\n");
                        StackInit(&the_stack);
                        clean = 0;
                        break;
                    }
                    StackPop(&the_stack, &pop2);
                    StackPush(&the_stack, pop2 * pop1);
                    break;
                default:
                    gotval = 1;
                    float tmp;
                    if (sscanf(token, "%f", &tmp) != 1) {
                        printf("Invalid character in RPN string.\n");
                    }
                    clean = 0;
                    StackInit(&the_stack);
                    break;
                }
            }
            if (gotval == 0) {
                sscanf(token, "%f", &val);
                if (StackIsFull(&the_stack) == TRUE) {
                    printf("Stack overflow.\n");
                    StackInit(&the_stack);
                    clean = 0;
                    break;
                }
                StackPush(&the_stack, val);
            }
            token = strtok(NULL, " ");

        }
        float answer;
        StackPop(&the_stack, &answer);
        if (StackIsEmpty(&the_stack) != TRUE) {
            printf("Invalid RPN calculation: more or less than one item in the stack.\n");
            StackInit(&the_stack);
            clean = 0;
        }
        if (clean) {
            printf("Result: %4.2f\n", (double) answer);
            StackInit(&the_stack);
        }
    }

    /*********************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence)
{
    return 0;
}
