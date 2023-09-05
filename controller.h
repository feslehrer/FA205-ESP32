// Technische Richtlinie FA205
// Bibliothek:       controller.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  4.9.2023
// Autor:            Rahm

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// Controllerspezifische Registerdefinitionen
#include <arduino.h>
#include "math.h"

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
