/**
 * File: bounce_part3.c
 * @brief Turns LEDS on and off depending on switches and buttons pressed.
 * @author Ric Rodriguez, 2016.05.11  */

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Buttons.h"
#include "Leds.h"
// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****

struct ButtonEvent {
    uint8_t flag;
    uint8_t value;
};
static struct ButtonEvent buttonEvent;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    LEDS_INIT();
    ButtonsInit();

    while (1) {
        if (buttonEvent.flag) {
            uint8_t switches = SWITCH_STATES();
            if (((switches & SWITCH_STATE_SW1) == 0) && 
                (buttonEvent.value & BUTTON_EVENT_1DOWN))) {
                LEDS_SET((LEDS_GET()^0x03));

            }
            if (((switches & SWITCH_STATE_SW2) == 0) && 
                ((buttonEvent.value & BUTTON_EVENT_2DOWN))) {
                LEDS_SET((LEDS_GET()^0x0C));
            }

            if (((switches & SWITCH_STATE_SW3) == 0) && 
                ((buttonEvent.value & BUTTON_EVENT_3DOWN))) {
                LEDS_SET((LEDS_GET()^0x30));
            }
            if (((switches & SWITCH_STATE_SW4) == 0) && 
                ((buttonEvent.value & BUTTON_EVENT_4DOWN))) {
                LEDS_SET((LEDS_GET()^0xC0));
            }
            if ((switches & SWITCH_STATE_SW1) && 
                (buttonEvent.value & BUTTON_EVENT_1UP)) {
                LEDS_SET((LEDS_GET()^0x03));
            }

            if ((switches & SWITCH_STATE_SW2) &&
                (buttonEvent.value & BUTTON_EVENT_2UP)) {
                LEDS_SET((LEDS_GET()^0x0C));
            }

            if ((switches & SWITCH_STATE_SW3) && 
                (buttonEvent.value & BUTTON_EVENT_3UP)) {
                LEDS_SET((LEDS_GET()^0x30));
            }
            if ((switches & SWITCH_STATE_SW4) && 
                (buttonEvent.value & BUTTON_EVENT_4UP)) {
                LEDS_SET((LEDS_GET()^0xC0));
            }

            buttonEvent.flag = 0;
            buttonEvent.x=1;
        }
        ButtonsInit();
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    buttonEvent.value = ButtonsCheckEvents();
    if (buttonEvent.value) {
        buttonEvent.flag = 1;
    }
}