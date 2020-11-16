#include "TM1637.h"
#include <EEPROM.h>
#define CLK 3
#define DIO 2
#define OUTPUT1 5
#define OUTPUT2 1
#define BUTTON_UP 4
#define BUTTON_DOWN 0
#define BUTTON_DB_DELAY 150
#define OUTPUT_ON_TIME 150

TM1637 Display(CLK,DIO);

unsigned long time;
int interval;
int addr = 1;
bool button_up_flag = false;
bool button_down_flag = false;
unsigned long button_up_last_pressed = 0;
unsigned long button_down_last_pressed = 0;
unsigned char output_toggle = 0;
unsigned long loop_time = 0;
unsigned long output1_pressed_timer = 0;
unsigned long output2_pressed_timer = 0;

void setup() {
  Display.init();
  Display.set(BRIGHT_TYPICAL);

  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  delay(500);
  interval = EEPROM.read(addr);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
}

void loop() {
  get_button_down_press();
  get_button_up_press();
  update_screen();
  handle_outputs();
  handle_turning_off_outputs();
}

void handle_outputs() {
  unsigned long next_time = millis() % (interval*1000);
  if (loop_time > next_time) {
    if (output_toggle) {
      digitalWrite(OUTPUT1, LOW);
      output1_pressed_timer = millis();
    }
    else {
      digitalWrite(OUTPUT2, LOW);
      output2_pressed_timer = millis();
    }
    output_toggle = !output_toggle;
  }
  loop_time = next_time;
}

void handle_turning_off_outputs() {
  if (output1_pressed_timer < millis() - OUTPUT_ON_TIME) {
    digitalWrite(OUTPUT1, HIGH);
  }
  if (output2_pressed_timer < millis() - OUTPUT_ON_TIME) {
    digitalWrite(OUTPUT2, HIGH);
  }
}

void update_screen() {
  int tens_digit = interval/10;
  int singles_digit = interval - (tens_digit*10);
  Display.display(2, tens_digit);
  Display.display(3, singles_digit);
}

void get_button_up_press() {
  if (!digitalRead(BUTTON_UP) && button_up_last_pressed < millis() - BUTTON_DB_DELAY) {
    button_up_last_pressed = millis();
    if (interval < 60) {
      interval++;
      EEPROM.write(addr, interval);
    }
  }
}

void get_button_down_press() {
  if (!digitalRead(BUTTON_DOWN) && button_down_last_pressed < millis() - BUTTON_DB_DELAY) {
    button_down_last_pressed = millis();
    if (interval > 0) {
      interval--;
      EEPROM.write(addr, interval);
    }
  }
}
