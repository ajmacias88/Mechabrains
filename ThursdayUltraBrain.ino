
#include <Servo.h>



#define ARM_IN 4
#define SLIDE_IN  5
#define ARM_OUT 10
#define SLIDE_OUT 11

int count = 0;
int type = 1;
int freq1 = 100;
int wallDistance = 30;
int state;

float durationF, distanceF, durationR, distanceR, durationL, distanceL, durationB, distanceB;

Servo arm;

Servo slide;

void setup() {

  pinMode(ARM_IN, INPUT);
  pinMode(SLIDE_IN, INPUT);

  arm.attach(10);
  slide.attach(11);

  Serial.begin(9600);
  digitalWrite(ARM_IN, HIGH);
}

void loop() {


  if (digitalRead(ARM_IN) == HIGH) {
    arm.write(110);
  }
  if (digitalRead(ARM_IN) == 0) {
    arm.write(0);
  }
  if (digitalRead(SLIDE_IN) == HIGH) {
    slide.write(110);
  }
  if (digitalRead(SLIDE_IN) == LOW) {
    slide.write(0);
  }
}