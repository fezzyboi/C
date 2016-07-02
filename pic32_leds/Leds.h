/**
 * File: Leds.h
 * @brief Initiates LEDS and allows manipulation
 * @author Ric Rodriguez, 2016.05.11  */

#ifndef __LEDS_H__    /* Guard against multiple inclusion */
#define __LEDS_H__
#include "BOARD.h"
#include <xc.h>
#define LEDS_INIT() do { TRISE=0x00; LATE=0x0; } while(0)
#define LEDS_GET() LATE
#define LEDS_SET(x) LATE=(x)
#endif 
