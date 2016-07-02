// heap size 2048
// **** Include libraries here ****
// Standard C libraries
#include <stdlib.h>
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Morse.h"
#include "Tree.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)
#define OLED_WIDTH 21

// **** Declare any data types here ****

typedef struct main_state {
    MorseEvent state;
} MainState;
static MainState mainState;
char topLineBuffer[OLED_WIDTH + 1];
char bottomLineBuffer[3 * OLED_WIDTH + 1];
char lineBuffer[4 * OLED_WIDTH + 1];
// **** Define any module-level, global, or external variables here ****

// **** Declare any function prototypes here ****
static void ClearTopLine(void);
static void TopAppend(char input);
static void BottomAppend(char input);

int main()
{
    BOARD_Init();
    OledInit();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);



    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    if (MorseInit() == STANDARD_ERROR) {
        printf("MorseInit failed\n");
        FATAL_ERROR();
    }
    while (1) {
        char ret;
        if (mainState.state) {
            switch (mainState.state) {
            case MORSE_EVENT_DOT:
                TopAppend(MORSE_CHAR_DOT);
                ret = MorseDecode(MORSE_CHAR_DOT);
                if (ret == STANDARD_ERROR) {
                    ClearTopLine();
                    BottomAppend('#');
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                    mainState.state = MORSE_EVENT_NONE;
                }
                break;
            case MORSE_EVENT_DASH:
                TopAppend(MORSE_CHAR_DASH);
                ret = MorseDecode(MORSE_CHAR_DASH);
                if (ret == STANDARD_ERROR) {
                    ClearTopLine();
                    BottomAppend('#');
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                    mainState.state = MORSE_EVENT_NONE;
                }
                break;
            case MORSE_EVENT_INTER_LETTER:
                ClearTopLine();
                ret = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                BottomAppend(ret == STANDARD_ERROR ? '#' : ret);
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                break;
            case MORSE_EVENT_INTER_WORD:
                ClearTopLine();
                ret = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                BottomAppend(ret == STANDARD_ERROR ? '#' : ret);
                BottomAppend(' ');
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                break;
            default:
                break;
            }
            mainState.state = MORSE_EVENT_NONE;

        }
    }



    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

static void ClearTopLine(void)
{
    OledClear(0);
    int itor = 0;
    for (; itor < OLED_WIDTH; itor++) {
        topLineBuffer[itor] = '\0';
    }
    sprintf(lineBuffer, "%s\n%s", topLineBuffer, bottomLineBuffer);
    OledDrawString(lineBuffer);
    OledUpdate();
}

static void BottomAppend(char input)
{
    OledClear(0);
    int itor = 0;
    for (; bottomLineBuffer[itor] != '\0'; itor++) {
    }
    if (itor < (3 * OLED_WIDTH)) {
        bottomLineBuffer[itor] = input;
    } else {
        itor = 0;
        for (; itor < (3 * OLED_WIDTH) - 1; itor++) {
            bottomLineBuffer[itor] = bottomLineBuffer[itor + 1];
        }
        bottomLineBuffer[itor] = input;
    }
    sprintf(lineBuffer, "%s\n%s", topLineBuffer, bottomLineBuffer);
    OledDrawString(lineBuffer);
    OledUpdate();

}

static void TopAppend(char input)
{
    OledClear(0);
    int itor = 0;
    for (; topLineBuffer[itor] != '\0'; itor++) {
    }
    if (itor < OLED_WIDTH) {
        topLineBuffer[itor] = input;
    }
    sprintf(lineBuffer, "%s\n%s", topLineBuffer, bottomLineBuffer);
    OledDrawString(lineBuffer);
    OledUpdate();

}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    mainState.state = MorseCheckEvents();
}

