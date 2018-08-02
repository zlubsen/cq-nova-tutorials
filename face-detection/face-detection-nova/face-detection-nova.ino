#include <PID_v1.h>
#include <Servo.h>

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

namespace pid {
  typedef struct {
    double Setpoint;
    double Input;
    double Output;
    double Kp = 0.51; // tutorial default: 0.51
    double Ki = 1.4; // tutorial default: 1.1
    double Kd = 0.0; // tutorial default: 0.0
  } pidsetting;
}

pid::pidsetting face_pid_val_x;
PID FACE_PID_X(&face_pid_val_x.Input,
            &face_pid_val_x.Output,
            &face_pid_val_x.Setpoint,
            face_pid_val_x.Kp = 0.028, // x
            face_pid_val_x.Ki = 0.026, // x
            //face_pid_val_x.Kp = 0.016,
            //face_pid_val_x.Ki = 0.012,
            face_pid_val_x.Kd = 0.0,
            DIRECT);

pid::pidsetting face_pid_val_y;
PID FACE_PID_Y(&face_pid_val_y.Input,
            &face_pid_val_y.Output,
            &face_pid_val_y.Setpoint,
            face_pid_val_y.Kp = 0.016,  // y
            face_pid_val_y.Ki = 0.012,  // y
            //face_pid_val_y.Kp = 0.028,
            //face_pid_val_y.Ki = 0.026,
            face_pid_val_y.Kd = 0,
            DIRECT);

int serialCount = 0;
int serialInArray[2];

int posX = 90;
int posY = 90;

int errorX;
int errorY;

void setup() {
  Serial.begin(9600);
  
  NovaServo_1.attach(32);
  NovaServo_2.attach(34);
  NovaServo_3.attach(36);
  NovaServo_4.attach(38);
  NovaServo_5.attach(40);

  settings::createServoRangeStruct(&sr1, 85, 122, 160);
  settings::createServoRangeStruct(&sr2, 0, 90, 180);
  settings::createServoRangeStruct(&sr3, 35, 90, 175);
  settings::createServoRangeStruct(&sr4, 0, 70, 180);
  settings::createServoRangeStruct(&sr5, 75, 110, 145);

  NovaServo_1.write(sr1.mid);
  NovaServo_2.write(sr2.mid);
  NovaServo_3.write(sr3.mid);
  NovaServo_4.write(sr4.mid);
  NovaServo_5.write(sr5.mid);

  FACE_PID_X.SetMode(AUTOMATIC);
  FACE_PID_X.SetSampleTime(1);
  FACE_PID_X.SetOutputLimits(-35, 35);
  
  face_pid_val_x.Setpoint = sr4.mid;

  FACE_PID_Y.SetMode(AUTOMATIC);
  FACE_PID_Y.SetSampleTime(1);
  FACE_PID_Y.SetOutputLimits(-35, 35);
  
  face_pid_val_y.Setpoint = sr3.mid;
}

void loop() {
  while(Serial.available() == 0);
  serialInArray[serialCount] = Serial.read();
  serialCount++;
  
  if(serialCount > 1) {
    face_pid_val_x.Input = serialInArray[0];
    face_pid_val_y.Input = serialInArray[1];

    FACE_PID_X.Compute();
    FACE_PID_Y.Compute();

    posX = posX + face_pid_val_x.Output;
    posY = posY + face_pid_val_y.Output;

    Serial.print("x: ");
    Serial.print(face_pid_val_x.Input);
    Serial.print(" +/- ");
    Serial.print(face_pid_val_x.Output);
    Serial.print(" : ");
    Serial.print(NovaServo_4.read());
    Serial.print(" -> ");
    Serial.println(posX);
  
    Serial.print("y: ");
    Serial.print(face_pid_val_y.Input);
    Serial.print(" +/- ");
    Serial.print(face_pid_val_y.Output);
    Serial.print(" : ");
    Serial.print(NovaServo_3.read());
    Serial.print(" -> ");
    Serial.println(posY);

    NovaServo_4.write(posX);
    if(posY > sr3.min && posY < sr3.max) NovaServo_3.write(posY);

    serialCount = 0;
  }
}
