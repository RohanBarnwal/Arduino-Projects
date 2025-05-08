#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "TextAnimation.h"

const int trigPin = 2;
const int echoPin = 3;
const int buzzerPin = 4;

ArduinoLEDMatrix matrix;
TEXT_ANIMATION_DEFINE(anim, 100)

bool warningShown = false; // Flag: warning ek baar hi dikhani hai jab distance 1–9 me aaye

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  matrix.begin();
  matrix.textFont(Font_5x7);
}

void loop() {
  float distance = measureDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Buzzer logic
  if (distance <= 30) {
    int beepDelay = map((int)distance, 5, 30, 100, 1000);
    beepDelay = constrain(beepDelay, 100, 1000);

    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(beepDelay);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Show warning ONLY ONCE when entering 1–9 cm zone
  if (distance >= 1 && distance <= 9) {
    if (!warningShown) {
      showWarning();
      warningShown = true;
    }
  } else {
    // Reset flag when out of warning zone
    if (warningShown) {
      warningShown = false;
      matrix.clear();
    }
  }

  delay(50);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * 0.034 / 2;
  return distanceCm;
}

void showWarning() {
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println("WARNING");
  matrix.endTextAnimation(SCROLL_LEFT, anim);
  matrix.loadTextAnimationSequence(anim);
  matrix.play();
}
