// Technische Richtlinie FA205
// Bibliothek:       interrupt.c
// Controller:       ESP32
//     
// Version:          1.0
// erstellt am:      4.9.2023
// letzte Änderung:  02.02.2025
// Autor:            Rahm

#include "interrupt.h"
#include "math.h"

// Lokale initialisierungen
static uint8_t trigger;

static hw_timer_t *timer = NULL;
//const int timerNr = 0;    // ESP-Arduino-IDF 2.0.x
//#define _DIVIDER_ 80
//const unsigned long Alarm = 1'000'000;      // ab ESP-Arduino-IDF 3.0.x
const unsigned long frequency = 1'000'000;

void note_isr( void );

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

  #ifdef INT_DEBUG
    rs232_init();
	rs232_print("\r\nInterrupt0 init");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN0_);
	rs232_print("\r\n  Trigger: ");
	switch (trigger) 
	{
	 case 1: rs232_byte(trigger); rs232_print(" (Rising)"); break;
	 case 2: rs232_byte(trigger); rs232_print(" (Falling)"); break;
	 case 3: rs232_byte(trigger); rs232_print(" (Change)"); break;
	}	
  #endif 
  
}

void IRAM_ATTR interrupt0_isr(void)
{
  my_interrupt0();
}

void ext_interrupt_enable( void )
{
  attachInterrupt(_INTERRUPT_PIN0_,interrupt0_isr,trigger);
  #ifdef INT_DEBUG
	rs232_print("\r\nInterrupt0 enable");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN0_);
	rs232_print("\r\n  Trigger: ");
	switch (trigger) 
	{
	 case 1: rs232_byte(trigger); rs232_print(" (Rising)"); break;
	 case 2: rs232_byte(trigger); rs232_print(" (Falling)"); break;
	 case 3: rs232_byte(trigger); rs232_print(" (Change)"); break;
	}	
  #endif   
}

void ext_interrupt_disable( void )
{
  detachInterrupt(_INTERRUPT_PIN0_);
  #ifdef INT_DEBUG
	rs232_print("\r\nInterrupt0 disable");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN0_);
  #endif 
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

  #ifdef INT_DEBUG
    rs232_init();
	rs232_print("\r\nInterrupt1 init");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN1_);
	rs232_print("\r\n  Trigger: ");
	switch (trigger) 
	{
	 case 1: rs232_byte(trigger); rs232_print(" (Rising)"); break;
	 case 2: rs232_byte(trigger); rs232_print(" (Falling)"); break;
	 case 3: rs232_byte(trigger); rs232_print(" (Change)"); break;
	}
  #endif 	
}

void IRAM_ATTR interrupt1_isr(void)
{
  my_interrupt1();
}

void ext_interrupt1_enable( void )
{
  attachInterrupt(_INTERRUPT_PIN1_,interrupt1_isr,trigger);
  #ifdef INT_DEBUG
	rs232_print("\r\nInterrupt1 enable");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN1_);
	rs232_print("\r\n  Trigger: ");
	switch (trigger) 
	{
	 case 1: rs232_byte(trigger); rs232_print(" (Rising)"); break;
	 case 2: rs232_byte(trigger); rs232_print(" (Falling)"); break;
	 case 3: rs232_byte(trigger); rs232_print(" (Change)"); break;
	}	
  #endif 
}

void ext_interrupt1_disable( void )
{
  detachInterrupt(_INTERRUPT_PIN1_);
  #ifdef INT_DEBUG
	rs232_print("\r\nInterrupt1 disable");
	rs232_print("\r\n  on GPIO: ");rs232_byte(_INTERRUPT_PIN1_);
  #endif 
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
  //return ((uint64_t)((float)(F_CPU/_DIVIDER_/1000) * time));
  return ((uint64_t)(time * (frequency / 1000.0)));
}

void IRAM_ATTR timer0_isr(void)
{
  my_timer();                    // aufrufen der Interrupt-Serviceroutine
}

void timer_ms_init ( void (*ti) (void), float time)
{
  my_timer = ti;					         // Pointer auf isr im User-Code (normalerweise: timer_ms_isr) !!
  //timer = timerBegin(timerNr,_DIVIDER_,true);  // ESP-Arduino-IDF 2.0.x
  //timerAlarmWrite(timer,calculate_alarm(time),true);
  //timerAttachInterrupt(timer,timer0_isr,true);

  int alarm = calculate_alarm(time);
  timer = timerBegin(frequency);      // ab ESP-Arduin-IDF 3.0.x
  timerAttachInterrupt(timer,&timer0_isr);
  timerAlarm(timer,alarm,true,0);

  #ifdef TIMER_DEBUG
	  rs232_init();
	  rs232_print("\n\rTimer init onTime: ");rs232_byte(time);
	  rs232_print("\n\r        Frequency: ");rs232_int(timerGetFrequency(timer));
	  rs232_print("\n\r       Resolution: ");rs232_int(alarm);
	#endif
  timerStop(timer);
}

void timer_ms_enable( void )
{
  timerStart(timer);
  //timerAlarmEnable(timer); // ESP-Arduino-IDF 2.0.x
  #ifdef TIMER_DEBUG
    rs232_print("\n\r       Timer Mode: ");rs232_print("START");
	#endif
}

void timer_ms_disable( void )
{
  timerStop(timer);
  //timerAlarmDisable(timer); // ESP-Arduino-IDF 2.0.x
  #ifdef TIMER_DEBUG
    rs232_print("\n\r       Timer Mode: ");rs232_print("STOPP");    
	#endif
}

// Ab hier keine FA205-Funktionen

/* Soundausgabe auf Lautsprecher an PORTy,1 (GPIO 27).
   Verwendet timer_ms
*/ 
#ifndef TON_PORT
  #define TON_PORT PORTy
#endif
#ifndef TON_BIT
  #define TON_BIT 1
#endif

void note_isr( void )
{  
  bit_write(TON_PORT,TON_BIT,~bit_read(TON_PORT,TON_BIT));
}

void sound_init(void)
{
    bit_init(TON_PORT,TON_BIT,OUT);
    timer_ms_init(note_isr, 1000);
    timer_ms_disable;
}

void note_on(float frequenz)
{
  float millisec;
  
  millisec = 500/frequenz;    // Zeit für Halbe Periodendauer in ms
  
  timerAlarm(timer,calculate_alarm(millisec),true,0);
  //timer_ms_init(note_isr, millisec);
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

/*
// Data Received Interrupt (not working yet!!)
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