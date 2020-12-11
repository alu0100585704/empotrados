#ifndef SIETESEG
#define SIETESEG

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <ad.h>

void sieteSeg_valor (uint16_t valor);

void sieteSeg_digitos (uint8_t * valor);

void sieteSeg_init ();

void refresh();

#endif
