#include "Stack.h"
#include "BOARD.h"
#define STACK_SIZE 20

void StackInit(struct Stack *stack)
{
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

int StackPush(struct Stack *stack, float value)
{
    if (StackIsFull(stack) == TRUE || stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }
    ++(stack->currentItemIndex);
    stack->stackItems[stack->currentItemIndex] = value;
    return SUCCESS;
}

int StackPop(struct Stack *stack, float *value)
{
    if (StackIsEmpty(stack) == TRUE || stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }

    *value = stack->stackItems[stack->currentItemIndex];
    stack->currentItemIndex--;
    return 1;
}

int StackIsEmpty(const struct Stack *stack)
{
    if (stack->initialized == TRUE && stack->currentItemIndex == -1) {
        return TRUE;
    }
    return FALSE;
}

int StackIsFull(const struct Stack *stack)
{

    if (stack->currentItemIndex != (STACK_SIZE - 1) || stack->initialized != TRUE) {
        return FALSE;
    }
    return TRUE;
}

int StackGetSize(const struct Stack *stack)
{
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    }
    if (StackIsEmpty(stack) == TRUE || stack->initialized == TRUE) {
        return 0;
    }
    return stack->currentItemIndex;
}
