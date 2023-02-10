#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node *makeTree(char *expression, int *index);

int getNumChar(char *expression, int *index);

void preorder(struct Node *parent);

void postorder(struct Node *parent);

struct Node
{
    char item[4];
    struct Node *left;
    struct Node *right;
};

int main(int argc, char *argv[])
{
    struct Node *root = NULL;
    int index = 0;
    root = makeTree(argv[1], &index);

    int option = 0;

    while (option != 3)
    {
        printf("\nSelect Option:\n");
        printf("1. Preorder\n");
        printf("2. Postorder\n");
        printf("3. Exit\n");
        printf("\nEnter: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            preorder(root);
            printf("\n");
            break;
        case 2:
            postorder(root);
            printf("\n");
            break;
        case 3:
            break;
        default:
            printf("Invalid Option\n");
            break;
        }
    }
    return 0;
}

//parses input and creates the binary tree for the expression
struct Node *makeTree(char *expression, int *index)
{

    if (expression[*index] == '(')
    {
        *index += 1; //skips opening bracket at start of expression
    }

    struct Node *parent = (struct Node *)malloc(sizeof(struct Node));
    struct Node *lnode = (struct Node *)malloc(sizeof(struct Node));
    struct Node *rnode = (struct Node *)malloc(sizeof(struct Node));

    parent->item[0] = '\0';
    parent->left = NULL;
    parent->right = NULL;
    lnode->item[0] = '\0';
    lnode->left = NULL;
    lnode->right = NULL;
    rnode->item[0] = '\0';
    rnode->left = NULL;
    rnode->right = NULL;

    if (expression[*index] == '(') //if there is a smaller expression in brackets, make a subtree
    {
        lnode = makeTree(expression, index);
        *index += 1; // skip closing bracket
    }
    else
    {
        int i = getNumChar(expression, index);
        strncpy(lnode->item, expression + *index, i);//copies from start of the operand to the end of the operand
        lnode->item[i] = '\0';
        *index += i;
    }
    parent->left = lnode;

    parent->item[0] = expression[*index];
    parent->item[1] = '\0';
    *index += 1;

    if (expression[*index] == '(') 
    {
        rnode = makeTree(expression, index);
        *index += 1; // skip closing bracket
    }
    else
    {
        int i = getNumChar(expression, index);
        strncpy(rnode->item, expression + *index, i);
        rnode->item[i] = '\0';
        *index += i;
    }
    parent->right = rnode;

    return parent;
}

//returns number of characters in the operand, stops when it finds an operator or bracket
int getNumChar(char *expression, int *index)
{
    int i = 0;
    while (expression[*index + i] != '+' && expression[*index + i] != '-' &&
           expression[*index + i] != '*' && expression[*index + i] != '/' && expression[*index + i] != ')')
    {
        i++;
    }
    return i;
}

void preorder(struct Node *parent)
{
    printf("%s ", parent->item);
    if (parent->left != NULL)
    {
        preorder(parent->left);
    }
    if (parent->right != NULL)
    {
        preorder(parent->right);
    }
}

void postorder(struct Node *parent)
{
    if (parent->left != NULL)
    {
        postorder(parent->left);
    }
    if (parent->right != NULL)
    {
        postorder(parent->right);
    }
    printf("%s ", parent->item);
}
