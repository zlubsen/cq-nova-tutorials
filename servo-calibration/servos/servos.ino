#include <Servo.h>

Servo NovaServo_1; // Head Movement - Front and Back
Servo NovaServo_2; // Head Rotation - Clockwise and Anticlockwise
Servo NovaServo_3; // Head Rotation - Up and Down
Servo NovaServo_4; // Whole Body Rotation - Z Axis
Servo NovaServo_5; // Head Movement - Up and Down

namespace servoinfo {
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
}

servoinfo::servorange sr1;
servoinfo::servorange sr2;
servoinfo::servorange sr3;
servoinfo::servorange sr4;
servoinfo::servorange sr5;

Servo servos[5] = {NovaServo_1, NovaServo_2, NovaServo_3, NovaServo_4, NovaServo_5};
servoinfo::servorange ranges[5] = {sr1, sr2, sr3, sr4, sr5};

void moveServoThroughRange(Servo* servo, servoinfo::servorange* sr) {
  int x = 1;
  for (int i = sr->mid; i > (sr->min); i = i + x){
      servo->write(i);
      if (i == sr->max) x = -1;             // switch direction at peak
      delay(30);
  }
  
  for (int i = sr->min; i < sr->mid; i++) {
    servo->write(i);
    delay(30);
  }
}

void moveServoThroughRangeSmooth(Servo* servo, servoinfo::servorange* sr) {
  int x = 1;
  int timer = 45;
  for (int i = sr->mid; i > sr->min; i = i + x){
      servo->write(i);
      if (i < sr->mid && x > 0) timer = timer - 1; // when going min->mid, speed up
      if (i < sr->mid && x < 0) timer = timer + 1; // when going mid->min, slow down
      if (i > sr->mid && x > 0) timer = timer - 1; // when going mid->max, slow down
      if (i > sr->mid && x < 0) timer = timer + 1; // when going max->mid, speed up
      if (i == sr->max) x = -1;             // switch direction at peak
      delay(timer);
  }
  
  for (int i = sr->min; i < sr->mid; i++) {
    servo->write(i);
    delay(30);
  }
}


void setup() {
  // put your setup code here, to run once:

  servoinfo::createServoRangeStruct(&sr1, 85, 122, 160);
  servoinfo::createServoRangeStruct(&sr2, 0, 90, 180);
  servoinfo::createServoRangeStruct(&sr3, 35, 90, 175);
  servoinfo::createServoRangeStruct(&sr4, 0, 93, 180);
  servoinfo::createServoRangeStruct(&sr5, 75, 110, 145);

  NovaServo_1.attach(32);
  NovaServo_2.attach(34);
  NovaServo_3.attach(36);
  NovaServo_4.attach(38);
  NovaServo_5.attach(40);
  
  NovaServo_1.write(sr1.mid); // min: 85 (when head is @90), max: 160, mid: 122
  NovaServo_2.write(sr2.mid); // min: 53 (when body servo5 completely down), max: 180, mid: 90 | when head is clear: 0-180 
  NovaServo_3.write(sr3.mid); // min: 35, max: 175, mid: 90
  NovaServo_4.write(sr4.mid); // min: 0, max: 180, mid: 93 -> might need to reposition frame on the servo
  NovaServo_5.write(sr5.mid); // min: 75, max: 145, mid: (145-11/2)+min= 87
  
  //moveServoThroughRangeSmooth(&NovaServo_1, &sr1);
  //moveServoThroughRangeSmooth(&NovaServo_2, &sr2);
  //moveServoThroughRangeSmooth(&NovaServo_3, &sr3);
  //moveServoThroughRangeSmooth(&NovaServo_4, &sr4);
  //moveServoThroughRangeSmooth(&NovaServo_5, &sr5);
}

void loop() {
  NovaServo_1.write(sr1.mid);
  NovaServo_2.write(sr2.mid);
  NovaServo_3.write(sr3.mid);
  NovaServo_4.write(sr4.mid);
  NovaServo_5.write(sr5.mid);
}

