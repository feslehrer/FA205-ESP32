// Technische Richtlinie FA205
// Bibliothek:       in_out.c
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  11.8.2023
// Autor:            Rahm

#include "in_out.h"
#include <arduino.h>

const int portx[] = {9,10,14,4,33,15,13,32};
const int porty[] = {2,27,5,23,19,18,36,39};

// Definition der Funktionen
//***************************************************************
// ab hier Funktionen für Port-Ein-/Ausgabe
//***************************************************************
void bit_init(uint8_t byte_adr, uint8_t bit_nr, uint8_t direction)
{

  uint8_t dir;
  switch (direction)
  {
    case IN: dir = INPUT_PULLUP; break;
    case OUT: dir = OUTPUT; break;
    default: dir = INPUT; break;
  }
  if (byte_adr == PORTx)
    pinMode(portx[bit_nr],dir);
  else if (byte_adr == PORTy)
    pinMode(porty[bit_nr],dir);
}

void byte_init(uint8_t byte_adr, uint8_t direction)
{
  uint8_t dir;
  switch (direction)
  {
    case IN: dir = INPUT_PULLUP; break;
    case OUT: dir = OUTPUT; break;
    default: dir = INPUT; break;
  }

  for (uint8_t i=0;i<8;i++)
  {
    if (byte_adr == PORTx)
      pinMode(portx[i],dir);
    else if (byte_adr == PORTy)
      pinMode(porty[i],dir);
  }
}

uint8_t bit_read(uint8_t byte_adr, uint8_t bit_nr)
{
  if (byte_adr == PORTx)
    return digitalRead(portx[bit_nr]);
  else if (byte_adr == PORTy)
    return digitalRead(porty[bit_nr]);
  else return 0;
}

void bit_write(uint8_t byte_adr, uint8_t bit_nr, uint8_t val)
{
  if (byte_adr == PORTx)
    digitalWrite(portx[bit_nr],val&0x01);
  else if (byte_adr == PORTy)
    digitalWrite(porty[bit_nr],val&0x01);
}

uint8_t byte_read(uint8_t byte_adr)
{
  uint8_t temp=0;

  for (int8_t i=7;i>=0;i--)
  {
    temp <<= 1;
    if (byte_adr == PORTx)
      temp |= digitalRead(portx[i]);
    else if (byte_adr == PORTy)
      temp |= digitalRead(porty[i]);
  }
  return temp;
}

//PORT als Ausgabe
void byte_write(uint8_t byte_adr, uint8_t byte_wert)
{
    for (uint8_t i=0;i<8;i++)
    {
      if (byte_adr == PORTx)
        digitalWrite(portx[i],byte_wert&0x01);
      else if (byte_adr == PORTy)
        digitalWrite(porty[i],byte_wert&0x01);
      byte_wert >>= 1;
    }
}

// void bit_toggle(volatile uint8_t *byte_adr, uint8_t bit_nr, volatile uint8_t *_bit_status_)
// {
//     if( !bit_read(byte_adr,bit_nr) )
//     {
//       delay_ms(20);                     // Entprellzeit
//       while( !bit_read(byte_adr,bit_nr));
//       delay_ms(20);
//       *_bit_status_ = ~*_bit_status_;
//       *_bit_status_ &= 0x01;                   // Bit 0 ausmaskieren!
//     }
// }

//***************************************************************
// ab hier PWM-Funktionen
//***************************************************************
const int pwm_channel[] = {2,27,5,23,19,18,9,10,14,4,33,15,13,32};

void pwmx_init( uint8_t bit_nr )
{
  for(uint8_t i=0;i<14;i++)
  {
    if (pwm_channel[i]==bit_nr)
    {
      ledcSetup(i,1000,8);
      ledcWrite(i,127);
      return;
    }
  }
}

void pwmx_start( uint8_t bit_nr )
{
  for(uint8_t i=0;i<14;i++)
  {
    if (pwm_channel[i]==bit_nr)
    {
      ledcAttachPin(bit_nr,i);
      return;
    }
  }
}

void pwmx_stop( uint8_t bit_nr )
{
  for(uint8_t i=0;i<14;i++)
  {
    if (pwm_channel[i]==bit_nr)
    {
      ledcDetachPin(bit_nr);
      return;
    }
  }
}
void pwmx_duty_cycle ( uint8_t bit_nr, uint8_t value )
{
  for(uint8_t i=0;i<14;i++)
  {
    if (pwm_channel[i]==bit_nr)
    {
      ledcWrite(i,value);
      return;
    }
  }
}

void pwm_init(void)
{
  pwmx_init(PWM_PIN);
}

void pwm_start()
{
  pwmx_start(PWM_PIN);
}

void pwm_stop()
{
  pwmx_stop(PWM_PIN);
}

void pwm_duty_cycle ( uint8_t value )
{
  pwmx_duty_cycle(PWM_PIN,value);
}

//***************************************************************
// ab hier ADC-Funktionen
//***************************************************************

//uint16_t adc_in(uint8_t ch);       // Prototyp! Lokale Funktion mit Kanalwahl

// Initialisierung des ADU.
void adc_init(void)
{

}

// Einlesen des ADC-Kanals 1
uint8_t adc_in1( void )
{
  return (adc_in(CH3));
}

// Einlesen des ADC-Kanals 2
uint8_t adc_in2( void )
{
  return (adc_in(CH0));
}

// Funktion mit Kanal-Parameter
uint8_t adc_in(uint8_t ch)
{
  return ((uint8_t) (analogRead(ch)>>4));
}

// Gibt den 12 Bit-Wert des ADC zurück. Funktion mit Kanal-Parameter
uint16_t adc_in12(uint8_t ch)
{
  return analogRead(ch);
}