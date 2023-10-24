#include <SPI.h>
#include <mcp2515.h>



#define budrateOne      115200
#define budrateTwo      9600
#define led             12
#define timne_rcv       50
#define arduinoLEd_ID   0x0BE

const int potPin = A0;

struct can_frame LedMsg;
struct can_frame servoMsg;
MCP2515 mcp2515(10);


void setup() {

  servoMsg.can_id  = arduinoLEd_ID;
  servoMsg.can_dlc = 1;
  while (!Serial);
  Serial.begin(budrateOne);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  pinMode(led, OUTPUT);

}

void loop() {

  int potValue = analogRead(potPin);
  int mappedValue = map(potValue, 0, 1023, 0, 180); // Map potentiometer value to servo angle range (0-180)
  mappedValue>>2;
  // Send potentiometer value via UART to slave
  //Serial.write(mappedValue);
  servoMsg.data[0] = mappedValue;
  delay(100);

  if (mcp2515.readMessage(&LedMsg) == MCP2515::ERROR_OK) {
    int state = servoMsg.data[0];
    digitalWrite(led, state);
    delay(timne_rcv) ;
  }
}
