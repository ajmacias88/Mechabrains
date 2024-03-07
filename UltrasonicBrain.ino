#define USE_TIMER_2 true
#include <TimerInterrupt.h>

const int leftUSTrig = 4;
const int leftUSEcho = 5;
const int leftOUT = 12;

const int rightUSTrig = 8;
const int rightUSEcho = 9;
const int rightOUT = 11;

const int frontUSTrig = 2;
const int frontUSEcho = 3;
const int frontOUT = 10;

const int backUSTrig = 6;
const int backUSEcho = 7;
const int backOUT = 13;

int count = 0;
int type = 4;
int freq1 = 100;
int wallDistance = 30;
int state;

float durationF, distanceF, durationR, distanceR, durationL, distanceL, durationB, distanceB;

void TimerHandler1() {
  if (count == 0) {
    state = HIGH;
    digitalWrite(frontUSTrig, state);
    digitalWrite(rightUSTrig, state);
    digitalWrite(leftUSTrig, state);
    digitalWrite(backUSTrig, state);
  } else {
    state = LOW;
    digitalWrite(frontUSTrig, state);
    digitalWrite(rightUSTrig, state);
    digitalWrite(leftUSTrig, state);
    digitalWrite(backUSTrig, state);
  }
  count++;
  if (count == 2) {
    if (type == 1) {
      durationF = pulseIn(frontUSEcho, HIGH);
      distanceF = (durationF * .0343) / 2;
      Serial.println(distanceF);
      if (distanceF < wallDistance) {
        digitalWrite(frontOUT, HIGH);
      }
    } else if (type == 2) {
      durationL = pulseIn(leftUSEcho, HIGH);
      distanceL = (durationL * .0343) / 2;
      Serial.println(distanceL);
      if (distanceL < wallDistance){
        digitalWrite(leftOUT, HIGH);
      }
    } else if (type == 3) {
      durationR = pulseIn(rightUSEcho, HIGH);
      distanceR = (durationR * .0343) / 2;
      Serial.println(distanceR);
      if (distanceR < wallDistance){
        digitalWrite(rightOUT, HIGH);
      }
    }
     else if (type == 4) {
      durationB = pulseIn(backUSEcho, HIGH);
      distanceB = (durationB * .0343) / 2;
      Serial.println(distanceB);
      if (distanceB < wallDistance){
        digitalWrite(backOUT, HIGH);
      }
    }

    count = 0;
  }
}

void setup() {
  pinMode(leftUSTrig, OUTPUT);
  pinMode(leftUSEcho, INPUT);

  pinMode(rightUSTrig, OUTPUT);
  pinMode(rightUSEcho, INPUT);

  pinMode(frontUSTrig, OUTPUT);
  pinMode(frontUSEcho, INPUT);

  pinMode(backUSTrig, OUTPUT);
  pinMode(backUSEcho, INPUT);

  pinMode(leftOUT, OUTPUT);
  pinMode(rightOUT, OUTPUT);
  pinMode(frontOUT, OUTPUT);
  pinMode(backOUT, OUTPUT);

  ITimer2.init();
  ITimer2.setFrequency(freq1, TimerHandler1);

  Serial.begin(9600);
}

void loop() {
}