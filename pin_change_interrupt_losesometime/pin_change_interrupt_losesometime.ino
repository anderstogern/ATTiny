/*
Code for measuring air temperature and relative humidity using a RHT03 connected to t TinyTX3.
By Anders S. Tøgern, heavily inspired by Nathan Chantrell's TinyTX3: https://github.com/nathanchantrell/TinyTX
and Martin Harizanov's updated SMS doorbell: http://harizanov.com/2012/09/updating-my-sms-doorbell/

Licenced under the Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0) licence:
http://creativecommons.org/licenses/by-sa/3.0/
*/

#include <JeeLib.h>

#define PIN_REED    10
#define PIN_CLOSED  8
#define PIN_OPEN    9

static int gotPCI;

void handleInterrupt(boolean flash) {
  //Read and transmit measures
  int measure = digitalRead(PIN_REED);
  
  if (measure == HIGH) {
    digitalWrite(PIN_CLOSED, HIGH);
    digitalWrite(PIN_OPEN, LOW);
    
    if (flash) {
      Sleepy::loseSomeTime(100);
      digitalWrite(PIN_CLOSED, LOW);
      digitalWrite(PIN_OPEN, HIGH);
      Sleepy::loseSomeTime(100);
      digitalWrite(PIN_CLOSED, HIGH);
      digitalWrite(PIN_OPEN, LOW);  
    }
  } else {
    digitalWrite(PIN_CLOSED, LOW);
    digitalWrite(PIN_OPEN, HIGH);
    
    if (flash) {
      Sleepy::loseSomeTime(100);
      digitalWrite(PIN_CLOSED, HIGH);
      digitalWrite(PIN_OPEN, LOW);  
      Sleepy::loseSomeTime(100);
      digitalWrite(PIN_CLOSED, LOW);
      digitalWrite(PIN_OPEN, HIGH);
    }
  }
}

void setup() {
  pinMode(PIN_REED, INPUT);
  pinMode(PIN_CLOSED, OUTPUT);
  pinMode(PIN_OPEN, OUTPUT);
  
  gotPCI=false;
  
  GIMSK |= _BV(PCIE0);                    // Enable Pin Change Interrupts
  PCMSK0 |= _BV(PCINT0);                  // Use PA0 as interrupt pin
  
  handleInterrupt(false);
  
  Sleepy::loseSomeTime(100);
  
  sei(); //Enable interrupts
}

void loop() {
  if(gotPCI) {
    handleInterrupt(false);
    Sleepy::loseSomeTime(5000);
  }
  
  handleInterrupt(true);
  
  gotPCI=false;
  
  Sleepy::loseSomeTime(5000);
}

ISR(WDT_vect) {
  Sleepy::watchdogEvent();
}

ISR(PCINT0_vect) {
  gotPCI=true;
}

