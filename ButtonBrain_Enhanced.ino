#define PIN_1             2
#define PIN_2             3
#define PIN_3             4
#define PIN_4             5
#define PIN_5             6
#define PIN_6             7
#define PIN_7             8
#define PIN_8             9
#define PIN_9             10
#define PIN_10            11
#define PIN_11            12
#define PIN_12            A1
#define PIN_OUT           A5

int count = 0;
long timer = 0;

void setup() {
  pinMode(PIN_OUT, OUTPUT);
  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, INPUT);
  pinMode(PIN_4, INPUT);
  pinMode(PIN_5, INPUT);
  pinMode(PIN_6, INPUT);
  pinMode(PIN_7, INPUT);
  pinMode(PIN_8, INPUT);
  pinMode(PIN_9, INPUT);
  pinMode(PIN_10, INPUT);
  pinMode(PIN_11, INPUT);
  pinMode(PIN_12, INPUT);

  Serial.begin(9600);
}

void loop() {
  for (int i = 1; i <= 12; i++) {
    if (digitalRead(getPin(i)) == HIGH) {
      count++;
      timer = millis();
      while (millis() - timer < 2000) {
        if (digitalRead(getPin(i)) && (millis() - timer >= 200)) {
          count++;
        }
      }
      if (count > 1) {
        analogWrite(PIN_OUT, 2);
        Serial.print("Pin ");
        Serial.print(i);
        Serial.println(": 2");
      } else {
        analogWrite(PIN_OUT, 1);
        Serial.print("Pin ");
        Serial.print(i);
        Serial.println(": 1");
      }
      count = 0;
    }
  }
}

int getPin(int pinNumber) {
  switch (pinNumber) {
    case 1: return PIN_1;
    case 2: return PIN_2;
    case 3: return PIN_3;
    case 4: return PIN_4;
    case 5: return PIN_5;
    case 6: return PIN_6;
    case 7: return PIN_7;
    case 8: return PIN_8;
    case 9: return PIN_9;
    case 10: return PIN_10;
    case 11: return PIN_11;
    case 12: return PIN_12;
    default: return -1;
  }
}
