//Code developed by NV Jumpstarter
//Last edited by: John Kirkpatrick


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
  if(motorMode > 2)
  {
    motorMode = 0;
  }
  switch(motorMode)
  {
    case 0:
      analogWrite(MOTOR_PIN, 0);
      break;
    case 1:
      analogWrite(MOTOR_PIN, 500);
      break;
    case 2:
      analogWrite(MOTOR_PIN, 1023);
      break;
  }
}
