#include <Servo.h>
#include <NewPing.h>
#include <LiquidCrystal.h>

// Digital I/0 pins 0..13
// Usable for PWM = 3,5,6,9,10,11
// Usable for interrupts = 2,3
int serialRX = 0;
int serialTX = 1;
int motorL_forward = 3;
int motorL_backward = 2;
int motorR_backward = 4;
int motorR_speed = 6;
int motorL_speed = 5;
int motorR_forward = 7;
int servoV_signal = 8;
int servoH_signal = 9;
int sonar_trig = 10;
int sonar_echo = 11;
int led = 13;

// Digital inputs A0..A5
int lcd_D4 = A3;
int lcd_D5 = A2;
int lcd_D6 = A1;
int lcd_D7 = A0;
int lcd_E  = A5;
int lcd_RS = A4;


// Drive var
boolean goesForward = false;

int motors_speed_forward = 255;
int motors_speed_backward = 255;
int motors_speed_turning = 255;
int motors_speed_stop = 0;
int motors_turn_time = 750;

// Servo to control direction of ultrasound
Servo servoH; // Ultrasonic sensor direction control horizontal
Servo servoV; // Ultrasonic sensor direction control vertical
int servoH_center = 70;
int servoH_look_right = 100;
int servoH_look_left = 45;
int servoV_center = 80;

// Ultrasonic
int sonar_distance_max = 200;
int sonar_distance = 100;
NewPing sonar(sonar_trig, sonar_echo, sonar_distance_max);

// LCD
LiquidCrystal lcd(lcd_RS,lcd_E,lcd_D4,lcd_D5,lcd_D6,lcd_D7);

void setup() {

  lcd.begin(16, 2);
  lcd.print("Setup...");
    
  // Serial
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("\n\nRobot Car Control Test");


  // Internal led
  pinMode(led, OUTPUT);

  // Set all the motor control pins to outputs
  pinMode(motorL_speed, OUTPUT);
  pinMode(motorR_speed, OUTPUT);
  pinMode(motorL_forward, OUTPUT);
  pinMode(motorR_forward, OUTPUT);
  pinMode(motorL_backward, OUTPUT);
  pinMode(motorR_backward, OUTPUT);

  pinMode(lcd_RS, OUTPUT);
  pinMode(lcd_E, OUTPUT);
  pinMode(lcd_D4, OUTPUT);
  pinMode(lcd_D5, OUTPUT);
  pinMode(lcd_D6, OUTPUT);
  pinMode(lcd_D7, OUTPUT);

  // Servo for ultrasonic sensor direction
  servoH.attach(servoH_signal);
  servoV.attach(servoV_signal);

  // Sonar init
  Serial.println("Sonar init - running");
  servoH.write(servoH_center);
  servoV.write(servoV_center);
  delay(2000);
  sonar_distance = readPing();
  delay(100);
  sonar_distance  = readPing();
  delay(100);
  sonar_distance  = readPing();
  delay(100);
  sonar_distance  = readPing();
  delay(100);
  Serial.println("Sonar init - done");
  

}

void demoUltrasonic() {
  Serial.println("Ultrasonic sensor test");


}

void demoServo()
{
  Serial.println("Servo test");
  servoH.write(0);
  Serial.println("  angle 0");
  blink();
  delay(1000);
  servoH.write(45);
  Serial.println("  angle 45");
  blink();
  delay(1000);
  servoH.write(90);
  Serial.println("  angle 90");
  blink();
  delay(1000);
  servoH.write(135);
  Serial.println("  angle 135");
  blink();
  delay(1000);
  servoH.write(180);
  Serial.println("  angle 180");
  blink();
  delay(1000);
  servoH.write(135);
  Serial.println("  angle 135");
  blink();
  delay(1000);
  servoH.write(90);
  Serial.println("  angle 90");
  blink();
  delay(1000);
  servoH.write(45);
  Serial.println("  angle 45");
  blink();
  delay(1000);
  servoH.write(0);
  Serial.println("  angle 0");
  blink();
  delay(1000);
  Serial.println("  done");
  blink();
}

void demoMotor()
{
  Serial.println("Motor test");

  // This function will run the motors across the range of possible speeds
  // Note that maximum speed is determined by the motor itself and the operating voltage

  // Turn on motors
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, HIGH);
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, HIGH);

  // Accelerate from zero to maximum speed

  Serial.println("  accelerating");
  for (int i = 0; i < 256; i++)

  {

    analogWrite(motorL_speed, i);
    analogWrite(motorR_speed, i);

    delay(20);
    blink();

  }

  // Decelerate from maximum speed to zero

  Serial.println("  declarating");
  for (int i = 255; i >= 0; --i)

  {

    analogWrite(motorL_speed, i);
    analogWrite(motorR_speed, i);

    delay(20);
    blink();

  }

  // Now turn off motors

  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, LOW);

  Serial.println("  done");

}

void blink() {
  static int state = 0 ;

  if (state == 0) {
    state = 1;
    digitalWrite(led, HIGH);

  }
  else
  {
    state = 0;
    digitalWrite(led, LOW);

  }
}


