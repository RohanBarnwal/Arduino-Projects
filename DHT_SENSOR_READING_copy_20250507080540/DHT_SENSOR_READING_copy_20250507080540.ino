//Made by Rohan Barnwal 
//www.youtube.com/ArduinoBoy

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C OLED
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT sensor setup
#define DHTPIN 7         // DHT sensor connected to digital pin D7
#define DHTTYPE DHT11    // Use DHT22 if needed
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Step 1: Show "DHT READING"
  display.setCursor(0, 20);
  display.println("DHT READING");
  display.display();
  delay(2000); // Wait 2 seconds

  // Step 2: Clear and show "ROHAN BARNWAL"
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("ROHAN BARNWAL");
  display.display();
  delay(2000); // Wait 2 seconds

  // Start DHT
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C  Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  display.clearDisplay();
  display.setTextSize(2);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Hum: ");
  display.print(humidity, 1);
  display.println(" %");

  display.display();
  delay(2000);
}
