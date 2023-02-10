// Ken Phanthavong
// 1194116
// 2022/11/29
// CIS2520 Assignment 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// row and column size global constants
#define M 21
#define N 10

void readFile(int array[M][N]);
void printArray(int array[M][N]);
void minheapify(int array[M][N]);
int key(int array[]);

int main()
{
    int array[M][N];
    readFile(array);
    // prints original 2d array if needed
    // printArray(array);
    minheapify(array);
    printArray(array);

    return 0;
}

// reads the text file and stores its contents intoa  2d array
void readFile(int array[M][N])
{

    FILE *fp;
    int buff = 0;
    // accepted filename is f.dat
    fp = fopen("f.dat", "r");

    if (fp == NULL)
    {
        printf("Can't open the file");
        exit(0);
    }

    for (int i = 1; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (fscanf(fp, "%d", &buff) == 1)
            {
                array[i][j] = buff;
            }
        }
    }

    fclose(fp);
}

// prints the contents of the 2d array
void printArray(int array[M][N])
{
    printf("\n");
    for (int i = 1; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%02d ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Creates min heap from current array/tree
void minheapify(int array[M][N])
{
    // applies downheap to every parent node from the last parent node to the root
    for (int i = M / 2; i > 0; i--)
    {
        // below is downheap algorithm (not very efficient)
        int j = i;
        int minInd = 0;
        while (j <= M / 2 && minInd != j)
        {
            if (key(array[j]) < key(array[2 * j]))
            {
                minInd = j;
            }
            else
            {
                minInd = 2 * j;
            }

            // checks if the child node exists (this only applies to a one child subtree which exists at the 10th parent node)
            if (2 * j + 1 < M)
            {
                if (key(array[minInd]) > key(array[2 * j + 1]))
                {
                    minInd = 2 * j + 1;
                }
            }

            // swaps the minimum node and the parent node
            if (minInd != j)
            {
                int tempAr[N];
                memcpy(tempAr, array[j], N * 4);
                memcpy(array[j], array[minInd], N * 4);
                memcpy(array[minInd], tempAr, N * 4);

                j = minInd;
                minInd = 0;
            }
        }
    }
}

// returns the key of the object/row
int key(int array[])
{
    return array[0] + array[1] + array[2];
}
