#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
MFRC522 rfid(SS_PIN, RST_PIN);

const int ledPin = 7;    // ضوء القفل
const int buzzerPin = 6; // البزر

// UID المسموح به (بدّلي إذا تغير)
byte allowedUID[] = {0x33, 0x41, 0x81, 0x11};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  Serial.println("مرر الكارت");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("الكارت UID: ");
  bool access = true;

  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");

    if (rfid.uid.uidByte[i] != allowedUID[i]) {
      access = false;
    }
  }
  Serial.println();

  if (access) {
    Serial.println("✅ كارت صحيح - فتح القفل");
    digitalWrite(ledPin, HIGH);
    delay(3000);
    digitalWrite(ledPin, LOW);
  } else {
    Serial.println("❌ كارت غير معروف - صوت إنذار");
    digitalWrite(buzzerPin, HIGH);
    delay(1000);  // الصوت لمدة ثانية
    digitalWrite(buzzerPin, LOW);
  }

  rfid.PICC_HaltA();
}