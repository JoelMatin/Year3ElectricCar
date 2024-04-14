// Dernière modif: 02/05 par David

#pragma once 

#ifndef REGULATION_H
#define	REGULATION_H

extern float dutyCycle1; 
extern float dutyCycle2; 
extern float lapCount1;
extern float lapCount2;

extern float accelDuration;
extern float decelDuration;
extern float accelDistance;
extern float decelDistance;
extern float totDuration; 
extern float totalDistance; 
extern float Kp; 
extern float continuousDistance; 
extern float continuousDuration; 


void parameters(float* totalDistance, float* Kp, float* continuousDistance, 
        float* continuousDuration, float* totDuration, float* accelDistance, 
        float* decelDistance, float* accelDuration, float* decelDuration);
void T1_init(); 


void calculatePosition1(); 

void calculatePosition2(); 


void regulate(float inputSignal); 


void accelerate(); 

void continueMotion(); 

void decelerate(); 

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void); 

#endif	
