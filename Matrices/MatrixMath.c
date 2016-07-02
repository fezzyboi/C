#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MatrixMath.h"
#define FP_DELTA 0.0001

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) 
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (j = 0; j < 3; j++) {
            result[i][j] = (mat1[i][0] * mat2[0][j] + mat1[i][1] * mat2[1][j] + mat1[i][2] * mat2[2][j]);
        }
    }
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            result[i][j] = (mat1[i][j] + mat2[i][j]);
        }
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            if (fabs(mat1[i][j] - mat2[i][j]) > FP_DELTA) {
                return 0;
            }
        }
    }
    return 1;
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) 
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            result[i][j] = x * mat[i][j];
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) 
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            result[i][j] = x + mat[i][j];
        }
    }
}

float MatrixDeterminant(float mat[3][3]) 
{
    return (mat[0][0]*(mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
            - mat[0][1]*(mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) +
            mat[0][2]*(mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]));
}

void MatrixInverse(float mat[3][3], float result[3][3])
{
    float det = MatrixDeterminant(mat);
    if (det == 0) {
        int i = 0;
        for (; i < 3; i++) {
            int j = 0;
            for (; j < 3; j++) {
                result[i][j] = 0;
            }
        }
        return;
    }
    float tmp[3][3];
    tmp[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];

    tmp[0][1] = -1 * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]);

    tmp[0][2] = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

    tmp[1][0] = -1 * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]);

    tmp[1][1] = mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2];

    tmp[1][2] = -1 * (mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1]);

    tmp[2][0] = mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2];

    tmp[2][1] = -1 * (mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2]);

    tmp[2][2] = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

    MatrixTranspose(tmp, result);
    MatrixScalarMultiply(1 / det, result, tmp);
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            result[i][j] = tmp[i][j];
        }
    }
    

}

float MatrixTrace(float mat[3][3])
{
    int i = 0, j = 0;
    float result = 0;
    for (; i < 3; i++, j++) {
        result += mat[i][j];
    }
    i = 0, j = 2;
    for (; i < 3; i += 2, j -= 2) {
        result += mat[i][j];
    }
    return result;
}

void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        for (; j < 3; j++) {
            int k = 2;
            result[i][j] = mat[j][i];
        }
    }
}

/*
void MatrixAdjugate(float mat[3][3], float result[3][3])
{
    int i=0;
    for(;i<3;i++){
        int j=0;
        for(;j<3;j++){
        }
    }
    MatrixTranspose(result, result);
}
 */

void MatrixPrint(float mat[3][3]) 
{
    int i = 0;
    for (; i < 3; i++) {
        int j = 0;
        printf("{ ");
        for (; j < 3; j++) {
            if (j < 2) {
                printf("%4.2f | ", (double) mat[i][j]);
            } else {
                printf("%4.2f", (double) mat[i][j]);

            }
        }
        printf(" }\n");
    }
    printf("\n");
}
