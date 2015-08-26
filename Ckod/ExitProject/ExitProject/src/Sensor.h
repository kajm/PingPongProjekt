/*
 * Sensor.h
 *
 * Created: 2015-06-11 09:05:57
 *  Author: Karl Mösenbacher
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#define BUFFER_SIZE		10					// Buffert för filtrering av sensorvärden.
#define ADC_CHANNEL		ADC_CHANNEL_10		// Pinne (A8) för AD-omvanlding

void adc_setup(void);
void read_sensor(void);
void set_global_d(void);

#endif /* SENSOR_H_ */