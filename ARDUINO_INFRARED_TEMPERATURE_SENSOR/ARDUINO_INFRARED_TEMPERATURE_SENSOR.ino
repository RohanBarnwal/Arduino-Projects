#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 20, 4);  // 20x4 LCD

#define BUZZER_PIN 2  // Buzzer on pin 2

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!mlx.begin()) {
    Serial.println("MLX90614 not detected!");
    while (1);
  }

  lcd.init();           // ✅ UNO R4 compatible
  lcd.backlight();
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Make sure it's off initially

  lcd.setCursor(0, 0);
  lcd.print("MLX90614 Ready");
  delay(2000);
}

void loop() {
  float objectTemp = mlx.readObjectTempC();

  Serial.print("Object Temp: ");
  Serial.print(objectTemp);
  Serial.println(" °C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Object Temp:");
  lcd.print(objectTemp, 1);
  lcd.print((char)223); // degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1
  );
  if (objectTemp > 30.0) {
    lcd.print("Status: High Temp");

    // Fast buzzer beep
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);  // 100ms on
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);  // 100ms off
    }

  } else {
    lcd.print("Status: Normal");
    digitalWrite(BUZZER_PIN, LOW);  // Make sure buzzer is off
  }

  delay(2000);
}
