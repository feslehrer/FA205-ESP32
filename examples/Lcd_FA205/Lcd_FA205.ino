/* Beschreibung: Testprogramm (Sketch) zur Umsetzung der techn.
   Richtlinie FA205 für Arduino.
   Vorschlag für LCD-Ansteuerung.
   TR 3.4 -LC-Display-
   Datei:            Lcd_FA205.ino
   Autor:            Baier/Rahm
   Datum:            04.10.2015
   Letzte Änderung:  27.09.2023

Hinweis: LC-Display über PCF8574 am I2C-Bus
*/

//FA 205 Bibliothek
#include "controller.h"

//Globale Variable
char meinText[]="Hallo Welt!";

void setup() 
{
  lcd_init();  
}

void loop() 
{
  lcd_setcursor(1,1);
  lcd_print(meinText);
  delay_ms(1000);
  lcd_setcursor(2,1);
  delay_ms(1000);
  lcd_print("Guten Tag...");
  delay_ms(2000);
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_char('A');
  delay_ms(1000);
  lcd_setcursor(1,6);
  lcd_byte(155);
  delay_ms(1000);
  lcd_setcursor(1,11);
  lcd_int(65432);
  delay_ms(1000);
  lcd_setcursor(2,1);
  lcd_print("Programmende.");

  while(1);                       // Stop
}