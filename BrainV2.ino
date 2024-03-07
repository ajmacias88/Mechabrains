// #define USE_TIMER_2 true
// #include <TimerInterrupt.h>

const int frontIN = 3;
const int rightIN = 4;
const int leftIN = 5;
const int backIN = 6;

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
#define LINEDELAY 450  //ignore start box
#define DELAY_2 200     // drive to contact zone
#define DELAY_3 500     // drive to contact zone

#define DARKLINE 700


volatile byte state;
int count = 0;
int timer;
int type = 1;


/*---------------State Definitions--------------------------*/
typedef enum {
  ORIENTING_A,
  LINE_FINDING_A,
  STOP
} States_t;


void backward() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 210);
  analogWrite(ML_PWM, 210);
}


void forward() {
  digitalWrite(MR_1, LOW);
  digitalWrite(MR_2, HIGH);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 90);
  analogWrite(ML_PWM, 110);
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
  digitalWrite(ML_1, HIGH);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 190);
  analogWrite(ML_PWM, 190);
}


void right() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, HIGH);
  analogWrite(MR_PWM, 190);
  analogWrite(ML_PWM, 190);
}

void left1() {
  digitalWrite(MR_1, HIGH);
  digitalWrite(MR_2, LOW);
  digitalWrite(ML_1, LOW);
  digitalWrite(ML_2, LOW);
  analogWrite(MR_PWM, 190);
  analogWrite(ML_PWM, 190);
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

  pinMode(frontIN, INPUT);
  pinMode(rightIN, INPUT);
  pinMode(backIN, INPUT);
  pinMode(leftIN, INPUT);

  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
  digitalWrite(MR_1, 0);
  digitalWrite(MR_2, 1);
  digitalWrite(ML_1, 0);
  digitalWrite(ML_2, 1);

  state = ORIENTING_A;
  Serial.begin(9600);

  // ITimer2.init();
  // ITimer2.setFrequency(freq1, TimerHandler1);
}


void loop() {
  // put your main code here, to run repeatedly:
  switch (state) {
    case ORIENTING_A:
    left();
      if (digitalRead(rightIN) == HIGH & digitalRead(backIN) == HIGH) {
        timer = millis();
        while (millis() - timer < TIMER_30) {
          stop();
        }
        timer = millis();
        while (millis() - timer < TIMER_30) {
          left();
        }
        state = LINE_FINDING_A;
      }
        break;

    case LINE_FINDING_A:
      timer = millis();
      Serial.println("delay");
      while (millis() - timer < LINEDELAY){
        forward();
      }
      Serial.println("delay");
      if (analogRead(IR_1) < DARKLINE || analogRead(IR_2) < DARKLINE || analogRead(IR_3) < DARKLINE || analogRead(IR_4) < DARKLINE) {
        timer = millis();
        Serial.println("I SEEEEEEEN ITTTTT");
        while (millis() - timer < DELAY_2){
          backward();
        }
        state = STOP;
      }
      break;


    case STOP:
      stop();
      break;
  }
}