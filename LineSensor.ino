//Motor test: 
#define MR_1 13
#define MR_2 12
#define MR_PWM 11


#define ML_1 8
#define ML_2 9
#define ML_PWM 10

#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4




void setup() {
 // put your setup code here, to run once:
 pinMode(MR_1, OUTPUT);
 pinMode(MR_2, OUTPUT);
 pinMode(ML_PWM, OUTPUT);
 pinMode(ML_1, OUTPUT);
 pinMode(ML_2, OUTPUT);
 pinMode(MR_PWM, OUTPUT);


 analogWrite(MR_PWM, 0);
 analogWrite(ML_PWM, 0);
 digitalWrite(MR_1, LOW);
 digitalWrite(MR_2, HIGH);
 digitalWrite(ML_1, LOW);
 digitalWrite(ML_2, HIGH);
}


void loop() {
  Serial.println(analogRead(IR2));
  analogWrite(MR_PWM, 140);
  analogWrite(ML_PWM, 140);
  if ((analogRead(IR1) < 400) & (analogRead(IR2) < 400) & (analogRead(IR3) < 400)){
    digitalWrite(MR_1, LOW);
    digitalWrite(MR_2, LOW);
    digitalWrite(ML_1, LOW);
    digitalWrite(ML_2, LOW);
    Serial.println("stopped");
  }
}