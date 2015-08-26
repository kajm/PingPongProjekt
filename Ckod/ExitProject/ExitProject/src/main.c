/*
 * main.c
 *
 * Created: 2015-03-30 12:46:18
 *  Author: Andreas Bj�rntoft
 */ 

#include <asf.h>
#include "GlobalVar.h"
#include "PIDController.h"
#include "Communication.h"
#include "Sensor.h"
#include "PWMFunctions.h"

// Globala variabler till plot initieras.
float global_u = 1.0;
float global_d = 1.0;
float global_e = 1.0;


// Globala variabler till PID-reglering initieras.
float global_Bv = 1;		// V�ljs av anv�ndaren via Matlab.
float global_Kp = 0.75;		// 0.36 enligt Ziegler-Nichols sv�ngningsmetod.
float global_Ti = 1.95;		// 1.77 enligt Ziegler-Nichols sv�ngningsmetod.
float global_Td = 0.65;		// 0.4425 enligt Ziegler-Nichols sv�ngningsmetod.
float global_dT = 0.05;		// Samplingstid p� 50 ms.


/* HUVUDPROGRAM */
int main (void)
{
	/* INITIERING */
	sysclk_init();			// Systemklocka.
	board_init();			// Arduino-kort.
	configure_console();	// Konsoll-/terminalf�nster.
	adc_setup();			// AD-omvandlare.
	pwm_setup();			// PWM-signal.
	motor_shield_setup();	// Motor-shield.

	/* PROCESS 1 - PID-REGLERING */
	if (xTaskCreate(pid, (const signed char * const) "PID-reglering", 1024, NULL, 2, NULL) != pdPASS)
	{
		printf("Misslyckades med att skapa process f�r PID-reglering.\r\n");
	}
	
	/* PROCESS 2 - KOMMUNIKATION MATLAB */
	if (xTaskCreate(matlab, (const signed char * const) "Matlab-kommunikation", 1024, NULL, 1, NULL) != pdPASS)
	{
		printf("Misslyckades med att skapa process f�r kommunikation med Matlab.\r\n");
	}
	
	// Schemal�ggning startar.
	vTaskStartScheduler();
}
