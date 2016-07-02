/* 
 * File:   Part4.c
 * Author: Ric
 *
 * Created on April 6, 2016, 7:16 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"

/*
 * 
 */
void main(void) 
{
    OledInit();
    const char *string = "Hello, World!";
    OledDrawString(string);
    OledUpdate();
    while (1);
}

