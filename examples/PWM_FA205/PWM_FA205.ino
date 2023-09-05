/* 
 Beschreibung:	  Testprogramm (Sketch) zur Umsetzung
                  der Technischen Richtlinie FA205 für ESP32.
     Datei:           PWM_FA205.ino
     Autor:           Baier/Rahm
     Datum:           01.10.2015
     Letzte Änderung: 05.09.2023
     TR 3.2 -Pulsweitenmodulation-
     
   Das PWM-Signal wird an PORTy,0 (GPIO 2) ausgegeben (ggf. ändern in in_out.h).
   
   Tastgrad   0% (  0) -> ti= 0,0  * T (low-aktive LED max. hell)
   Tastgrad  25% ( 64) -> ti= 0,25 * T
   Tastgrad  50% (128) -> ti= 0,50 * T (low-aktive LED 50%  hell)
   Tastgrad  75% (192) -> ti= 0,75 * T
   Tastgrad 100% (255) -> ti= 1,0  * T (low-aktive LED min. hell)
*/

#include "controller.h"

void setup()
{ 
  pwm_init();
}

void loop()
{	
  pwm_duty_cycle(64);      //Tastgrad=25% setzen
  pwm_start();
  delay_ms(5000);
  pwm_stop();
  
  while(1);   // Endlos
}

/*
  Alternativ ist die Funktion pwmx_...- implementiert. Hier kann beim Aufruf
  ein GPIO angegeben werden. Erlaubt sind: 2,27,5,23,19,18,9,10,14,4,33,15,13,32
  Damit können insgesamt 14 unabhängige PWM-Kanäle realisiert werden.
  Bsp.: pwmx_init(27);    // PWM an GPIO 27
        pwmx_start(27);
        pwmx_duty_cycle(27,192); // duty_cycle auf 75%
*/