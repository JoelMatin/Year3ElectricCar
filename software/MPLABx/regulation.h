/* 
 * File:   regulation.h
 * Author: joelm
 *
 * Created on April 25, 2023, 5:09 PM
 */

#pragma once 

#ifndef REGULATION_H
#define	REGULATION_H

extern float nb_tours1;
extern float nb_tours2;
extern float dc1; 
extern float dc2; 

extern int rot ;
extern float angle_total ;
extern float distance_totale; 
extern float Kp; 
extern float distance_continue; 
extern float duree_continue; 
extern float duree_totale; 
extern float duree_accel;
extern float duree_deccel;
extern float distance_accel;
extern float distance_deccel;


void calcul(float* distance_totale, float* Kp, float* distance_continue, 
        float* duree_continue, float* duree_totale,float* distance_accel, 
        float* distance_deccel,float* duree_accel, float* duree_deccel);
void T1_init(); 

//calculates the distances in meters done by prototype
void calculate_pos1(); 

void calculate_pos2(); 


void reguler(float csg); 

//defines the input signal
void accel(); 

void continu(); 

void deccel(); 

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void); 

#endif	/* REGULATION_H */


