#include <avr/sleep.h>
#include <IRremote.h>

#ifndef __AVR_ATtiny85__
#error "This program only works with an ATtiny85."
#endif

// The IR LED should be on pin 1.

#define BUTTON_PIN 2

// Remote constants from https://github.com/nothans/sleepylights-ir-codes

unsigned long POWER       = 0x00FF609F;
unsigned long SLEEP       = 0x00FF50AF;
unsigned long TIMER       = 0x00FF7887;
unsigned long RAINBOW     = 0x00FF708F;
unsigned long LAMP        = 0x00FF58A7;

unsigned long ORANGE      = 0x00FF00FF;
unsigned long YELLOW      = 0x00FF02FD;
unsigned long PURPLE      = 0x00FF20DF;
unsigned long CYAN        = 0x00FF32CD;
unsigned long LIGHT_BLUE  = 0x00FF48B7;
unsigned long LAVENDER    = 0x00FF6897;
unsigned long PINK        = 0x00FF8877;
unsigned long WHITE       = 0x00FF906F;
unsigned long DARK_GREEN  = 0x00FF9867;
unsigned long RED         = 0x00FFA857;
unsigned long LIGHT_RED   = 0x00FFB24D;
unsigned long DARK_BLUE   = 0x00FFD827;
unsigned long LIGHT_GREEN = 0x00FFE817;

unsigned long COLORS[14] = {
  ORANGE, YELLOW, PURPLE, CYAN, LIGHT_BLUE, LAVENDER, PINK, WHITE, DARK_GREEN,
  RED, LIGHT_RED, DARK_BLUE, LIGHT_GREEN, RAINBOW
};

IRsend irsend;

int color = ORANGE;

void step() {
  color = color + 1;
  if (color >= 14) {
    color = 0;
  }
  irsend.sendNEC(COLORS[color], 32);
}

volatile bool was_clicked = false;

ISR(PCINT0_vect) {
  was_clicked = (digitalRead(BUTTON_PIN) != HIGH);
}

void setup() {
  // Set a calibration value based on TinyTuner output.
  OSCCAL = 0xA4;
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Enable interrupts for the button pin.
  GIMSK = 0b00100000;
  PCMSK = 1 << BUTTON_PIN;
  sei();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // Disable the ADC, since we aren't using it.
  ADCSRA &= ~(1 << ADEN);
}

void loop() {
  if (was_clicked) {
    was_clicked = false;
    step();
  }
  sleep_mode();
}
