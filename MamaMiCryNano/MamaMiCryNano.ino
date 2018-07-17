//Code developed by NV Jumpstarter
//Last edited by: John Kirkpatrick

#include "LowPower.h"

//D13                         //D12
//3V3                         //D11 (PWM)
//AREF                        //D10 (PWM)
//A0                          //D9  (PWM)
//A1                          //D8
//A2                          //D7
//A3                          //D6  (PWM)
//A4                          //D5  (PWM)
//A5                          //D4
//A6                          //D3  (PWM) - Motor Pin
//A7                          //D2  Button Pin
//5V                          //GND
//RST                         //RST
//GND                         //RX D0
//VIN                         //TX D1


#define MOTOR_PIN 3
#define MOTOR_BUTTON 2

byte motorMode = 0;
unsigned long timeSinceLastButtonPush = 0;
unsigned long currentTime = 0;
unsigned long sleepTime = 0;

void changeMotorMode() 
{
  motorMode++;
}

void setup() 
{
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTOR_BUTTON), changeMotorMode, CHANGE);
}

void loop() 
{
  currentTime = millis();
  if(motorMode > 3)
  {
    motorMode = 0;
  }
  switch(motorMode)
  {
    case 0:  // Motor Off
      analogWrite(MOTOR_PIN, 0);
      break;
    case 1:  // 150 Hz
      analogWrite(MOTOR_PIN, 1023);
      timeSinceLastButtonPush = millis();
      break;
    case 2:  // 100 Hz
      analogWrite(MOTOR_PIN, 512);
      timeSinceLastButtonPush = millis();
      break;
    case 3:  // Heart Beat
      analogWrite(MOTOR_PIN, 50);
      timeSinceLastButtonPush = millis();
      break;
  }
  while(digitalRead(MOTOR_BUTTON) == LOW)
  {
    if(sleepTime - currentTime > 2000)
    {
      timeSinceLastButtonPush = 999999; //Trip the if statement to go into deep sleep
    }
    sleepTime = millis();
  }
  if(timeSinceLastButtonPush - currentTime > 900000)
  {
    analogWrite(MOTOR_PIN, 0);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(digitalPinToInterrupt(MOTOR_BUTTON)); 
  }
}
