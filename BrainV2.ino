#define USE_TIMER_2 true
#include <TimerInterrupt.h>

const int leftUSTrig = 6;
const int leftUSEcho = 7;

#define rightUSTrig A1
#define rightUSEcho A0

const int frontUSTrig = 2;
#define frontUSEcho A4

const int backUSTrig = 4;
const int backUSEcho = 5;

int type = 1;
int freq1 = 100;
int stateU;
int celebrate = 0;

float durationF, distanceF, durationR, distanceR, durationL, distanceL, durationB, distanceB;

#define LS A5


#define ARM A2
#define SLIDE A3


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
#define LINEDELAY 1200  //ignore start box
#define DELAY_2 200     // drive to contact zone
#define DELAY_3 500     // drive to contact zone

#define DARKLINE 700


volatile byte state;
int count = 0;
int timer;


/*---------------State Definitions--------------------------*/
typedef enum {
  ORIENTING_A,
  LINE_FINDING_A,
  STOP,
  CONTACT_ZONE,
  SHOOTING,
  CELEBRATION,
  MIDDLE,
  LAST
} States_t;


void backward() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 255);
}

void backward1() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 0);
}

void backward2() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 255);
}

void uturn() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 95);
}

void uturn1() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 80);
  analogWrite(ML_PWM, 255);
}


void forward() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 160);
  analogWrite(ML_PWM, 190);
}

void forward2() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 187);
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
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 200);
  analogWrite(ML_PWM, 200);
}

void left1() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 150);
  analogWrite(ML_PWM, 150);
}


void right() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 224);
  analogWrite(ML_PWM, 220);
}

void right1() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 150);
  analogWrite(ML_PWM, 150);
}

void TimerHandler1() {
  if (count == 0) {
    stateU = HIGH;
    digitalWrite(frontUSTrig, stateU);
    digitalWrite(rightUSTrig, stateU);
    digitalWrite(leftUSTrig, stateU);
    digitalWrite(backUSTrig, stateU);
  } else {
    stateU = LOW;
    digitalWrite(frontUSTrig, stateU);
    digitalWrite(rightUSTrig, stateU);
    digitalWrite(leftUSTrig, stateU);
    digitalWrite(backUSTrig, stateU);
  }
  count++;
  if (count == 2) {
    if (type == 1) {
      durationF = pulseIn(frontUSEcho, HIGH);
      distanceF = (durationF * .0343) / 2;
    } else if (type == 2) {
      durationL = pulseIn(leftUSEcho, HIGH);
      distanceL = (durationL * .0343) / 2;
    } else if (type == 3) {
      durationR = pulseIn(rightUSEcho, HIGH);
      distanceR = (durationR * .0343) / 2;
    }
     else if (type == 4) {
      durationB = pulseIn(backUSEcho, HIGH);
      distanceB = (durationB * .0343) / 2;
      type = 0;
    }
    type++;

    count = 0;
  }
}

void setup() {
  // put your setup code here, to run once
  pinMode(MR_1, OUTPUT);
  pinMode(MR_2, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(ML_1, OUTPUT);
  pinMode(ML_2, OUTPUT);
  pinMode(MR_PWM, OUTPUT);

  pinMode(leftUSTrig, OUTPUT);
  pinMode(leftUSEcho, INPUT);

  pinMode(rightUSTrig, OUTPUT);
  pinMode(rightUSEcho, INPUT);

  pinMode(frontUSTrig, OUTPUT);
  pinMode(frontUSEcho, INPUT);

  pinMode(backUSTrig, OUTPUT);
  pinMode(backUSEcho, INPUT);

  pinMode(ARM, OUTPUT);
  pinMode(SLIDE, OUTPUT);
  pinMode(LS, INPUT);

  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
  digitalWrite(MR_1, 0);
  digitalWrite(MR_2, 1);
  digitalWrite(ML_1, 0);
  digitalWrite(ML_2, 1);

  digitalWrite(SLIDE, LOW);


  state = ORIENTING_A;
  Serial.begin(9600);
  stateU = LOW;

  ITimer2.init();
  ITimer2.setFrequency(freq1, TimerHandler1);
}


void loop() {
  // put your main code here, to run repeatedly:
  switch (state) {
    case ORIENTING_A: 
      left();
      if (distanceR < 10 & distanceB < 10 & distanceR != 0 & distanceB != 0) {
        timer = millis();
        while (millis() - timer < TIMER_30) {
          stop();
        }
        while (distanceB > 5) {
          backward();
          Serial.println(distanceB);
        }
        timer = millis();
        while (millis() - timer < 800) {
          backward1();
        }
        timer = millis();
        while (millis() - timer < 800) {
          backward2();
        }
        // timer = millis();
        // while (millis() - timer < 300) {
        //   backward1();
        // }
        Serial.println("backward");
        Serial.println(distanceF);
        while (distanceF > 25 || distanceF == 0) {
          forward();
          //TimerHandler1();
          Serial.println(distanceF);
        }
  
        Serial.println(distanceF);
        timer = millis();
        while (millis() - timer < 200) {
          stop();
        }
        timer = millis();
        while (millis() - timer < 2200) {
          uturn();
        }
      state = CONTACT_ZONE;    
      }
    break;
    
  case CONTACT_ZONE:
    timer = millis();
    while (millis() - timer < 300) {
      right();
    }

    while (distanceF > 30 ) {
      forward();
    }
    timer = millis();
    while (millis() - timer < 600) {
      stop();
    }
    timer = millis();
    while (millis() - timer < 600) {
      right1();
    }
    timer = millis();
    while (millis() - timer < 600) {
     stop();
    }
    state = MIDDLE;
    break;
  
  case MIDDLE:
    while (distanceF > 20 ) {
      forward();
    }
    stop();
    timer = millis();
    while (millis() - timer < 600) {
      left();
    }
    state = LAST;
    break;
  
  case LAST:
    while (distanceF > 15 ) {
      forward();
    }
    timer = millis();
    while (millis() - timer < 600) {
      forward();
    }
    while (millis() - timer < 2000) {
      stop();
    }

    state = CELEBRATION;
    break;


  case CELEBRATION:
    digitalWrite(SLIDE, HIGH);
    timer = millis();
    while (millis() - timer < 100) {
      digitalWrite(ARM, HIGH);
    }
    timer = millis();
    while (millis() - timer < 100) {
      digitalWrite(ARM, LOW);
    }
    celebrate++;
    if (celebrate == 10) {
      state = STOP;
    }
    break;

  case STOP:
    stop();
    break;
  }
}