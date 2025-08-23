#define PWM_PIN 3
#define AIN1_PIN 5
#define AIN2_PIN 7
#define STBY_PIN 9

#define MOTOR_SPEED 120

void setup() {
  Serial.begin(9600);

  pinMode(PWM_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);

  digitalWrite(STBY_PIN, HIGH);
  Serial.println("Motor Driver Test Started");
}

void loop() {
  // Forward
  Serial.println("Motor Forward");
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWM_PIN, MOTOR_SPEED);
  delay(1000);

  // Reverse
  Serial.println("Motor Reverse");
  digitalWrite(AIN1_PIN, HIGH);
  digitalWrite(AIN2_PIN, LOW);
  analogWrite(PWM_PIN, MOTOR_SPEED);
  delay(1000);

  // Stop
  Serial.println("Motor Stop");
  analogWrite(PWM_PIN, 0);
  digitalWrite(STBY_PIN, LOW);
  delay(1000);

  // Enable again for next loop
  digitalWrite(STBY_PIN, HIGH);
}
