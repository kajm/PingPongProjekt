/*
* PWMFunctions.c
*
* Created: 2015-06-11 09:57:35
*  Author: Karl Mösenbacher
*/

#include <asf.h>
#include "PWMFunctions.h"

static pwm_channel_t pwm_channel_instance;


/* Funktionen initierar PWM-signalen */
void pwm_setup(void)
{
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL);
	
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PWM_PERIOD,
		.ul_clkb = 0,
		.ul_mck = SYS_CLOCK
	};
	
	pwm_init(PWM, &clock_setting);
	
	pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	pwm_channel_instance.polarity = PWM_LOW;
	pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwm_channel_instance.ul_period = PWM_PERIOD;
	pwm_channel_instance.ul_duty = PWM_INIT_DUTY_CYCLE;
	pwm_channel_instance.channel = PWM_CHANNEL;
	
	pwm_channel_init(PWM, &pwm_channel_instance);
	pio_set_peripheral(PWM_PIO, PWM_PERIPHERAL, PWM_PIN);
	pwm_channel_enable(PWM, PWM_CHANNEL);
}


/* Funktionen uppdaterar PWM-signalens duty-cycle */
void update_pwm(int ul_duty)
{
	// Kontrollerar så att angiven duty-cycle befinner sig inom rätt område (0-1000).
	if(ul_duty < PWM_MIN_DUTY_CYCLE)
	{
		pwm_channel_update_duty(PWM, &pwm_channel_instance, PWM_MIN_DUTY_CYCLE);
	}
	else if(ul_duty > PWM_MAX_DUTY_CYCLE)
	{
		pwm_channel_update_duty(PWM, &pwm_channel_instance, PWM_MAX_DUTY_CYCLE);
	}
	else
	{
		pwm_channel_update_duty(PWM, &pwm_channel_instance, ul_duty);
	}
}


/* Funktionen initierar motor-shielden */
void motor_shield_setup(void)
{
	ioport_set_pin_dir(PIO_PD8_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PD8_IDX, IOPORT_PIN_LEVEL_HIGH);
	
	ioport_set_pin_dir(PIO_PC21_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PC21_IDX, IOPORT_PIN_LEVEL_LOW);
}