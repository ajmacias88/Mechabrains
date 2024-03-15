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

void loop() {
  forward();
}