#include <Servo.h>

#define SERVO_PIN 10
#define CENTER_ANGLE 48
#define LEFT_ANGLE 10
#define RIGHT_ANGLE 95

Servo steeringServo;

void setup() {
  Serial.begin(9600);
  steeringServo.attach(SERVO_PIN);
  Serial.println("Servo Test Started");
}

void loop() {
  Serial.println("Servo → LEFT");
  steeringServo.write(LEFT_ANGLE);
  delay(1000);

  Serial.println("Servo → CENTER");
  steeringServo.write(CENTER_ANGLE);
  delay(1000);

  Serial.println("Servo → RIGHT");
  steeringServo.write(RIGHT_ANGLE);
  delay(1000);
}
