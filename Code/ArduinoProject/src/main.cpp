#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int calculateDistance();

Servo myservo;   
const int trigPin = 13;
const int echoPin = 14;
const int passLed = 0;
const int redLed = 1;
const int buzzer = 2;
const String PASSCODE = "1234";
int distance;
int carCount = 0;

LiquidCrystal lcd(A1, A2, A3, A4, A5, A6);

void setup() {
  lcd.begin(16, 2);
  myservo.attach(12); 
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(passLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.setCursor(0, 1);
  lcd.print("Cars : 0");
  myservo.write(0);
}

void loop() {
  distance = calculateDistance();
  if (distance < 20) {
    lcd.setCursor(0, 0);
    lcd.print("Enter Passcode. ");
    Serial.println("Enter Passcode. ");
    while(Serial.available() == 0) {
      distance = calculateDistance();
      if (distance > 20) {
        lcd.setCursor(0, 0);
        lcd.print("Terminated.     ");
        Serial.println("Terminated.");
        delay(800);
        return;
      }
    }
    String pass = Serial.readStringUntil('\n');
    if (pass == PASSCODE) {
      carCount++;
      lcd.setCursor(0, 0);
      lcd.print("Gate Opening.   ");
      lcd.setCursor(0, 1);
      lcd.print("Cars : ");
      lcd.print(carCount);
      Serial.println("Gate is now opening.");
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      digitalWrite(passLed, HIGH);
      for (int i = 0; i <= 80; i++) { 
        myservo.write(i);             
        delay(15);                       
      }
      while(distance < 20) {
        distance = calculateDistance();
      }
      delay(1000);
      for (int i = 80; i >= 0; i--) { 
        myservo.write(i);
        delay(15);                                     
      }
      digitalWrite(passLed, LOW);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Wrong Passcode. ");
      Serial.println("Wrong Passcode. ");
      digitalWrite(redLed, HIGH);
      delay(1500);
      digitalWrite(redLed, LOW);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("                ");
  }
}

int calculateDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}