/*
Programm:           Carrier Testprogramm für Arduino-IDE
Beschreibung:       Verwendet die FA205-Bibliothek für Arduino

Autor:              Rahm
Datum:              24.04.2017
letzte Änderung:    16.12.2021
*/

#include <controller.h>
//#include "datalogger.h"

// i2c-Adressen für Temperatursensor
#define  LM75_ADDR_R  0x91
#define  LM75_ADDR_W  0x90

#define LEDs PORTx
#define S1 PORTy,2
#define S2 PORTy,3
#define S3 PORTy,4
#define S4 PORTy,5

void backlight(void);
int16_t lm75_read1(void);

uint8_t status2 = 0;
uint8_t status3 = 0;
uint8_t status4 = 0;
uint8_t status5 = 0;
volatile uint16_t mil = 0;
volatile uint16_t sek = 0;
volatile uint8_t n = 0;

char adcpic1[] =   {0x01,0x1D,0x15,0x15,0x15,0x15,0x17,0x10};
char adcpic2[] =   {0x02,0x14,0x18,0x1C,0x01,0x0A,0x0C,0x0E};
char temppic1[] =  {0x04,0x0A,0x0B,0x0A,0x0B,0x0E,0x0F,0x0E};
char temppic2[] =  {0x04,0x0A,0x0B,0x0E,0x0F,0x0E,0x0F,0x0E};
char lamptest1[] = {0x00,0x17,0x12,0x12,0x12,0x12,0x1A,0x00};
char lamptest2[] = {0x1F,0x08,0x0D,0x0D,0x0D,0x0D,0x05,0x1F};
char sound1[] =    {0x01,0x03,0x0D,0x09,0x09,0x0D,0x03,0x01};
char sound2[] =    {0x01,0x03,0x0F,0x0F,0x0F,0x0F,0x03,0x01};
  
#define _HAPPY_
#ifdef _HAPPY_
 char title1[] = " Happy Birthday ";
 char title2[] = "     to YOU     ";
 char midi_key[] = { 60, 60, 62, 60, 65, 64, 60, 60, 62, 60, 67, 65, 60, 72, 69, 65, 64, 62, 70, 70, 69, 65, 67,  65, 0};  
 uint16_t duration[]= {338,113,451,451,451,902,338,113,451,451,451,902,338,451,451,451,451,451,338,113,451,451,451,1402};
 uint16_t silence[] = { 37, 12, 49, 49, 49, 98, 37, 12, 49, 49, 49, 98, 37, 49, 49, 49, 49, 49, 37, 12, 49, 49, 49,   0};
#else
 char title1[] = "   Also sprach  ";
 char title2[] = "   Zarathustra  ";
 char midi_key[] = {  60,  67,  72, 63,  64,  60,  67,  72,  63,  64,  60,  67,  72,  60,  65, 64,  65,  67,  67,  67,  67,  69,  71,  72, 0};
 uint16_t duration[]= {1854,1802,3312,125,4145,1625,1718,3468, 114,4093,1687,1833,3447, 156,3635,416, 427,2010, 125, 125,1802,1072,1135,4000};
 uint16_t silence[] = { 219, 208, 324,292,3792, 219, 208, 324, 292,3793, 313, 167, 344, 167,3500, 43, 104, 209, 166, 156, 157,  63,  53,  10};
#endif

void setup (void)   // Initialisierungen
{ 
  //Serial.begin(9600);
  rs232_init();
  rs232_print("Hallo\n");
  byte_init(LEDs,OUT);
  byte_write(LEDs,0xff);
  bit_init(S1,IN);
  bit_init(S2,IN);
  bit_init(S3,IN);
  bit_init(S4,IN);
  adc_init();
  i2c_init();
  lcd_init();
  lcd_clear();
  
  timer1ms_init(timer1ms_isr);
  
  lcd_defchar(adcpic1,0);
  lcd_defchar(adcpic2,1);
  lcd_defchar(temppic1,2);
  lcd_defchar(temppic2,3);
  lcd_defchar(sound1,4);
  lcd_defchar(sound2,5);
  lcd_defchar(lamptest1,6);
  lcd_defchar(lamptest2,7);

  lcd_setcursor(1,1);   lcd_print("Board Test   ");

  timer1ms_enable();
  pwm_init();
  pwm_start();
}

uint8_t temp, i;
    
