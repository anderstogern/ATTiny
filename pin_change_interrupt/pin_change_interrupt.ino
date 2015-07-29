/*
Code for using pin change interrupts on ATTiny(84).
By Anders S. Tøgern

Licenced under the Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0) licence:
http://creativecommons.org/licenses/by-sa/3.0/

The code raises or lowers digital out pins 8 and 9 based on the state of digital input pin 10,
the change of which triggers an interrupt (the code works with a reed switch as interrupt
trigger and LEDs as outputs/indicators).
While not handling an interrupt the ATTiny is powered down to a minimum.
*/

#include <avr/sleep.h>

#define PIN_REED    10
#define PIN_CLOSED  8
#define PIN_OPEN    9

void handleInterrupt() {
  int measure = digitalRead(PIN_REED);
  
  if (measure == HIGH) {
    digitalWrite(PIN_CLOSED, HIGH);
    digitalWrite(PIN_OPEN, LOW);
  } else {
    digitalWrite(PIN_CLOSED, LOW);
    digitalWrite(PIN_OPEN, HIGH);
  }
}

void sleep() {
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  
  sleep_cpu();                            // sleep

  cli();                                  // Disable interrupts
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on

  sei();                                  // Enable interrupts
}

void setup() {
  pinMode(PIN_REED, INPUT);
  pinMode(PIN_CLOSED, OUTPUT);
  pinMode(PIN_OPEN, OUTPUT);
  
  GIMSK |= _BV(PCIE0);                    // Enable Pin Change Interrupts
  PCMSK0 |= _BV(PCINT0);                  // Use PA0 as interrupt pin
  
  handleInterrupt();
  
  delay(100);
}

void loop() {
  sleep();
  
  handleInterrupt();
}

ISR(PCINT0_vect) {
  
}
