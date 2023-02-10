// Ken Phanthavong
// 1194116
// 2022/10/22
// Assignment 2 Question 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Car
{
    char plate_number[8]; // 9 char array
    int mileage;
    int return_date; // expected return date of the car in YYMMDD format

    struct Car *previous;
    struct Car *next;
};

void printList(struct Car *head);

struct Car *fileToList(struct Car *head, char *filename);

void listToFile(struct Car *head, char *filename);

struct Car *add_end(struct Car *head, char plate_number[8], int mileage, int return_date);

struct Car *add_available(struct Car *head, char plate_number[8], int mileage, int return_date);

struct Car *add_rented(struct Car *head, char plate_number[8], int mileage, int return_date);

struct Car *remove_car(int *store_mileage, int *check, struct Car *head, char plate_number[8]);

int main()
{
    struct Car *head_a = NULL;
    struct Car *head_r = NULL;
    struct Car *head_aW = NULL;
    int transaction_code = 0;
    float income = 0; // total income from all rented cars

    head_a = fileToList(head_a, "available.txt");
    head_r = fileToList(head_r, "rented.txt");
    head_aW = fileToList(head_aW, "atWkshop.txt");

    while (transaction_code != 7)
    {
        char plate_number[8];
        int mileage = 0;
        int return_date = 0;
        int check = 1;         // for checking if car to be removed exists in the list - 0 if not in the list
        int store_mileage = 0; // mileage of the returned car from before it was rented
        float charge = 0;      // the charge for each return transaction

        printf("\nTransaction Codes\n");
        printf("1 - add a new car to the available-for-rent list\n");
        printf("2 - return a car to the available-for-rent list\n");
        printf("3 - return a car to the repair list\n");
        printf("4 - transfer a car from the repair list to the available-for-rent list\n");
        printf("5 - rent the first available car\n");
        printf("6 - print all the lists,\n");
        printf("7 - quit\n");

        printf("\nEnter a transaction code: ");

        if ((scanf("%d", &transaction_code)) != 1)
        {
            printf("Not a valid transaction code.\n\n");
            return 0;
        }
        else
        {
            switch (transaction_code)
            {
            case 1:
                printf("Enter plate number: ");
                scanf("%s", plate_number);
                printf("Enter mileage: ");
                scanf("%d", &mileage);

                head_a = add_available(head_a, plate_number, mileage, 0);
                break;
            case 2:
                printf("Enter plate number: ");
                scanf("%s", plate_number);
                printf("Enter mileage: ");
                scanf("%d", &mileage);

                head_r = remove_car(&store_mileage, &check, head_r, plate_number);

                if (check == 1)
                {
                    head_a = add_available(head_a, plate_number, mileage, 0);

                    charge = 80.00;
                    if ((mileage - store_mileage) > 200)
                    {
                        charge = 80.00 + (mileage - store_mileage) * 0.15;
                    }
                    printf("\n200km Flat Rate: $80.00\n");
                    printf("+ $0.15/additional km\n");
                    printf("Price Charged: %0.2f\n", charge);
                    income += charge;
                }
                break;
            case 3:
                printf("Enter plate number: ");
                scanf("%s", plate_number);
                printf("Enter mileage: ");
                scanf("%d", &mileage);

                head_r = remove_car(&store_mileage, &check, head_r, plate_number);

                if (check == 1)
                {
                    head_aW = add_end(head_aW, plate_number, mileage, 0);

                    charge = 80.00;
                    if ((mileage - store_mileage) > 200)
                    {
                        charge = 80.00 + (mileage - store_mileage) * 0.15;
                    }
                    printf("\n200km Flat Rate: $80.00\n");
                    printf("+ $0.15/additional km\n");
                    printf("Price Charged: %0.2f\n", charge);
                    income += charge;
                }

                break;
            case 4:
                printf("Enter plate number: ");
                scanf("%s", plate_number);

                head_aW = remove_car(&store_mileage, &check, head_aW, plate_number);

                if (check == 1)
                {
                    head_a = add_available(head_a, plate_number, store_mileage, 0);
                }

                break;
            case 5:
                strcpy(plate_number, head_a->plate_number);
                mileage = head_a->mileage;
                printf("\nEnter expected return date: ");
                scanf("%d", &return_date);

                head_a = remove_car(&store_mileage, &check, head_a, plate_number);

                if (check == 1)
                {
                    head_r = add_rented(head_r, plate_number, mileage, return_date);
                }
                break;
            case 6:
                printf("Available:\n");
                printList(head_a);
                printf("Rented:\n");
                printList(head_r);
                printf("At Workshop:\n");
                printList(head_aW);
                break;
            case 7:
                printf("\nTotal Income: %0.2f\n\n", income);
                listToFile(head_a, "available.txt");
                listToFile(head_r, "rented.txt");
                listToFile(head_aW, "atWkshop.txt");
                break;
            default:
                printf("Not a valid transaction code.\n\n");
                break;
            }
        }
    }
    return 0;
}

/*Prints list from the given the node c to the end of the list*/
// sourced from lecture notes L06(LinkedLists)
void printList(struct Car *c)
{
    while (c != NULL)
    {
        printf(" %10s ", c->plate_number);
        printf(" %10d ", c->mileage);
        printf(" %10d\n", c->return_date);
        c = c->next;
    }
    printf("\n");
}

