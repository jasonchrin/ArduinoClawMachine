#define ledDigitsData 2
#define ledDigitsLatch 3
#define ledDigitsClock 4
#define clawButton 6
#define ledDigitsOne 8
#define ledDigitsTwo 9
#define startButtonLed 10
#define startButton 11
#define clawButtonLed 12
#define baseClockTime 45

int startingTime; 
int DisplayDigits[] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 152};

void setup() {
  startingTime = 0;

  pinMode(ledDigitsLatch, OUTPUT);
  pinMode(ledDigitsClock, OUTPUT);
  pinMode(ledDigitsData, OUTPUT);
  pinMode(ledDigitsOne, OUTPUT);
  pinMode(ledDigitsTwo, OUTPUT);
  pinMode(startButtonLed, OUTPUT);
  pinMode(clawButtonLed, OUTPUT);
  pinMode(startButton, INPUT);
  pinMode(clawButton, INPUT);
  TIMSK1=0x01; // enabled global and timer overflow interrupt;
  TCCR1A = 0x00; // normal operation page 148 (mode0);
  TCNT1=0x0BDC; // set initial value to remove time error (16bit counter register)
  TCCR1B = 0x00; // start timer/ set clock
  Serial.begin(9600); //Open Serial connection for debugging
  digitalWrite(startButtonLed, HIGH);
  digitalWrite(clawButtonLed, LOW);
  digitalWrite(ledDigitsOne, LOW);
  digitalWrite(ledDigitsTwo, LOW);
}

void loop() {
  if(startingTime > 0){
    IsClawButtonPressed();
    DisplayTimer(startingTime);
  }else{
    StopTimerDisplay();
    IsStartButtonPressed();
  }
}

void IsStartButtonPressed(){
  int buttonState = digitalRead(startButton);
  if(buttonState == HIGH){
   StartButtonPress();
  }
}

void StartButtonPress(){
  Serial.println("START PRESS");
  TCCR1B = 0x04; 
  startingTime = baseClockTime;
  digitalWrite(startButtonLed, LOW);
  digitalWrite(clawButtonLed, HIGH);
}

void IsClawButtonPressed(){
  int pressState = digitalRead(clawButton);
  if(pressState == HIGH){
    ClawButtonPress();
  }
}
void ClawButtonPress(){
  Serial.println("CLAW PRESS");
  startingTime = 0;
  StopTimerDisplay();
}

void StopTimerDisplay(){
  TCCR1B = 0x00; // start timer/ set clock
  digitalWrite(ledDigitsOne, LOW);
  digitalWrite(ledDigitsTwo, LOW);
  digitalWrite(startButtonLed, HIGH);
  digitalWrite(clawButtonLed, LOW);
}

ISR(TIMER1_OVF_vect) {
  TCNT1=0x0BDC; // set initial value to remove time error (16bit counter register)
  startingTime = startingTime - 1;
}

void DisplayTimer(int displayTime){
  int firstDigit = 0;
  if(displayTime > 9){
    firstDigit = displayTime/10;
  }
  int secondDigit = displayTime - firstDigit*10;

  DisplayDigit(ledDigitsOne, DisplayDigits[firstDigit]);
  DisplayDigit(ledDigitsTwo, DisplayDigits[secondDigit]);  
}

void DisplayDigit(int pinNo, int number){
  digitalWrite(ledDigitsOne, LOW);
  digitalWrite(ledDigitsTwo, LOW);
  digitalWrite(ledDigitsLatch, LOW);
  shiftOut(ledDigitsData, ledDigitsClock, MSBFIRST, number);  
  digitalWrite(ledDigitsLatch, HIGH);
  digitalWrite(pinNo, HIGH);
  digitalWrite(ledDigitsLatch, LOW);
  delay(10);
}

