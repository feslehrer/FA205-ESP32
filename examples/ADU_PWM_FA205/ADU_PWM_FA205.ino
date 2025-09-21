/* 
    Beschreibung: Testprogramm (Sketch) zur Umsetzung
                  der Technischen Richtlinie FA205 für Arduino.
                  
    Datei:           ADU_PWM_FA205.ino
    Autor:           Baier/Rahm
    Datum:           01.10.2015
    Letzte Änderung: 05.09.2023 
    
    TR 3.2 -Analg-Digital-Konverter (ADC) und Pulsweitenmodulaion-
*/

#include "controller.h"
#include <stdio.h>

void setup()
{ 
  pwm_init();
  adc_init();
  rs232_init();  //zur Testausgabe am seriellen Monitor    
}

void loop()
{	/* Analogwert an A1 holen und als PWM-Tastgrad ausgeben */
   char send_buffer[10];

   uint8_t anWert = adc_in1();  //Tastgrad Analogwert holen
   sprintf(send_buffer,"%d\n\r",anWert);       
   rs232_print(send_buffer);      //Testausgabe serieller Monitor
   pwm_duty_cycle(anWert);      //PWM Tastgrad setzen
   pwm_start();
   delay(5000);
   pwm_stop();
       
   while(1);                     //Endlosschleife
}
