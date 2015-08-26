/*
 * Communication.c
 *
 * Created: 2015-04-01 11:02:53
 *  Author: Andreas Bjärntoft
 */ 

#include <asf.h>
#include "GlobalVar.h"
#include "Communication.h"

// Variabel för lagring av avläst data.
char recivedData[40];		


/* Funktionen konfiguerar kommunikationskanal via UART */
void configure_console(void) {
	const usart_serial_options_t uart_serial_options = { .baudrate = CONF_UART_BAUDRATE, .paritytype = CONF_UART_PARITY };
	
	/* Konfigurera konsol UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	ioport_set_pin_mode(PIO_PA8_IDX, IOPORT_MODE_PULLUP);
	
	/* Specifiera att stdout inte ska buffras */
	#if defined(__GNUC__)
	setbuf(stdout, NULL);
	#endif
}


/* Funktionen hanterar kommunikationen med Matlab (sändning och mottagning av data). */
void matlab(void *p)
{
	for(;;)
	{
		scanf("%s", recivedData);
		recive();
		send();
		recivedData[0] = '0';
			
		vTaskDelay(100);
	}
}


/* Funktionen sänder data till Matlab baserat på mottaget önskemål. */
void send(void)
{
	// Sänder data baserat på mottaget önskemål.
	switch(recivedData[0])
	{
		case 's':					// Önskar handskakning.
			printf("%d \n", 1111);
			recivedData[0] = '0';
			break;
		case 'd':					// Önskar data från reglerig.
			printf("%d %d %d \n", (int)global_d, (int)global_e, (int)global_u);
			recivedData[0] = '0';
			break;
		default:
			break;
	}
}


/* Funktionen tar emot önskemål från Matlab. */
void recive(void)
{
	int i;
	int j = 1;
	int k = 0;
	int l = 0;
	char temp[10];
	float tempBv = 0.0;
	
	// Kontrollerar om mottagen data innehåller variabler för reglering.
	if(recivedData[0] == 'r')
	{
		// Samtlig mottagen data gås igenom i arrayn.
		for(i=1; i<40; i++)
		{
			// Om tecknet avser en "stopbit"
			if(recivedData[i] == ';')
			{
				// Avgör i vilken variabel som värdet ska sparas.
				switch(j)
				{
					case 1:						
						// Överför data till temporär variabel.
						tempBv = atof(temp);
						
						// Omvandling av avstånd till binärt tal (baserat på linjärisering).
						//global_Bv = ((-4.75) * tempBv) + 562.5;		// Beräkning i juni månad
						global_Bv = ((-4.6) * tempBv) + 533;			// Beräkning i augusti månad
						
						// Tömmer temporär array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 2:
						// Överför data till variabel.
						global_Kp = atof(temp);
						
						// Tömmer temporär array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 3:
						// Överför data till variabel.
						global_Ti = atof(temp);
						
						// Tömmer temporär array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 4:
						// Överför data till variabel.
						global_Td = atof(temp);
						
						// Tömmer temporär array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 5:
						// Överför data till variabel.
						global_dT = atof(temp);
						
						// Tömmer temporär array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					default:
						break;
				}
				
				// Markerar näste variabel.
				j++;
				k = 0;
			}
			else
			{
				if(k==10)
				{
					k = 0;
				}
				
				temp[k] = recivedData[i];
				k++;
			}
		}
	}
}