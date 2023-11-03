/***************************************************************************************/
/******************       Title:      ASURT_Final _task PT01        ********************/
/******************       Name:       Kareem Nader                  ********************/
/******************       DAte:       03/11/2023                    ********************/
/******************       File_Title: Lcd_Dashboard v1.0            ********************/
/******************       Team_NO:    1                             ********************/
/***************************************************************************************/
#include <SoftwareSerial.h>
#include <Servo.h>

#define rx    2
#define tx    3

/*the main elements for communication frame*/
byte buffer[20];
byte buffer_len = 0;
unsigned char flag = false;




/*the button addresses*/
#define Speed_address       1000
#define Rpm_address         1050 
#define Distance_address    1100
#define Voltage_address     1150
#define Menu_address        1200
#define Buzzer_address      1250
#define LED_address         1300


/*the pins defines*/
#define UltraSonic_TRIG        A0//will change//
#define UltraSonic_ECHO        A0//will change//
#define VoltageSensor_Pin      A0//will change//
#define POT_PIN_For_Speed_RPM  A0
#define voltage_sensor_pin     A1
#define Buzzer_PIN             5
#define LED_PIN                6



/*frame difults*/
#define Header_High            0x5A
#define Header_Low             0xA5
#define Write_command          0x82
#define Read_command           0x83



/*VPs*/
#define VP_Address_high        0x82



/*DATAs*/
#define Data_high              0x20


  int distace       = 0;
  int t             = 0;
  int rpm           = 0;
  int speed         = 0;
  int pot_val       = 0;
  float adc_voltage = 0.0;
  float in_voltage  = 0.0;
  float adc_value   = 0.0;

  const int wheel_radus =     17;
  const int speed_constanat = 3/25;
  const int Max_RPM =         21000;

  const float ref_voltage =     5.0;
  const float R1 =              30000.0;
  const float R2 =              7500.0; 


/*make the UART runs on pins 2 & 3 to enable the interrupt for led and buzzer*/
SoftwareSerial mySerial(rx, tx); // RX, TX

/*      functions prototypes       */
void disp_LEDRead(void);
void disp_BUZZERRead(void);
int selector();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(UltraSonic_TRIG, OUTPUT);
  pinMode(UltraSonic_ECHO, INPUT);
  pinMode(POT_PIN_For_Speed_RPM, INPUT);
  pinMode(voltage_sensor_pin,INPUT);
  pinMode(Buzzer_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
 
}

void loop() {
  //////////////////////////////////////
  /*        distance measuremnt       */
  //////////////////////////////////////
  /*the following block of code is for the distance measurement*/
    digitalWrite(UltraSonic_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(UltraSonic_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(UltraSonic_TRIG, LOW);
    t = pulseIn(UltraSonic_ECHO, HIGH);
    distace = t / 58;
    disp_readingSend(distace, Distance_address);


  ////////////////////////////////////
  /*          speed and rpm         */
  ////////////////////////////////////
  // reads the POT value
  pot_val = analogRead(POT_PIN_For_Speed_RPM);
  rpm = map(POT_PIN_For_Speed_RPM, 0, 1023, 0, Max_RPM);
  //display the values of the rpm readings
  disp_readingSend(rpm, Rpm_address);
  //converting from RPM to speed in KM/H from the formula 
  speed = speed_constanat *PI * wheel_radus * rpm;
  //display the speed readings after convertion
  disp_readingSend(speed, Speed_address);


  /////////////////////////////////////
  /*          voltage sensor        */
  ////////////////////////////////////
  adc_value = analogRead(voltage_sensor_pin);
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;
  // Calculate voltage at divider input
  in_voltage = adc_voltage*(R1+R2)/R2;
  // Pdisplay the voltage sensor reading 
  disp_readingSend(in_voltage, Voltage_address);


/*making an unterrupt to make the consumer order of buzzer and led */
  /*for make sure that there is a signal on rx*/
  if(digitalRead(rx) == LOW)
  {
    delay(10);
    if(digitalRead(rx) == LOW)
    {
      attachInterrupt(digitalPinToInterrupt(rx), selector(), FALLING);
    }
  }
}



int selector()
{
    if(mySerial.available())
  {
    buffer[buffer_len] = mySerial.read();
    buffer_len++;
    flag = true;
  }
  else
  {
    if(flag)
    {
      if (buffer[0] == Header_High && buffer[1] == Header_Low)
      {
        /*5A A5 04 83 1200 01 0001
        5A A5 header
        04 data length
        83 read vps
        1200 ram address
        01 data length words
        00 01 switch value 0002
        */
        if (buffer[4] == 0x12 && buffer[5] == 0x50)
        {
          disp_BUZZERRead();
        }
        else if(buffer[4] == 0x13 && buffer[5] == 0x00)
        {
          disp_LEDRead();
        }
      }
      buffer_len = 0;
      flag = false;
    }
  }
}




void disp_BUZZERRead(void)
{
  if(mySerial.available())
  {
    buffer[buffer_len] = mySerial.read();
    buffer_len++;
    flag = true;
  }
  else
  {
    if(flag)
    {
      if (buffer[0] == Header_High && buffer[1] == Header_Low)
      {
        /*5A A5 04 83 1200 01 0001
        5A A5 header
        04 data length
        83 read vps
        1200 ram address
        01 data length words
        00 01 switch value 0002
        */
        if (buffer[4] == 0x12 && buffer[5] == 0x50)
        {
          digitalWrite(Buzzer_PIN, HIGH);
        }
      }
      buffer_len = 0;
      flag = false;
    }
  }
}



void disp_LEDRead(void)
{
  if(mySerial.available())
  {
    buffer[buffer_len] = mySerial.read();
    buffer_len++;
    flag = true;
  }
  else
  {
    if(flag)
    {
      if (buffer[0] == Header_High && buffer[1] == Header_Low)
      {
        /*5A A5 04 83 1200 01 0001
        5A A5 header
        04 data length
        83 read vps
        1200 ram address
        01 data length words
        00 01 switch value 0002
        */
        if (buffer[4] == 0x13 && buffer[5] == 0x00)
        {
          digitalWrite(LED_PIN, HIGH);
        }
      }
      buffer_len = 0;
      flag = false;
    }
  }
}




void disp_readingSend(int data, int address)
{
  mySerial.write(Header_High);                // Header
  mySerial.write(Header_Low);                 // Header
  mySerial.write(0x05);                       // Length
  mySerial.write(0x82);                       // write command
  mySerial.write(highByte(address));          // Vp address high byte
  mySerial.write(lowByte(address));           // Vp address low  byte
  mySerial.write(highByte(data));             // data high byte
  mySerial.write(lowByte(data));              // data low  byte
}