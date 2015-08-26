/*
 * Communication.c
 *
 * Created: 2015-04-01 11:02:53
 *  Author: Andreas Bj�rntoft
 */ 

#include <asf.h>
#include "GlobalVar.h"
#include "Communication.h"

// Variabel f�r lagring av avl�st data.
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


/* Funktionen hanterar kommunikationen med Matlab (s�ndning och mottagning av data). */
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


/* Funktionen s�nder data till Matlab baserat p� mottaget �nskem�l. */
void send(void)
{
	// S�nder data baserat p� mottaget �nskem�l.
	switch(recivedData[0])
	{
		case 's':					// �nskar handskakning.
			printf("%d \n", 1111);
			recivedData[0] = '0';
			break;
		case 'd':					// �nskar data fr�n reglerig.
			printf("%d %d %d \n", (int)global_d, (int)global_e, (int)global_u);
			recivedData[0] = '0';
			break;
		default:
			break;
	}
}


/* Funktionen tar emot �nskem�l fr�n Matlab. */
void recive(void)
{
	int i;
	int j = 1;
	int k = 0;
	int l = 0;
	char temp[10];
	float tempBv = 0.0;
	
	// Kontrollerar om mottagen data inneh�ller variabler f�r reglering.
	if(recivedData[0] == 'r')
	{
		// Samtlig mottagen data g�s igenom i arrayn.
		for(i=1; i<40; i++)
		{
			// Om tecknet avser en "stopbit"
			if(recivedData[i] == ';')
			{
				// Avg�r i vilken variabel som v�rdet ska sparas.
				switch(j)
				{
					case 1:						
						// �verf�r data till tempor�r variabel.
						tempBv = atof(temp);
						
						// Omvandling av avst�nd till bin�rt tal (baserat p� linj�risering).
						//global_Bv = ((-4.75) * tempBv) + 562.5;		// Ber�kning i juni m�nad
						global_Bv = ((-4.6) * tempBv) + 533;			// Ber�kning i augusti m�nad
						
						// T�mmer tempor�r array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 2:
						// �verf�r data till variabel.
						global_Kp = atof(temp);
						
						// T�mmer tempor�r array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 3:
						// �verf�r data till variabel.
						global_Ti = atof(temp);
						
						// T�mmer tempor�r array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 4:
						// �verf�r data till variabel.
						global_Td = atof(temp);
						
						// T�mmer tempor�r array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					case 5:
						// �verf�r data till variabel.
						global_dT = atof(temp);
						
						// T�mmer tempor�r array.
						for(l=0; l<10; l++)
						{
							temp[l] = '\0';
						}
						
						break;
					default:
						break;
				}
				
				// Markerar n�ste variabel.
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