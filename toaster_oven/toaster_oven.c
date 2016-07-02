// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define LONG_PRESS 5

// **** Declare any datatypes here ****

typedef enum cookState {
    START,
    COUNTDOWN,
    PENDING_RESET,
    PENDING_SELECTOR_CHANGE,
    RESET,
    FLASH_ON,
    FLASH_OFF

} CookState;

typedef struct toaster {
    int cookTimeLeft;
    int cookTimeStart;
    int temperature;
    int cookMode;
    uint8_t on;
    CookState ovenState;
    int buttonPressCounter;
    uint8_t inputSel;
    uint8_t buttonEvent;

} ToasterData;

// **** Define any module-level, global, or external variables here ****
static ToasterData toasterData;

// Configuration Bit settings

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    OledInit();
    AdcInit();
    ButtonsInit();
    LEDS_INIT();
    char buffer[128];
    toasterData.ovenState = START;
    toasterData.cookMode = 0;
    toasterData.on = 0;
    toasterData.cookTimeStart = 2;
    toasterData.temperature = 350;
    int startCount;
    int startTime = 0;
    toasterData.buttonPressCounter = 0;
    while ((toasterData.buttonEvent & BUTTON_EVENT_4UP) == 0);

    TIMER_2HZ_RESET();

    while (1) {

        switch (toasterData.ovenState) {
        case START:
            if (toasterData.cookMode == 0) {
                if ((toasterData.buttonEvent & BUTTON_EVENT_3DOWN)) {
                    toasterData.ovenState = PENDING_SELECTOR_CHANGE;
                    startCount = toasterData.buttonPressCounter;
                    toasterData.buttonEvent = 0;
                    break;
                }
                if (toasterData.buttonEvent & BUTTON_EVENT_4DOWN) {
                    toasterData.ovenState = COUNTDOWN;
                    toasterData.cookTimeLeft = toasterData.cookTimeStart;
                    TIMER_2HZ_RESET();
                    toasterData.on = 0;
                    toasterData.buttonEvent = 0;
                    OledClear(0);
                }

                int min = (toasterData.cookTimeStart >> 2) / 60;
                int sec = (toasterData.cookTimeStart >> 2) % 60;
                if (toasterData.inputSel == 0) {
                    toasterData.cookTimeStart = ((AdcRead() >> 2) + 1) << 2;

                } else {
                    if (AdcChanged()) {
                        toasterData.temperature = (AdcRead() >> 2) + 300;
                    }
                }
                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     | %cTime: %d:%02d\n\
|-----| %cTemp: %d%cF \n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Bake", toasterData.inputSel == 0 ? '>' : ' ',
                        min, sec, toasterData.inputSel == 1 ? '>' : ' ', toasterData.temperature,248,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();
            } else if (toasterData.cookMode == 1) {
                if ((toasterData.buttonEvent & BUTTON_EVENT_3DOWN)) {
                    toasterData.ovenState = PENDING_SELECTOR_CHANGE;
                    startCount = toasterData.buttonPressCounter;
                    toasterData.buttonEvent = 0;
                    break;
                }
                if (toasterData.buttonEvent & BUTTON_EVENT_4DOWN) {
                    toasterData.ovenState = COUNTDOWN;
                    toasterData.cookTimeLeft = toasterData.cookTimeStart;
                    TIMER_2HZ_RESET();
                    toasterData.on = 0;
                    toasterData.buttonEvent = 0;
                    OledClear(0);
                }
                toasterData.cookTimeStart = ((AdcRead() >> 2) + 1) << 2;

                int min = (toasterData.cookTimeStart >> 2) / 60;
                int sec = (toasterData.cookTimeStart >> 2) % 60;

                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  \n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Toast",
                        min, sec,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();


            } else {
                if ((toasterData.buttonEvent & BUTTON_EVENT_3DOWN)) {
                    toasterData.ovenState = PENDING_SELECTOR_CHANGE;
                    startCount = toasterData.buttonPressCounter;
                    toasterData.buttonEvent = 0;
                    break;
                }
                if (toasterData.buttonEvent & BUTTON_EVENT_4DOWN) {
                    toasterData.ovenState = COUNTDOWN;
                    toasterData.cookTimeLeft = toasterData.cookTimeStart;
                    TIMER_2HZ_RESET();
                    toasterData.on = 0;
                    toasterData.buttonEvent = 0;
                    OledClear(0);
                }
                toasterData.cookTimeStart = ((AdcRead() >> 2) + 1) << 2;

                int min = (toasterData.cookTimeStart >> 2) / 60;
                int sec = (toasterData.cookTimeStart >> 2) % 60;
                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  Temp: %d%cF\n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Broil",
                        min, sec, 500,248,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();

            }
            break;


        case COUNTDOWN:
            if (toasterData.buttonEvent & BUTTON_EVENT_4DOWN) {
                startTime = toasterData.buttonPressCounter;
                toasterData.buttonEvent = 0;
                toasterData.ovenState = PENDING_RESET;

            }
            if ((toasterData.on) && (toasterData.cookTimeLeft > 0)) {
                toasterData.on = 0;
                toasterData.cookTimeLeft = ((toasterData.cookTimeLeft >> 1) - 1) << 1;
                if (toasterData.cookMode == 0) {
                    int min = (toasterData.cookTimeLeft >> 2) / 60;
                    int sec = (toasterData.cookTimeLeft >> 2) % 60;
                    sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     | %cTime: %d:%02d\n\
|-----| %cTemp: %d%cF \n\
|%c%c%c%c%c|", 0x01, 0x01, 0x01, 0x01, 0x01, "Bake", toasterData.inputSel == 0 ? '>' : ' ',
                            min, sec, toasterData.inputSel == 1 ? '>' : ' ', toasterData.temperature,248,
                            0x03, 0x03, 0x03, 0x03, 0x03);
                    OledDrawString(buffer);
                    OledUpdate();
                } else if (toasterData.cookMode == 1) {
                    int min = (toasterData.cookTimeLeft >> 2) / 60;
                    int sec = (toasterData.cookTimeLeft >> 2) % 60;
                    sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  \n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Toast",
                            min, sec,
                            0x03, 0x03, 0x03, 0x03, 0x03);
                    OledDrawString(buffer);
                    OledUpdate();
                } else {
                    int min = (toasterData.cookTimeLeft >> 2) / 60;
                    int sec = (toasterData.cookTimeLeft >> 2) % 60;
                    sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  Temp: %d%cF\n\
|%c%c%c%c%c|", 0x01, 0x01, 0x01, 0x01, 0x01, "Broil",
                            min, sec, 500,248,
                            0x04, 0x04, 0x04, 0x04, 0x04);
                    OledDrawString(buffer);
                    OledUpdate();
                }
                if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart)) {
                    LEDS_SET(0xFF);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (2 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xFE);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (3 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xFC);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (4 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xF8);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (5 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xF0);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (6 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xE0);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (toasterData.cookTimeStart - (7 * (toasterData.cookTimeStart >> 3)))) {
                    LEDS_SET(0xC0);
                    TIMER_2HZ_RESET();
                } else if ((toasterData.cookTimeLeft) > (0)) {
                    LEDS_SET(0x80);
                    TIMER_2HZ_RESET();
                } else {
                    LEDS_SET(0x00);
                    TIMER_2HZ_RESET();
                    toasterData.ovenState = FLASH_ON;
                    break;
                }
            }
            break;
        case PENDING_SELECTOR_CHANGE:
            if ((toasterData.buttonPressCounter - startCount < LONG_PRESS) && (toasterData.buttonEvent & BUTTON_EVENT_3UP)) {
                toasterData.cookTimeStart = 2;
                toasterData.temperature = 350;
                toasterData.inputSel = 0;
                toasterData.cookMode++;
                if (toasterData.cookMode == 3) {
                    toasterData.cookMode = 0;
                }
                toasterData.buttonEvent = 0;
                toasterData.ovenState = START;
                OledClear(0);
            } else if ((toasterData.buttonPressCounter - startCount) >= LONG_PRESS) {
                toasterData.inputSel ^= 1;
                OledClear(0);
                toasterData.ovenState = START;
            }
            break;
        case FLASH_ON:
            OledSetDisplayInverted();
            if (toasterData.buttonEvent & BUTTON_EVENT_4UP) {
                toasterData.ovenState = RESET;
                break;
            }
            if (toasterData.on) {
                toasterData.on = 0;
                toasterData.ovenState = FLASH_OFF;
                break;

            }
            break;

        case FLASH_OFF:
            if (toasterData.buttonEvent & BUTTON_EVENT_4UP) {
                toasterData.ovenState = RESET;
                break;
            }
            OledSetDisplayNormal();
            if (toasterData.on) {
                toasterData.on = 0;
                toasterData.ovenState = FLASH_ON;
                break;
            }
            break;

        case RESET:
            toasterData.inputSel = 0;
            toasterData.temperature = 350;
            LEDS_SET(0x00);
            OledClear(0);
            if (toasterData.cookMode == 0) {
                int min = (toasterData.cookTimeLeft >> 2) / 60;
                int sec = (toasterData.cookTimeLeft >> 2) % 60;
                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     | %cTime: %d:%02d\n\
|-----| %cTemp: %d%cF \n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Bake", toasterData.inputSel == 0 ? '>' : ' ',
                        min, sec, toasterData.inputSel == 1 ? '>' : ' ', toasterData.temperature,248,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();
            } else if (toasterData.cookMode == 1) {
                int min = (toasterData.cookTimeLeft >> 2) / 60;
                int sec = (toasterData.cookTimeLeft >> 2) % 60;
                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  \n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Toast",
                        min, sec,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();
            } else {
                int min = (toasterData.cookTimeLeft >> 2) / 60;
                int sec = (toasterData.cookTimeLeft >> 2) % 60;
                sprintf(buffer, "|%c%c%c%c%c|  Mode: %s\n\
|     |  Time: %d:%02d\n\
|-----|  Temp: %d%cF\n\
|%c%c%c%c%c|", 0x02, 0x02, 0x02, 0x02, 0x02, "Broil",
                        min, sec, 500, 248,
                        0x04, 0x04, 0x04, 0x04, 0x04);
                OledDrawString(buffer);
                OledUpdate();
            }
            OledUpdate();
            toasterData.ovenState = START;
            break;

        case PENDING_RESET:
            if (toasterData.buttonEvent & BUTTON_EVENT_4UP) {
                toasterData.buttonEvent = 0;
                toasterData.ovenState = COUNTDOWN;
            }

            if (toasterData.buttonPressCounter - startTime > LONG_PRESS) {
                toasterData.ovenState = RESET;
                if (toasterData.on && toasterData.cookTimeLeft == 0) {
                    toasterData.on = 0;
                }
            }
            break;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{
    // Clear the interrupt flag.

    IFS0CLR = 1 << 4;
    toasterData.on = 1;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.

    IFS0CLR = 1 << 12;
    toasterData.buttonPressCounter++;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    toasterData.buttonEvent = ButtonsCheckEvents();
}

