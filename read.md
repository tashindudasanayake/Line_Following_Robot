# Fast Line Following Robot

This project is a simple line-following robot that uses an 8-element line sensor (BeeLineSensorPro) and two DC motors. The robot follows a line reliably and detects intersections (T-, L- junctions), dead-ends, and dotted lines. The control loop uses a PID-like correction (P and D terms by default) to steer the motors.

## Features

- 8-channel line sensor (analog A0..A7)
- PID-style control (P / I / D constants configurable in the sketch)
- Detects: L-junctions, T-junctions, dead-ends, and dotted lines
- Simple motor interface using a direction pin + PWM pin per motor

## Files

- `fastLineFollowRBT/fastLineFollowRBT.ino` — main Arduino sketch
- `arduino_secrets.h` — (project secrets/config, not required for basic operation)

## Hardware

- Arduino Uno (or compatible)
- BeeLineSensorPro (8-channel line sensor) or equivalent 8 IR reflectance array
- Motor driver or dual H-bridge (a driver with separate DIR + PWM inputs per motor is easiest)
- 2 DC motors
- Power supply for motors (battery pack) and for the Arduino (USB or regulated supply)
- Jumper wires, chassis, wheels

Important: Always share the ground between the Arduino and the motor power supply.

## Pinout / Wiring (from sketch)

The following pin assignments are used in `fastLineFollowRBT.ino`:

- Motor 1 direction: digital pin 7 (symbol `M1`)
- Motor 1 PWM/speed: PWM pin 5 (symbol `M1pwm`)
- Motor 2 direction: digital pin 3 (symbol `M2`)
- Motor 2 PWM/speed: PWM pin 6 (symbol `M2pwm`)
- Line sensors: analog pins A0, A1, A2, A3, A4, A5, A6, A7 (in that order)

Generic wiring notes:

- Connect the BeeLineSensorPro outputs to the Arduino analog inputs A0..A7 in the order shown above.
- Connect the sensor module power pins to 5V and GND (confirm module voltage requirements first).
- For motors, use a motor driver board. The sketch controls each motor using one digital 'direction' pin and one PWM pin per motor. That maps to motor driver inputs as follows (driver-specific names vary):
  - DIR pin (or IN1)  <- connect to `M1` (7) / `M2` (3)
  - PWM / ENA pin     <- connect to `M1pwm` (5) / `M2pwm` (6)

Driver-specific examples:
- TB6612-like drivers: they usually provide AIN1/AIN2 and PWMA. If your driver requires two direction pins per motor (IN1/IN2), you can wire one Arduino digital pin to the direction input and tie the other input to the inverse signal with additional wiring or change the sketch to drive two direction pins. The simplest option is to use a driver board that exposes a DIR and PWM input for each motor.
- L298N modules: L298N exposes IN1/IN2 and ENA. To use the sketch as-is, connect the Arduino direction pin to IN1, and connect the Arduino PWM pin to ENA; then tie IN2 to the inverse of IN1 (or modify the code to drive both IN1 and IN2). If unsure, consult your motor driver documentation.

Powering motors:
- Use a separate battery pack for motors capable of providing sufficient current for your motors.
- Connect motor pack GND to Arduino GND.

## Software setup

1. Install Arduino IDE (or use Arduino CLI / PlatformIO).
2. Install the BeeLineSensorPro library used by the sketch (if available through Library Manager or from the library source).
3. Open `fastLineFollowRBT/fastLineFollowRBT.ino` in the IDE.
4. Select the correct board (e.g., Arduino Uno) and the correct serial port.
5. Upload the sketch.

## How it works (quick)

- On boot the sketch performs sensor calibration in setup() by calling `sensor.calibrate()` many times (~300 iterations) while you place the robot over the line area for a reliable baseline.
- In loop() the sketch reads a single integer `err` from `sensor.readSensor()` which encodes the line position error.
- A PD controller (P and D terms by default) computes a correction which is applied by changing the left/right motor speeds around a `baseSpeed` (default 70).
- Special conditions (dead-end, L-junction, T-junction and dotted line) are detected using raw sensor thresholds and handled with simple turn or slow-forward behavior.

Key configurable parameters in the sketch (top of file):

- kP = 0.04  (proportional gain)
- kI = 0.0   (integral gain)
- kD = 0.3   (derivative gain)
- baseSpeed = 70 (nominal motor speed)

Adjust these values to tune performance for your robot and track.

## Serial debug

- The sketch prints the current `err` value and line color switch messages to the Serial Monitor at 9600 baud. Open Serial Monitor to see telemetry for tuning and debugging.

## Calibrating and testing

1. Place the robot over the track area (ideally covering both line and background) so the sensor can see both surface types.
2. Power the robot and allow the calibration routine in setup() to run (the sketch calls `sensor.calibrate()` repeatedly for about 3 seconds). During this time, slowly rock the robot across the line area for better calibration.
3. After upload, open Serial Monitor (9600 baud) and watch the printed `err` values while you move the robot to check responses.
4. Place the robot on the line and let it run. If the motors are reversed, switch motor wiring or invert the sign in `mdrive()` / invert pin logic.

## PID / tuning tips

- If the robot oscillates, reduce kP or increase kD.
- If the robot is too slow to correct, increase kP.
- If the robot has steady offset (drifting), try adding a small kI to compensate slowly.
- Increase `baseSpeed` gradually once control is stable.

## Troubleshooting

- Robot doesn't move: verify motor driver wiring, power to motors, common ground, and that the PWM pins support analogWrite on your board.
- Motors spin in the wrong direction: swap motor polarity on the driver or invert the direction logic in `mdrive()`.
- Sensor doesn't detect the line: check sensor wiring and power, ensure sensors are aimed correctly and the surface contrast is sufficient. Try re-running calibration while placing the sensor over both line and background.
- Erratic readings: increase calibration time, ensure sensors are clean, check that sensor analog pins are connected correctly.

## Safety

- Disconnect motors while wiring.
- Work on a non-conductive surface.

## License & Credits

This project is provided as-is. Feel free to reuse and adapt the code and wiring for educational and hobby projects. Credit the original author if you publish derived work.

---

If you'd like, I can also create a `README.md` with images and a wiring diagram (SVG/PNG) or modify the sketch to support L298N explicitly — tell me which motor driver you're using.
