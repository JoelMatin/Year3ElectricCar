

#include <stdio.h>
#include <stdlib.h>
#define FCY 40000000     // cycle frequency. 
//#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h" 
//#include <p33FJ128MC802.h>
#include "regulation.h"
#include"configuration.h"
/*
 * 
 */

#define MAX_REV_CNT = 1439;  
#define REF_SPEED 1000


    char element = 0 ;
    int compteur = 0  ;
    int param = 0;

    



void PWM_init (void) {
     
    P1TCONbits.PTMOD = 0b00; 
    P1TCONbits.PTCKPS = 0b00; 
    P1TCONbits.PTOPS = 0b0000;
    P1TMR = 0;
    P1TPER = 800; 
    
    PWM1CON1bits.PMOD1 = 0;
    PWM1CON1bits.PMOD2 = 0;
    PWM1CON1bits.PEN1H = 1;
    PWM1CON1bits.PEN2H = 1;
    PWM1CON1bits.PEN1L = 1;
    PWM1CON1bits.PEN2L = 1;
    
    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD2L = 1;
    P1OVDCONbits.POVD1L = 1;
    
    IFS3bits.PWM1IF     = 0;
    IFS3bits.FLTA1IF    = 0;
    
    
    P1OVDCON            = 0x3F00;   //Disable override; override disables PWM
    PWM1CON2            = 0x0000;   
    
    //P1FLTACON = 0x0000;   //Disable faults
    
    
    P1TCONbits.PTEN = 1; 
    //counting period of P1TPER = FCY/(Fpwm*P1TMRprescaler*2) -1 
            
}

void PORT_init(void){
    AD1PCFGL = 0xFFFF;
    
    //allows output of counting direction 
    QEI1CONbits.PCDOUT = 1; 
    QEI2CONbits.PCDOUT = 1; 
    
    
   
    //assignment of pins to QEI peripheral inputs and outputs 
    /*RPINR16bits.QEB2R = 0b00011;     //RB2 to A2  (left motor)
    RPINR16bits.QEA2R = 0b00010;     //RB3 to B2
    
    RPINR14bits.QEB1R = 0b00110;     //RB6 to A1 
    RPINR14bits.QEA1R = 0b00111;     //RB7 to B1   (right motor)*/
     
    RPINR18bits.U1RXR = 0b01000; 
    
    
}


void ENCODER_init(void)
{
    //Enables the filter for QEI input
    DFLT1CONbits.QEOUT = 1; 
    DFLT2CONbits.QEOUT = 1; 
    
    //Clock divide 1:16 of filter
    DFLT1CONbits.QECK = 0b011; 
    DFLT2CONbits.QECK = 0b011; 
    
    //position measurement resolution x4 with reset 
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111; 
    
    //Phase A of QE connected to A of QEI and B to B 
    QEI1CONbits.SWPAB = 0;
    QEI2CONbits.SWPAB = 0; 
    
    //Clear any count errors
    QEI1CONbits.CNTERR = 0; 
    QEI2CONbits.CNTERR = 0; 
    
    //Event interrupt enable bits
    IEC3bits.QEI1IE = 1; 
    IEC4bits.QEI2IE = 1;
    
    //configure MAXxCNT limit of POSxCNT 
    MAX1CNT = 4*360-1; 
    MAX2CNT = 4*360-1; 
}

//when QEIM = 101, POSxCNT serves as a timer register and MAXxCNT as a period such that 
//when MAXxCNT = POSxCNT: IF is asserted 
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

