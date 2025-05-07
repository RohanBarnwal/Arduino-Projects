#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DFRobot_GR10_30.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
DFRobot_GR10_30 gr10_30(GR10_30_DEVICE_ADDR, &Serial1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  if (gr10_30.begin() == 0) {
    Serial.println("Sensor initialized");
    lcd.setCursor(0, 1);
    lcd.print("Sensor OK          ");
  } else {
    Serial.println("Sensor init failed!");
    lcd.setCursor(0, 1);
    lcd.print("Sensor FAIL        ");
    while (1);
  }

  gr10_30.enGestures(
    GESTURE_UP |
    GESTURE_DOWN |
    GESTURE_LEFT |
    GESTURE_RIGHT |
    GESTURE_FORWARD |
    GESTURE_BACKWARD |
    GESTURE_CLOCKWISE |
    GESTURE_COUNTERCLOCKWISE |
    GESTURE_CLOCKWISE_C |
    GESTURE_COUNTERCLOCKWISE_C
  );

  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gesture Ready...");
}

void loop() {
  if (gr10_30.getDataReady()) {
    uint16_t gestures = gr10_30.getGesturesState();
    String gestureText = "";

    if (gestures & GESTURE_UP) gestureText = "Up";
    if (gestures & GESTURE_DOWN) gestureText = "Down";
    if (gestures & GESTURE_LEFT) gestureText = "Left";
    if (gestures & GESTURE_RIGHT) gestureText = "Right";
    if (gestures & GESTURE_FORWARD) gestureText = "Forward";
    if (gestures & GESTURE_BACKWARD) gestureText = "Backward";

    // 👇 Yahan se Clockwise merge logic
    if ((gestures & GESTURE_CLOCKWISE) || (gestures & GESTURE_CLOCKWISE_C)) {
      gestureText = "Clockwise";
    }

    if ((gestures & GESTURE_COUNTERCLOCKWISE) || (gestures & GESTURE_COUNTERCLOCKWISE_C)) {
      gestureText = "Counter Clockwise";
    }

    if (gestures & GESTURE_WAVE) gestureText = "Wave";
    if (gestures & GESTURE_HOVER) gestureText = "Hover";

    if (gestureText != "") {
      Serial.println(gestureText);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gesture:");
      lcd.setCursor(0, 1);
      lcd.print(gestureText);
    }
  }

  delay(100);
}
