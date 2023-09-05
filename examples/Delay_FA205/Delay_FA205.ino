/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für Arduino Vorschlag für Delay mit Bit-IO.
    TR 3.1 -Delay-
    Datei:            Delay_FA205.ino
    Autor:            Baier/Rahm
    Datum:            01.10.2015
    Letzte Änderung:  05.09.2023
*/

#include "controller.h"

#define ledGruen  3  // LED's auf PORTx
#define ledGelb   4  
#define ledRot    5  

#define EIN       0  // lowaktive LEDs
#define AUS       1

void setup()
{
  bit_init(PORTx, ledGruen, OUT); // Arduino: PB3 als Ausgang
  bit_init(PORTx, ledGelb, OUT);  // Arduino: PB4 als Ausgang
  bit_init(PORTx, ledRot, OUT);   // Arduino: PB5 als Ausgang
  //Alle LEDs ausschalten
  bit_write(PORTx, ledGruen, AUS); //Grün aus (lowaktiv)
  bit_write(PORTx, ledGelb, AUS); //Gelb aus
  bit_write(PORTx, ledRot, AUS);  //Rot  aus
}

void loop()
{
  bit_write(PORTx, ledGruen, EIN);  //Grün ein
  delay_ms(5000);                   //5s Grün ein
  bit_write(PORTx, ledGruen, AUS);  //Grün aus
  delay_ms(1000);                   //1s Grün aus

  bit_write(PORTx, ledGelb, EIN);   //Gelb ein
  delay_100us(50000);               //5s Gelb ein
  bit_write(PORTx, ledGelb, AUS);   //Gelb aus
  delay_100us(10000);               //1s gelb aus

  bit_write(PORTx, ledRot, EIN);    //Rot ein
  delay_100us(50000);               //5s Rot ein
  bit_write(PORTx, ledRot, AUS);    //Rot aus
  delay_100us(10000);               //1s Rot aus
}

