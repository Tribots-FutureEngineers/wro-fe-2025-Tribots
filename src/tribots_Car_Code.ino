#include <Wire.h>
#include <VL53L0X.h>  // ToF sensor library
#include <Servo.h>
#include <Pixy2.h>

// --- Pixy Setup ---
Pixy2 pixy;
#define RED_SIGNATURE 2
#define GREEN_SIGNATURE 1
int magenta_count = 0; // Not used anymore

// TCA9548A Multiplexer Address
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
#define RIGHT_ANGLE 95

// Motor Driver Pins (TB6612)
#define PWM_PIN 3
#define AIN1_PIN 5
#define AIN2_PIN 7
#define STBY_PIN 9

// Encoder Pins (reserved for future)
#define ENCODER_A 16
#define ENCODER_B 14

// Thresholds (in mm)
#define FRONT_THRESHOLD 350
#define RIGHT_SIDE_THRESHOLD 350
#define LEFT_SIDE_THRESHOLD 300

// Motor Speed
#define MOTOR_SPEED 120

// VL53L0X sensor (single instance with multiplexer switching)
VL53L0X sensor;
Servo steeringServo;

// --- Dead-End Constants ---
#define DEADEND_COUNT 6       // Number of consecutive readings to consider dead-end
#define DEADEND_TOLERANCE 5   // Tolerance in mm for “constant” reading
int frontHistory[DEADEND_COUNT] = {50};
int frontIndex = 0;

// Multiplexer channel select
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Read distance from a channel
uint16_t readDistance(uint8_t channel) {
  tcaSelect(channel);
  uint16_t distance = sensor.readRangeContinuousMillimeters();
  if (sensor.timeoutOccurred()) {
    return 9999; // Large value if timeout/error
  }
  return distance;
}

// Drive motor forward
void driveForward(int speed) {
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWM_PIN, speed);
}

// Reverse motor
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

// Stop motor
void stopMotor() {
  analogWrite(PWM_PIN, 0);
  digitalWrite(STBY_PIN, LOW);
}

// --- Dead End Handler (Front sensor constant reading) ---
void checkDeadEndConstant(uint16_t front_distance) {
  frontHistory[frontIndex] = front_distance;
  frontIndex = (frontIndex + 1) % DEADEND_COUNT;

  // Check if all readings are almost identical
  bool constant = true;
  for (int i = 1; i < DEADEND_COUNT; i++) {
    if (abs(frontHistory[i] - frontHistory[0]) > DEADEND_TOLERANCE) {
      constant = false;
      break;
    }
  }

  if (constant) {
    Serial.println("Dead-end detected by constant front reading → Reversing for 0.5s");
    setMotorReverse(MOTOR_SPEED);  // Reverse
    delay(500);                    // Half second
    driveForward(MOTOR_SPEED);     // Resume forward
    // Reset history after dead-end
    for (int i = 0; i < DEADEND_COUNT; i++) frontHistory[i] = 0;
  }
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
    sensor.setTimeout(300);
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

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  // Enable motor driver
  digitalWrite(STBY_PIN, HIGH);
  driveForward(MOTOR_SPEED);

  // Initialize Pixy
  pixy.init();

  Serial.println("Robot Setup Complete");
}

void loop() {
  // --- Pixy Color Detection (Priority) ---
  pixy.ccc.getBlocks();
  bool pixyDetected = false;

  if (pixy.ccc.numBlocks) {
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      uint8_t sig = pixy.ccc.blocks[i].m_signature;

      if (sig == RED_SIGNATURE) {
        steeringServo.write(RIGHT_ANGLE);
        driveForward(MOTOR_SPEED);
        Serial.println("Pixy detected RED → Turn RIGHT");
        pixyDetected = true;
        break; // Take first detected block only
      } 
      else if (sig == GREEN_SIGNATURE) {
        steeringServo.write(LEFT_ANGLE);
        driveForward(MOTOR_SPEED);
        Serial.println("Pixy detected GREEN → Turn LEFT");
        pixyDetected = true;
        break;
      }
    }
  }

  // --- If Pixy sees nothing relevant, fallback to ToF logic ---
  if (!pixyDetected) {
    uint16_t right_corner = readDistance(RIGHT_CORNER_CH);
    uint16_t front = readDistance(FRONT_CH);
    uint16_t left_corner = readDistance(LEFT_CORNER_CH);
    uint16_t right_extreme = readDistance(RIGHT_EXTREME_CH);
    uint16_t left_extreme = readDistance(LEFT_EXTREME_CH);

    // Dead-end check based on constant front readings
    checkDeadEndConstant(front);

    // Debug
    Serial.print("RC: "); Serial.print(right_corner);
    Serial.print(" | F: "); Serial.print(front);
    Serial.print(" | LC: "); Serial.print(left_corner);
    Serial.print(" | RE: "); Serial.print(right_extreme);
    Serial.print(" | LE: "); Serial.println(left_extreme);

    // --- ToF Logic ---
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

    } else if (right_corner > RIGHT_SIDE_THRESHOLD) {
      steeringServo.write(RIGHT_ANGLE);
      Serial.println("Too far from right wall → Steer RIGHT");

    } else if (right_corner < RIGHT_SIDE_THRESHOLD) {
      steeringServo.write(LEFT_ANGLE);
      Serial.println("Too close to right wall → Steer LEFT");

    } else if (left_corner < LEFT_SIDE_THRESHOLD) {
      steeringServo.write(RIGHT_ANGLE);
      Serial.println("Too close to left wall → Steer RIGHT");

    } else if (left_corner > LEFT_SIDE_THRESHOLD) {
      steeringServo.write(LEFT_ANGLE);
      Serial.println("Too far from left wall → Steer LEFT");

    } else {
      steeringServo.write(CENTER_ANGLE);
      driveForward(MOTOR_SPEED);
      Serial.println("No side walls detected → Straight");
    }
  }

  delay(90);
}