void loop (void)
{
  uint8_t hell = adc_in1();                    // Analogen Helligkeitswert lesen
  pwm_duty_cycle(hell);                // PWM Tastgrad ändern

  if (!bit_read(S1))
  {
    timer1ms_disable();
    delay_ms(20);
    lcd_setcursor(1,1); lcd_print("Switch BL to PWM");
 //   pwm_init();
 //   pwm_start();
    while (!bit_read(S1))
    {
      backlight();
      delay_ms(100);
    }      
 //   pwm_stop();
  //  bit_init(S2,IN);
    lcd_setcursor(1,1); lcd_print("Board Test      ");
    timer1ms_enable();
  }

  if(!bit_read(S2))
  {
    int16_t   temperatur;
    uint8_t   dezimale,ganzzahl;
    char   buffer[LCD_LEN+1];
    
    timer1ms_disable();
    //rs232_init();
    delay_ms(20);

    while(!bit_read(S2))
    {
      temperatur = lm75_read1();
      dezimale = temperatur;
      if (dezimale!=0) dezimale = 5; else dezimale = 0;
      ganzzahl = temperatur >>8;
      sprintf(buffer,"%3d,%1u°C",ganzzahl,dezimale);
      
      // Ausgabe auf LC-Display
      lcd_setcursor(1,1); lcd_char(' ');lcd_char(2); lcd_print(": ");lcd_print(buffer);
      lcd_setcursor(2,1); lcd_print("  Test run ...  ");
      // Ausgabe über RS232
      //rs232_print(buffer);
      //rs232_put('\r');            // Wagenrücklauf

      delay_ms(500);
    }
   
    lcd_setcursor(1,1); lcd_print("Board Test      ");
    byte_init(LEDs,OUT);
    timer1ms_enable();
  }
  
  bit_toggle(S4,&status5);
  if (status5)
  {
    timer1ms_disable();
    lcd_setcursor(1,1); lcd_print("Lamptest..      ");
    lcd_setcursor(2,1); lcd_print("  Test run ...  ");
    for(i=10;i!=0;i--)
    {
            byte_write(LEDs,~0xAA); delay_ms(i*50);
            byte_write(LEDs,~0x55); delay_ms(i*50);
    }
    delay_ms(300);

    for ( i=0; i<=6; i++)
    {
      temp = ~byte_read(LEDs);  // LEDs sind lowaktiv
      temp <<= 1;               // 1 Bit nach links schieben
      byte_write(LEDs,~temp);
      delay_ms(150);
    }
    for ( i=0; i<=7; i++)
    {
      temp = ~byte_read(LEDs);  // LEDs sind lowaktiv
      temp >>= 1;               // 1 Bit nach rechts schieben
      byte_write(LEDs,~temp);
      delay_ms(150);
    }
    status5 = 0;
    lcd_setcursor(1,1); lcd_print("Board Test      ");
    timer1ms_enable();
  }
  
  bit_toggle(S3,&status4);
  if(status4)
  {
    timer1ms_disable();
    sound_init();
    lcd_setcursor(1,1); lcd_print(title1);
    lcd_setcursor(2,1); lcd_print(title2);

    uint8_t stopp = 0;
    for (int i=0; midi_key[i]!=0; i++)
    {
      play_note(midi_key[i],duration[i],silence[i]);
      if (!bit_read(S3)) 
      {
         stopp = 1;
         delay_ms(20);
         while(!bit_read(S3));
         delay_ms(20);
      }
      if (stopp == 1) break;            
    }
    
    status4 = 0;
    bit_write(S2,1);
    lcd_setcursor(1,1); lcd_print("Board Test      ");
    timer1ms_init(timer1ms_isr);
    timer1ms_enable();
  }
}

void timer1ms_isr( void )
{
  if( mil < 999) mil++;
  else
  {
    mil = 0;
    if (sek < 60) sek++;
    else sek = 0;
    
    if (sek%4 == 0) { lcd_setcursor(2,1);   lcd_print("S1:"); lcd_char(1);   // 1: LM75
                      lcd_print(" 2:"); lcd_char(2);   // 2: Analog
                      lcd_print(" 3:"); lcd_char(4);   // 3: Sound
                      lcd_print(" 4:"); lcd_char(7);   // 4: Lamptest
                    }
    if (sek%4 == 1) { lcd_setcursor(2,1);   lcd_print("S1:"); lcd_char(1);
                      lcd_print(" 2:"); lcd_char(3);
                      lcd_print(" 3:"); lcd_char(4);
                      lcd_print(" 4:"); lcd_char(6);
                      }
    if (sek%4 == 2) { lcd_setcursor(2,1);   lcd_print("S1:"); lcd_char(0);
                      lcd_print(" 2:"); lcd_char(3);
                      lcd_print(" 3:"); lcd_char(5);
                      lcd_print(" 4:"); lcd_char(6);
                    }
    if (sek%4 == 3) { lcd_setcursor(2,1);   lcd_print("S1:"); lcd_char(0);
                      lcd_print(" 2:"); lcd_char(2);
                      lcd_print(" 3:"); lcd_char(5);
                      lcd_print(" 4:"); lcd_char(7);
                    }
    
    if (n > 0) n = 0;
    else n = 1;
  }
}

void backlight(void)
{
  volatile uint8_t hell;
  volatile uint8_t i;
  
  hell = adc_in1();                    // Analogen Helligkeitswert lesen
  pwm_duty_cycle(hell);                // PWM Tastgrad ändern
  
  lcd_setcursor(2,1);

  for (i=1; i<=16;i++)
  {
    if (hell > 15)
    {
      lcd_char(0xff);                  // Blocksymbol anzeigen
      hell -= 15;
    }
    else   lcd_char(' ');              // Leerzeichen anzeigen
  }
}

int16_t lm75_read1(void)
{
  uint16_t msb,lsb,data16;
  
  i2c_start();                // Startbedingung
  i2c_write(LM75_ADDR_W);     // Schreibwunsch an LM75 senden
  i2c_write(0x00);            // Pointer auf 0 setzen
  i2c_stop();
  
  i2c_start();
  i2c_write(LM75_ADDR_R);     // Lesewunsch an LM75 senden
  msb = i2c_read(ACK);        // MSB lesen und weiteren Wert anfordern.
  lsb = i2c_read(NACK);       // LSB lesen
  i2c_stop();                 // Stoppbedingung
  
  msb <<= 8;
  data16 = msb | lsb;
  data16 &= 0xff80;           // niederwertige Bits ausmaskieren! 
  return data16;              // Genauigkeit: +/- 0,5°C
}
