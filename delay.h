// Technische Richtlinie FA205
// Bibliothek:       delay.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  9.8.2023
// Autor:            Rahm

#ifndef _DELAY_H_
#define _DELAY_H_

#include "controller.h"

// Funktionsdefinitionen für Technische Richtlinie FA205
extern void delay_100us ( uint16_t mikrosec);
extern void delay_ms    ( uint16_t millisec);

// erweiterte Funktionen
//extern void delay_us( uint16_t mikrosekunden);
extern void delay_s( uint16_t sekunden);

#endif