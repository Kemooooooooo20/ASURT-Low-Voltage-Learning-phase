/*****************************************************************************/
/******************       Title:      ASURT task03        ********************/
/******************       Name:       Kareem Nader        ********************/
/******************       DAte:       24/10/2023          ********************/
/******************       File_Title: servo reciver       ********************/
/*****************************************************************************/
#include <SoftwareSerial.h>
#include <Servo.h>


#define rx      2
#define tx      3
#define pushbtn 5

SoftwareSerial mySerial(rx, tx);
// Pins for servo
const int servoPin = 9;

Servo myservo;

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(pushbtn , INPUT_PULLUP);
}
byte oldValue = HIGH;
byte state = LOW;

void loop() {
  //manual detection of push button
  byte value = digitalRead(pushbtn);
  if(value == LOW && oldValue == HIGH)
  state = (state == LOW)?HIGH:LOW;
  mySerial.write(state);
  oldValue = value;

  //recive the servo signal via UART
  if (mySerial.available() > 0) {
    // Read the potentiometer value from master
    int servoAngle = mySerial.read();

    // Control servo
    myservo.write(servoAngle);

    delay(50);
  }
  delay(50);
}