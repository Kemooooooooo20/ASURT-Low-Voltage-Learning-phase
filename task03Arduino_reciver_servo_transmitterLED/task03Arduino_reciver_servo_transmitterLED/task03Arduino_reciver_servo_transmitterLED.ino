#include <SPI.h>
#include <mcp2515.h>
#include <Servo.h>

#define arduino_servo_id    0x0F6   //to be changed
#define pushbtn             5
#define budrateOne          115200
#define budrateTwo          9600
#define servoPin            9
struct can_frame servoMsg;
struct can_frame LedMsg;
MCP2515 mcp2515(10);


Servo myservo;
void setup() {
  
  myservo.attach(servoPin);
  pinMode(pushbtn , INPUT_PULLUP);
  
  LedMsg.can_id  = arduino_servo_id;
  LedMsg.can_dlc = 1;

  while (!Serial);
  Serial.begin(budrateOne);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}


byte oldValue = HIGH;
byte state = LOW;


void loop() {
  
  byte value = digitalRead(pushbtn);
  if(value == LOW && oldValue == HIGH)
  state = (state == LOW)?HIGH:LOW;
  LedMsg.data[0] = state;
  mcp2515.sendMessage(&servoMsg);
  oldValue = value;
  delay(100);

  if (mcp2515.readMessage(&LedMsg) == MCP2515::ERROR_OK) {
    // Read the potentiometer value from master
    int servoAngle = servoMsg.data[0];

    // Control servo
    myservo.write(servoAngle);

    delay(50);
  }
  //mcp2515.sendMessage(&canMsg2);

  Serial.println("Messages sent");
  
  delay(100);
}