/*Converts given file to list using given head node*/
struct Car *fileToList(struct Car *head, char *filename)
{
    FILE *fp;
    char buff1[8]; // buffer for first line of 3 lines - plate number
    int buff2 = 0; // buffer for second line of 3 lines - mileage
    int buff3 = 0; // buffer for third line of 3 lines - return_date

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Can't open the file: %s\n", filename);
        exit(0);
    }

    /* Appending the cars into the list*/
    while (fscanf(fp, "%s", buff1) == 1)
    {
        fscanf(fp, "%d", &buff2);
        fscanf(fp, "%d", &buff3);
        head = add_end(head, buff1, buff2, buff3);
    }

    fclose(fp);

    return head;
}

/*Stores list into its respective file*/
void listToFile(struct Car *head, char *filename)
{
    FILE *fp;

    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Can't open the file: %s\n", filename);
        exit(0);
    }

    while (head != NULL)
    {
        fprintf(fp, "%s\n", head->plate_number);
        fprintf(fp, "%d\n", head->mileage);
        fprintf(fp, "%d\n", head->return_date);
        fprintf(fp, "\n");
        head = head->next;
    }
}

/*creates a node and adds it to the end of the list using given node attributes*/
struct Car *add_end(struct Car *head, char plate_number[8], int mileage, int return_date)
{
    struct Car *ptr, *temp;

    ptr = head;

    temp = (struct Car *)malloc(sizeof(struct Car));
    strcpy(temp->plate_number, plate_number);
    temp->mileage = mileage;
    temp->return_date = return_date;
    temp->previous = NULL;
    temp->next = NULL;

    if (head == NULL) // list is empty?
    {
        head = temp;
        return head;
    }

    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    temp->previous = ptr;
    ptr->next = temp;
    return head;
}

/*creates a node and adds it based on mileage to the list using given node attributes*/
struct Car *add_available(struct Car *head, char plate_number[8], int mileage, int return_date)
{
    struct Car *ptr, *temp;

    ptr = head;

    temp = (struct Car *)malloc(sizeof(struct Car));
    strcpy(temp->plate_number, plate_number);
    temp->mileage = mileage;
    temp->return_date = return_date;
    temp->previous = NULL;
    temp->next = NULL;

    if (head == NULL) // list is empty?
    {
        head = temp;
        return head;
    }

    // Car node will be placed after the node with mileage less than and before the node with mileage greater than
    // it will also stop at the last node if it has the highest mileage
    while (mileage > ptr->mileage && ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    if (ptr->next != NULL) // if not at end of the list
    {

        if (ptr->previous != NULL) // if not at the start of the list
        {
            ptr->previous->next = temp;
            temp->previous = ptr->previous;
        }
        else
        {
            head = temp;
        }
        ptr->previous = temp;
        temp->next = ptr;
    }
    else
    { // if it is at the end of the list (the current node (*ptr) is pointing to nothing)
        ptr->next = temp;
        temp->previous = ptr;
    }

    return head;
}

/*creates a node and adds it based on return date to the list using given node attributes*/
struct Car *add_rented(struct Car *head, char plate_number[8], int mileage, int return_date)
{
    struct Car *ptr, *temp;

    ptr = head;

    temp = (struct Car *)malloc(sizeof(struct Car));
    strcpy(temp->plate_number, plate_number);
    temp->mileage = mileage;
    temp->return_date = return_date;
    temp->previous = NULL;
    temp->next = NULL;

    if (head == NULL) // list is empty?
    {
        head = temp;
        return head;
    }

    // Car node will be placed after the node with mileage less than and before the node with mileage greater than
    // it will also stop at the last node if it has the highest mileage
    while (return_date > ptr->return_date && ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    if (ptr->next != NULL) // if not at end of the list
    {

        if (ptr->previous != NULL) // if not at the start of the list
        {
            ptr->previous->next = temp;
            temp->previous = ptr->previous;
        }
        else
        {
            head = temp;
        }
        ptr->previous = temp;
        temp->next = ptr;
    }
    else
    { // if it is at the end of the list (the current node (*ptr) is pointing to nothing)
        ptr->next = temp;
        temp->previous = ptr;
    }

    return head;
}

/*removes car node given by its plate number, stores the mileage of the car for further use*/
struct Car *remove_car(int *store_mileage, int *check, struct Car *head, char plate_number[8])
{
    struct Car *ptr;
    ptr = head;

    if (head == NULL) // list is empty?
    {
        printf("There are no cars in the list.\n\n");
        return head;
    }

    while (!(strcmp(ptr->plate_number, plate_number) == 0) &&
           ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    /*************************************************/
    *store_mileage = ptr->mileage; // stores mileage of removed car to the given int variable
    /*************************************************/

    // if not at the end of the list
    if (ptr->next != NULL)
    {
        ptr->next->previous = ptr->previous;
    }

    // if not at the start or is at the end and is equal to the search
    if (ptr->previous != NULL && strcmp(ptr->plate_number, plate_number) == 0)
    {
        ptr->previous->next = ptr->next;
    }
    else if (ptr->previous == NULL) // if at the start, set new head
    {
        head = ptr->next;
    }
    else
    {
        printf("Car was not found.\n\n");
        *check = 0;
    }

    return head;
}