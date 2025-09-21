/*
 * dht11.c
 *
 * Created: 26.07.2018 19:47:49
 *  Author: rahm
 */ 

#include "controller.h"
#include "dht11.h"

uint8_t dht11_read(uint8_t *data)
{
  volatile uint8_t temp;
  volatile uint8_t byte, bit;
  volatile uint8_t counter;

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // DHT11  Lesen initialisieren
  bit_init(DHT11_PIN,OUT);   // DHT11pin als Ausgang
  bit_write(DHT11_PIN,0);    // Low ausgeben
  delay_ms( 18 );                       // 18ms warten
  
  bit_init(DHT11_PIN,IN);    // DHT11 pin als Eingang (mit Pullup)
  while (bit_read(DHT11_PIN)==1);  // Dauer 20-40us
  while (bit_read(DHT11_PIN)==0);  // Dauer ~80us
  while (bit_read(DHT11_PIN)==1);  // Dauer ~80us
  
  // Starte Transmission
  // 5 Byte Daten lesen
  for (byte = 0; byte < 5; byte++)
  {
    temp = 0;
    for (bit = 0; bit < 8; bit++)
    {
      while (bit_read(DHT11_PIN)==0);      // Dauer ~50us
      
      counter = 0;
      while ( bit_read(DHT11_PIN) == 1)    // Dauer: 0 ca. 26-28us
      {                                    //        1 ca. 70us
        delayMicroseconds( 10 ); counter++;
        
        if ( counter == 255 ) return 1;    // Error
      }
      temp <<= 1;
      
      if (counter > 3)   temp |= 0x01;     // >30us
    }
    data[byte] = temp;
  }
  return 0;    // OK
}