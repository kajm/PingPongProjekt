/*
* Sensor.c
*
* Created: 2015-06-11 09:05:41
*  Author: Karl M�senbacher
*/

#include <asf.h>
#include "GlobalVar.h"
#include "Sensor.h"

static int sensorBuffer[BUFFER_SIZE] = {0.0};


/* Funktionen initierar 10-bitars ADC p� pinne A8 */
void adc_setup(void)
{
	pmc_enable_periph_clk(ID_ADC);
	
	adc_init(ADC, sysclk_get_main_hz(), 1000000, 8);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_10);
	adc_enable_channel(ADC, ADC_CHANNEL);
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
}


/* Funktionen l�ser av sensorv�rdet */
void read_sensor(void)
{
	adc_start(ADC);
	
	int i = 0;
	
	// Ompositionering i buffert.
	for(i=BUFFER_SIZE-1; i>0; i--)
	{
		sensorBuffer[i] = sensorBuffer[i-1];
	}
	
	// Inl�sning av nytt sensorv�rde i buffert.
	sensorBuffer[0] = adc_get_channel_value(ADC,ADC_CHANNEL_10);
}


/* Funktionen ber�knar nytt distansv�rde */
void set_global_d(void)
{
	// Nollst�ller befintligt distansv�rde.
	global_d = 0.0;
	
	// Summerar de senaste sensorv�rdena (f�r att kunna ber�kna medelv�rde).
	int i = 0;
	for(i=0; i<BUFFER_SIZE; i++)
	{
		global_d = global_d + (float)sensorBuffer[i];
	}
	
	// Ber�knar nytt distansv�rde (motsvarar medelv�rdet av de senaste sensorv�rdena).
	global_d = global_d / BUFFER_SIZE;
}