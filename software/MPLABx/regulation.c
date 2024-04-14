#include <stdio.h>
#include <stdlib.h>
#define FCY 40000000     // µcycle frequency. 
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h" 
#include <p33FJ128MC802.h>
#include <math.h>
#include "regulation.h"

#define KPt 3.69
#define KPr 11      //1.03/(2*0.135 )
#define PI 3.14159265358979323846
#define E 0.135
#define RAYON 0.041


float Kp = 0; 
int rot  ; 
float angle_total ; 
float distance_totale ;

float distance_accel = 0.25;             //const because acc= 1
float distance_deccel = 0.25; 
float distance_continue = 0;         //const because decc= -1

float duree_accel = 1 ; 
float duree_deccel = 1;
float duree_continue = 0; 
float duree_totale = 0; 

void calcul(float* distance_totale, float* Kp, float* distance_continue, 
        float* duree_continue, float* duree_totale,float* distance_accel, 
        float* distance_deccel,float* duree_accel, float* duree_deccel){
    if(rot){
        *distance_totale = angle_total * PI/180 * E/2;
        *Kp = KPr;
        if(*distance_totale>0.5){
        *distance_continue = *distance_totale - *distance_accel - *distance_deccel; 
        *duree_continue = *distance_continue/0.5; 
        *duree_totale = *duree_accel + *duree_deccel + *duree_continue;
        }
        else{
        *distance_continue = 0; 
        *duree_continue = 0; 
        *distance_accel = *distance_totale/2;
        *distance_deccel = *distance_totale/2;
        *duree_accel = sqrt(2 * *distance_accel/0.5); 
        *duree_deccel = sqrt(2 * *distance_deccel/(0.5)); 
        *duree_totale = *duree_accel + *duree_deccel ;
        }
    }
    else{
        *distance_totale = 2.5; 
        *Kp = KPt;
        *distance_continue = *distance_totale - *distance_accel - *distance_deccel; 
        *duree_continue = *distance_continue/0.5; 
        *duree_totale = *duree_accel + *duree_deccel + *duree_continue;
    }
     
}

float t = 0; 
float csg = 0; 
float seuil = 0.01; 

float nb_tours1 = 0; 
float nb_tours2 = 0; 

float pos1 = 0; 
float pos2 = 0; 
float pos1_degree = 0;
float pos2_degree = 0; 

float error1 = 0; 
float error2= 0;
float error_totale1 = 0; 
float error_totale2 = 0;


float dc1 = 0;  
float dc2  = 0; 

void T1_init(){
    T1CONbits.TCS = 0; // Select internal instruction cycle clock 
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 =  FCY/1000+1; // Load the period value= 1ms
    IPC0bits.T1IP = 0b010; // Set Timer1 Interrupt Priority Level
    //IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}

//calculates the distances in meters done by prototype
void calculate_pos1(){
    float pos1_degree = nb_tours1*360 + (POS1CNT+1)/4;      //POSCNT from 0 to 1439 because 4x360 mode 
    pos1 = pos1_degree*2*PI/360*RAYON;                   // 0.041 radius of the wheels 
}

void calculate_pos2(){
    pos2_degree = nb_tours2*360 + (POS2CNT+1)/4;
    pos2 = pos2_degree*2*PI/360*RAYON;
}


void reguler(float csg){
    calculate_pos1(); 
    calculate_pos2(); 
    
    error1 = csg-pos1; 
    error2 = csg-pos2;  
    dc1 = Kp * error1; 
    dc2 = Kp * error2; 
    
}

//defines the input signal
void accel(){
    csg = 0.5*pow(t,2)/2;  ///2.5000e-07
    reguler(csg);  
}

void continu(){
    float t_continue = t-duree_accel; 
    csg = 0.5*t_continue + distance_accel; 
    reguler(csg); 
}

void deccel(){
    float t_deccel = t-duree_accel-duree_continue; 
    if (distance_totale<=0.5){
        csg = -0.5*pow(t_deccel,2)/2 + distance_accel + (0.5*duree_accel)*t_deccel; 
        reguler(csg);
    }
    else{
        csg = -0.5*pow(t_deccel,2)/2 + distance_accel + distance_continue + 0.5*t_deccel; 
        reguler(csg);
    }
     
}

void arrete(){
    dc1 = 0; 
    dc2 = 0; 
    /*calculate_pos1(); 
    calculate_pos2(); 
    csg = distance_totale; 
    error1 = csg-pos1; 
    error2 = csg-pos2;  
    dc1 = Kp * error1; 
    dc2 = Kp * error2; */
}



void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
t = t+0.001; 
error_totale1 = distance_totale-pos1; 
error_totale2 = distance_totale-pos2; 
if ( (error_totale1 > seuil) || (error_totale2 > seuil) ){
    if (t<duree_accel && t>0){
       accel();
    }
    //else if(t<duree_continue + duree_accel){
    //    continu(); 
    //}

    else if(t>duree_accel +duree_continue){
        deccel(); 
    }
}
else{
    
    arrete(); 
    
}
IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
}
