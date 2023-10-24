/*****************************************************************************/
/******************       Title:      ASURT task03        ********************/
/******************       Name:       Kareem Nader        ********************/
/******************       DAte:       24/10/2023          ********************/
/******************       File_Title: servo sender        ********************/
/*****************************************************************************/

#include <SoftwareSerial.h>
#define rx    2
#define tx    3
#define led   12

const int potPin = A0;
SoftwareSerial mySerial(rx, tx); // RX, TX
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  // Read potentiometer value
  int potValue = analogRead(potPin);
  int mappedValue = map(potValue, 0, 1023, 0, 180); // Map potentiometer value to servo angle range (0-180)
  mappedValue>>2;
  // Send potentiometer value via UART to slave
  //Serial.write(mappedValue);
  mySerial.write(mappedValue);

//the following block recieve the signal via UART and toggle the led
    if (mySerial.available() > 0) {
    // Read the potentiometer value from master
    int state = mySerial.read();
    digitalWrite(led, state);
    delay(50);
  }

  delay(50);
}