const unsigned int IN1 = 25;
const unsigned int IN2 = 26;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  delay(3000);
  Serial.println("Test");
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  Serial.println("Forward 3s");
  delay(3000);

  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.println("Stopped 3s");
  delay(3000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  Serial.println("Backward 3s");
  delay(3000);

  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.println("Stopped 3s");
  delay(3000);
}
