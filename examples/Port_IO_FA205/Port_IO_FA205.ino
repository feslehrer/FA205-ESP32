/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für ESP32 (Bit- und Byte-IO)
    Datei:            Port_IO_FA205.ino
    Autor:            Rahm,Baier
    Datum:            01.10.2015
    Letzte Änderung:  05.09.2023
    TR 3.2 -Bit Ein-Ausgabe-
    Hinweise:  Die IO-Funktionen sind so erstellt, dass Sie
    auf dem ESP32-Carrier-Board direkt lauffähig sind.
*/

#include "controller.h"
#define Schalter PORTy
#define LEDs	   PORTx
#define S1        2  // PORTy,2: GPIO 5
#define S2        3  // PORTy,3: GPIO 23
#define ledGruen  3  // PORTx,3: GPIO 4 
#define ledGelb   4  // PORTx,4: GPIO 33 
#define ledRot    5  // PORTx,5: GPIO 15 

void setup()
{
  //byte_init(LEDs,OUT);              // PORTx als Ausgang
  //byte_init(DipSchalter,IN);        // PORTy als Eingang
  bit_init(Schalter, S1, IN);       // PORTy,2 als Eingang (GPIO 5)
  bit_init(Schalter, S2, IN);       // PORTy,3 als Eingang (GPIO 23)
  bit_init(LEDs, ledGruen, OUT);    // PORTx,3 als Ausgang (GPIO 4)
  bit_init(LEDs, ledGelb, OUT);     // PORTx,4 als Ausgang (GPIO 33)
  bit_init(LEDs, ledRot, OUT);      // PORTx,5 als Ausgang (GPIO 15)
}

void loop()
{
  uint8_t temp;
  //temp = byte_read(Schalter);     // Byte-Operationen
  //byte_write(LEDs,temp);
  temp = bit_read(Schalter, S1);	    // Bit-Operationen
  bit_write(LEDs, ledGruen, temp);   
  bit_write(LEDs, ledGelb, temp);    
  bit_write(LEDs, ledRot,  temp);
}

