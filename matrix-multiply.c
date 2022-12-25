#include <stdio.h>
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
#pragma omp parallel
    {
        int numThreads = omp_get_num_threads();
        int threadNo = omp_get_thread_num();

        unsigned short int temp;
        for (int i = threadNo; i < NO_OF_ROWS; i += numThreads)
        {
            for (int j = 0; j < NO_OF_COLS; j++)
            {
                temp = 0;
                for (int k = 0; k < NO_OF_ROWS; k++)
                {
                    temp += matA[i][k] * matB[k][j];
                }

#pragma omp atomic
                mat[j][i] += temp;
            }
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
    // unsigned short int resultMatA[NO_OF_ROWS][NO_OF_COLS];
    unsigned short int resultMatB[NO_OF_ROWS][NO_OF_COLS];

    double startTime = omp_get_wtime();

    for (int t = 0; t < 2; t++)
    {
        // initMat(matA, matB, resultMatA);
        initMat(matA, matB, resultMatB);

        // matMultiplySequential(matA, matB, resultMatA);
        matMultiplyOmp(matA, matB, resultMatB);
    }

    printf("Time elapsed: %f\n", (omp_get_wtime() - startTime) / 2);

    // printMat(matA);
    // printMat(matB);
    // printMat(resultMatA);
    // printMat(resultMatB);

    return 0;
}