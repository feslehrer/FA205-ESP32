/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für Arduino mit FA205-Bibliothek controller.h
    Vorschlag für I2C-Bus
    TR 3.5 -I2C-Bussystem-
    Datei:            I2C_Master_Read_ADC_FA205.ino
    Autor:            Baier/Rahm
    Datum:            04.10.2015
    Letzte Änderung:  05.09.2023
 
/*   Daten von I2C-Slave empfangen
 *   hier: ADC (12 Bit) an I2C-Adr. 0x4D (siehe communication.h)
*/

#include "controller.h"   //FA205-Bibliothek Bibliothek

#define ADDR_R   0x4D
#define ADDR_W   0x4C
#define ACK   0
#define NACK  1

//Variablen
uint8_t adc_HByte;
uint8_t adc_LByte;

void setup() 
{
  rs232_init();
  i2c_init();              //Initialisiert Arduino als Master 
}

int8_t send_buf[30];

void loop() 
{
  i2c_start();
  i2c_write(ADDR_R);
  adc_HByte = i2c_read(ACK);     // High-Byte (Index) lesen
  adc_LByte = i2c_read(NACK);    // Low-Byte (Index) lesen
  i2c_stop();                    // Ab hier erscheinen die Signale auf dem Bus 

  #ifdef _HARD_I2C_
   adc_HByte = i2c_data[adc_HByte];  // Daten aus Empfangs-Array holen 
   adc_LByte = i2c_data[adc_LByte];
  #endif

  sprintf(send_buf,"High: %X, Low: %X\r\n",adc_HByte,adc_LByte);
  rs232_print(send_buf);

  i2c_start();
  i2c_write(ADDR_R);
  adc_HByte = i2c_read(NACK);    // noch einmal nur High-Byte lesen
  i2c_stop();
  
  #ifdef _HARD_I2C_
   adc_HByte = i2c_data[adc_HByte];  // Daten aus Empfangs-Array holen 
  #endif
  
  sprintf(send_buf,"High: %X\r\n",adc_HByte);
  rs232_print(send_buf);
  
  delay_ms(2000);
}
