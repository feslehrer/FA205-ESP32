// Technische Richtlinie FA205
// Bibliothek:       lcd.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      10.8.2023
// letzte Änderung:  10.8.2023
// Autor:            Rahm

#ifndef _LCD_H_
#define _LCD_H_

#include "controller.h"
#include <arduino.h>

//**** Schalter ********************************************************
#define LCD_LEN 16              // Display-Zeilenlänge 20 oder 16 (default)
//#define LCD_WAIT 10             // Bei Timing-Problemen LCD_WAIT vergrößern!!
//**********************************************************************
//Funktionsdefinitionen für Technische Richtlinie FA205
void lcd_init      ( void );                            // Initialisierung des Displays
void lcd_clear     ( void );
void lcd_setcursor ( uint8_t row, uint8_t column );     // Setzen der Cursorposition
// row (Zeile) = 1,2, ...
// column (Spalte) = 1,2,3, ...
void lcd_print     (char *text );                  // Textausgabe ab Cursorposition, bis '\0'
void lcd_char      ( uint8_t  value  );                 // Ausgabe eines Zeichens an der aktuellen Cursorposition
void lcd_byte      ( uint8_t  value  );                 // 0 ... 255
void lcd_int       ( uint16_t value  );                 // 0 ... 65535

//erweiterte Funktionen nicht Teil der Technischen Richtlinie FA205
void lcd_defchar   ( byte *pixtab, uint8_t char_nr );
// Definition von 7 eigenen Zeichen. char_nr: 1 bis 7
// char_nr = 0 funktioniert nur, wenn das Zeichen nicht in Zeichenketten ('\0' = 0) verwendet wird.
void lcd_clearline ( uint8_t lineNr );
// Ausgabe von 00..99  z.B. für Datum, Uhrzeit
void lcd_dd        (uint8_t val);
// Zeigt 0000 bis FFFF auf dem Display an!
void lcd_hhhh      (uint16_t val);
// Gibt ein Byte als Bitfolge am LC-Display aus. Z.B.: 01101101
void lcd_debug(uint8_t byte);
#endif
