/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für Arduino.
    Vorschlag für externer Interrupt
    TR 3.3 -Externer Interrupt-
 Datei:            ExtInterrupt_FA205.ino
 Autor:            Baier/Rahm
 Datum:            04.10.2015
 Letzte Änderung:  05.09.2023
 
 Hinweis: Standardmäßig liegt der externe Interrupt an GPIO_14 */

#include "controller.h"

#define S1        PORTy,2   // PORTy.2

#define ledGruen  PORTx,3   // PORTx.3
#define ledGelb   PORTx,4   //      .4
#define ledRot    PORTx,5   //      .5

#define EIN       0         // lowaktive LEDs
#define AUS       1

volatile int zustand = 1;     // Globale Variablen unbedingt volatile deklarieren

void setup (void)
{
  //Initialisierung der Eingabe (1 Schalter)
  bit_init(S1,IN);            // GPIO 14 als Eingang. Pullup wird aktiviert
  //Initialisierung der Ausgabe (3 LEDs)
  bit_init(ledGruen,OUT);     // GPIO 4  als Ausgang
  bit_init(ledGelb,OUT);      // GPIO 33 als Ausgang
  bit_init(ledRot,OUT);       // GPIO 15 als Ausgang
  //Alle LEDs ausschalten
  bit_write(ledGruen, AUS);   //Grün aus (lowaktiv)
  bit_write(ledGelb, AUS);    //Gelb aus
  bit_write(ledRot, AUS);     //Rot  aus
  
  //Initialisierung des externen Interrupts mit Übergabe der ISR
  ext_interrupt_init( ext_interrupt_isr );    // ext_interrupt_isr ohne () !!!
                                              // Der Funktionsname ist ein Pointer auf die Funktion 
  ext_interrupt_enable();
  rs232_init();
}

void loop (void)
{
  bit_write(ledGruen, zustand); //Grün ein
  bit_write(ledGelb, zustand); //Gelb ein
  bit_write(ledRot, zustand); //Rot ein
}

// Interrupt-Serviceroutine
// Die Funktion ext_interrupt_isr() wird bei steigender Flanke an PORTx.2 (GPIO 14) aufgerufen!
// Funktionsname sollte nicht geändert werden. Ansonsten muss er deklariert werden.
void ext_interrupt_isr(void)
{        
  zustand = !zustand;
  if(zustand==0)
    rs232_print("LEDs ein\r\n");
  else
    rs232_print("LEDs aus\r\n");
}
