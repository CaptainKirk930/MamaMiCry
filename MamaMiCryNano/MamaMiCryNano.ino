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
bool buttonPushed = false;
bool resetFlag = false;

void changeMotorMode() 
{
  buttonPushed = true;
}

void setup() 
{
  Serial.begin(115200);
  Serial.print("BOOT_UP");
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTOR_BUTTON), changeMotorMode, FALLING);
}

void loop() 
{
  currentTime = millis();
  if( (buttonPushed == true) && (currentTime - timeSinceLastButtonPush > 250) )
  {
    motorMode++;
    timeSinceLastButtonPush = millis();
    Serial.print("Motor Mode: ");
    Serial.println(motorMode);
  }
  buttonPushed = false;
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
      analogWrite(MOTOR_PIN, 150);
      break;
    case 2:  // 100 Hz
      analogWrite(MOTOR_PIN, 100);
      break;
    case 3:  // Heart Beat
      analogWrite(MOTOR_PIN, 20);
      break;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  
  while(digitalRead(MOTOR_BUTTON) == LOW)
  {
    sleepTime = millis();
    if(sleepTime - currentTime > 500)
    {
      resetFlag = true; //Trip the if statement to go into deep sleep
      break;
    }
  }
  
  if( (currentTime - timeSinceLastButtonPush > 900000) || resetFlag == true)
  {
    resetFlag = false;
    analogWrite(MOTOR_PIN, 0);
    delay(500);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    //detachInterrupt(digitalPinToInterrupt(MOTOR_BUTTON)); 
  }
  
}
