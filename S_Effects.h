#ifndef SEFFECTS_H
#define SEFFECTS_H

#include "mbed.h"

//Functions for sound
void playTone(float freq, float duration);
void LooseSound();
void CorrectSound();
void introSound();
void Beep();
void countdownBeep(int seconds);

//Functions For LED
void LED_send(uint8_t green, uint8_t );
void flashGreen(int times = 8);
void flashRed(int times = 8);


#endif