/*defines*/
#define interruptPin_index  1   
#define pushButton          3
#define ultrasonicTrig      11
#define ultrasonicEcho      12
/*libraries used*/
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

/*main variables*/
int t,distace;

/*initializing the lcd*/
LiquidCrystal_I2C lcd(0x20,20,4);

void setup() {
  // put your setup code here, to run once:
  pinMode(pushButton,INPUT);
  pinMode(ultrasonicTrig, OUTPUT);
  pinMode(ultrasonicEcho, INPUT); 
  //stateCheck = LOW;
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  //attachInterrupt(interruptPin_index, changeState, RISING);
}


void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(ultrasonicTrig, LOW);
    delayMicroseconds(5);
    digitalWrite(ultrasonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonicTrig, LOW);
    t = pulseIn(ultrasonicEcho, HIGH);
    distace = t / 58;
    lcd.print(distace);
    lcd.setCursor(5,0);

  int value = digitalRead(pushButton);

      lcd.print(value);

    lcd.setCursor(1,1);
    for (int i=0; i<=6; i++)
    {
      lcd.print(i);
      delay(1000);
    }
    lcd.clear();
}
