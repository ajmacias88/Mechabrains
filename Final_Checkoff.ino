#include <Servo.h>

#define USE_TIMER_2 true
#include <TimerInterrupt.h>

#define leftUSTrig A4
#define leftUSEcho 8

#define rightUSTrig A1
#define rightUSEcho 12

#define frontRUSTrig A2
#define frontRUSEcho A5

#define frontLUSTrig A3
#define frontLUSEcho 13

#define backUSTrig A0
#define backUSEcho 11

#define IR A5

int error = 2;

float durationFR, distanceFR, durationFL, distanceFL, durationR, distanceR, durationL, distanceL, durationB, distanceB;
#define MR_1 3
#define MR_2 4
#define MR_PWM 5

#define ML_1 7
#define ML_2 2
#define ML_PWM 6

int count = 0;
int timer;
int timer2;
int freq1 = 100;
int stateU = LOW;
int type = 0;

int servoPin = 10;

// Create a servo object
Servo myServo;

// Define the pin connected to the servo


volatile byte state;

/*---------------State Definitions--------------------------*/
typedef enum {
  ORIENTING,
  FORWARD1,
  LEFT1,
  EXITING1,
  ENTERING2,
  LEFT2,
  FORWARD2,
  CONTACTZONE,
  POSTCONTACT,
  FULLAHEAD,
  PREDART,
  DART,
  CELEB,
  STOP
} States_t;

void TimerHandler1() {
  if (count == 0) {
    stateU = HIGH;
    digitalWrite(frontRUSTrig, stateU);
    digitalWrite(frontLUSTrig, stateU);
    digitalWrite(rightUSTrig, stateU);
    digitalWrite(leftUSTrig, stateU);
    digitalWrite(backUSTrig, stateU);
  } else {
    stateU = LOW;
    digitalWrite(frontRUSTrig, stateU);
    digitalWrite(frontLUSTrig, stateU);
    digitalWrite(rightUSTrig, stateU);
    digitalWrite(leftUSTrig, stateU);
    digitalWrite(backUSTrig, stateU);
  }
  count++;
  if (count == 2) {
    if (type == 1) {
      durationFR = pulseIn(frontRUSEcho, HIGH);
      distanceFR = (durationFR * .0343) / 2;
      
    } else if (type == 2) {
      durationFL = pulseIn(frontLUSEcho, HIGH);
      distanceFL = (durationFL * .0343) / 2;

    }  else if (type == 3) {
      durationL = pulseIn(leftUSEcho, HIGH);
      distanceL = (durationL * .0343) / 2;

    } else if (type == 4) {
      durationR = pulseIn(rightUSEcho, HIGH);
      distanceR = (durationR * .0343) / 2;

    }
     else if (type == 5) {
      durationB = pulseIn(backUSEcho, HIGH);
      distanceB = (durationB * .0343) / 2;
      type = 0;
    }
    type++;

    count = 0;
  }
}

void setup() {
  myServo.attach(servoPin);
  myServo.write(65);
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

  pinMode(frontRUSTrig, OUTPUT);
  pinMode(frontRUSEcho, INPUT);

  pinMode(frontLUSTrig, OUTPUT);
  pinMode(frontLUSEcho, INPUT);

  pinMode(backUSTrig, OUTPUT);
  pinMode(backUSEcho, INPUT);

  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
  digitalWrite(MR_1, 0);
  digitalWrite(MR_2, 1);
  digitalWrite(ML_1, 0);
  digitalWrite(ML_2, 1);


  Serial.begin(9600);
  stateU = LOW;
  state = ORIENTING;

  ITimer2.init();
  ITimer2.setFrequency(freq1, TimerHandler1);
}

void forward() {
  if ((abs(distanceFR - distanceFL) <= error) && (distanceFR!= 0) && (distanceFL != 0)) {
    Sforward();
  }
  if ((distanceFR - distanceFL > error) && (distanceFR!= 0) && (distanceFL != 0)) {
    Lforward();

  }
  if ((distanceFR - distanceFL < (-error)) && (distanceFR!= 0) && (distanceFL != 0)){
    Rforward();
}
}

