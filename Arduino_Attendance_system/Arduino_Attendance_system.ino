#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adjust if needed

// Student structure
struct Student {
  byte uid[4];
  const char* name;
  const char* section;
  bool present;
};

// Student list
Student students[] = {
  {{0xD7, 0x84, 0xD5, 0x00}, "Harchit Singh", "10A", false},
  {{0x31, 0x0A, 0xF4, 0x00}, "Agamdeep Singh",   "10A", false},
  {{0xE2, 0xD2, 0xD5, 0x00}, "Harjot Singh", "10A", false},
  {{0x7E, 0xB6, 0xF3, 0x00}, "Jasman Dua",  "10A", false},
  {{0x82, 0x7E, 0xD6, 0x00}, "Gurjas Singh",     "10A", false}
};

const int studentCount = sizeof(students) / sizeof(Student);
String inputCommand = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);

  showWelcome(); // Show startup message

  Serial.println("System ready. Scan RFID card or type 'how many student are present'");
}

void loop() {
  checkRFID();
  checkSerialInput();
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  beepBuzzer(); // 🔔 Beep on scan

  for (int i = 0; i < studentCount; i++) {
    if (compareUID(rfid.uid.uidByte, students[i].uid)) {
      if (!students[i].present) {
        students[i].present = true;
        showOnLCD(students[i].name, students[i].section);
        Serial.print("Marked present: ");
        Serial.println(students[i].name);
      } else {
        showOnLCD(students[i].name, students[i].section); // Show info again
        Serial.print("Already marked: ");
        Serial.println(students[i].name);
      }
      delay(3000); // 3 seconds delay for message
      showWelcome(); // Back to welcome screen
      break;
    }
  }

  rfid.PICC_HaltA();
}

bool compareUID(byte* a, byte* b) {
  for (int i = 0; i < 4; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void showOnLCD(const char* name, const char* section) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Name: ");
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print("Class: ");
  lcd.print(section);
  lcd.setCursor(0, 2);
  lcd.print("Status: Present");
}

void showWelcome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Student Attendance");
  lcd.setCursor(0, 1);
  lcd.print("System");
}

void checkSerialInput() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputCommand.trim();
      if (inputCommand.equalsIgnoreCase("how many student are present")) {
        showAttendance();
      }
      inputCommand = "";
    } else {
      inputCommand += c;
    }
  }
}

void showAttendance() {
  Serial.println("---- Attendance Report ----");
  for (int i = 0; i < studentCount; i++) {
    Serial.print(students[i].name);
    Serial.print(" - ");
    Serial.println(students[i].present ? "Present" : "Absent");
  }
  Serial.println("---------------------------");
}

void beepBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}
