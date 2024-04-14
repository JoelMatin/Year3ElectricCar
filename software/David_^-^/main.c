// Modifié par David 12/05

#include <stdio.h>
#include <stdlib.h>
#define FCY 3685000     // cycle frequency. 
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h" 
#include <p33FJ128MC802.h>
#include "regulation.h"
/*
 * 
 */

#define MAX_REV_CNT 1439
#define REF_SPEED 1000

void PWM_init (void) {
     
    P1TCONbits.PTMOD = 0b00; 
    P1TCONbits.PTCKPS = 0b00; 
    P1TCONbits.PTOPS = 0b0000;
    P1TMR = 0;
    P1TPER = 800; 
    
    PWM1CON1bits.PMOD1 = 0;
    PWM1CON1bits.PMOD2 = 0;
    PWM1CON1bits.PMOD3 = 0;
    PWM1CON1bits.PEN1H = 1;
    PWM1CON1bits.PEN2H = 1;
    PWM1CON1bits.PEN3H = 1;
    PWM1CON1bits.PEN1L = 1;
    PWM1CON1bits.PEN2L = 1;
    PWM1CON1bits.PEN3L = 1;
    
    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD3L = 1;
    P1OVDCONbits.POVD2L = 1;
    P1OVDCONbits.POVD1L = 1;
    
    IFS3bits.PWM1IF     = 0;
    IFS3bits.FLTA1IF    = 0;
    
    P1OVDCON            = 0x3F00;
    PWM1CON2            = 0x0000;
    
    P1TCONbits.PTEN = 1;
}

void PORT_init(void){
    AD1PCFGL = 0xFFFF;
    
    QEI1CONbits.PCDOUT = 1;
    QEI2CONbits.PCDOUT = 1;
    
    RPINR16bits.QEB2R = 0b00011;
    RPINR16bits.QEA2R = 0b00010;
    RPOR2bits.RP4R = 0b11011;
    
    RPINR14bits.QEB1R = 0b00110;
    RPINR14bits.QEA1R = 0b00111;
    RPOR4bits.RP8R    =    0b11010;
}

void ENCODER_init(void)
{
    DFLT1CONbits.QEOUT = 1;
    DFLT2CONbits.QEOUT = 1;
    
    DFLT1CONbits.QECK = 0b011;
    DFLT2CONbits.QECK = 0b011;
    
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;
    
    QEI1CONbits.SWPAB = 0;
    QEI2CONbits.SWPAB = 0;
    
    QEI1CONbits.CNTERR = 0;
    QEI2CONbits.CNTERR = 0;
    
    IEC3bits.QEI1IE = 1;
    IEC4bits.QEI2IE = 1;
    
    MAX1CNT = 4*360-1;
    MAX2CNT = 4*360-1;
}

void __attribute__((interrupt, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF=0;
    nb_tours1 = nb_tours1 +1;
}

void __attribute__((interrupt, no_auto_psv)) _QEI2Interrupt(void)
{
    IFS4bits.QEI2IF=0;
    nb_tours2 = nb_tours2 +1;
}

int main(void){
    
    IPC14bits.QEI1IP = 0b001; 
    IPC18bits.QEI2IP = 0b001; 


    PWM_init(); 
    PORT_init();
    ENCODER_init(); 
    
    TRISAbits.TRISA1 = 0;   
    TRISAbits.TRISA0 = 0;   
    LATAbits.LATA0 = 1; //RM: A0 = 1: devant
    LATAbits.LATA1 = 0; //LM: A1 = 1: devant 
    
    parameters(&totalDistance, &Kp, &continuousDistance,  &continuousDuration, &totDuration, &accelDistance, 
        &decelDistance, &accelDuration, &decelDuration);
;
    T1_init(); 
    while(1) {
        P1DC1 = 1000*dutyCycle1; 
        P1DC2 = 1000*ddutyCycle2; 
}
    return 0;
}
