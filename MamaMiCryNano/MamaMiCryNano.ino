a//Code developed by NV Jumpstarter
//Last edited by: John Kirkpatrick

#include "LowPower.h"
#include <avr/sleep.h>
#include <avr/power.h>

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

#define ON 0
#define HALF_ON 600
#define LOW_ON 420
#define OFF 1023

byte motorMode = 0;
unsigned long timeSinceLastButtonPush = 0;
unsigned long currentTime = 0;
unsigned long sleepTime = 0;
bool buttonPushed = false;
bool resetFlag = true;

void changeMotorMode() 
{
  buttonPushed = true;
  delayMicroseconds(16383);
}

void setup() 
{
  // disable ADC
//  ADCSRA = 0; 
//  for (byte i = 0; i <= A5; i++)
//  {
//    pinMode (i, OUTPUT);    // changed as per below
//    digitalWrite (i, LOW);  //     ditto
//  }
//  power_adc_disable(); // ADC converter
  //power_spi_disable(); // SPI
  //power_usart0_disable();// Serial (USART) 
  //power_timer0_disable();// Timer 0
  //power_timer1_disable();// Timer 1
  //power_timer2_disable();// Timer 2
  //power_twi_disable(); // TWI (I2C)
  set_sleep_mode (SLEEP_MODE_IDLE); 
  noInterrupts (); 
  sleep_enable();
  interrupts ();
  sleep_cpu (); 
  Serial.begin(115200);
  Serial.print("BOOT_UP");
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTOR_BUTTON), changeMotorMode, FALLING);
}

void loop() 
{
  currentTime = millis();
  if( (buttonPushed == true) && (currentTime - timeSinceLastButtonPush > 500) )
  {
    motorMode++;
    timeSinceLastButtonPush = millis();
    Serial.print("Motor Mode: ");
    Serial.println(motorMode);
  }
  buttonPushed = false;
  if(motorMode > 2)
  {
    motorMode = 0;
  }
  switch(motorMode)
  {
    case 0:  // 150 Hz
      analogWrite(MOTOR_PIN, ON);
      break;
    case 1:  // 100 Hz
      analogWrite(MOTOR_PIN, LOW_ON);
      break;
    case 2:  // Heart Beat
      heartBeat();
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
    analogWrite(MOTOR_PIN, OFF);
    delay(500);
    
    set_sleep_mode (SLEEP_MODE_PWR_DOWN); 
    noInterrupts ();           // timed sequence follows
    sleep_enable();
   
    // turn off brown-out enable in software
    MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
    MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
    interrupts ();             // guarantees next instruction executed
    sleep_cpu ();              // sleep within 3 clock cycles of above 
    //LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  } 
}

void heartBeat()
{
  analogWrite(MOTOR_PIN, ON);
  delay(100);
  analogWrite(MOTOR_PIN, OFF);
  delay(100);
  analogWrite(MOTOR_PIN, HALF_ON);
  delay(150);
  analogWrite(MOTOR_PIN, OFF);
  delay(150);
  delay(850);
  
}

