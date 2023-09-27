// Technische Richtlinie FA205
// Bibliothek:       communication.c
// Controller:       ESP32
//     I2C und RS232 Routinen
// Version:          1.0
// erstellt am:      18.8.2023
// letzte Änderung:  4.9.2023
// Autor:            Rahm

#include "communication.h"
#include <arduino.h>
//#include "esp_log.h"
#include "driver/i2c.h"

#ifdef _SERIALBT_
  #include "BluetoothSerial.h" 
  BluetoothSerial SerialBT;

#endif

// *****************************************************************************************
// I2C- Routinen zur Ansteuerung eins I2C-Slaves
// I2C-Bus-Funktionen i2c_init, i2c_start, i2c_stop, i2c_write, i2c_read, i2c_ack, i2c_nack
// *****************************************************************************************

#define I2C_MASTER_SCL_IO           22          /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           21          /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0   /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0           /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0           /*!< I2C master do not need buffer */
#define ACK_CHECK_EN                0x1         /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS               0x0         /*!< I2C master will not check ack from slave */
#define ACK_VAL                     (i2c_ack_type_t) ACK         /*!< I2C ack value */
#define NACK_VAL                    (i2c_ack_type_t) NACK        /*!< I2C nack value */
#define I2C_MASTER_FREQ_HZ          _I2C_FREQUENZ_               /*!< I2C master clock frequency */

// ****** Initialiserung I2C-Bus **********************************************

i2c_cmd_handle_t cmd;
//#define _I2C_MAX_DATAREAD_ 5
uint8_t i2c_data[_I2C_MAX_DATAREAD_];  //max. 5 Byte lesen
volatile uint8_t i2c_dp = 0;  // Index auf i2c_data[]

void i2c_init (void)
{
  int i2c_master_port = I2C_MASTER_NUM;

  i2c_config_t conf = {
      conf.mode = I2C_MODE_MASTER,
      conf.sda_io_num = I2C_MASTER_SDA_IO,
      conf.scl_io_num = I2C_MASTER_SCL_IO,
      conf.sda_pullup_en = GPIO_PULLUP_DISABLE,
      conf.scl_pullup_en = GPIO_PULLUP_DISABLE,
      conf.master.clk_speed = I2C_MASTER_FREQ_HZ,
  };

  i2c_param_config(i2c_master_port, &conf);
  i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
  i2c_filter_enable(i2c_master_port, 4);           // cyc_num = 0...7
}

// ****** Startbedingung I2C-Bus **********************************************

void i2c_start (void)
{
  cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
}

void i2c_rstart (void)
{
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS); //bestehenden cmd (ohne stop) senden
  i2c_cmd_link_delete(cmd);
  cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
}

//****** Stoppbedingung I2C-Bus ***********************************************************
void i2c_stop (void)
{
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
}

// ****************************************************************************************
// * Byte ausgeben an I2C-Bus , Rückgabewert = ack = ACK/NACK
// ****************************************************************************************
uint8_t i2c_write (uint8_t xbyte)
{
  esp_err_t ret = i2c_master_write_byte(cmd, xbyte, ACK_CHECK_EN);

  if (ret == ESP_OK) return ACK;      // Tatsächlich wird hier nur geprüft, ob
  else               return NACK;     // die Parameterübergabe korrekt ist.
                                      // ack vom Slave kann erst bei i2c_stop() gecheckt werden.
}

// ****************************************************************************************
// * Byte einlesen vom I2C-Bus.
// ****************************************************************************************
uint8_t i2c_read (uint8_t ack)
{
  uint8_t i;

  if (i2c_dp >= _I2C_MAX_DATAREAD_)  i2c_dp = 0;

  if(ack==ACK)
  {
    i2c_master_read_byte(cmd, &i2c_data[i2c_dp], ACK_VAL);
    i = i2c_dp;
    i2c_dp++;
  }
  else
  {
    i2c_master_read_byte(cmd, &i2c_data[i2c_dp], NACK_VAL);
    i = i2c_dp;
    i2c_dp = 0;
  }

  return i;          // ret Anzahl Index auf Datenpuffer
}

// ****************************************************************************
// RS232-Routinen zur Kommunikation mit PC-Terminal
//
// ****************************************************************************
void rs232_init(void)
{ 
  rs232_baud(BAUD);
}