void motortest()
{
  Serial.println("Motor test");

  // This function will run the motors across the range of possible speeds
  // Note that maximum speed is determined by the motor itself and the operating voltage

  // Turn on motors

  analogWrite(motorL_speed, 255);
  analogWrite(motorR_speed, 255);

  Serial.println("Forward L");
  digitalWrite(motorL_forward, HIGH);
  digitalWrite(motorL_backward, LOW);
  delay(2000);
  Serial.println("Stop L");
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, LOW);
  delay(1000);

  Serial.println("Forward R");
  digitalWrite(motorR_forward, HIGH);
  digitalWrite(motorR_backward, LOW);
  delay(2000);
  Serial.println("Stop R");
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, LOW);
  delay(1000);

  Serial.println("Back L");
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, HIGH);
  delay(2000);
  Serial.println("Stop L");
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, LOW);
  delay(1000);

  Serial.println("Back R");
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, HIGH);
  delay(2000);
  Serial.println("Stop R");
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, LOW);
  delay(1000);

  Serial.println("All stop");
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorR_backward, LOW);
  analogWrite(motorL_speed, 0);
  analogWrite(motorR_speed, 0);
  delay(3000);

  Serial.println("done");

  delay(3000);
}



void servotest() {

  Serial.println("Servo test");

  int pos;


  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoH.write(pos);              // tell servo to go to position in variable 'pos'
    servoV.write(pos / 2);            // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(100);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servoH.write(pos);              // tell servo to go to position in variable 'pos'
    servoV.write(pos / 2);            // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(100);                       // waits 15ms for the servo to reach the position
  }

}

int readPing() {
  Serial.println("readPing");
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

int lookRight() {
  Serial.println("lookRight");
  servoH.write(servoH_look_right);
  delay(500);
  int distance = readPing();
  delay(100);
  servoH.write(servoH_center);
  return distance;
}

int lookLeft() {
  Serial.println("lookLeft");
  servoH.write(servoH_look_left);
  delay(500);
  int distance = readPing();
  delay(100);
  servoH.write(servoH_center);
  return distance;
}


void moveStop(){

  Serial.println("moveStop");

  analogWrite(motorL_speed, motors_speed_stop);
  analogWrite(motorR_speed, motors_speed_stop);

  
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorR_forward, LOW);
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_backward, LOW);
}

void moveForward(){

  if(!goesForward){
    Serial.println("moveForward");

    goesForward=true;

    analogWrite(motorL_speed, motors_speed_forward);
    analogWrite(motorR_speed, motors_speed_forward);
    
    digitalWrite(motorL_forward, HIGH);
    digitalWrite(motorR_forward, HIGH);
  
    digitalWrite(motorL_backward, LOW);
    digitalWrite(motorR_backward, LOW); 
  }
}

void moveBackward(){

  Serial.println("moveBackward");

  goesForward=false;

  analogWrite(motorL_speed, motors_speed_backward);
  analogWrite(motorR_speed, motors_speed_backward);

  digitalWrite(motorL_backward, HIGH);
  digitalWrite(motorR_backward, HIGH);
  
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorR_forward, LOW);
  
}

void turnRight(){

  Serial.println("turnRight");

  analogWrite(motorL_speed, motors_speed_turning);
  analogWrite(motorR_speed, motors_speed_turning);

  digitalWrite(motorL_forward, HIGH);
  digitalWrite(motorR_backward, HIGH);
  
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_forward, LOW);
  
  delay(motors_turn_time);

  analogWrite(motorL_speed, motors_speed_forward);
  analogWrite(motorR_speed, motors_speed_forward);
  
  digitalWrite(motorL_forward, HIGH);
  digitalWrite(motorR_forward, HIGH);
  
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_backward, LOW);
 
  
  
}

void turnLeft(){

  Serial.println("turnLeft");

  analogWrite(motorL_speed, motors_speed_turning);
  analogWrite(motorR_speed, motors_speed_turning);

  digitalWrite(motorL_backward, HIGH);
  digitalWrite(motorR_forward, HIGH);
  
  digitalWrite(motorL_forward, LOW);
  digitalWrite(motorR_backward, LOW);

  delay(motors_turn_time);

  analogWrite(motorL_speed, motors_speed_forward);
  analogWrite(motorR_speed, motors_speed_forward);
  
  digitalWrite(motorL_forward, HIGH);
  digitalWrite(motorR_forward, HIGH);
  
  digitalWrite(motorL_backward, LOW);
  digitalWrite(motorR_backward, LOW);
}

void drive() {

  Serial.println("drive");

  int sonar_distance_left; // variable for the duration of sound wave travel
  int sonar_distance_right; // variable for the duration of sound wave travel

  delay(50);

  if (sonar_distance <= 20){
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    sonar_distance_right = lookRight();
    delay(300);
    sonar_distance_left = lookLeft();
    delay(300);

    if (sonar_distance >= sonar_distance_left){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    sonar_distance = readPing();


}

void loop() {
  blink();
  //demoUltrasonic();
  //Serial.println("Delay 5s");
  //blink();
  //delay(5000);
  //demoMotor();
  //Serial.println("Delay 5s");
  //blink();
  // delay(5000);
  //demoServo();
  //blink();
  //Serial.println("Delay 5s");
  //delay(5000);
  //motortest();
  //servotest();

  //delay(2000);
  //lookLeft();
  //delay(1000);
  //lookRight();
  //moveForward();
  //delay(2000);
  //moveBackward();
  //delay(5000);
  //turnLeft();
  //delay(2000);
  //turnRight();
  //delay(2000);

  drive();

}
