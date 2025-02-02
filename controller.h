// Technische Richtlinie FA205
// Bibliothek:       controller.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  27.9.2023
// Autor:            Rahm

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// Controllerspezifische Registerdefinitionen
#include <arduino.h>
#include "math.h"  //Für Berechnung der Baudrate

//*********************************
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
//#define TIMER_DEBUG
//**********************************

#ifdef F_CPU
 #undef  F_CPU
#endif
#define F_CPU 80000000  //MHz

// Technische Richtlinie FA205 Bibliotheken
#include "communication.h"
#include "in_out.h"
#include "interrupt.h"
#include "lcd.h"
#include "delay.h"

#endif
