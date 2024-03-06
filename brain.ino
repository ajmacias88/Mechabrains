#define USE_TIMER_2 true
#include <TimerInterrupt.h>

#define INPUT_PIN A0

#define IR_1 A1
#define IR_2 A2
#define IR_3 A3
#define IR_4 A4


#define MR_1 13
#define MR_2 12
#define MR_PWM 9


#define ML_1 8
#define ML_2 11
#define ML_PWM 10


#define TIMER_30 500    //30
#define TIMER_60 400   //60
#define TIMER_90 500    //90
#define TIMER_120 500   //120
#define TIMER_150 500   //150
#define TIMER_180 500   //180
#define LINEDELAY 600  //ignore start box
#define DELAY_2 100     // drive to contact zone
#define DELAY_3 1000     // drive to contact zone




volatile byte state;
int count = 0;
int timer;
int type = 1;
const int trigPinF = 3;
const int echoPinF = 2;
const int trigPinR = 5;
const int echoPinR = 4;
const int trigPinL = 7;
const int echoPinL = 6;
const int freq1 = 100;
int countU = 0;

float durationF, distanceF = 500, durationL, distanceL, durationR, distanceR;
int stateU;


/*---------------State Definitions--------------------------*/
typedef enum {
  ORIENTING_A,
  ORIENTING_B,
  LINE_FINDING_A,
  LINE_FINDING_B,
  LINE_FOLLOWING_A,
  LINE_FOLLOWING_B,
  CONTACT_ZONE_A,
  CONTACT_ZONE_B,
  SHOOTING_A,
  SHOOTING_B,
  CELEBRATION
} States_t;


void b_run() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 250);
}


void f_run() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 210);
  analogWrite(ML_PWM, 210);
}


void stop() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
}


void left() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 200);
  analogWrite(ML_PWM, 200);
}


void right() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 200);
  analogWrite(ML_PWM, 200);
}

void left1() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 230);
  analogWrite(ML_PWM, 230);
}


void right1() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 230);
  analogWrite(ML_PWM, 230);
}

void TimerHandler1() {
  if (count == 0) {
    state = HIGH;
    digitalWrite(trigPinF, state);
    digitalWrite(trigPinL, state);
    digitalWrite(trigPinR, state);
  } else {
    state = LOW;
    digitalWrite(trigPinF, state);
    digitalWrite(trigPinL, state);
    digitalWrite(trigPinR, state);
  }
  count++;
  if (count == 2) {
    if (type == 1) {
      durationF = pulseIn(echoPinF, HIGH);
      distanceF = (durationF * .0343) / 2;
      Serial.println(distanceF);
    } else if (type == 2) {
      durationL = pulseIn(echoPinL, HIGH);
      distanceL = (durationL * .0343) / 2;
      Serial.println(distanceL);
    } else if (type == 3) {
      durationR = pulseIn(echoPinR, HIGH);
      distanceR = (durationR * .0343) / 2;
      Serial.println(distanceR);
    }

    count = 0;
  }
}

void setup() {
  // put your setup code here, to run once
  pinMode(INPUT_PIN, INPUT);
  pinMode(MR_1, OUTPUT);
  pinMode(MR_2, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(ML_1, OUTPUT);
  pinMode(ML_2, OUTPUT);
  pinMode(MR_PWM, OUTPUT);

  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);

  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
  digitalWrite(MR_1, 0);
  digitalWrite(MR_2, 1);
  digitalWrite(ML_1, 0);
  digitalWrite(ML_2, 1);

  state = ORIENTING_A;
  Serial.begin(9600);

  stateU = LOW;
  ITimer2.init();
  ITimer2.setFrequency(freq1, TimerHandler1);
  digitalWrite(trigPinF, stateU);
  digitalWrite(trigPinL, stateU);
  digitalWrite(trigPinR, stateU);
}


