/* 
 * File:   Part2.c
 * Author: Ric
 *
 * Created on April 6, 2016, 7:16 PM
 */
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>
void main(void)
{
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Declare variables
    float fahr, celsius;
    int lower, upper, step;

    // Initialize variables
    lower = 0;
    upper = 300;
    step = 20;
    fahr = lower;

    // Print out table
    printf("%5s%6s\n", "F", "C");
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)*(fahr - 32.0);

        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;

    }
    printf(" %5s%6s\n", "K", "F");

    float kelv = lower;

    while (kelv <= upper) {
        fahr = (1.8)*(kelv - 273) + 32;

        printf("%03.3f %5f\n", (double) kelv, (double) fahr);
        kelv = kelv + step;

    }



    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
while (1);
}
