#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN A0
#define SS_PIN  53

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  S
  Serial.println(F("RFID reader initialized. Scan your card/tag..."));
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
  }

  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}