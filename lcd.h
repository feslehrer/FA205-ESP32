// Technische Richtlinie FA205
// Bibliothek:       lcd.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      30.9.2015
// letzte Änderung:  22.8.2023
// Autor:            Rahm

#ifndef _LCD_H_
#define _LCD_H_

#include "controller.h"

//**** Schalter ********************************************************
#define LCD_LEN 16              // Display-Zeilenlänge 20 oder 16 (default)
#define LCD_I2C                 // Aktiviert das I2C-Interface
#define LCD_WAIT 10              // Bei Timing-Problemen LCD_WAIT vergrößern!!
//**********************************************************************

//Anzeige Einstellungen
#define _CURSOR_      0x0c        // 0b00001100  ->  Display on, Cursor off, Blink off
#define _DISPLAY_     0x14        // 0b00010100  ->  Cursor shift right
										
#define ZEILE1      0x00        // Startadresse für erste Displayzeile
#define ZEILE2      0x40

#if (LCD_LEN == 16)
 #define ZEILE3     0x10        // Für 16 Zeichen LCDs Zeile3 = 0x10
 #define ZEILE4     0x50        // "       "       "   Zeile4 = 0x50
#elif (LCD_LEN == 20) 
 #define ZEILE3     0x14        // Für 20 Zeichen LCDs Zeile3 = 0x14
 #define ZEILE4     0x54        // "       "       "   Zeile4 = 0x54
#endif

// Für I2C-Display mit PCF8574T (D7..D4 => P7..P4)!!!
#define RS       0
#define RW       1
#define EN       2
#define BL       3   //Backlight
//***** I2C-Adressen und Kontrollbyte für den PCF8574 ****************
#define LCD_ADDR_R  0x4f   // 01001111 ->  0100 fest, 111 durch Jumper, 1 Wert lesen
#define LCD_ADDR_W  0x4e   // 01001110 ->  0100 fest, 111 durch Jumper, 0 Wert schreiben

//Funktionsdefinitionen für Technische Richtlinie FA205
extern void lcd_init      ( void );                            // Initialisierung des Displays
extern void lcd_clear     ( void );
extern void lcd_setcursor ( uint8_t row, uint8_t column );     // Setzen der Cursorposition
// row (Zeile) = 1,2, ...
// column (Spalte) = 1,2,3, ...
void lcd_print     ( const char  *text );                 // Textausgabe ab Cursorposition, bis '\0'
extern void lcd_char      ( uint8_t  value  );                 // Ausgabe eines Zeichens an der aktuellen Cursorposition
extern void lcd_byte      ( uint8_t  value  );                 // 0 ... 255
extern void lcd_int       ( uint16_t value  );                 // 0 ... 65535

//erweiterte Funktionen nicht Teil der Technischen Richtlinie FA205
extern void lcd_defchar   ( uint8_t *pixtab, uint8_t char_nr );
// Definition von 7 eigenen Zeichen. char_nr: 1 bis 7
// char_nr = 0 funktioniert nur, wenn das Zeichen nicht in Zeichenketten ('\0' = 0) verwendet wird.
extern void lcd_clearline ( uint8_t lineNr );
// Ausgabe von 00..99  z.B. für Datum, Uhrzeit
extern void lcd_dd        (uint8_t val);
// Zeigt 0000 bis FFFF auf dem Display an!
extern void lcd_hhhh      (uint16_t val);
// Gibt ein Byte als Bitfolge am LC-Display aus. Z.B.: 01101101
void lcd_debug(uint8_t byte);
#endif
