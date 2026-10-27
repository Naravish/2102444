#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  lcd.init();                      
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC Not Found!");
    Serial.println("Couldn't find RTC module.");
    while (1); 
  }
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time to compile time...");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.year(), DEC);
  lcd.print('/');
  if (now.month() < 10) lcd.print('0');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  if (now.day() < 10) lcd.print('0');
  lcd.print(now.day(), DEC);

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print('0');
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second(), DEC);

  delay(1000);
}