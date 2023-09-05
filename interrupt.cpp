// Technische Richtlinie FA205
// Bibliothek:       interrupt.c
// Controller:       ESP32
//     
// Version:          1.0
// erstellt am:      4.9.2023
// letzte Änderung:  4.9.2023
// Autor:            Rahm

#include "interrupt.h"
//#include "math.h"
//#include "driver/uart.h"

// Lokale initialisierungen
static uint8_t trigger;

static hw_timer_t *timer = NULL;
const int timerNr = 0;
#define _DIVIDER_ 80

// Pointer auf Interrupt-Service-Routinen
void (*my_interrupt0) (void);
void (*my_interrupt1) (void);
void (*my_datareceived)  (void);
void (*my_timer)      (void);

//externer Interrupt 0
void ext_interrupt_init( void (*ip) (void) )
{
  my_interrupt0 = ip;
	#if defined(_FALLING_EDGE_TRIGGER_)
    trigger = FALLING;
	#elif defined(_RISING_EDGE_TRIGGER_)
	  trigger = RISING;
  #elif defined(_ANY_EDGE_TRIGGER_) 
	  trigger = CHANGE;
	#endif
}

void IRAM_ATTR interrupt0_isr(void)
{
  my_interrupt0();
}

void ext_interrupt_enable( void )
{
  attachInterrupt(_INTERRUPT_PIN0_,interrupt0_isr,trigger);
}

void ext_interrupt_disable( void )
{
  detachInterrupt(_INTERRUPT_PIN0_);
}

// Externer Interrupt 1
void ext_interrupt1_init( void (*ip) (void) )
{
	my_interrupt1 = ip;
	#if defined(_FALLING_EDGE_TRIGGER_)
    trigger = FALLING;
	#elif defined(_RISING_EDGE_TRIGGER_)
	  trigger = RISING;
  #elif defined(_ANY_EDGE_TRIGGER_) 
	  trigger = CHANGE;
	#endif
}

void IRAM_ATTR interrupt1_isr(void)
{
  my_interrupt1();
}

void ext_interrupt1_enable( void )
{
  attachInterrupt(_INTERRUPT_PIN1_,interrupt1_isr,trigger);
}

void ext_interrupt1_disable( void )
{
  detachInterrupt(_INTERRUPT_PIN1_);
}


//*******************************************************************
//** ab hier timer-Funktionen
//*******************************************************************
void timer1ms_init ( void (*ti) (void))
{
  timer_ms_init(ti,1.0);
}

void timer1ms_enable( void )
{
  timer_ms_enable();
}

void timer1ms_disable( void )
{
  timer_ms_disable();
}

// Die timer-Interrupt-Serviceroutine timer1ms_isr() muss
// selbst geschrieben werden!!
uint64_t calculate_alarm ( float time)
{ // time in ms
  return ((uint64_t)((float)(F_CPU/_DIVIDER_/1000) * time));
}

void IRAM_ATTR timer0_isr(void)
{
  my_timer();                    // aufrufen der Interrupt-Serviceroutine
}

void timer_ms_init ( void (*ti) (void), float time)
{
  my_timer = ti;					         // Pointer auf isr im User-Code (normalerweise: timer_ms_isr) !!
  timer = timerBegin(timerNr,_DIVIDER_,true);
  timerAlarmWrite(timer,calculate_alarm(time),true);
  timerAttachInterrupt(timer,timer0_isr,true);
}

void timer_ms_enable( void )
{
  timerStart(timer);
  timerAlarmEnable(timer);
}

void timer_ms_disable( void )
{
  timerStop(timer);
  timerAlarmDisable(timer);
}

// Data Received Interrupt
/*
void IRAM_ATTR uart_isr(void *arg)  // Interrupt-Vektor
{
  uint8_t buf;
  uart_read_bytes(EX_UART_NUM,&buf,1,1);
	my_datareceived();                // Aufruf der Benutzer-ISR
}

void serial_interrupt_init( void (*sr) (void))
{ 
  	my_datareceived = sr;

	  //rs232_init();  // Serielle Schnittstelle initialisieren
  	
    uart_config_t uart_config = {
		uart_config.baud_rate = 9600,
		uart_config.data_bits = UART_DATA_8_BITS,
		uart_config.parity = UART_PARITY_DISABLE,
		uart_config.stop_bits = UART_STOP_BITS_1,
		uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

  uart_param_config(EX_UART_NUM, &uart_config);
  uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
  uart_isr_free(EX_UART_NUM);
  uart_isr_register(EX_UART_NUM,uart_isr,NULL, ESP_INTR_FLAG_IRAM, rs232_handle);
}

extern void serial_interrupt_enable  ( void )
{
  uart_enable_rx_intr(EX_UART_NUM);
}

extern void serial_interrupt_disable ( void )
{
	uart_disable_rx_intr(EX_UART_NUM);           // Empfangsinterrupt ausschalten
}
*/

/*

//  
// Soundausgabe auf Lautsprecher an Port B.3
// 
#ifndef TON_PORT
  #define TON_PORT _PORTB_
#endif
#ifndef TON_BIT
  #define TON_BIT 3
#endif

void sound_init(void)
{
    bit_init(TON_PORT,TON_BIT,OUT);
}

void note_isr( void )
{  
  bit_write(TON_PORT,TON_BIT,~bit_read(TON_PORT,TON_BIT));
}

void note_on(float frequenz)
{
  float millisec;
  
  millisec = 500/frequenz;    // Zeit f�r Halbe Periodendauer in ms
  
  timer_ms_init(note_isr, millisec);
  timer_ms_enable();
}

void note_off(void)
{
  timer_ms_disable();
}

void play_note(uint8_t notenwert, uint16_t duration, uint16_t silence)
{
  float exponent = ((float)notenwert - 69.0) / 12.0;
  float frequenz = 440 * pow(2,exponent);

  note_on(frequenz);
  delay_ms(duration);
  note_off();
  delay_ms(silence);
}

*/