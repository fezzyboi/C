// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FP_DELTA 0.0001

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "MatrixMath.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

void main() 
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    float test1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    float test2[3][3] = {
        {1.62, -2, -0.5},
        {1.43, 1, -0.12},
        {9.29, -4.92, 5.27}};

    float test3[3][3] = {
        {7.2, -4.8, 3.6},
        {-4.2, 5.01, 6.33},
        {-9.12, 1.99, -3}};
    float result[3][3];
    float oneplustwo[3][3] = {
        {2.62, 0, 2.5},
        {5.43, 6, 5.88},
        {16.29, 3.08, 14.27}};
    float twoplusthree[3][3] = {
        {8.82, -6.8, 3.1},
        {-2.77, 6.01, 6.21},
        {0.17, -2.93, 2.27}};
    float onetimestwo[3][3] = {
        {32.35, -14.76, 15.07},
        {69.37, -32.52, 29.02},
        {106.39, -50.28, 42.97}};
    float twotimesthree[3][3] = {
        {24.624, -18.791, -5.328},
        {7.1904, -2.0928, 11.838},
        {39.4896, -58.7539, -13.5096}};
    float det1 = 0;
    float det2 = 33.045552;
    float onescalarM[3][3] = {
        {3, 6, 9},
        {12, 15, 18},
        {21, 24, 27}};
    float twoscalarM[3][3] = {
        {-1.944, 2.4, 0.6},
        {-1.716, -1.2, 0.144},
        {-11.148, 5.904, -6.324}};
    float onescalarA[3][3] = {
        {3.5, 4.5, 5.5},
        {6.5, 7.5, 8.5},
        {9.5, 10.5, 11.5}};
    float twoscalarA[3][3] = {
        {0.62, -3, -1.5,},
        {0.43, -0, -1.12},
        {8.29, -5.92, 4.27}};
    float oneI[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}};
    float twoI[3][3] = {
        {0.1416, 0.3934, 0.0224},
        {-0.2618, 0.3989, -0.0158},
        {-0.494, -0.3211, 0.1356}};
    float oneTrace = 25;
    float twoTrace = 16.68;
    float oneT[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}};
    float twoT[3][3] = {
        {1.62, 1.43, 9.29},
        {-2, 1, -4.92},
        {-0.5, -0.12, 5.27}};
    float equ1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    float equ2[3][3] = {
        {1.63, -2, -0.5},
        {1.43, 1, -0.12},
        {9.29, -4.92, 5.27}};
    //float adj1[3][3]={{-3, 6, -3}, {6, -12, 6}, {-3, 6, -3}};
    //float adj2[3][3]={{4.6796, 13, 0.74}, {-8.6509, 13.1824, -0.5206}, {-16.3256, -10.6096, 4.48}};

    // MatrixEquals test
    int counter = 0, total = 0;
    if (MatrixEquals(test1, equ1)) {
        counter++;
    }
    if (!MatrixEquals(test2, equ2)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixEquals()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixEquals()\n", counter);
    }

    // MatrixMultiply test
    counter = 0;
    MatrixMultiply(test1, test2, result);
    if (MatrixEquals(result, onetimestwo)) {
        counter++;
    }
    MatrixMultiply(test2, test3, result);
    if (MatrixEquals(result, twotimesthree)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixMultiply()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixMultiply()\n", counter);
    }

    // MatrixScalarMultiply test
    counter = 0;
    MatrixScalarMultiply(3, test1, result);
    if (MatrixEquals(result, onescalarM)) {
        counter++;
    }
    MatrixScalarMultiply(-1.2, test2, result);
    if (MatrixEquals(result, twoscalarM)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixScalarMultiply()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixScalarMultiply()\n", counter);
    }

    // MatrixDeterminant test
    counter = 0;
    if (MatrixDeterminant(test1) == det1) {
        counter++;
    }
    if (MatrixDeterminant(test2) == det2) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixDeterminant()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixDeterminant()\n", counter);
    }

    counter = 0;
    MatrixAdd(test1, test2, result);
    if (MatrixEquals(result, oneplustwo)) {
        counter++;
    }
    MatrixAdd(test2, test3, result);
    if (MatrixEquals(result, twoplusthree)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixAdd()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixAdd()\n", counter);
    }

    counter = 0;
    MatrixScalarAdd(2.5, test1, result);
    if (MatrixEquals(result, onescalarA)) {
        counter++;
    }
    MatrixScalarAdd(-1, test2, result);
    if (MatrixEquals(result, twoscalarA)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixScalarAdd()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixScalarAdd()\n", counter);
    }


    counter = 0;
    MatrixTranspose(test1, result);
    if (MatrixEquals(result, oneT)) {
        counter++;
    }
    MatrixTranspose(test2, result);
    if (MatrixEquals(result, twoT)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixTranspose()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixTranspose()\n", counter);
    }

    counter = 0;
    if (MatrixTrace(test1) == oneTrace) {
        counter++;
    }
    if (MatrixTrace(test2) == twoTrace) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixTrace()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixTrace()\n", counter);
    }

    counter = 0;
    MatrixInverse(test1, result);
    if (MatrixEquals(result, oneI)) {
        counter++;
    }
    MatrixInverse(test2, result);
    if (MatrixEquals(result, twoI)) {
        counter++;
    }
    if (counter == 2) {
        printf("PASSED (%d/2): MatrixInverse()\n", counter);
        total += 1;
    } else {
        printf("FAILED (%d/2): MatrixInverse()\n", counter);
    }


    int itor = 0;
    for (; itor < 30; itor++) {
        printf("-");
    }
    printf("\n%d out of 9 functions passed (%.2f%)\n\n", total, 100 * (total / 9.0));
    printf("Output of MatrixPrint():\n");
    MatrixPrint(test1);
    printf("Expected output of MatrixPrint(): \n\
{ 1.00 | 2.00 | 3.00 }\n\
{ 4.00 | 5.00 | 6.00 }\n\
{ 7.00 | 8.00 | 9.00 }\n");







    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.

    while(1);
}

