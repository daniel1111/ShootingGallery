
#include <ManchesterIr.h>

/*

  Manchester Transmitter example
  
  In this example transmitter will send 10 bytes array  per transmittion

  try different speeds using this constants, your maximum possible speed will 
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define TX_PIN  -1  // N/A for IR

#define FIRE_TIME 500 // ms laser is transmitting for

#define MIN_TIME_BETWEEN_SHOT 1000 // ms

#define TRIGGER_PIN 2

#define GUNID_SWITCH 9

#define LED_PIN 8

uint8_t data[20] = {2, 0x01,0x01, '3', '4', '5', '6', '7', '8', '9','1','2','3','4','5','6','7','8','9'};

void button_loop();

unsigned long _trigger_pressed;
boolean _firing;

void setup() 
{
 
  Serial.begin(9600);
  Serial.println("START!");
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(TRIGGER_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, HIGH);
  
  pinMode(GUNID_SWITCH, INPUT);
  digitalWrite(GUNID_SWITCH, HIGH);
  
 // man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_2400);
  
  man.enableIROut(38);
  
  pinMode(7, OUTPUT);
  _firing = false;
}


uint8_t datalength=3;   //at least two data
void loop() 
{

  uint8_t gun_id;
  
  if (digitalRead(GUNID_SWITCH) == HIGH)
    gun_id = 0x01;
  else
    gun_id = 0x00;
 
  
  data[0] = datalength;
  data[1] = gun_id;

  if (_firing)
  {
    digitalWrite(LED_PIN, LOW);
    man.enableIROut(38);
    
    
    
    man.transmitArray(datalength, data);
    if (millis() - _trigger_pressed > FIRE_TIME)
    {
      _firing = false;
    }
  } 
  else
  {
    analogWrite(3, 0);
    // 30 for focusable laser
    //  5 for non-focusable laser
    //  0 for IR LED
  }


  button_loop();

}


void button_loop()
{
  
  if (_firing)
    return;
    
  if (millis() - _trigger_pressed < MIN_TIME_BETWEEN_SHOT)
  {
    digitalWrite(LED_PIN, LOW);
    return;
  }
    
  digitalWrite(LED_PIN, HIGH);
    
  if (digitalRead(TRIGGER_PIN) == LOW)
  {
    _firing = true;
    _trigger_pressed = millis();
    Serial.println("pressed");
  }
  
  
}


