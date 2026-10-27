#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define RST_PIN A0
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PROGRAM_MODE 0

const int EEPROM_FLAG_ADDR = 0;
const int EEPROM_UID_ADDR = 1;
const byte UID_LENGTH = 4;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  lcd.init();
  lcd.backlight();
  lcd.clear();

#if PROGRAM_MODE == 1
  lcd.setCursor(0, 0);
  lcd.print("PROGRAMMING MODE");
  lcd.setCursor(0, 1);
  lcd.print("Scan new card...");

  while (true) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      for (byte i = 0; i < UID_LENGTH; i++) {
        EEPROM.write(EEPROM_UID_ADDR + i, mfrc522.uid.uidByte[i]);
      }
      EEPROM.write(EEPROM_FLAG_ADDR, 0x55);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card Saved!");
      delay(2000);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Change flag to 0");
      lcd.setCursor(0, 1);
      lcd.print("& re-upload code");
      while (true);
    }
  }

#else
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
  lcd.print("Scan your card");
#endif
}

void loop() {
#if PROGRAM_MODE == 0
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  byte flag = EEPROM.read(EEPROM_FLAG_ADDR);
  bool match = true;
  
  if (flag != 0x55) {
    match = false;
  } else {
    for (byte i = 0; i < UID_LENGTH; i++) {
      byte storedByte = EEPROM.read(EEPROM_UID_ADDR + i);
      if (storedByte != mfrc522.uid.uidByte[i]) {
        match = false;
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  if (match) {
    lcd.print("Access Granted");
  } else {
    lcd.print("Access Denied");
  }

  lcd.setCursor(0, 1);
  for (byte i = 0; i < mfrc522.uid.size && i < 4; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) lcd.print("0");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
    lcd.print(" ");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2500);
  lcd.clear();
  lcd.print("Scan your card");
#endif
}