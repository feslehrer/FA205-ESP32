// Technische Richtlinie FA205
// Bibliothek:       in_out.h
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  10.8.2023
// Autor:            Rahm

#ifndef _IN_OUT_H_
#define _IN_OUT_H_

#include "controller.h"
#include <Arduino.h>

//*******************************************************************************
// Globale Schalter und Symbole
// ... für Bit- und Byte-I/O
//#define NOPULLUPS
#define IN  INPUT
#define OUT OUTPUT
// Portregisterdefinitionen. Können an dieser Stelle angepasst werden. 
#define PORTx 1
#define PORTy 2

// ... für PWM
// Der PWM-Ausgang ist standardmäßig Pin 2  (LC-Backlight PWM)
#define PWM_PIN 2

// ... für ADC
#define CH0 38   // Kanal 1
#define CH1 37   // Kanal 2
#define CH2 35   // Kanal 3
#define CH3 34   // Kanal 4: Poti, LDR auf ESP32-Carrier-Board
//*******************************************************************************

// Funktionsprototypen für Bit- und Byte Ein-/Ausgabe

void    bit_init   ( uint8_t byte_adr, uint8_t bitnr, uint8_t direction );
uint8_t bit_read   ( uint8_t byte_adr, uint8_t bitnr );
void    bit_write  ( uint8_t byte_adr, uint8_t bitnr, uint8_t value );

void    byte_init  ( uint8_t port, uint8_t direction );
uint8_t byte_read  ( uint8_t port );
void    byte_write ( uint8_t port, uint8_t value );

//void    bit_toggle ( volatile uint8_t port, uint8_t bitnr, volatile uint8_t *status );

// Funktionsprototypen für PWM
void    pwm_init   ( void );
void    pwm_start  ( void );
void    pwm_stop   ( void );
void    pwm_duty_cycle ( uint8_t value);

void    pwmx_init  ( uint8_t bitnr );
void    pwmx_start ( uint8_t bitnr );
void    pwmx_stop  ( uint8_t bitnr );
void    pwmx_duty_cycle ( uint8_t bitnr, uint8_t value);

// Funktionsprototypen für AD-Wandler
void    adc_init   ( void );    // 8-Bit
uint8_t adc_in1    ( void );
uint8_t adc_in2    ( void );

uint8_t  adc_in    ( uint8_t channel );  // ADC  (8 Bit) mit freier Kanalwahl (CH1...CH6)
uint16_t adc_in12  ( uint8_t channel );  // dito. aber in 12Bit-Auflösung
#endif
