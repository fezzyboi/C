/* 
 * File:   Cmath.c
 * Author: Ric
 *
 * Created on April 4, 2016, 11:07 AM
 */
#include "CMath.h"

float myabs(float x)
{
    return x > 0 ? x : (x*-1);
}

float enorm(float px, float py, float qx, float qy) 
{
    float dx = myabs(qx - px);
    float dy = myabs(qy - py);
    float g = 0, e = 0, t = 0, r = 0, s = 0;
    if (dx > dy) {
        g = dx;
        e = dy;
    } else {
        g = dy;
        e = dx;
    }
    int i = 0;
    while (i < 2) {
        t = e / g;
        r = t*t;
        s = r / (4.0 + r);
        g = g + 2 * s*g;
        e = e*s;
        i++;
    }
    return g;
}

float arctangent2(float p, float x) 
{
    float PI = 3.14159265359;
    float y = myabs(p), r = 0, rho = 0;
    if (x > 0) {
        r = (x - y) / (x + y);
        rho = 0.1963 * r * r * r - 0.9817 * r + PI / 4.0;
    } else {
        r = (x + y) / (x - y);
        rho = 0.1963 * r * r * r - 0.9817 * r + 3 * PI / 4.0;
    }
    if (y < 0) {
        rho *= -1;
    }
    return rho;
}
