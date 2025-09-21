/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für ESP32.
    Vorschlag für Timer-Interrupt
    Datei:            Timer_FA205.ino
    Autor:            Baier/Rahm
    Datum:            04.10.2015
    Letzte Änderung:  05.09.2023
    TR 3.3 -Timer(Quelle ist Systemtakt)
    Hinweis: Es wird nur ein Timer unterstützt 
*/

#include "controller.h"

#define ledGruen  PORTx,3  //GPIO 4
#define ledGelb   PORTx,4  //GPIO 33
#define ledRot    PORTx,5  //GPIO 15

#define EIN       0 //lowaktive LEDs
#define AUS       1

volatile uint8_t  zustand = AUS;
volatile uint16_t millisec = 0;   //alle 1000ms=1s LEDs ein/aus

void setup (void)
{
  //Initialisierung der Ausgabe (3 LEDs)
  bit_init(ledGruen,OUT);     // Arduino: Pin D11 als Ausgang
  bit_init(ledGelb,OUT);      // Arduino: Pin D12 als Ausgang
  bit_init(ledRot,OUT);       // Arduino: Pin D13 als Ausgang
  //Alle LEDs ausschalten
  bit_write(ledGruen, AUS);   //Grün aus (lowaktiv)
  bit_write(ledGelb, AUS);    //Gelb aus
  bit_write(ledRot, AUS);     //Rot  aus
  
  //Initialisierung des Timers mit Übergabe der ISR
  timer1ms_init(timer_1ms_isr);
  timer1ms_enable();
}

void loop (void)
{
  //warte auf Timer-Interrupt
  delay(4000);             //2x ein und 2x aus    
  timer1ms_disable();      //Timer aus für 5 Sek.
  delay(5000);
  timer1ms_enable();       //Timer wieder ein
  while(1);                //hier stoppt die loop(); LEDs blinken
}

// Timer-Serviceroutine
// Die Funktion timer_1ms_isr() wird alle 1ms aufgerufen!
// Funktions-Name sollte nicht geändert werden. Sonst muss er deklariert werden.
void timer_1ms_isr(void)
{
    millisec++;
    if(millisec > 999)   //Nach 1000ms = 1s
    {
      millisec = 0;
      zustand = !zustand;
      bit_write(ledGruen, zustand); //Grün ein/aus
      bit_write(ledGelb, zustand);  //Gelb ein/aus
      bit_write(ledRot, zustand);   //Rot ein/aus
    }
}

