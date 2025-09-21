/*  
   Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
   Richtlinie FA205 für Arduino mit FA205-Bibliothek controller.h.
   Vorschlag für I2C-Bus (Master-write-to-Slave)
   TR 3.5 -I2C-Bussystem-
   Datei:            I2C_Master_Write_FA205.ino
   Autor:            Baier/Rahm
   Datum:            04.10.2015
   Letzte Änderung:  05.09.2023
*/

#include "controller.h"   //FA205-Bibliothek Bibliothek

#define ADDR_W   0x4C

void setup() 
{
   i2c_init();  //init. als Master  
}


void loop()
{
   i2c_start();         // Startbedingung
   i2c_write(ADDR_W);   // Schreibadresse des Slaves
   i2c_write(12);       // sende Byte 12 (dezimal)
   i2c_stop();          // Übertragung abschließen
                        // Erst hier werden die Signale aus der
                        // Befehlsqueue auf den Bus ausgegeben.
   
   i2c_start();         
   i2c_write(ADDR_W);
   i2c_write(0xFF);     // sende Byte 255 
   i2c_stop();       

   i2c_start();      
   i2c_write(ADDR_W);
   i2c_write('A');      // sende Ascii-Code 'A' = 65 
   i2c_stop();
   
   i2c_start();
   i2c_write(ADDR_W);
   for(int i=1; i<=20;i++) 
   {
      i2c_write(i);     // sende Dezimalzahlen 1..19 
   }
   i2c_stop();
   
   while(1);            // Endlosschleife
}

