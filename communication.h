// Technische Richtlinie FA205
// Bibliothek:       communication.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      18.8.2023
// letzte Änderung:  4.9.2023
// Autor:            Rahm

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "controller.h"

//*************************************************************
//Globale Definitionen
// ... für I2C-Funktionen
#define _SOFT_I2C_
//#define _HARD_I2C_

#ifdef _HARD_I2C_
 #define _I2C_FREQUENZ_ 40000      // 40kHz (reicht für die meisten Anwendungen gut)
 #define _I2C_MAX_DATAREAD_ 5      // maximale Anzahl von Bytes die in einem Lesezyklus gelesen werden können.
 extern uint8_t i2c_data[];        // Vom Slave gelesene Daten werden in hier übergeben.
#endif

#define ACK  0
#define NACK 1

// ... für RS232-Funktionen
#define BAUD  9600 //9600 //19200 //115200          //Baudraten können beliebig eingestellt werden.
#define _SERIAL0_  // _SERIAL0_ // _SERIAL1_ // _SERIALBT_

#ifdef _SERIALBT_
 #define _DEVICENAME_ "myESP32"    // Hier eindeutigen Geätenamen definieren
#endif

//*************************************************************
// Funktionsprototypen für I2C-Busschnittstelle (Master-Mode)
extern void    i2c_init    ( void );
extern void    i2c_start   ( void );
extern void    i2c_rstart  ( void );
extern void    i2c_stop    ( void );
extern uint8_t i2c_write   ( uint8_t value);     // Rückgabewert ACK / NACK
extern uint8_t i2c_read    ( uint8_t ack );      // Rückgabewert Value

// Funktionsprototypen für RS232-Schnittstelle
extern void    rs232_init  ( void );
extern uint8_t rs232_get   ( void );
extern void    rs232_put   ( uint8_t value );
extern void    rs232_print ( const char *text );

extern void    rs232_baud    ( uint32_t baud );     // ändert die Baudrate
extern uint8_t rs232_inputdd ( void );              // Erzeugt ein Eingabeprompt für eine Zahl von 00 ... 99
extern void    rs232_printdd ( uint8_t value );     // Ausgabe einer Dezimalzahl 00..99 auf RS232
extern void    rs232_byte    ( uint8_t value );     // Ausgabe einer Dezimalzahl 0...255 auf RS232
extern void    rs232_int     ( uint16_t value );    // Ausgabe einer Dezimalzahl 0...65535 auf RS232
extern uint8_t rs232_binary_get ( void );
//extern uint8_t rs232_binary_get ( uint8_t *buf );   // Gibt ein empfangenes Byte im Binärmodus zurück.
                                                    // buf = empfangenes Zeichen
                                                    // Rückgabe: 1 = Byte empfangen, 0 = keine Byte empfangen
extern uint8_t rs232_readbytes(uint8_t * buffer, int8_t length);
extern uint8_t rs232_is_received( void );           // Ist ein Byte im Empfangspuffer? Ja = 1; Nein = 0

#endif
