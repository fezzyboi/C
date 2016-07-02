/*
 * Ric Rodriguez
 * rirrodri
 * Lab2.c
 * CE13L-02
 * PIC-32 Calculator
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

double Round(double operand);
double AbsoluteValue(double number);
double FahrenheitToCelsius(double number);
double CelsiusToFahrenheit(double number);
double Average(double firstNumber, double secondNumber);
double Tangent(double number);


// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
   // BOARD_Init();

#else

int their_main(void)
{
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    printf("Welcome to Ric's calculator program\n");
    while (1) {
        printf("Enter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        char input, newLine;
        scanf("%c%c", &input, &newLine);
        double num1 = 0, num2 = 0, result;
        switch (input) {
        case '*':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            printf("Enter the second operand: ");
            scanf("%lf%c", &num2, &newLine);
            result = num1*num2;
            printf("Result of(%f * %f): %f\n", num1, num2, result);
            break;
        case '/':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            printf("Enter the second operand: ");
            scanf("%lf%c", &num2, &newLine);
            result = (num2==0)?0:(num1/num2);
            printf("Result of(%f / %f): %f\n", num1, num2, result);
            break;
        case '+':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            printf("Enter the second operand: ");
            scanf("%lf%c", &num2, &newLine);
            result = num1 + num2;
            printf("Result of(%f + %f): %f\n", num1, num2, result);
            break;
        case '-':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            printf("Enter the second operand: ");
            scanf("%lf%c", &num2, &newLine);
            result = num1 - num2;
            printf("Result of(%f - %f): %f\n", num1, num2, result);
            break;
        case 'a':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            result = AbsoluteValue(num1);
            printf("Result of |%f|: %f\n", num1, result);
            break;
        case 'v':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            printf("Enter the second operand: ");
            scanf("%lf%c", &num2, &newLine);
            result = Average(num1, num2);
            printf("Result of Average(%f, %f: %f\n", num1, num2, result);
            break;
        case 'f':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            result = CelsiusToFahrenheit(num1);
            printf("Result of %f deg->F: %f\n", num1, result);
            break;
        case 'c':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            result = FahrenheitToCelsius(num1);
            printf("Result of %f deg->C: %f\n", num1, result);
            break;
        case 't':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            result = Tangent(num1);
            printf("Result of tan(%f): %f\n", num1, result);
            break;
        case 'r':
            printf("Enter the first operand: ");
            scanf("%lf%c", &num1, &newLine);
            result = Round(num1);
            printf("Result of round(%f): %f\n", num1, result);
            break;
        default:
            printf("Invalid operator\n");
            break;
        }
    }

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1);
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double number)
{
    return number > 0 ? number : number*-1;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double number)
{
    return (5.0 / 9.0)*(number - 32.0);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double number)
{
    return number / (5.0 / 9.0) + 32;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double firstNumber, double secondNumber)
{
    return (firstNumber + secondNumber) / 2.0;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double Tangent(double number)
{
    return tan(number * M_PI / 180.0);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    int result = 0;
    if (operand > 0) {
        return result = (int) (operand + 0.5);
    }
    return result = (int) (operand - 0.5);
}
