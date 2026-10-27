#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

constexpr uint8_t RST_PIN = A0;     
constexpr uint8_t SS_PIN = 53;     

MFRC522 mfrc522(SS_PIN, RST_PIN);  

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  SPI.begin();      
  mfrc522.PCD_Init();   

  Wire.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
}

void loop() {
  
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidString += "0";
    }
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      uidString += " ";
    }
  }
  uidString.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(uidString);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Card UID:");
  lcd.setCursor(0, 1);
  lcd.print(uidString);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}