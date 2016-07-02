/**
 * File: bounce_part1.c
 * @brief Bounces Leds at a speed in correspondence with the
 * number of switches and switch number activated.
 * @author Ric Rodriguez, 2016.05.11  */

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"
// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct TimerResult {
    uint8_t event;
    uint8_t value;
};

// **** Define global, module-level, or external variables here ****
static struct TimerResult timerData;

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

#define LEFT 1
#define RIGHT 0

    // Initialize direction and first led
    int direction = RIGHT;
    LEDS_SET(0x80);

    while (1) {

        // Check to see if event is triggered
        if (timerData.event) {

            // Check direction of leds
            if (direction == RIGHT) {

                // If at rightmost led, turn left
                // else shift to the right led
                if (LEDS_GET()&0x01) {
                    direction = LEFT;
                    LEDS_SET(0x02);
                } else {
                    int nextLed = LEDS_GET() >> 1;
                    LEDS_SET(nextLed);
                }
            } else {
                
                // If at leftmost led, turn right
                // else shift to the left led
                if (LEDS_GET()&0x80) {
                    direction = RIGHT;
                    LEDS_SET(0x40);
                } else {
                    int nextLed = LEDS_GET() << 1;
                    LEDS_SET(nextLed);
                }
            }
            
            // Reset event to false
            timerData.event = 0;
        }
    }
    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    
    // Increment timer value
    timerData.value++;
    
    // Compare timervalue with switch number
    if (timerData.value > SWITCH_STATES()) {
        timerData.event = 1;
        timerData.value = 0;
    }




}