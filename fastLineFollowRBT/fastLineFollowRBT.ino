#include "arduino_secrets.h"

#include <BeeLineSensorPro.h>


#define M1 7
#define M1pwm 5
#define M2 3
#define M2pwm 6

unsigned char sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
BeeLineSensorPro sensor(sensorPins, LINE_WHITE);
bool isWhiteLine = true;

float kP = 0.04;
float kI = 0.0;
float kD = 0.3;
int last_error = 0;
int integral = 0;
int baseSpeed = 70;

void setup() {
  Serial.begin(9600);

  pinMode(M1, OUTPUT); pinMode(M1pwm, OUTPUT);
  pinMode(M2, OUTPUT); pinMode(M2pwm, OUTPUT);

  for (int i = 0; i < 300; i++) {
    sensor.calibrate();
    delay(10);
  }
}

void loop() {
  updateLineColor();

  int err = sensor.readSensor();
  Serial.println(err);

  if (isDeadEnd()) {
    turnRight(); delay(200);
    return;
  }

  if (isTJunction()) {
    turnRight(); delay(200);
    return;
  }

  if (isLJunction()) {
    turnLeft(); delay(200);
    return;
  }

  if (isDottedLine(err)) {
    mdrive(baseSpeed, baseSpeed); delay(80);
    return;
  }

  lineFollow(err);
}

void lineFollow(int err) {
  int derivative = err - last_error;
  integral += err;
  integral = constrain(integral, -1000, 1000);
  last_error = err;

  int correction = (int)(err * kP + derivative * kD + integral * kI);
  int m1 = baseSpeed + correction;
  int m2 = baseSpeed - correction;

  m1 = constrain(m1, -255, 255);
  m2 = constrain(m2, -255, 255);

  mdrive(m1, m2);
}

void mdrive(int m1, int m2) {
  digitalWrite(M1, m1 > 0); analogWrite(M1pwm, abs(m1));
  digitalWrite(M2, m2 > 0); analogWrite(M2pwm, abs(m2));
}

void turnRight() {
  mdrive(90, -90); delay(300);
}

void turnLeft() {
  mdrive(-90, 90); delay(300);
}

void stopMotors() {
  analogWrite(M1pwm, 0);
  analogWrite(M2pwm, 0);
}

void updateLineColor() {
  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += sensor.readSensorValue(i);
  }
  int avg = total / 8;

  if (avg > 800 && !isWhiteLine) {
    isWhiteLine = true;
    sensor.setLineColor(LINE_WHITE);
    Serial.println("Switched to WHITE LINE mode");
  }
  else if (avg < 200 && isWhiteLine) {
    isWhiteLine = false;
    sensor.setLineColor(LINE_BLACK);
    Serial.println("Switched to BLACK LINE mode");
  }
}

bool isDeadEnd() {
  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += sensor.readSensorValue(i);
  }
  return total < 100;
}

bool isLJunction() {
  return sensor.readSensorValue(0) > 800 && sensor.readSensorValue(7) < 100;
}

bool isTJunction() {
  return sensor.readSensorValue(0) > 800 && sensor.readSensorValue(7) > 800;
}

bool isDottedLine(int err) {
  static int unstableFrames = 0;
  if (abs(err) > 400) unstableFrames++;
  else unstableFrames = 0;
  return unstableFrames > 3;
}
