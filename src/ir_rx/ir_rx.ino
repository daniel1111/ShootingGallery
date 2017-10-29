
#include "ManchesterIr.h"
#include <Servo.h>

/*

  Manchester Receiver example
  
  In this example receiver will receive array of 10 bytes per transmittion

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

#define RX_PIN 11
#define LED_PIN 13
 
#define BUFFER_SIZE 3
uint8_t buffer[BUFFER_SIZE];

void target_loop();
void target_hit(uint8_t gun_id);

Servo myservo;  

struct gun
{
  uint8_t pin;
  unsigned long detected_time;
  boolean state;
};

#define GUN_COUNT 2

#define TARGET_TIMEOUT 300 // ms

gun _guns[GUN_COUNT];

#define GUN_0 0
#define GUN_0_LED 6 // digital x

#define GUN_1 1 
#define GUN_1_LED 7
 

void setup() 
{
  pinMode(GUN_1_LED, OUTPUT);  
  digitalWrite(GUN_1_LED, LOW);
  
  
  memset(_guns, 0, sizeof(_guns));
  _guns[0].pin = GUN_0_LED;
  _guns[1].pin = GUN_1_LED;  
  
  
  
 
  Serial.begin(19200);
  Serial.println("Rx INIT");
  man.setupReceive(RX_PIN, MAN_2400);
  man.beginReceiveArray(BUFFER_SIZE, buffer);

  myservo.attach(4);
  myservo.write(0);
}

void loop() 
{
  if (man.receiveComplete()) 
  {
    uint8_t receivedSize = 0;

    //do something with the data in 'buffer' here before you start receiving to the same buffer again
    receivedSize = buffer[0];
    for(uint8_t i=1; i<receivedSize; i++)
    {
      Serial.print(i);
      Serial.print("=");
      Serial.print(buffer[i], HEX);
      Serial.println();
    }
    Serial.println("RA START");
    man.beginReceiveArray(BUFFER_SIZE, buffer);
    Serial.println("RA DONE");
    
    target_hit(buffer[1]);
  }
  
  target_loop();
}


void target_hit(uint8_t gun_id)
{
  
  if (gun_id >= GUN_COUNT)
  {
    Serial.println("Invalid gun_id");
    return;
  }
    
  _guns[gun_id].detected_time = millis();
  
  digitalWrite(_guns[gun_id].pin, HIGH);
  myservo.write(90);
  _guns[gun_id].state = HIGH;  
}


void target_loop()
{
  for (int gun_id=0; gun_id < GUN_COUNT; gun_id++)
  {
    if (_guns[gun_id].state == HIGH)
    {
      if (millis() - _guns[gun_id].detected_time > TARGET_TIMEOUT)
      {
         digitalWrite(_guns[gun_id].pin, LOW);
         myservo.write(0);
         _guns[gun_id].state = LOW;
      }
    }
    
  }
}


