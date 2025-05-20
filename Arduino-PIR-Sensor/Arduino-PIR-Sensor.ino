#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int pirPin = 2;            // PIR sensor input
int buzzerPin = 8;         // Buzzer output pin
int pirState = LOW;        // Previous state

LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD I2C address and size

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rohan's Room");
}

void loop() {
  int motion = digitalRead(pirPin);

  if (motion == HIGH && pirState == LOW) {
    // Motion just started
    Serial.println("Motion Detected!");

    // Show welcome message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hi Rohan");
    lcd.setCursor(0, 1);
    lcd.print("Welcome To Your");
    lcd.setCursor(0, 2);
    lcd.print("Room");

    // Beep buzzer once
    digitalWrite(buzzerPin, HIGH);
    delay(1000);  // Beep for 1 second
    digitalWrite(buzzerPin, LOW);

    pirState = HIGH;
  }
  else if (motion == LOW && pirState == HIGH) {
    // Motion ended
    Serial.println("No Motion.");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rohan's Room");

    pirState = LOW;
  }

  delay(200); // Sensor stability delay
}
