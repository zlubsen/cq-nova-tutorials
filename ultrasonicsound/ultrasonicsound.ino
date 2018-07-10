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

const int trigPin = 46;
const int echoPin = 44;

long duration;
int distance;
int servoAngle = 110;

const int setpointDistance = 15;
const int maxDistance = 30;
const int minDistance = 9;

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

pid::pidsetting us_pid_val;
PID US_PID(&us_pid_val.Input,
            &us_pid_val.Output,
            &us_pid_val.Setpoint,
            us_pid_val.Kp,
            us_pid_val.Ki,
            us_pid_val.Kd,
            DIRECT);

void setup() {
  NovaServo_1.attach(32);
  NovaServo_2.attach(34);
  NovaServo_3.attach(36);
  NovaServo_4.attach(38);
  NovaServo_5.attach(40);
  
  settings::createServoRangeStruct(&sr1, 85, 122, 160);
  settings::createServoRangeStruct(&sr2, 0, 90, 180);
  settings::createServoRangeStruct(&sr3, 35, 90, 175);
  settings::createServoRangeStruct(&sr4, 0, 90, 180);
  settings::createServoRangeStruct(&sr5, 75, 110, 145);
  
  NovaServo_1.write(sr1.mid);
  NovaServo_2.write(sr2.mid);
  NovaServo_3.write(sr3.mid);
  NovaServo_4.write(sr4.mid);
  NovaServo_5.write(sr5.mid);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  US_PID.SetMode(AUTOMATIC);
  US_PID.SetSampleTime(10);
  US_PID.SetOutputLimits(-3, 3);
  
  us_pid_val.Setpoint = setpointDistance; 
  us_pid_val.Input = distance;
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034/2;
  
  us_pid_val.Input = distance;
  
  US_PID.Compute();
  
  if(distance < maxDistance && distance > minDistance){
       servoAngle = servoAngle - us_pid_val.Output;
       
       if(servoAngle < sr1.max && servoAngle > sr1.min){
            NovaServo_1.write(servoAngle);
       }
  }
}
