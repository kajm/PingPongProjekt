/*
 * PIDController.c
 *
 * Created: 2015-04-01 10:57:27
 *  Author: Andreas Bjärntoft & Karl Mösenbacher
 */ 

#include <asf.h>
#include "GlobalVar.h"
#include "PIDController.h"
#include "Sensor.h"
#include "PWMFunctions.h"


// Variabler för reglering.
float distanceSensor = 0.0;
float eDif = 0.0;
float eSum = 0.0;


/* Funktionen genomför en PID-reglering */
void pid(void *p)
{
	// Initiering av variabler för PID-reglering.
	float P = 0.0;
	float I = 0.0;
	float D = 0.0;
	
	for(;;) 
	{
		// Säkerställer jämn samplingstid.
		portTickType xLastWakeTime;
		const portTickType xTimeIncrement = SAMPLING_TIME;
			
			
		/* PID-REGLERING */
			
		// Avstånd läses av och beräknas.
		read_sensor();		// Avläser nytt sensorvärde
		set_global_d();		// Beräknar nytt avståndsvärde
			
		// Variabler till regleringen beräknas.
		eDif = global_e;						// Förbereder beräkningen av skillnaden mellan föregående och nuvarande felvärde.
		global_e = global_Bv - global_d;		// Beräknar felvärdet.	
		eDif = global_e - eDif;					// Beräknar skillnaden mellan föregående och nuvarande felvärde.
			
		// Motverkar skenande I-del.
		if(global_u <= 1000.0 && global_u >= 0)
		{
			eSum = eSum + global_e;				// Beräknar summan av samtliga felvärden.
			I = (global_dT/global_Ti) * eSum;	// Beräknar I-delen.
		}
			
		P = global_e;							// Beräknar P-delen.
		D = (global_Td/global_dT) * eDif;		// Beräknar D-delen.
			
		// Styrsignalen beräknas (PID-reglering).
		global_u = global_Kp * (P+I+D);
			
		// Styrsignalen skickas som PWM-signal.
		update_pwm((uint32_t)global_u);
		
		// Paus som motsvarar samplingstiden.
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}