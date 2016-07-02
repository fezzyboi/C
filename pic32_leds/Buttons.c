/* ************************************************************************** */
/** Descriptive File Name

  @Author
    Ric Rodriguez

  @File Name
    Buttons.c

  @Summary
 Implementation of Buttons.h

  @Description
 * gets values of buttons 1-4 on pic32
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "Buttons.h"
#include "BOARD.h"
static uint8_t sample1 = 0;
static uint8_t sample2 = 0;
static uint8_t sample3 = 0;
static uint8_t sample4 = 0;
static uint8_t curr = 0;


static int counter = 0;

void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    curr = 0;
}

uint8_t ButtonsCheckEvents(void)
{
    curr = 0;
    if (BUTTON_STATES() & BUTTON_STATE_1) {
        curr |= BUTTON_EVENT_1DOWN;
    } else {
        curr |= BUTTON_EVENT_1UP;
    }
    if (BUTTON_STATES() & BUTTON_STATE_2) {
        curr |= BUTTON_EVENT_2DOWN;
    } else {
        curr |= BUTTON_EVENT_2UP;
    }
    if (BUTTON_STATES() & BUTTON_STATE_3) {
        curr |= BUTTON_EVENT_3DOWN;
    } else {
        curr |= BUTTON_EVENT_3UP;
    }
    if (BUTTON_STATES() & BUTTON_STATE_4) {
        curr |= BUTTON_EVENT_4DOWN;
    } else {
        curr |= BUTTON_EVENT_4UP;
    }

    sample4 = sample3;
    sample3 = sample2;
    sample2 = sample1;
    sample1 = curr;

    if (curr == sample1 && curr == sample2 && curr == sample3 && sample4 != curr) {
        return curr;
    }
    return BUTTON_EVENT_NONE;
}
