#include "mbed.h"

#include "S_Effects.h"
using namespace mbed;


SPI sw(p3, NC, p2);     
DigitalOut cs(p5);     

PwmOut buzzer(p27);


float frequency[] = {659,554,659,554,440,494,554,587,494,659,554,440};
float beat[]      = {1,1,1,1,1,0.5,0.5,1,1,1,1,2};

void playTone(float freq, float duration) {
    buzzer.period(1.0f / freq);
    buzzer.write(0.5f); 
    wait_us(duration * 200000); 
    buzzer.write(0);
    wait_us(30000);
}
void Beep() {
    playTone(880, 0.15);  // produces a short beep sound as the countdown
}                         // is running
void countdownBeep(int seconds) {
    for (int i = seconds; i > 0; i--) {
        Beep();
        wait_us(850000);  // ~0.85-second spacing
    }
}


void introSound() {
    playTone(659, 0.25);   
    playTone(784, 0.25);   
    playTone(988, 0.25);   
    playTone(1318, 0.35);  
}


void LED_send(uint8_t green, uint8_t red) {
    cs = 0;
    sw.write(green);   // When it is byte 1, green leds are sent
    sw.write(red);     // When it is byte 2, red leds sent
    cs = 1;
    cs = 0;
}

void flashGreen(int times) {
    for (int i = 0; i < times; i++) {
        LED_send(0x22, 0x22);   // green turns on
        wait_us(200000);
        LED_send(0x00, 0x00);   // green turns
        wait_us(200000);
    }
}

void flashRed(int times) {
    for (int i = 0; i < times; i++) {
        LED_send(0xFF, 0xFF);   
        wait_us(200000);
        LED_send(0x00, 0x00);   
        wait_us(200000);
    }
}

void CorrectSound() {
    playTone(880, 0.2);   
    playTone(988, 0.2);   
    playTone(1047, 0.25); 
}
void LooseSound() {
    for (int i = 0; i < 12; i++) {
        playTone(frequency[i], beat[i])
}
