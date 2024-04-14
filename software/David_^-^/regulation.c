// Modifié par David le 12/05

#include <stdio.h>
#include <stdlib.h>
#define FCY 3685000     
#include "libpic30.h"   
#include "xc.h"
#include <p33FJ128MC802.h>
#include <math.h>
#include "regulation.h"

#define KPt 3.69
#define KPr 12.59      
#define PI 3.14159
#define E 0.135
#define RADIUS 0.041


float accelDuration = 1;
float decelDuration = 1;
float continuousDuration = 0;
float totalDuration = 0;

float Kp = 0;
bool isRot = true;
float totalAngle = 90;
float totalDistance = 0;
float accelDistance = 0.25;             
float decelDistance = 0.25;
float continuousDistance = 0;        


void parameters(float* totalDistance, float* Kp, float* continuousDistance,
        float* continuousDuration, float* totalDuration, float* accelDistance,
        float* decelDistance, float* accelDuration, float* decelDuration) {
    if (isRot) {
        *totalDistance = totalAngle * PI / 180 * E / 2;
        *Kp = KPr;
        if (*totalDistance > 0.5) {
            *continuousDistance = *totalDistance - *accelDistance - *decelDistance;
            *continuousDuration = *continuousDistance / 0.5;
            *totalDuration = *accelDuration + *decelDuration + *continuousDuration;
        }
        else {
            *continuousDistance = 0;
            *continuousDuration = 0;
            *accelDistance = *totalDistance / 2;
            *decelDistance = *totalDistance / 2;
            *accelDuration = sqrt(2 * *accelDistance / 0.5);
            *decelDuration = sqrt(2 * *decelDistance / (0.5));
            *totalDuration = *accelDuration + *decelDuration;
        }
    }
    else {
        *totalDistance = 2.5;
        *Kp = KPt;
        *continuousDistance = *totalDistance - *accelDistance - *decelDistance;
        *continuousDuration = *continuousDistance / 0.5;
        *totalDuration = *accelDuration + *decelDuration + *continuousDuration;
    }
}

float t = 0;
float setPoint = 0;
float threshold = 0.01;

float lapCount1 = 0;
float lapCount2 = 0;

float pos1 = 0;
float pos2 = 0;
float pos1_degree = 0;
float pos2_degree = 0;

float error1 = 0;
float error2 = 0;
float totalError1 = 0;
float totalError2 = 0;

float dutyCycle1 = 0;
float dutyCycle2 = 0;

void T1_init() {
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = FCY / 1000 + 1; // Load the period value= 1ms
    IPC0bits.T1IP = 0b010; // Set Timer1 Interrupt Priority Level
    //IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}


void calculate_pos1() {
    float pos1_degree = lapCount1 * 360 + (POS1CNT + 1) / 4;      
    pos1 = pos1_degree * 2 * PI / 360 * RADIUS;                   
}

void calculate_pos2() {
    pos2_degree = lapCount2 * 360 + (POS2CNT + 1) / 4;
    pos2 = pos2_degree * 2 * PI / 360 * RADIUS;
}

void regulate(float setPoint) {
    calculate_pos1();
    calculate_pos2();

    error1 = setPoint - pos1;
    error2 = setPoint - pos2;
    dutyCycle1 = Kp * error1;
    dutyCycle2 = Kp * error2;
}


void accelerate() {
    setPoint = 0.5 * pow(t, 2) / 2;  ///2.5000e-07
    regulate(setPoint);
}

void continueMotion() {
    float t_continue = t - accelDuration;
    setPoint = 0.5 * t_continue + accelDistance;
    regulate(setPoint);
}

void decelerate() {
    float t_decel = t - accelDuration - continuousDuration;
    if (totalDistance <= 0.5) {
        setPoint = -0.5 * pow(t_decel, 2) / 2 + accelDistance + (0.5 * accelDuration) * t_decel;
        regulate(setPoint);
    }
    else {
        setPoint = -0.5 * pow(t_decel, 2) / 2 + accelDistance + continuousDistance + 0.5 * t_decel;
        regulate(setPoint);
    }
}

void stopMotion() {
    dutyCycle1 = 0;
    dutyCycle2 = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    t = t + 0.001;
    totalError1 = totalDistance - pos1;
    totalError2 = totalDistance - pos2;
    if ((totalError1 > threshold) || (totalError2 > threshold)) {
        if (t < accelDuration && t > 0) {
            accelerate();
        }
        

        else if (t > accelDuration + continuousDuration) {
            decelerate();
        }
    }
    else {

        stopMotion();

    }
    IFS0bits.T1IF = 0; 
}
