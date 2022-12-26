#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NO_OF_ROWS 1000
#define NO_OF_COLS 1000

void initMat(unsigned short int matA[NO_OF_ROWS][NO_OF_COLS], unsigned short int matB[NO_OF_ROWS][NO_OF_COLS], unsigned short int mat[NO_OF_ROWS][NO_OF_COLS])
{
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        for (int j = 0; j < NO_OF_COLS; j++)
        {
            matA[i][j] = rand() % 5;
            matB[i][j] = rand() % 5;
            mat[i][j] = 0;
        }
    }
}

void matMultiplySequential(unsigned short int matA[NO_OF_ROWS][NO_OF_COLS], unsigned short int matB[NO_OF_ROWS][NO_OF_COLS], unsigned short int mat[NO_OF_ROWS][NO_OF_COLS])
{
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        for (int j = 0; j < NO_OF_COLS; j++)
        {
            for (int k = 0; k < NO_OF_ROWS; k++)
            {
                mat[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
}

void matMultiplyOmp(unsigned short int matA[NO_OF_ROWS][NO_OF_COLS], unsigned short int matB[NO_OF_ROWS][NO_OF_COLS], unsigned short int mat[NO_OF_ROWS][NO_OF_COLS])
{
#pragma omp parallel num_threads(12) shared(matA, matB, mat)
    {
        int numThreads = omp_get_num_threads();
        int threadNo = omp_get_thread_num();

        unsigned short int temp[NO_OF_COLS];
        // unsigned short int temp = 0;

        for (int i = threadNo; i < NO_OF_ROWS; i += numThreads)
        {
            memset(temp, 0, NO_OF_COLS * sizeof(short int));
            for (int j = 0; j < NO_OF_COLS; j++)
            {
                // temp = 0;
                for (int k = 0; k < NO_OF_ROWS; k++)
                {
                    temp[k] += matA[i][j] * matB[j][k];
                    // temp += matA[i][k] * matB[k][j];
                }
                // mat[i][j] = temp;
            }
            memcpy(mat[i], temp, NO_OF_COLS * sizeof(short int));
        }
    }
}

void printMat(unsigned short int mat[NO_OF_ROWS][NO_OF_COLS])
{
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        for (int j = 0; j < NO_OF_COLS; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));

    unsigned short int matA[NO_OF_ROWS][NO_OF_COLS];
    unsigned short int matB[NO_OF_ROWS][NO_OF_COLS];
    unsigned short int resultMat[NO_OF_ROWS][NO_OF_COLS];

    double startTime = omp_get_wtime();

    for (int t = 0; t < 5; t++)
    {
        initMat(matA, matB, resultMat);
        // matMultiplySequential(matA, matB, resultMat);
        matMultiplyOmp(matA, matB, resultMat);
    }

    printf("Time elapsed: %f\n", (omp_get_wtime() - startTime) / 5);

    // printMat(matA);
    // printMat(matB);
    // printMat(resultMat);

    return 0;
}