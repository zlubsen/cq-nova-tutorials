import gab.opencv.*;
import java.awt.Rectangle;
import processing.serial.*;
import processing.video.*;

OpenCV opencv;

Serial novaPort;

Rectangle[] faces;

Capture video;

int avgX;
int avgY;

int lastTime;

void setup() {
  size(320 , 240);
  video = new Capture(this, 320, 240);
  opencv = new OpenCV(this, 320, 240);
  opencv.loadCascade(OpenCV.CASCADE_FRONTALFACE); 
  video.start();
   
  novaPort = new Serial(this, "COM4", 9600);
  novaPort.bufferUntil('\n');
   
  faces = opencv.detect();
  
  lastTime = millis();
}

void draw() {
  scale(1);
  opencv.loadImage(video);
  image(video, 0, 0 );
  noFill();
  stroke(0, 255, 0);
  strokeWeight(3);
  Rectangle[] faces = opencv.detect();
  println(faces.length);
  
  for (Rectangle face : faces) {
    println(face.x + "," + face.y + "," + face.width + "," + face.height);
    rect(face.x, face.y, face.width, face.height);
               
    avgX = face.x + face.width/2;
    avgY = face.y + face.height/2;
     
    fill(255);
    strokeWeight(2.0);
    stroke(0);
    ellipse(avgX, avgY, 8, 8);
       
    stroke(255, 0, 0);
    point(avgX, avgY);
    
    avgX = ( avgX * 100 / 177 );
    avgY = ( avgY * 100 / 133 );
    
    novaPort.write(avgX);
    novaPort.write(avgY);
  }
      int currentTime = millis();
    int loopDuration = currentTime - lastTime;
    lastTime = currentTime;
    
    println("loop took: " + loopDuration);
}

void captureEvent(Capture c) {
  c.read();
}