void run(void){
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 230);
  analogWrite(ML_PWM, 215);
}

void backUp(void) {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 230);
  analogWrite(ML_PWM, 215);
}

void stop(void){
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
}

void left(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 240);
}


void right(void) {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 255);
}

void circleLeft(void) {
  timer = millis();
  while (millis() - timer < 70){
    right();
  }
  timer = millis();
  while (millis() - timer < 300){
    stop();
  }
}

void circleRight(void) {
  timer = millis();
  while (millis() - timer < 70){
    left();
  }
  timer = millis();
  while (millis() - timer < 300){
    stop();
  }
}


void Sforward(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 200);
  analogWrite(ML_PWM, 200);
}

void Lforward(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 200);
  analogWrite(ML_PWM, 0);
}

void Rforward(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 200);
}

void circleBackRight(void) {
  timer = millis();
  while (millis() - timer < 70){
    backupRight();
  }
  timer = millis();
  while (millis() - timer < 300){
    stop();
  }
}

void circleBackLeft(void) {
  timer = millis();
  while (millis() - timer < 70){
    backupLeft();
  }
  timer = millis();
  while (millis() - timer < 300){
    stop();
  }
}

void backupRight(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 255);
  analogWrite(ML_PWM, 0);
}

void backupLeft(void) {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 255);
}

void finalTurn(void) {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 50);
  analogWrite(ML_PWM, 255);
}

void loop() {
  switch (state) {
    case ORIENTING: 
      circleLeft();
      if ((distanceB < 25) && (distanceR < 25) && (distanceFR < 75) && (abs(distanceFR - distanceFL) <= 2)) {
        state = FORWARD1;
      }
      break;

    case FORWARD1:
      forward();
      if (distanceFR < 25 && distanceFR != 0) {
        state = LEFT1;
      }
      break;

    case LEFT1:
      circleLeft();
      if ((distanceB < 25) && (distanceR < 25) && (distanceFR > 80) && (abs(distanceFR - distanceFL) <= 25)) {
        state = FORWARD2;
      }
      break;

    case FORWARD2:
      forward();
      if (distanceFL < 56)  {
        state = EXITING1;
      }
      break;

  case EXITING1:
    circleRight();
    if ((abs(distanceFR - distanceFL) <= 12) && (distanceFL > 70)) {
      state = ENTERING2;
    }
    break;
  
  case ENTERING2:
    timer = millis();
    while (millis() - timer < 1900) {
      error = 12;
      forward();
      }

    while (analogRead(IR) > 900) {
      forward();
    }
    state = LEFT2;
    break;

  case LEFT2:
    timer = millis();
    while (millis() - timer < 100) {
      stop();
    }
    circleLeft();
    if ((abs(distanceFR - distanceFL) <= 4) && (distanceFR < 70)) {
      state = CONTACTZONE;
    }
    break;
  
  case CONTACTZONE:
    error = 2;
    forward();
    if (distanceFR < 10) {
      state = POSTCONTACT;
      }
    
    break;

  case POSTCONTACT:
    circleBackRight();
    if ((abs(distanceFR - distanceFL) <= 10) && (distanceFL > 100) && (distanceFL < distanceFR)) {
      state = FULLAHEAD;
    }
    break;


  case FULLAHEAD:
    error = 1;
    forward();
    if (distanceFR < 11) {
      state = PREDART;
      }
    
    break;

  case PREDART:
    timer2 = millis();
    while (millis() - timer2 < 700) {
      finalTurn();
      }
    state = DART;
    break;

  case DART:
    timer2 = millis();
    while (millis() - timer2 < 1500) {
      run();
    } 
    myServo.write(150);
    state = CELEB;
    break;
  
  case CELEB:
    count = 0;
    while (count < 10){
    timer = millis();
    while (millis() - timer < 500) {
      myServo.write(180);
    } 
    timer2 = millis();
    while (millis() - timer2 < 500) {
      myServo.write(260);
    }
    count ++;
    }
    state = STOP;
    break;    
  case STOP:
    stop();
    break;
  }
}