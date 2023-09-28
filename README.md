# FA205-ESP32
**Technische Richtlinie FA205**-Bibliotheken für **ESP32-Carrier-Board** und **Arduino-IDE**
Die Technische Richtlinie FA205 wird in Baden-Württemberg im Unterricht im Beruf **Elektroniker/in für Geräte und Systeme** eingesetzt, um einheitliche und systemunabhängige Abschlußprüfungen zu ermöglichen. 
Die Richtlinie enthält:
+ Übersicht über die Programmiersprache C
+ Blockschaltbild des (Modell-)Mikrocontrollers
+ C-Funktionsbibliotheken (delay, in_out, interrupt, lcd, communication)

## Installation
+ Auf GitHub Resource: https://github.com/feslehrer/FA205-ESP32.git
<br>Download der Bibliothek als Zip-Datei (<>Code --> Download Zip)
+ In der Arduino-IDE auf **Sketch --> Bibliothek einbinden --> .Zip-Bibliothek hinzufügen...**
+ Im Sketch muss **controller.h** inkludiert werden.
<br>Beispiel: Blink-Sketch mit FA205-Funktionen
```c
#include "controller.h"     // FA205-Bibliotheken einbinden

void setup(void)
{
  bit_init(PORTx,7,OUT);    // PORTx,7 als Ausgang initialisieren
}

void loop(void)
{
  bit_write(PORTx,7,0);      // PORTx,7 --> 0
  delay_ms(500);             // 500ms warten
  bit_write(PORTx,7,1);      // PORTx,7 --> 1
  delay_ms(500);             // 500ms warten
}
```
**Anmerkung:** Im Boardmanager muss das ESP32 Boardpackage installiert sein. Als Board wird **ESP32 PICO-D4** ausgewählt.
## Implementierung für Arduino-IDE
Die Richtlinien-Funktionen mussten für die Verwendung in der Arduino-IDE leicht modifiziert werden. Die Änderungen sind im einzelnen:
+ Da der ESP32 keine 8 Bit-Ports mehr besitzt, wurden 8 Pins zum **PORTx** zusammengefasst, damit die 8 LEDs auf dem ESP32-Carrier-Board mit einem einzigen Byte-Zugriff geschrieben werden können. 8 weitere Pins wurden zum **PORTy** zusammengefasst. Hier befinden sich u.a. die 4 Taster des ESP32-Carrier-Boards.
+ Abweichend von der Definition in der Technischen Richtlinie müssen für die Funktionen **lcd_print()** und **rs232_print()** konstante Zeichenketten vom Typ **char** und nicht **uint8_t** übergeben werden.
```c
char text[] = "Hallo Welt";     // Definition als char und nicht uint8_t

lcd_print ( text );
rs232_print ( text );
```
alternativ:
```c
lcd_print ( "Hallo Welt" );
```
+ Die i2c-Routinen für die Kommunikation auf dem I2C-Bus sind in der Technischen Richtlinie so aufgebaut, dass der Elektroniker das Übertragungsprotokoll aus dem Datenblatt eines IC direkt nachbilden kann. Da die Arduino-Wire-Bibliothek eine solche Vorgehensweise nicht zulässt, wurden die i2c-Funktionen mit der **Espressif-IDF** realisiert. Die Wire-Bibliothek darf daher nicht verwendet werden. Ebenso wird für das I2C-LCD keine weitere Bibliothek benötigt.
Zu beachten ist, dass ein sogenannter **Repeated-Start** ohne **i2c_stop()** nur mit der - nicht richtlinienkonformen - Funktion **i2c_rstart()** funktioniert. Da die Espressif-IDF mit einer Befehlsqueue arbeitet, wird ein mit i2c_start() eingeleiteter Übertragungsrahmen erst mit einem i2c_stop() oder bei i2c_rstart() physikalisch auf dem Bus erscheinen. Problematisch wird dies beim Lesen vom Slave. Hier übergibt die Funktion **i2c_read()** nicht die gelesenen Daten (weil der Befehl erst in die Befehlsqueue geschrieben wird), sondern nur einen Index auf das Array **i2c_data[]**. Das Beispiel unten soll die Vorgehensweise verdeutlichen:
```c
  i2c_start();                // Startbedingung
  i2c_write(ADDR_W);          // Schreibwunsch an LM75 senden
  i2c_write(0x00);            // Pointerbyte auf 0 setzen
  //i2c_stop();

  i2c_rstart();                  // repeated Start oder i2c_start(), wenn vorher i2c_stop()
  i2c_write(ADDR_R);             // Lesewunsch an LM75 senden
  uint8_t msb = i2c_read(ACK);        // MSB (index) lesen und weiteren Wert anfordern.
  uint8_t lsb = i2c_read(NACK);       // LSB (index) lesen und kein weiters Byte anfordern.
  i2c_stop();                    // Stoppbedingung
  
  // Zugriff auf gelesene Daten über i2c_data[index]
  msb = i2c_data[msb]; 
  lsb = i2c_data[lsb];
```
Das dekodierte Signal auf dem I2C-Bus stimmt mit dem Übertragungsprotokoll im Datenblatt überein:
<img src="https://github.com/feslehrer/FA205-ESP32/assets/24614659/03f29c73-361f-4601-beec-0a0540069e8a" alt="Decodiertes I²C-Signal LM75" width="800">

