/*  Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
    Richtlinie FA205 für ESP32 mit FA205-Bibliothek controller.h.
    Vorschlag für serielle Kommunikation.
    TR 3.5 -RS232-
    Datei:            Serial_FA205.ino
    Autor:            Baier/Rahm
    Datum:            04.10.2015
    Letzte Änderung:  27.09.2023
 
    Gibt einen Text über RS232 aus und wartet anschließend
    auf ein Zeichen von der seriellen Schnittstelle. Dieses wird als Echo
    zurückgegeben.

In controller.h können 3 serielle Kanäle eingestellt werden:
  _SERIAL0_  --> virtuelle Comport über USB (Rx0,Tx0).
  _SERIAL1_  --> Rx1 (GPIO 9), Tx1 (GPIO 10)
  _SERIALBT_ --> virtuelle ComPort über Bluetooth. Der Gerätename wird über
                 #define _DEVICENAME_ "myESP32"    

*/

//Präprozessor: fügt Datei in Quelltext ein
#include "controller.h"

char meinText[] = "\n\rController sind toll";   

void setup(void)
{
    rs232_init();
}

void loop(void)
{
  rs232_print(meinText);
  delay_ms(500);
  rs232_print("...aber klar!\n\r");
  rs232_print("gell!\n\r");
  
  uint8_t c;
  while((c=rs232_get()) == '\0');   // Warten auf Zeichen 
  rs232_put(c);                     // Echo zurückgeben
  rs232_print("\n\r");

  // Zeichen senden
  rs232_put('A');  //'A'
  rs232_put('\n'); //Zeilenvorschub (LF line feed)
  rs232_put('\r'); //Wagenrücklauf  (CR carriage return)
  rs232_put(90);   //90 = 'Z'
  
  // Endlos
  while(1);
}