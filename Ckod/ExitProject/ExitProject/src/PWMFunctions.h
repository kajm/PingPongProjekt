/*
 * PWMFUNCTONS.h
 *
 * Created: 2015-06-11 09:57:50
 *  Author: Karl Mösenbacher
 */ 


#ifndef PWMFUNCTONS_H_
#define PWMFUNCTONS_H_

void pwm_setup(void);
void update_pwm(int);
void motor_shield_setup(void);

// Generella PWM-inställningar.
#define SYS_CLOCK			sysclk_get_cpu_hz()	// Systemklocka (Hz).
#define PWM_FREQUENCY		1000				// PWM-frekvens (Hz).
#define PWM_PERIOD			1000				// PWM-perioder ("bits").
#define PWM_INIT_DUTY_CYCLE	0					// Duty-cycle vid initiering.
#define PWM_MIN_DUTY_CYCLE	0					// Minsta värde för duty-cycle.
#define PWM_MAX_DUTY_CYCLE	1000				// Största värde för duty-cycle.

// PIO och PWM för reglerfläkt (pinne 7).
#define PWM_PIO			PIOC					// Pin base address.
#define PWM_PERIPHERAL	PIO_PERIPH_B			// Pin peripheral.
#define PWM_PIN			PIO_PC23B_PWML6			// Pin port.
#define PWM_CHANNEL		PWM_CHANNEL_6			// PWM-channel.

#endif /* PWMFUNCTONS_H_ */