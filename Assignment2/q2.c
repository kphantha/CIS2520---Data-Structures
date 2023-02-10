// Ken Phanthavong
// 1194116
// 2022/10/22
// Assignment 2 Question 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack
{
    int top;           // the number of items in the stack
    unsigned capacity; // the maximum number of items in the stack
    float *array;      // the array of items
};

struct Stack *createStack(unsigned capacity);

void push(struct Stack *stack, float digit);

float pop(struct Stack *stack);

int main(int argc, char *argv[])
{
    int numChar = strlen(argv[1]); // number of characters

    char *ptr;
    ptr = argv[1]; // ptr is a char pointer for the string from the command line
                   // allows pointer arithmetic and traversing through the characters in the string

    struct Stack *stack = NULL;
    stack = createStack(numChar); // making the stack with capacity of number of characters of the string from cmd line

    for (int i = 0; i < numChar; i++)
    {
        //'item' is assigned to the char item of the expression at the current index
        char item = *(ptr + i);

        switch (item)
        {
        case '+':
            // takes out the last two digits and adds them and then pushes the sum into the stack
            push(stack, pop(stack) + pop(stack));
            break;
        case '-':
            // the first digit of the operation is the second popped item
            // so the first popped item is what is subtracted from the second popped item
            push(stack, - pop(stack) + pop(stack));
            break;
        case '*':
            push(stack, pop(stack) * pop(stack));
            break;
        case '/':
            // the first digit of the operation is the second popped item
            // so the first popped item is what is divided from the second popped item
            push(stack, 1 / pop(stack) * pop(stack));
            break;
        default:
            // if its not an operator, its a number so its int value gets pushed to the stack
            // ASCII value of char 48 is 0. 48 - 48 = 0
            push(stack, item - 48);
            break;
        }
    }

    printf("%0.2f", pop(stack));

    return 0;
}

// Creates a stack with the size/capacity given
struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));

    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (float *)malloc(stack->capacity * sizeof(float));
    return stack;
}

// pushes the given digit to the given stack
void push(struct Stack *stack, float digit)
{
    stack->array[++stack->top] = digit;
}

// pops the top digit of the given stack
float pop(struct Stack *stack)
{
    return stack->array[stack->top--];
}