## ESP32-Carrier-Board von AS-Elektronik
<img src="https://user-images.githubusercontent.com/24614659/235747329-3b294437-124a-4d40-9fe2-bfb1395ae811.jpg" alt="ESP32-Carrier-Board" width="600">
https://www.ase-schlierbach.de

### FA205-Portbelegungen für ESP32-Carrier-Board
Das ESP32-Carrier-Board ist mit den GPIO-Nummern bedruckt.
Für die Technische Richtlinie müssen daher die GPIO-Nummern entsprechend dem abgebildeten Schema ersetzt werden:
<img src="https://github.com/feslehrer/FA205-ESP32/assets/24614659/eaa6cf4a-80f9-4a24-b3ef-3d3ee630ba00" alt="FA205 Pinbelegung" width="800">

### FA205-Prototypen:
+ **delay.h**
```c
void delay_100us ( uint16_t mikrosec);
void delay_ms    ( uint16_t millisec);
```
+ **in_out.h**
```c
void    bit_init   ( uint8_t port, uint8_t bitnr, uint8_t direction );
uint8_t bit_read   ( uint8_t port, uint8_t bitnr );
void    bit_write  ( uint8_t port, uint8_t bitnr, uint8_t value );

void    byte_init  ( uint8_t port, uint8_t direction );
uint8_t byte_read  ( uint8_t port );
void    byte_write ( uint8_t port, uint8_t value );

void    pwm_init   ( void );        // Standard-PWM Pin ist GPIO 2
void    pwm_start  ( void );
void    pwm_stop   ( void );
void    pwm_duty_cycle ( uint8_t value);

void    adc_init   ( void );    // 8-Bit
uint8_t adc_in1    ( void );
uint8_t adc_in2    ( void );
```
+ **interrupt.h**
```c
//externer Interrupt
void ext_interrupt_init    ( void (*ip) (void));        // ip = ext_interrupt_isr (Standard)
                                                        // _INTERRUPT_PIN0_ GPIO 14 (Standard)
void ext_interrupt_enable  ( void );
void ext_interrupt_disable ( void );
void ext_interrupt_isr     ( void );                    // Muss im Sketch definiert werden.

void timer1ms_init         ( void (*ti) (void) );       // ti = timer1ms_isr (Standard)
                                                        // _FALLING_EDGE_TRIGGER_ (Standard)
void timer1ms_enable       ( void );             
void timer1ms_disable      ( void );
void timer1ms_isr          ( void );                    // Muss im Sketch definiert werden. 
```
+ **lcd.h**
```c
void lcd_init      ( void );                            // Initialisierung des Displays
void lcd_clear     ( void );                            // Displayanzeige löschen
void lcd_setcursor ( uint8_t row, uint8_t column );     // Setzen der Cursorposition
                                                        // row (Zeile) = 1,2, ...
                                                        // column (Spalte) = 1,2,3, ...
void lcd_print     ( const char  text[] );              // Textausgabe ab Cursorposition, bis '\0'
                                                        // text[] = /0-terminierte Zeichenkette
void lcd_char      ( uint8_t  value  );                 // Ausgabe eines Zeichens an der aktuellen Cursorposition
                                                        // value = ASCII-Code (8(7)-Bit)
void lcd_byte      ( uint8_t  value  );                 // Rechtsbündige Ausgabe einer Dezimalzahl ohne führende 0
                                                        // value = Byte (8 Bit) 0 ... 255
void lcd_int       ( uint16_t value  );                 // dito.
                                                        // value = Word (16 bit) 0 ... 65535
```
+ **communication.h**
```c
// I2C-Schnittstelle
void    i2c_init    ( void );                   // Initialisierung
void    i2c_start   ( void );                   // Startbedingung
void    i2c_rstart  ( void );                   // Repeatet-Start. Vorherige Befehlsqueue wird ohne
                                                // Stoppbedingung gesendet und eine erneute Startbedingung erzeugt.
void    i2c_stop    ( void );                   // Befehlsqueue wird gesendet.
uint8_t i2c_write   ( uint8_t value);           // value = Byte (8-Bit). Rückgabe: ACK/NACK
uint8_t i2c_read    ( uint8_t ack );            // ack = ACK/NACK.       Rückgabe: Index auf i2c_data[]

// RS232-Schnittstelle
void    rs232_init  ( void );                   // In communication.h:
                                                // _BAUD_ 9600 (Standard) //19200 //115200
                                                // _SERIAL0_ (USB-Virtueller ComPort)
                                                // _SERIAL1_ (Rx1 (GPIO 9), Tx1 (GPIO 10))
                                                // _SERIALBT_ (SerialBluetooth) Gerätename: _DEVICENAME_
uint8_t rs232_get   ( void );                   // Rückgabe ASCII-Code (8(7) Bit)
void    rs232_put   ( uint8_t value );          // value = ASCII-Code (8(7)-Bit)
void    rs232_print ( const char text[] );      // text[] = /0-terminierte Zeichenkatte
```
**Anmerkung:** In den Bibliotheken sind einige ergänzende Funktionen enthalten. Informationen dazu finden sich im jeweiligen Header-Files.

