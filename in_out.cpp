// Technische Richtlinie FA205
// Bibliothek:       in_out.c
// Controller:       ESP32
// Version:          1.0
// erstellt am:      9.8.2023
// letzte Änderung:  27.9.2023
// Autor:            Rahm

#include "in_out.h"
#include <arduino.h>

const int PWM_FREQU = 1'000;
const int PWM_RESOLUTION = 8;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

uint16_t duty_cycle = MAX_DUTY_CYCLE/2;

#if defined(_ESP32_CARRIER_BOARD_)   //ESP32-Carrier-Board
  const int portx[] = {9,10,14,4,33,15,13,32};
  const int porty[] = {2,27,5,23,19,18,36,39};
  //const int pwm_channel[] = {2,27,5,23,19,18,9,10,14,4,33,15,13,32};
  #define PWM_PIN 2     // LCD-Backlight
  #define ADC1_PIN 34   // CH1: Poti/LDR/Exp.-Port
  #define ADC2_PIN 38   // CH2: Exp.-Port
#elif defined(_ESP32_ESPRIT_BOARD_)   //ESP32-Esprit-Board
  const int portx[] = {16,17,2,27,25,26,15,13};	
  const int porty[] = {12,14,5,23,19,18,33,32};
  //const int pwm_channel[] = {23,12,14,5,19,18,16,17,2,27,26,25,15,13};
  #define PWM_PIN 23   // LCD-Backlight an Arduino-Carrier-Board
  #define ADC1_PIN 39  // CH1: Poti/LDR/Exp.-Port
  #define ADC2_PIN 36  // CH2: Exp.-Port
#else   //default: ESP32-Carrier-Board
  const int portx[] = {9,10,14,4,33,15,13,32};
  const int porty[] = {2,27,5,23,19,18,36,39};
  //const int pwm_channel[] = {2,27,5,23,19,18,9,10,14,4,33,15,13,32};
  #define PWM_PIN 2     // LCD-Backlight
  #define ADC1_PIN 34   // CH1: Poti/LDR/Exp.-Port
  #define ADC2_PIN 38   // CH2: Exp.-Port
#endif

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

// bit_toggle()-Funktion zum Schalten mit Taster und einfacher Tasterentprellung
void bit_toggle(uint8_t byte_adr, uint8_t bit_nr, uint8_t *_bit_status_)
{
    if( !bit_read(byte_adr,bit_nr) )
    {
      delay_ms(20);                     // Entprellzeit
      while( !bit_read(byte_adr,bit_nr));
      delay_ms(20);
      *_bit_status_ = ~*_bit_status_;
      *_bit_status_ &= 0x01;                   // Bit 0 ausmaskieren!
    }
}

//***************************************************************
// ab hier PWM-Funktionen
//***************************************************************
void pwmx_init( uint8_t bit_nr )
{
  // ab ESP-Arduino-EDF 3.0.x
  // erfolg die Kanalauswahl automatisch durch die IDF.
  ledcAttach(bit_nr, PWM_FREQU, PWM_RESOLUTION);
  ledcWrite(bit_nr,0);              // PWM stoppen (dutycycle = 0)

  #ifdef PWM_DEBUG
	  rs232_init();
	  rs232_print("\n\rPWM init on GPIO: ");rs232_byte(bit_nr);
	  rs232_print("\n\r       Frequency: ");rs232_int(PWM_FREQU);
    rs232_print("\n\r      Resolution: ");rs232_int(PWM_RESOLUTION);
    rs232_print("\n\r            Mode: ");rs232_print("Stopped");
	#endif
}

void pwmx_start( uint8_t bit_nr )
{
  // ab ESP-Arduino-EDF 3.0.x
  ledcWrite(bit_nr,duty_cycle);

  #ifdef PWM_DEBUG
	  rs232_print("\n\rPWM      on GPIO: ");rs232_byte(bit_nr);
	  rs232_print("\n\r       dutycycle: ");rs232_int(duty_cycle);
    rs232_print("\n\r            Mode: ");rs232_print("Run");   
	#endif
}

void pwmx_stop( uint8_t bit_nr )
{
  // ab ESP-Arduino-EDF 3.0.x
  ledcWrite(bit_nr,0);
	
  #ifdef PWM_DEBUG
	  rs232_print("\n\rPWM      on GPIO: ");rs232_byte(bit_nr);
    rs232_print("\n\r            Mode: ");rs232_print("Stopped");
	#endif
}
void pwmx_duty_cycle ( uint8_t bit_nr, uint16_t value )
{
  // ab ESP-Arduino-EDF 3.0.x
  duty_cycle = value;
  ledcWrite(bit_nr,duty_cycle);

	#ifdef PWM_DEBUG
  	rs232_print("\n\rPWM      on GPIO: ");rs232_byte(bit_nr);
	  rs232_print("\n\rPWM    dutycycle: ");rs232_int(duty_cycle);
	#endif
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

void pwm_duty_cycle( uint8_t value )
{
  pwmx_duty_cycle(PWM_PIN,(uint16_t) value);
}

//***************************************************************
// ab hier ADC-Funktionen
//***************************************************************

//uint16_t adc_in(uint8_t ch);       // Prototyp! Lokale Funktion mit Kanalwahl

// Initialisierung des ADU.
void adc_init(void)
{
  #ifdef ADC_DEBUG
   rs232_init();
   rs232_print("\r\nADC init");
  #endif	 
}

// Einlesen des ADC-Kanals 1
uint8_t adc_in1( void )
{
  return (adc_in(ADC1_PIN));
}

// Einlesen des ADC-Kanals 2
uint8_t adc_in2( void )
{
  return (adc_in(ADC2_PIN));
}

// Funktion mit Kanal-Parameter
uint8_t adc_in(uint8_t pin_nr)
{
  uint8_t temp = (uint8_t) (adc_in12(pin_nr)>>4);
  
  #ifdef ADC_DEBUG
    rs232_print("\r\n 8-Bit Value: "); rs232_byte(temp);	
  #endif
  
  return (temp);
}

// Gibt den 12 Bit-Wert des ADC zurück. Funktion mit Kanal-Parameter
uint16_t adc_in12(uint8_t pin_nr)
{
  uint16_t temp = analogRead(pin_nr);
  
  #ifdef ADC_DEBUG
    rs232_print("\r\nADC on GPIO: ");rs232_printdd(pin_nr);
    rs232_print("\r\n12-Bit Value: ");rs232_int(temp);	
  #endif
  return temp;
}