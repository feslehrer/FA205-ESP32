// Technische Richtlinie FA205
// Bibliothek:       lcd.c 
// Controller:       für ESP32 
//                   Mit Support für I2C-Display mit PCF8574
// Version:          2.0
// erstellt am:      10.8.2023
// letzte Änderung:  10.8.2023
// Autor:            Rahm

// Headerdateien inkludieren
#include "lcd.h"
#include <arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Deklaration lokale Funktionen
uint8_t lcd_lookup        ( uint8_t ascii );

/******************************************************************************************
 * Initialisierung des LCD- Displays für den 4-Bit-Modus                                         
 ******************************************************************************************/
void lcd_init (void)	
{ 
  Wire.begin(21,22);
  lcd.init();
  lcd.clear();
  lcd.setCursor(0,0);
}

// Ab hier sind alle Funktionen Controllerunabhängig!!!!
/******************************************************************************************
 * Display löschen	                                                                                                      
 ******************************************************************************************/
void lcd_clear (void)     { lcd.clear(); }

/*****************************************************************************************
 * LCD-Cursor auf eine position setzen: Zeile 1..4, Spalte 1..16 (Displayabh�ngig)               
 ******************************************************************************************/
void lcd_setcursor (uint8_t zeile, uint8_t spalte)
{
  lcd.setCursor(spalte-1,zeile-1);
}								

/*****************************************************************************************
 * 	Ausgabe eines Zeichens an das LCD-Display                                                 
 *  Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden         
 ******************************************************************************************/
void lcd_char (uint8_t zeichen)
{
  zeichen = lcd_lookup(zeichen);           // Umlaute und Sonderzeichen ersetzen!
  lcd.write(zeichen);
}

/*****************************************************************************************
 * \0-terminierten Text an das LCD-Display ausgeben. 
 ******************************************************************************************/
void lcd_print (char *text)
{
  while (*text != '\0')             // Text													
  {
    if ((*text & 0xe0) == 0xe0)     // 3-Byte UTF8 ?
    {
      *text++;
      if ((*text & 0x80) == 0x80)  *text++;
    }          
    if ((*text & 0xc0) != 0xc0)     // 2-Byte UTF8 ?
      lcd_char (*text);             // nein, dann zeichenweise ausgeben 	
    *text++;                        // Pointer inkrementieren
  }
}

/*============================================================
Funktion:        lcd_byte(n)                Rahm, 17.2.15
Beschreibung:    Gibt das Byte n als 3 stelligen Dez-Wert aufs
                 Display. Führende Nullen werden zu blank.
Eingang:         Byte
Ausgang:         ---
==============================================================*/
void lcd_byte(uint8_t val)
{
  uint8_t buffer[3];
  uint8_t n = 0;	
	
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
				
  while (n<3)                             // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';					
  }

  while (n > 0)                           // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    lcd_char(buffer[n]);
  }
}

/*============================================================
Funktion:        lcd_int(n)                Rahm, 17.2.15
Beschreibung:    Gibt den Integer n als 5 stelligen Dez-Wert aufs
                 Display. Führende Nullen werden zu blank.
Eingang:         Byte
Ausgang:         ---
==============================================================*/
void lcd_int(uint16_t val)
{	
  uint8_t buffer[5];
  uint8_t n = 0;	
	
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
		
  while (n<5)                   // Rest von buffer mit Blank füllen
  {
    buffer[n++] = ' ';					
  }

  while (n > 0)                 // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    lcd_char(buffer[n]);
  }
}

/*============================================================
Funktion:     LookUpTable(char)                Rahm, 4.6.09
Beschreibung: Ersetzt ein ASCII-Umlaut durch den ent-
              sprechenden Display-Code.
Eingang:      ASCII-Code
Ausgang:      Display-Code
==============================================================*/
uint8_t lcd_lookup(uint8_t ascii)
{
  switch (ascii)
  {
    case 0xb0: return 0xdf;   // '°'
    case 0xa4: 
    case 0x84: return 0xe1;   //ä,Ä
    
    case 0xb6:  
    case 0x96: return 0xef;   //ö,Ö
    
    case 0xbc:  
    case 0x9c: return 0xf5;   //ü,Ü
    
    case 0x9f: return 0xe2;   // 'ß'
    case 0xb5: return 0xe4;   // 'µ'
    case '\\': return 0xa4;
    case 0xac: return 0xd3;   // '€'
  }
  return ascii;
}

/////////////////////////////////////////////////////////////////////////////////
// Ab hier sind ergänzende Funktionen definiert, die nicht Teil der technischen
// Richtlinie FA205 sind.
/////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * Pixelweise Definition eigener LCD-Zeichen, Ablegen von max 7 Zeichen im CG-RAM des LCD
 ***********************************************************************************/
void lcd_defchar (byte *pix_tab, uint8_t char_nr)
{
  lcd.createChar(char_nr, pix_tab);
}

/*************************************************************************
Löschen der Zeilen 1 bis 4 des Displays
Es wird ein Leerstring (16 Byte) auf die Zeile geschrieben
***************************************************************************/
void lcd_clearline(uint8_t line)
{
  uint8_t n;

  lcd_setcursor(line,0);
  for (n=0; n < LCD_LEN; n++)  lcd_char(' ');
}

// Zeigt 00 bis 99 auf dem Display an!
void lcd_dd(uint8_t val)
{
	uint8_t buffer[3];
	uint8_t n = 0;
	
	do
	{
		buffer[n++] = val%10 + '0';
		val /= 10;
	} while (n<3);

	n = 2;				// Nur 2 Stellen anzeigen
	while (n > 0)   
	{
		n--;
		lcd_char(buffer[n]);
	}
}

/*************************************************************************
Gibt ein Byte als Bitfolge am LC-Display aus
***************************************************************************/
void lcd_debug(uint8_t byte)
{
  for(int8_t i=7; i>=0;i-- )
  {
    if (((byte>>i)&0x01) == 0x01)  lcd_char('1');
    else lcd_char('0');
  }
}

// Zeigt 0000 bis FFFF auf dem Display an!
void lcd_hhhh(uint16_t val)
{
  uint8_t buffer[5];
  uint8_t temp;
  uint8_t n = 0;
  
  do
  {
    temp = val & 0x0f;
    if (temp < 0x0A)  temp += '0';
    else
    {
      temp -= 10;
      temp += 'A';
    }
    buffer[n++] = temp;
    val >>= 4;
  } while (n<4);

  n = 4;				// 4 Stellen anzeigen
  while (n > 0)
  {
    n--;
    lcd_char(buffer[n]);
  }
}