#include <Servo.h>
#include <Pixy2.h>

Pixy2 pixy;

// --- Servo Pins and Angles ---
#define SERVO_PIN 10
#define CENTER_ANGLE 48
#define LEFT_ANGLE 10
#define RIGHT_ANGLE 100

// --- Motor Driver Pins (TB6612) ---
#define PWM_PIN 3
#define AIN1_PIN 5
#define AIN2_PIN 7
#define STBY_PIN 9

#define MOTOR_SPEED 120   // Adjust speed for track

Servo steeringServo;

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Setup Started (Pixy Only)");

  // Servo setup
  steeringServo.attach(SERVO_PIN);
  steeringServo.write(CENTER_ANGLE);

  // Motor pins setup
  pinMode(PWM_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);

  // Enable motor driver
  digitalWrite(STBY_PIN, HIGH);
  driveForward(MOTOR_SPEED);

  // Init Pixy
  int8_t res = pixy.init();
  if (res < 0) {
    Serial.print("Pixy init failed: ");
    Serial.println(res);
    while (1);
  }
  Serial.println("Pixy init OK");
}

void driveForward(int speed) {
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWM_PIN, speed);
}

void stopMotor() {
  analogWrite(PWM_PIN, 0);
  digitalWrite(STBY_PIN, LOW);
}

void loop() {
  // Get blocks from Pixy
  int blocks = pixy.ccc.getBlocks();

  if (blocks) {
    int largestArea = 0;
    int chosenSignature = -1;

    // Pick the largest block (ignore tiny red floor lines)
    for (int i = 0; i < blocks; i++) {
      int area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
      if (area < 200) continue;  // filter out noise / thin lines

      if (area > largestArea) {
        largestArea = area;
        chosenSignature = pixy.ccc.blocks[i].m_signature;
      }
    }

    // Steering decision
    if (chosenSignature == 1) { // Green block → steer LEFT
      Serial.println("Pixy: Green detected → Steering LEFT");
      steeringServo.write(LEFT_ANGLE);
      driveForward(MOTOR_SPEED);

    } else if (chosenSignature == 2) { // Red block → steer RIGHT
      Serial.println("Pixy: Red detected → Steering RIGHT");
      steeringServo.write(RIGHT_ANGLE);
      driveForward(MOTOR_SPEED);

    } else {
      Serial.println("No valid block → Go Straight");
      steeringServo.write(CENTER_ANGLE);
      driveForward(MOTOR_SPEED);
    }

  } else {
    // No blocks detected → straight by default
    Serial.println("No blocks → Straight");
    steeringServo.write(CENTER_ANGLE);
    driveForward(MOTOR_SPEED);
  }

  delay(50);
}