void loop() {
  // put your main code here, to run repeatedly:
  switch (state) {
    case ORIENTING_A:
      Serial.println(distanceF);
      if (distanceF < 35) {
        timer = millis();
        while (millis() - timer < DELAY_3) {
          left1();
          Serial.println("delay");
        }
        timer = millis();
        while (millis() - timer < 2000) {
          right1();
          Serial.println("delay");
        }
      }
      while ((distanceF > 35) & (distanceF != 0)) {
        right1();
        Serial.println("left o");
        Serial.println(distanceF);
      }

      stop();
      timer = millis();
      while (millis() - timer < TIMER_60) {
        left1();
        Serial.println("right o");
        Serial.println(distanceF);
      }
      stop();
      state = LINE_FINDING_A;
      break;


    case LINE_FINDING_A:
      timer = millis();
      while (millis() - timer < LINEDELAY) {
        f_run();
        Serial.println("running");
        Serial.println(analogRead(IR_2));
      }
      while (analogRead(IR_2) > 500) {
        f_run();
        Serial.println(analogRead(IR_2));
      }
      timer = millis();
      while (millis() - timer < DELAY_2) {
        b_run();
        Serial.println("stop");
      }
      while (analogRead(IR_4) > 600) {
        left();
        Serial.println("left");
        Serial.println(analogRead(IR_4));
      }
      Serial.println("check");
      state = LINE_FOLLOWING_A;
      break;


    case LINE_FINDING_B:
      timer = millis();
      while (millis() - timer >= LINEDELAY) {
        f_run();
      }
      if (analogRead(IR_2) < 400) {
        stop();
      }
      while (analogRead(IR_4) > 400) {
        left();
      }
      stop();
      state = LINE_FOLLOWING_B;


    case LINE_FOLLOWING_A:
      Serial.println("check 2");
      stop();
      Serial.println("LINEA");
      if (analogRead(IR_1 < 500) & analogRead(IR_2 < 500) & (analogRead(IR_3) < 500)) {
        state = CELEBRATION;
      }
      break;
      //  while (analogRead(IR_1) < 400 || analogRead(IR_2) < 400 || analogRead(IR_3) < 400) {
      //    f_run();
      //  }
      //  state = CONTACT_ZONE_A;


    case LINE_FOLLOWING_B:
      while (analogRead(IR_1) > 400 || analogRead(IR_2) > 400 || analogRead(IR_3) > 400) {
        f_run();
      }
      state = CONTACT_ZONE_A;
      state = CONTACT_ZONE_B;


    case CONTACT_ZONE_A:
      if (analogRead(IR_1) < 400) {
        timer = millis();
        while (millis() - timer >= TIMER_90) {
          left();
        }
        timer = millis();
        while (millis() - timer >= DELAY_2) {
          f_run();
        }
        //analogWrite(S_PWM, 255);
        timer = millis();
        while (millis() - timer >= TIMER_90) {
          right();
        }
      }


      state = SHOOTING_A;




    case CONTACT_ZONE_B:
      if (analogRead(IR_2) < 400) {
        timer = millis();
        while (millis() - timer >= TIMER_90) {
          right();
        }
        timer = millis();
        while (millis() - timer >= DELAY_2) {
          f_run();
        }
        stop();
        //analogWrite(S_PWM, 255);
        timer = millis();
        while (millis() - timer >= TIMER_90) {
          right();
        }
      }
      state = SHOOTING_B;




    case SHOOTING_A:
      timer = millis();
      while (millis() - timer >= DELAY_3) {
        f_run();
      }
      stop();
      //analogWrite(S2_PWM, 150);
      state = CELEBRATION;




    case SHOOTING_B:
      timer = millis();
      while (millis() - timer >= DELAY_3) {
        b_run();
      }
      stop();
      //analogWrite(S2_PWM, 150);
      state = CELEBRATION;




    case CELEBRATION:
      stop();
      Serial.println("nice");
      state = CELEBRATION;
      break;

    default:
      stop();
      Serial.println("nice");
      break;
  }
}