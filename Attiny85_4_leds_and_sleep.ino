 /* Project
                            --------
D5(A0)/RESET/ADC0/PB5 -- 1  /      / 8 -------   Vcc   ---------
D3(A3)/XTAL1/ADC3/PB3 -- 2  /      / 7 -- PB2/SCL/SCK/------- D2(A1)
D4(A2)/XTAL2/ADC2/PB4 -- 3  /      / 6 -- PB1/MISO/---------- D1
-----   GND   ---------- 4  /      / 5 -- PB0/MOSI/SDA/AREF/- D0 
                             --------
Pin Pin Name    I/O Type  Description
1   PB5 (OC1A)  Output    PWM output channel A
2   PB3 (OC2A)  Output    PWM output channel B
3   PB4         Input     General-purpose digital I/O pin
4   GND –       Ground
5   PB0 (MOSI)  In/Out    SPI master-out, slave-in or general-purpose digital I/O pin
6   PB1 (MISO)  In/Out    SPI master-in, slave-out or general-purpose digital I/O pin
7   PB2 (SCK)   In/Out    SPI clock or general-purpose digital I/O pin
8   VCC –       Positive supply voltage

upload example 11.ArduinoISP
then select tools->board-> attiny85
programmer Arduino as ISP
code from http://www.technoblogy.com/list?113N
*/

#include <avr/sleep.h>   //Needed for sleep_mode
//#include <avr/wdt.h>   //Needed to enable/disable watch dog timer
//#include <Arduino.h>

// Utility macro
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)

const int Led1 = 0;        // PB0 (pin 5 of ATtiny85 )
const int Led2 = 1;        // PB1 (pin 6 of ATtiny85 )
const int Led3 = 2;        // PB2 (pin 7 of ATtiny85 ) 
const int Led4 = 4;        // PB4 (pin 3 of ATtiny85 )
const int Unused3 = 3;     // PB3
const int Unused5 = 5;     // PB5
int watchdog_counter = 0;  // add +1 at each awake of the processor..

int LedArray[] = {Led1, Led2, Led3, Led4};


// Watchdog timer **********************************************
//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec

void WDDelay(int n) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  WDTCR = 1<<WDIE | (n & 0x8)<<2 | 1<<WDE | (n & 0x7);
  sleep_enable();
  sleep_cpu();
}

ISR(WDT_vect) {
  // Special sequence to disable watchdog timer
  WDTCR = 1<<WDCE | 1<<WDE;
  WDTCR = 0;
}

void setup() {  
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Unused3, INPUT_PULLUP);
  pinMode(Unused5, INPUT_PULLUP);
  adc_disable(); // ADC uses ~320uA
  }

void loop() {
  adc_disable(); // ADC uses ~320uA
  WDDelay(9); // 8sec
  WDDelay(9); // 16sec
  WDDelay(9); // 24sec

for ( int y=0; y<4; y++) {  // 4 times
      int x = random(0,4);
          for ( int i=0; i<2; i++){               // 2 flashs
            digitalWrite(LedArray[x], HIGH);
            WDDelay(0);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
            digitalWrite(LedArray[x], LOW);
            WDDelay(3);  // 128ms
            }
    }
} // end of loop
