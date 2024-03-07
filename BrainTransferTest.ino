const int frontIN = 3;
const int rightIN = 4;
const int leftIN = 5;
const int backIN = 6;

void setup() {
  pinMode(frontIN, INPUT);
  pinMode(rightIN, INPUT);
  pinMode(backIN, INPUT);
  pinMode(leftIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(frontIN) == HIGH) {
    Serial.println("Front");
  } 
  if (digitalRead(leftIN) == HIGH) {
    Serial.println("Left");
  } 
  if (digitalRead(rightIN) == HIGH) {
    Serial.println("Right");
  } 
  if (digitalRead(backIN) == HIGH) {
    Serial.println("Back");
  } 
}
