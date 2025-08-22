#include <Wire.h>
#include <VL53L0X.h>   // ToF sensor library
#include <Servo.h>
#include <Pixy2.h>     // Pixy2 library

Pixy2 pixy;

// --- Multiplexer Address ---
#define TCA_ADDR 0x70

// Sensor Channels on Multiplexer
#define RIGHT_CORNER_CH 0
#define FRONT_CH 1
#define LEFT_CORNER_CH 2
#define RIGHT_EXTREME_CH 3
#define LEFT_EXTREME_CH 4

// Servo Pins and Angles
#define SERVO_PIN 10
#define CENTER_ANGLE 48
#define LEFT_ANGLE 10
#define RIGHT_ANGLE 100

// Motor Driver Pins (TB6612)
#define PWM_PIN 3
#define AIN1_PIN 5
#define AIN2_PIN 7
#define STBY_PIN 9

// Thresholds (in mm)
#define FRONT_THRESHOLD 400
#define SIDE_THRESHOLD 350

// Motor Speed
#define MOTOR_SPEED 100

VL53L0X sensor;
Servo steeringServo;

// Multiplexer select
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Read distance
uint16_t readDistance(uint8_t channel) {
  tcaSelect(channel);
  uint16_t distance = sensor.readRangeContinuousMillimeters();
  if (sensor.timeoutOccurred()) {
    return 9999;
  }
  return distance;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Setup Started");

  Wire.begin();

  // Init ToF sensors
  for (uint8_t ch = 0; ch < 5; ch++) {
    tcaSelect(ch);
    if (!sensor.init()) {
      Serial.print("Failed to init VL53L0X on channel ");
      Serial.println(ch);
      while (1);
    }
    sensor.setTimeout(500);
    sensor.startContinuous();
  }

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

  Serial.println("Robot Setup Complete");
}

// Drive forward
void driveForward(int speed) {
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWM_PIN, speed);
}

// Reverse
void setMotorReverse(int speed) {
  if (speed <= 0) {
    analogWrite(PWM_PIN, 0);
    digitalWrite(STBY_PIN, LOW);
    return;
  }
  digitalWrite(STBY_PIN, HIGH);
  digitalWrite(AIN1_PIN, HIGH);
  digitalWrite(AIN2_PIN, LOW);
  analogWrite(PWM_PIN, speed);
}

// Stop
void stopMotor() {
  analogWrite(PWM_PIN, 0);
  digitalWrite(STBY_PIN, LOW);
}

void loop() {
  // --- Read ToF distances ---
  uint16_t right_corner = readDistance(RIGHT_CORNER_CH);
  uint16_t front = readDistance(FRONT_CH);
  uint16_t left_corner = readDistance(LEFT_CORNER_CH);
  uint16_t right_extreme = readDistance(RIGHT_EXTREME_CH);
  uint16_t left_extreme = readDistance(LEFT_EXTREME_CH);

  Serial.print("RC: "); Serial.print(right_corner);
  Serial.print(" | F: "); Serial.print(front);
  Serial.print(" | LC: "); Serial.print(left_corner);
  Serial.print(" | RE: "); Serial.print(right_extreme);
  Serial.print(" | LE: "); Serial.println(left_extreme);

  // --- Pixy detection ---
  int blocks = pixy.ccc.getBlocks();

  int largestArea = 0;
  int closestColor = -1;

  for (int i = 0; i < blocks; i++) {
    int area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;

    if (area < 200) continue;   // 🚫 Ignore small objects (like floor lines)

    if (area > largestArea) {
      largestArea = area;
      closestColor = pixy.ccc.blocks[i].m_signature;
    }
  }

  // --- Decision Making ---
  if (closestColor == 1) {        // Green object
    Serial.println("Pixy: Green detected → Steering LEFT");
    steeringServo.write(LEFT_ANGLE);
    driveForward(MOTOR_SPEED);

  } else if (closestColor == 2) { // Red object
    Serial.println("Pixy: Red detected → Steering RIGHT");
    steeringServo.write(RIGHT_ANGLE);
    driveForward(MOTOR_SPEED);

  } else {
    // Default wall-following with ToF
    if (front < FRONT_THRESHOLD) {
      uint16_t left_space = min(left_corner, left_extreme);
      uint16_t right_space = min(right_corner, right_extreme);

      if (left_space > right_space) {
        steeringServo.write(LEFT_ANGLE);
        Serial.println("Front blocked → Turning LEFT");
      } else {
        steeringServo.write(RIGHT_ANGLE);
        Serial.println("Front blocked → Turning RIGHT");
      }

    } else if (right_corner > SIDE_THRESHOLD) {
      steeringServo.write(RIGHT_ANGLE);
      Serial.println("Too far from right wall → Steer RIGHT");

    } else if (right_corner < SIDE_THRESHOLD) {
      steeringServo.write(LEFT_ANGLE);
      Serial.println("Too close to right wall → Steer LEFT");

    } else {
      steeringServo.write(CENTER_ANGLE);
      driveForward(MOTOR_SPEED);
      Serial.println("Path clear → Straight");
    }
  }

  delay(90);
}

