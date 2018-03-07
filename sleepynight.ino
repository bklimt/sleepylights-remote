#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <IRremote.h>

#define LED_PIN 1
#define BUTTON_PIN 2

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

void step() {
  static int color = ORANGE;
  color = color + 1;
  if (color >= 14) {
    color = 0;
  }
  //irsend.sendNEC(COLORS[color], 32);
}

void wake() {
  digitalWrite(LED_PIN, HIGH);
}

void sleep() {
  digitalWrite(LED_PIN, LOW);
}

volatile unsigned long last_click = 0;
volatile unsigned long last_handled_click = 0;

void click() {
  last_click = millis();
  if (last_click - last_handled_click < 200) {
    return;
  }
  last_handled_click = last_click;
  // Serial.write("click!\n");
  wake();
  step();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  irsend.sendNEC(DARK_BLUE, 32);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), click, FALLING);

  // Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  if (now - last_click > 1000) {
    sleep();
  }
  delay(500);
}