void PARAM_init(int order){
    if(order == 0b00){
        rot = 0; 
        distance_totale = (float) param ;
        LATAbits.LATA0 = 1; //right motor: A0 = 1: forward movement
        LATAbits.LATA1 = 1; //left motor: A1 = 1: forward movement
        RPINR16bits.QEB2R = 0b00010;     //RB2 to A2  (left motor)
        RPINR16bits.QEA2R = 0b00011;      //RB3 to B2
    
        RPINR14bits.QEB1R = 0b00110;     //RB6 to A1 
        RPINR14bits.QEA1R = 0b00111;     //RB7 to B1   (right motor)
    }
    else if(order == 0b01){
        rot = 0; 
        distance_totale = (float) param ;
        LATAbits.LATA0 = 0; //right motor: A0 = 1: forward movement
        LATAbits.LATA1 = 0; //left motor: A1 = 1: forward movement 
        RPINR16bits.QEB2R = 0b00011;     //RB2 to A2  (left motor)
        RPINR16bits.QEA2R = 0b00010;     //RB3 to B2
    
        RPINR14bits.QEB1R = 0b00111;     //RB6 to A1 
        RPINR14bits.QEA1R = 0b00110;     //RB7 to B1   (right motor)
    }
    else if (order == 0b10){
        rot =1; 
        angle_total = (float) param ;
        LATAbits.LATA0 = 1; //right motor: A0 = 1: forward movement
        LATAbits.LATA1 = 0; //left motor: A1 = 1: forward movement
        
        
        RPINR16bits.QEB2R = 0b00010;     //RB2 to A2  (left motor)
        RPINR16bits.QEA2R = 0b00011;     //RB3 to B2
    
        RPINR14bits.QEB1R = 0b00111;     //RB6 to A1 
        RPINR14bits.QEA1R = 0b00110;    //RB7 to B1   (right motor)
    }
    else{
        
        angle_total = (float) param ;
        LATAbits.LATA0 = 0; //right motor: A0 = 1: forward movement
        LATAbits.LATA1 = 1; //left motor: A1 = 1: forward movement
        
        RPINR16bits.QEB2R = 0b00011;     //RB2 to A2  (left motor)
        RPINR16bits.QEA2R = 0b00010;     //RB3 to B2
    
        RPINR14bits.QEB1R = 0b00110;     //RB6 to A1 
        RPINR14bits.QEA1R = 0b00111;     //RB7 to B1   (right motor) 
        
    }
}


int main(void){
    
    frcPll40MHzConfig(); // On travail en 40MHz
    IPC14bits.QEI1IP = 0b001; 
    IPC18bits.QEI2IP = 0b001; 


    //PWM_init(); 
    //PORT_init();
    //ENCODER_init(); 
    
    // Uart Configuration
    
      /* Configuration du Peripheral Pin Select (PPS) pour connecter le signal
     * Rx de l'UART1 ? RB6/RP6 et le signal Tx ? RB7/RP7 */
	//_U1RXR = 12;    // U1RX -> RP6
    U1MODEbits.PDSEL = 0;       // 8 bits, pas de parit?
    U1MODEbits.STSEL = 0;       // 1 stop bit
    U1MODEbits.BRGH = 0;
    U1BRG = 10;

    U1MODEbits.UARTEN = 1;      // on active l'UART
    //U1STAbits.UTXEN = 1;        // on active l'?mission

    
    TRISAbits.TRISA1 = 0;   
    TRISAbits.TRISA0 = 0;   
         
    
    char donne[10] = {0} ;
    int order = 0 ;


    while(1) {
        while(compteur < 10){ // On attend de recevoir la trame de 10 bits
        if (U1STAbits.URXDA ) {      // si UART1 a re?u un octet
            element = U1RXREG;
            donne[compteur] =  element ;
            compteur++ ;
        }    
        }
    for (int i = 0; i < 2; i++) {  // Conversion des 2 MSB 
        order =  (order<<1)+ (donne[i]-'0') ;
        
    }    
    for (int i = 2; i < 10; i++) { // Conversion des 8 LSB
        param =  (param<<1)+ (donne[i]-'0') ; 
    }    
        
    PORT_init();
    PARAM_init(); 
    PWM_init(); 
    ENCODER_init(); 
    calcul(&distance_totale, &Kp, &distance_continue, &duree_continue, &duree_totale,&distance_accel, &distance_deccel,&duree_accel, &duree_deccel);
    T1_init(); 
    P1DC1 = 800*dc1; 
    P1DC2 = 800*dc2;

     //for (int i = 0; i < 10; i++) { /// Remise à 0 de data
     //  donne[i] = 0 ;
        
    //}   
       //param = 0 ;
       //order = 0 ;
      // compteur = 0 ; 
       //element = 0 ;
    }
    return 0;
}



