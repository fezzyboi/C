/* 
 * File:   Part3.c
 * Author: Ric
 *
 * Created on April 4, 2016, 11:24 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CMath.h"

/*
 * 
 */

void main(void) 
{
    printf("Starting calculator!\n");
    printf("%f %f\n", arctangent2(3.0, 4.0), atan2(3.0, 4.0));
    printf("%f %f\n", arctangent2(20, 10), atan2(20, 10));
    printf("%f %f\n", arctangent2(1, 1), atan2(1, 1));
    printf("%f %f\n", enorm(0, 0, 2, 2), sqrt(pow(2 - 0, 2) + pow(2 - 0, 2)));
    printf("%f %f\n", enorm(4, 5, 6, 7), sqrt(pow(6 - 4, 2) + pow(7 - 5, 2)));
    printf("%f %f\n", enorm(10, 2, -8, -45), sqrt(pow(-8 - 10, 2) + pow(-45 - 2, 2)));
    while (1);
}
