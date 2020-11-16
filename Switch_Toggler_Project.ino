#include "TM1637.h"
#define CLK 3
#define DIO 2
#define OUTPUT1 6
#define OUTPUT2 7
#define LED1 10
#define LED2 11
#define POT_INPUT A0
#define DELAY_INTERVAL 100

TM1637 Display(CLK,DIO);

int pot_value = 0;
double loop_count = 0;
bool which_output = false;

void setup() {
  Serial.begin(115200);
  Display.init();
  Display.set(BRIGHT_TYPICAL);

  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  pot_value = analogRead(POT_INPUT);
  int desired_time = pot_value/17;

  if ((double(DELAY_INTERVAL)/1000)*loop_count > desired_time) {
    if (!which_output) {
      digitalWrite(OUTPUT1, HIGH);
      digitalWrite(LED1, HIGH);
      which_output = true;
      Serial.println("OUTPUT 1");
      delay(200);
      digitalWrite(OUTPUT1, LOW);
      digitalWrite(LED1, LOW);
    }
    else {
      digitalWrite(OUTPUT2, HIGH);
      digitalWrite(LED2, HIGH);
      which_output = false;
      Serial.println("OUTPUT 2");
      delay(200);
      digitalWrite(OUTPUT2, LOW);
      digitalWrite(LED2, LOW);
    }
    loop_count = 0;
  }
  loop_count += 1;
  int tens_digit = desired_time/10;
  int singles_digit = desired_time - (tens_digit*10);
  Display.display(2, tens_digit);
  Display.display(3, singles_digit);
  Serial.println(desired_time);
  delay(DELAY_INTERVAL);
}