## Anpassen der Bibliotheken an weitere ESP32-Hardware
Prinzipiell ist die FA205_ESP32-Bibliothek auf allen ESP32-Controllern Lauffähig. Die Anpassung an die am Controller angeschlossene Hardware muß über die Header(*.h)- und/oder *.cpp-Bibliotheksdateien erfolgen. Eine Anpassung an das Frematics/Esprit-Board ist bereits implementiert und wird im folgenden als Vorlage für eigene Implementierungen beschrieben. Dabei kommt das in vielen Schulen bereits verwendete Arduino-Carrier-Board (https://www.ase-schlierbach.de) zum Einsatz. Folgendes Pinout des Esprit-Boards wird dabei implementiert:
<img src="https://github.com/feslehrer/FA205-ESP32/assets/24614659/15df384f-cf3e-455e-b0a4-cdff2d5bae4d" alt="Pinout ESP32-Esprit-Board" width="600">
Die Bibliotheksdateien sind normalerweise im Ordner ***libraries/FA205_ESP32*** im Arduino-Sketch-Sketchordner zu finden.
+ Anpassungen in ***controller.h***
</br>Für das ESP32-Esprit-Board in Komination mit dem Arduino-Carrier-Board muss lediglich der Schalter **_ESP32_ESPRIT_BOARD_** entkommentiert werden und der Schalter **_ESP32_CARRIER_BOARD_** auskommentiert werden. Als weitere Einstellungen können hier (jeweils durch Auskommentierung) der Trigger für die beiden externen Interrupts gewählt werden. Ebenso wird eine Serielle Schnittstelle für die rs232-Richtlinienfunktionen gewählt. Bei der Wahl der seriellen Bluetooth-Verbindung **_SERIALBT_** muss ein eindeutiger Gerätenamen (**_DEVICENAME_**) verwendet werden, was insbesondere bei der Verwendung mit vielen Nutzern im Klassenzimmer/Labor wichtig ist. Ebenso kann hier auch die ***Baudrate*** für die Serielle Schnittstelle geändert werden. An dieser Stelle kann auch die Ausgabe von Debug-Meldungen für PWM-, ADC- und Externer Interrupt aktiviert werden.
```c
/*********************************
// Board-Typ
//---------------------------------
#define _ESP32_CARRIER_BOARD_
//#define _ESP32_ESPRIT_BOARD_

//*********************************
// externer Interrupt Trigger
//---------------------------------
#define _FALLING_EDGE_TRIGGER_
//#define _RISING_EDGE_TRIGGER_
//#define _ANY_EDGE_TRIGGER_

//**********************************
// RS232/Serial
//----------------------------------
#define BAUD  9600 //9600 //19200 //115200          //Baudraten können beliebig eingestellt werden.
#define _SERIAL0_  // Alternativ
//#define _SERIAL1_
//#define _SERIALBT_

#ifdef _SERIALBT_
 // Hier eindeutigen Bluetooth-Geätenamen definieren
 #define _DEVICENAME_ "myESP32"         // Wichtig: Name in Gänsefüßchen (Anführungszeichen) setzen
#endif

//**********************************
//Ausgabe von Debugmeldungen über RS232 
// Entsprechenden Debug-Schalter auskommentieren
// Hinweis: zum Debuggen muss jeweils die zugehörige init-Funktion
//          im Programm (Sketch) aufgerufen werden. 
//----------------------------------
//#define PWM_DEBUG
//#define ADC_DEBUG
//#define INT_DEBUG
//**********************************
```
+ Anpassungen in ***in_out.cpp***
Hier wird die Zuordnung der IO-Pins, der beiden ADC-Pins sowie des PWM-Pin zu den GPIO's des Controllers vorgenommen. Für das ESP32-Carrier-Board und das ESP32-Esprit-Board sind die Pins über die beschriebenen Schalter einstellbar. Für eigene Hardware bietet es sich an, die Pinzuordnungen im **default**-Abschnitt anzupassen. Zur Aktivierung der **default**-Einstellungen müssen dann beide Schalter (**_ESP32_CARRIER_BOARD_**, **_ESP32_ESPRIT_BOARD_**) auskommentiert werden. Die Einstellungen im Einzelnen sind:
**portx[]**,**porty[]**: Im Array sind die Pins in aufsteigender Reihenfolge geordnet. Z.B.: ***PORTx,0 = GPIO9***; ***PORTy,5 = GPIO18***; usw.
```c
#else   //default
  const int portx[] = {9,10,14,4,33,15,13,32};		
  const int porty[] = {2,27,5,23,19,18,36,39};
  const int pwm_channel[] = {2,27,5,23,19,18,9,10,14,4,33,15,13,32};
  #define PWM_PIN   2
  #define ADC1_PIN 34
  #define ADC2_PIN 38
#endif
```
