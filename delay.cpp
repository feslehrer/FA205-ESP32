// Technische Richtlinie FA205
// Bibliothek:       delay.c
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  9.8.2023
// Autor:            Rahm
//#include "delay.h"
#include <arduino.h>

//void delay_us( uint16_t mikrosekunden)
//{
  //  delay(mikrosekunden);
//}

void delay_100us( uint16_t mikrosekunden)
{
	uint16_t n = 0;

	while(n < mikrosekunden)
	{
	 	delayMicroseconds(100);
	 	n++;
	}
}

void delay_ms( uint16_t millisekunden)
{
	uint16_t n = 0;
	
	while(n < millisekunden)
	{
		delay_100us(10);
		n++;
	}
}

void delay_s( uint16_t sekunden)
{
  uint16_t n = 0;
  
  while(n < sekunden)
  {
    delay_ms(1000);
    n++;
  }
}