void rs232_baud ( uint32_t baud )     // Ändert die Baudrate
{
  #if defined(_SERIAL0_)
    Serial.end();
	delay_ms(20);
    Serial.begin(baud);
	while (!Serial);
  #elif defined(_SERIAL1_)
    Serial1.end();
	delay_ms(20);
    Serial1.begin(baud);
	while (!Serial1);
  #elif defined(_SERIALBT_)
    SerialBT.end();
	delay_ms(20);
    SerialBT.begin(_DEVICENAME_);
	while (!SerialBT);
  #endif
}

uint8_t rs232_get ( void )
{
  #if defined(_SERIAL0_)
    if(Serial.available() > 0)    
          return Serial.read();
    else  return 0;              // Wenn kein Zeichen im Puffer, dann 0 zurückgeben!
  #elif defined(_SERIAL1_)
    if(Serial1.available() > 0)    
          return Serial1.read();
    else  return 0;              // Wenn kein Zeichen im Puffer, dann 0 zurückgeben!
  #elif defined(_SERIALBT_)
    if(SerialBT.available() > 0)    
          return SerialBT.read();
    else  return 0;              // Wenn kein Zeichen im Puffer, dann 0 zurückgeben!
  #endif
}

void rs232_put ( uint8_t value )
{
  #if defined(_SERIAL0_)
    Serial.write(value);
  #elif defined(_SERIAL1_)
    Serial1.write(value);
  #elif defined(_SERIALBT_)
    SerialBT.write(value);  
  #endif
}

void rs232_print ( const char *text )
{
  while (*text != '\0')
  rs232_put(*text++);
}

// RS232-Erweiterungen sind nicht Teil der Technischen Richtlinie FA205!!
// Erzeugt ein Eingabeprompt am Terminal 00 .. 99
uint8_t rs232_inputdd(void )
{
  uint8_t buf1,buf2;
  do
  {
    while((buf1 = rs232_get()) == 0);
  } while ((buf1 < '0') || (buf1 > '9'));

  rs232_put(buf1);    // Echo

  do
  {
    while((buf2 = rs232_get()) == 0);
  } while ((buf1 < '0') || (buf1 > '9'));

  rs232_put(buf2);    // Echo

  buf1 -= '0';
  buf2 -= '0';

  return (buf1*10 + buf2);
}

// Ausgabe einer Dezimalzahl 00..99 auf RS232
void rs232_printdd(uint8_t value)
{
  uint8_t buf;

  buf = value / 10;
  rs232_put(buf+'0');      // 10er-Stelle anzeigen
  buf = value % 10;
  rs232_put(buf+'0');      // 1er-Stelle anzeigen
}

void rs232_byte(uint8_t val)
{
  uint8_t buffer[3];
  uint8_t n = 0;	
	
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
				
  while (n<3)               // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';					
  }

  while (n > 0)             // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    rs232_put(buffer[n]);
  }
}

void rs232_int(uint16_t val)
{
  uint8_t buffer[5];
  uint8_t n = 0;
  
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
  
  while (n<5)                   // Rest von buffer mit blank f�llen
  {
    buffer[n++] = ' ';
  }

  while (n > 0)                 // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    rs232_put(buffer[n]);
  }
}

uint8_t rs232_binary_get ( void )
{
   while (!rs232_is_received());    // warten, bis Byte im Puffer!

  #ifdef _SERIAL1_
    return Serial1.read();                     // wert zurückgeben
  #else
    return Serial.read();                     // wert zurückgeben
  #endif
}

uint8_t rs232_readbytes(uint8_t *buf, int8_t _length)
{
   uint16_t timeout;
   uint8_t  i = 0;

   //Puffer löschen
   for (uint8_t n=0; n<=_length;n++)  buf[n] = 0;

   while(_length > 0)
   {
     timeout = 0;
     while(!rs232_is_received())
     {
        if(timeout>=0xfffe) return 0;
        else                 timeout++;
     }
     
     *buf++ = rs232_binary_get();
     _length--;
     i++;
   }
   return i; 
}

uint8_t rs232_is_received( void )
{
    #ifdef _SERIAL1_
      if (Serial1.available()<0) return 0;    // 0, wenn kein Byte im Puffer!
    #else
      if (Serial.available()<0) return 0;    // 0, wenn kein Byte im Puffer!
    #endif

    return 1;                      // 1, wenn ein Byte im Puffer
}