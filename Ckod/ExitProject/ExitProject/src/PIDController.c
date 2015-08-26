/*
 * PIDController.c
 *
 * Created: 2015-04-01 10:57:27
 *  Author: Andreas Bj�rntoft & Karl M�senbacher
 */ 

#include <asf.h>
#include "GlobalVar.h"
#include "PIDController.h"
#include "Sensor.h"
#include "PWMFunctions.h"


// Variabler f�r reglering.
float distanceSensor = 0.0;
float eDif = 0.0;
float eSum = 0.0;


/* Funktionen genomf�r en PID-reglering */
void pid(void *p)
{
	// Initiering av variabler f�r PID-reglering.
	float P = 0.0;
	float I = 0.0;
	float D = 0.0;
	
	for(;;) 
	{
		// S�kerst�ller j�mn samplingstid.
		portTickType xLastWakeTime;
		const portTickType xTimeIncrement = SAMPLING_TIME;
			
			
		/* PID-REGLERING */
			
		// Avst�nd l�ses av och ber�knas.
		read_sensor();		// Avl�ser nytt sensorv�rde
		set_global_d();		// Ber�knar nytt avst�ndsv�rde
			
		// Variabler till regleringen ber�knas.
		eDif = global_e;						// F�rbereder ber�kningen av skillnaden mellan f�reg�ende och nuvarande felv�rde.
		global_e = global_Bv - global_d;		// Ber�knar felv�rdet.	
		eDif = global_e - eDif;					// Ber�knar skillnaden mellan f�reg�ende och nuvarande felv�rde.
			
		// Motverkar skenande I-del.
		if(global_u <= 1000.0 && global_u >= 0)
		{
			eSum = eSum + global_e;				// Ber�knar summan av samtliga felv�rden.
			I = (global_dT/global_Ti) * eSum;	// Ber�knar I-delen.
		}
			
		P = global_e;							// Ber�knar P-delen.
		D = (global_Td/global_dT) * eDif;		// Ber�knar D-delen.
			
		// Styrsignalen ber�knas (PID-reglering).
		global_u = global_Kp * (P+I+D);
			
		// Styrsignalen skickas som PWM-signal.
		update_pwm((uint32_t)global_u);
		
		// Paus som motsvarar samplingstiden.
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}