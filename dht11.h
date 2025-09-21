/*
 * dht11.h
 *
 * Created: 26.07.2018 19:48:37
 *  Author: rahm
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#include "controller.h"

#define DHT11_PIN PORTy,2

// Liest den dht11 Feuchte und Temperatursensor
// data[0] = Feuchte (20 .. 95%)
// data[1] = Feuchte dezimal (0 bei DHT11)
// data[2] = Temperatur  (0..50°C)
// data[3] = Temperatur dezimal (0 bei DHT11)
// data[4] = Checksumme  (data[4] = data[0]+data[1]+data[2]+data[3])
// Rueckgabewert: 0 = error

uint8_t dht11_read(uint8_t *data);

#endif /* DHT11_H_ */