#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float temp;
int tempPin = A1;               // Temprature IN 
int pirPin = 7;                 // PIR Out pin 
int pirState = LOW;              // PIR Status
int val = 0;                
int data = 0;                   // Bluetooth Defalt Data

#define fan 9

void setup(){
  pinMode (fan, OUTPUT);         // FAN Output
  pinMode(pirPin, INPUT);        // PIR Input
  pinMode(tempPin, INPUT);       // Temprature Input
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("CSC 488");
  delay(1000);
  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print("Start");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Try Human Detect = ");
  delay(2000);
  lcd.clear();
}

void loop(){
  lcd.setCursor(0, 0);

  if(Serial.available() > 0){ 
    data = Serial.read();
    
    if (data == '0') {
      analogWrite(9,0);
      lcd.print("BT Fan Off");
      delay(2000);
      lcd.clear(); 
      data = 0;
      }
    else {
      analogWrite(9,255);
      lcd.print("BT Fan ON");
      delay(2000);
      lcd.clear();
      data = 0;    
    } 
  }

  val = digitalRead(pirPin);
  if (val == HIGH) {            // if motion detected
    lcd.setCursor(0, 0);
    lcd.print("Detect Human.");
    delay(2000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Measuring Temperature = ");
    delay(2000);
    lcd.clear();

    if (pirState == LOW) 
	  {
      temp = readTemp();
      if(temp<20){                       // Fan OFF
        analogWrite(9,0);
        lcd.print("Fan Off");
        delay(2000);
        lcd.clear();
      }
      else if(temp<=23){                // FAN 30%
        analogWrite(fan,110);
        lcd.print("Fan Spreed: 30% ");
        delay(5000);
        lcd.clear();
      }
      else if(temp<=26){                // FAN 60%
        analogWrite(fan,180);
        lcd.print("Fan Spreed: 60% ");
        delay(5000);
        lcd.clear();
      }
      else if(temp>=30){                 // FAN 100%
        analogWrite(fan,255);
        lcd.print("Fan Spreed: 100% ");
        delay(5000);
        lcd.clear();
      }
      pirState = HIGH;
    }
  }
  else{
    lcd.clear();
    if (pirState == HIGH)
	  {
      analogWrite(9,0);
      lcd.print("Empty Room");
      delay(2000);
      lcd.clear();
      pirState = LOW;
    }
  }
}

int readTemp() {  // get the temperature and convert it to celsius
  temp = analogRead(tempPin);
  float voltage = temp * (5.0 / 1024.0);
  return temp = voltage*100;
}