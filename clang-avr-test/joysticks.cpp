#include <Arduino.h>
#include <Servo.h>

#define ACTIVATED LOW

namespace settings {
  typedef struct {
    int min;
    int mid;
    int max;
  } servorange;

  void createServoRangeStruct(servorange* sr, int min, int mid, int max) {
    sr->min = min;
    sr->mid = mid;
    sr->max = max;
  }

  typedef struct {
    float x;
    float y;
  } filter_constant;
}

namespace input {
  typedef struct {
    int x;
    int y;
    int sw;
    int x_pin;
    int y_pin;
    int sw_pin;
  } joystick;
}

namespace output {
  typedef struct {
    int x;
    int y;
  } servo_axis;

  typedef struct {
    int degree;
  } servo_tacile;
}

Servo NovaServo_1; //Head Movement - Front and Back                --> maps to joy1.y
Servo NovaServo_2; //Head Rotation - Clockwise and Anticlockwise   --> maps to joy2.x
Servo NovaServo_3; //Head Rotation - Up and Down                   --> maps to joy2.y
Servo NovaServo_4; //Whole Body Rotation - Z axis                  --> maps to joy1.x
Servo NovaServo_5; //Head Movement - Up and Down                   --> maps to joy1 & joy2 tacile press

settings::servorange sr1;
settings::servorange sr2;
settings::servorange sr3;
settings::servorange sr4;
settings::servorange sr5;

settings::filter_constant filter1;
settings::filter_constant filter2;

input::joystick joy1;
input::joystick joy2;

output::servo_axis servo_axis1;
output::servo_axis servo_axis2;
output::servo_tacile servo_tacile;

void setupJoysticks() {
  joy1.x_pin = A9;
  joy1.y_pin = A11;
  joy1.sw_pin = 43;

  joy2.x_pin = A13;
  joy2.y_pin = A15;
  joy2.sw_pin = 45;

  pinMode(joy1.sw_pin, INPUT_PULLUP);
  pinMode(joy2.sw_pin, INPUT_PULLUP);
}

void setupServos() {
  NovaServo_1.attach(32);
  NovaServo_2.attach(34);
  NovaServo_3.attach(36);
  NovaServo_4.attach(38);
  NovaServo_5.attach(40);

  settings::createServoRangeStruct(&sr1, 85, 122, 160);
  settings::createServoRangeStruct(&sr2, 0, 90, 180);
  settings::createServoRangeStruct(&sr3, 35, 90, 175);
  settings::createServoRangeStruct(&sr4, 10, 90, 170);
  settings::createServoRangeStruct(&sr5, 75, 110, 145);

  servo_axis1.x = sr4.mid;   // tutorial default: 90
  servo_axis1.y = sr1.mid;   // tutorial default: 110
  servo_axis2.x = sr2.mid;   // tutorial default: 90
  servo_axis2.y = sr3.mid;   // tutorial default: 110

  servo_tacile.degree = sr5.mid; // tutorial default: 90
}

void setupFilter() {
  filter1.x = 0.05;
  filter1.y = 0.05;
  filter2.x = 0.08;
  filter2.y = 0.08;
}

void readJoystickInput() {
  joy1.x = analogRead(joy1.x_pin);
  joy1.y = analogRead(joy1.y_pin);
  joy2.x = analogRead(joy2.x_pin);
  joy2.y = analogRead(joy2.y_pin);

  joy1.sw = digitalRead(joy1.sw_pin);
  joy2.sw = digitalRead(joy2.sw_pin);
}

void mapRawInputToRange() {
  joy1.x = map(joy1.x, 0, 1024, sr4.max, sr4.min); // tutorial default: 170, 10
  joy1.y = map(joy1.y, 0, 1024, sr1.min, sr1.max); // tutorial default: 50, 170
  joy2.x = map(joy2.x, 0, 1024, sr2.min, sr2.max); // tutorial default: 40, 140
  joy2.y = map(joy2.y, 0, 1024, sr3.min, sr3.max); // tutorial default: 40, 160

  if (joy1.sw == ACTIVATED) {
    if(servo_tacile.degree < sr5.max) servo_tacile.degree = servo_tacile.degree + 2;  // tutorial default max: 144
  }
  if (joy2.sw == ACTIVATED) {
    if(servo_tacile.degree > sr5.min) servo_tacile.degree = servo_tacile.degree - 2;  // tutorial default min: 89
  }
}

void filterInput() {
  servo_axis1.x = servo_axis1.x * (1 - filter1.x) + joy1.x * filter1.x;
  servo_axis1.y = servo_axis1.y * (1 - filter1.y) + joy1.y * filter1.y;
  servo_axis2.x = servo_axis2.x * (1 - filter2.x) + joy2.x * filter2.x;
  servo_axis2.y = servo_axis2.y * (1 - filter2.y) + joy2.y * filter2.y;
}

void actuateServos() {
  if(servo_axis1.y > sr1.min && servo_axis1.y < sr1.max)
    NovaServo_1.write(servo_axis1.y);

  if(servo_axis2.x > sr2.min && servo_axis2.x < sr2.max)
    NovaServo_2.write(servo_axis2.x);

  if(servo_axis2.y > sr3.min && servo_axis2.y < sr3.max)
    NovaServo_3.write(servo_axis2.y);

  if(servo_axis1.x > sr4.min && servo_axis1.x < sr4.max)
    NovaServo_4.write(servo_axis1.x);

  NovaServo_5.write(servo_tacile.degree);
}

void setup() {
  setupJoysticks();
  setupServos();
  setupFilter();
}

void loop() {
  readJoystickInput();
  mapRawInputToRange();
  filterInput();
  actuateServos();

  delay(30);
}

void int main(int argc, char const *argv[]) {
  setup();
  while(true) {
    loop();
  }
  return 0;
}
