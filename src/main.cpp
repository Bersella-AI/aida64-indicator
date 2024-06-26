#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define ccSCREEN_ROWS 4
#define ccSCREEN_COLUMNS 20

const uint8_t ccCELSIUS[8] PROGMEM = {0b1000, 0b10100, 0b1000, 0, 0b111, 0b1000, 0b111, 0};
const uint8_t ccHEART[8] PROGMEM = {0, 0b01010, 0b11111, 0b11111, 0b1110, 0b100, 0, 0};
const uint8_t ciCHAR_CELSIUS = 0;
const uint8_t ciCHAR_HEART = 1;
const uint8_t ciLED_PIN = 13;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

char cRawData[8];

void setup() {
  // Serial's baudrate at default. Can be customized.
  Serial.begin(9600);
  //Serial.begin(115200);

  pinMode(ciLED_PIN,OUTPUT);
  while (lcd.begin(ccSCREEN_COLUMNS,ccSCREEN_ROWS)!=1)
  {
    digitalWrite(ciLED_PIN,HIGH);
    delay(750);
    digitalWrite(ciLED_PIN,LOW);
    delay(250);
  }
  lcd.blink();
  lcd.createChar(ciCHAR_CELSIUS,ccCELSIUS);
  lcd.createChar(ciCHAR_HEART, ccHEART);
  lcd.setCursor(0,0);
  lcd.print(F("AIDA64 Indicator"));
  lcd.setCursor(0,1);
  lcd.print(F("by Bersella AI"));
  lcd.write(ciCHAR_HEART);
  lcd.setCursor(0,3);
  lcd.print(F("Connecting..."));
  lcd.setCursor(14,1);
  while (!Serial.available())
  {
    digitalWrite(ciLED_PIN,HIGH);
    delay(500);
    digitalWrite(ciLED_PIN,LOW);
    delay(500);
  }
  lcd.clear();
  lcd.noBlink();
  lcd.print(F("G:"));
  lcd.setCursor(4,0);
  lcd.write(ciCHAR_CELSIUS);
  lcd.setCursor(18,0);
  lcd.write(0b11101000); // Char. √
  lcd.setCursor(12,0);
  lcd.write('W');
  lcd.setCursor(0,1);
  lcd.print(F("C:"));
  lcd.setCursor(4,1);
  lcd.write(ciCHAR_CELSIUS);
  lcd.setCursor(17,1);
  lcd.write('%');
  lcd.setCursor(12,1);
  lcd.write('W');
  lcd.setCursor(0,2);
  lcd.print(F("Mem:"));
  lcd.setCursor(0,3);
  lcd.print(F("VRAM:"));
  lcd.setCursor(13,2);
  lcd.print(F("MOS:"));
  lcd.setCursor(19,2);
  lcd.write(ciCHAR_CELSIUS);
  lcd.setCursor(13,3);
  lcd.print(F("SSD:"));
  lcd.setCursor(19,3);
  lcd.write(ciCHAR_CELSIUS);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    digitalWrite(ciLED_PIN,HIGH);
    uint8_t maxLen = 0;
    switch (Serial.read())
    {
    case 'T':
      // CPU Temp.
      lcd.setCursor(2,1);
      maxLen=2;
      break;
    case 't':
      // GPU Temp.
      lcd.setCursor(2,0);
      maxLen=2;
      break;
    case 'P':
      // CPU Package Power
      lcd.setCursor(6,1);
      maxLen=6;
      break;
    case 'p':
      // GPU Board Power
      lcd.setCursor(6,0);
      maxLen=6;
      break;
    case 'M':
      // System Memory Usage
      lcd.setCursor(4,2);
      maxLen=6;
      break;
    case 'm':
      // VRAM Usage
      lcd.setCursor(5,3);
      maxLen=5;
      break;
    case 'f':
      // GPU Core's Frequency
      lcd.setCursor(14,0);
      maxLen=4;
      break;
    case 'B':
      // Motherboard's MOS Temp.
      lcd.setCursor(17,2);
      maxLen=2;
      break;
    case 'b':
      // SSD Temp.
      lcd.setCursor(17,3);
      maxLen=2;
      break;
    case 'L':
      // CPU Usage %
      lcd.setCursor(14,1);
      maxLen=3;
      break;
    default:
      break;
    }
    if (maxLen>0){
      uint8_t len = 0;
      while (len<maxLen){
        if (Serial.available()){
          char d = Serial.read();
          if (d==' ' && len==0) continue;
          if (d==' ' && len>0) break;
          cRawData[len]=d;
          len++;
        }
      }
      cRawData[len]='\0';
      for (uint8_t i=0; i<maxLen-len; i++)
        lcd.write(' ');
      lcd.print(cRawData);
    }
    digitalWrite(ciLED_PIN,LOW);
  }
  
}