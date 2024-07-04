#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define ccSCREEN_ROWS 4
#define ccSCREEN_COLUMNS 20

// Serial's baudrate at default. Can be customized.
const uint32_t clBAUDRATE = 9600UL;

const uint8_t ccCELSIUS[8] PROGMEM = {0b1000, 0b10100, 0b1000, 0, 0b111, 0b1000, 0b111, 0};
const uint8_t ccHEART[8] PROGMEM = {0, 0b01010, 0b11111, 0b11111, 0b1110, 0b100, 0, 0};
const uint8_t ciCHAR_CELSIUS = 0;
const uint8_t ciCHAR_HEART = 1;
const uint8_t ciLED_PIN = 13;

LiquidCrystal_I2C periLCD(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

char cRawData[8];

void setup() {
  Serial.begin(clBAUDRATE);

  pinMode(ciLED_PIN,OUTPUT);
  while (periLCD.begin(ccSCREEN_COLUMNS,ccSCREEN_ROWS)!=1)
  {
    // Failed to detect screen
    digitalWrite(ciLED_PIN,HIGH);
    delay(750);
    digitalWrite(ciLED_PIN,LOW);
    delay(250);
  }
  periLCD.blink();
  periLCD.createChar(ciCHAR_CELSIUS,ccCELSIUS);
  periLCD.createChar(ciCHAR_HEART, ccHEART);
  periLCD.setCursor(0,0);
  periLCD.print(F("AIDA64 Indicator"));
  periLCD.setCursor(0,1);
  periLCD.print(F("by Bersella AI"));
  periLCD.write(ciCHAR_HEART);
  periLCD.setCursor(0,3);
  periLCD.print(F("Connecting..."));
  periLCD.setCursor(14,1);
  while (!Serial.available())
  {
    // Waiting for connection
    digitalWrite(ciLED_PIN,HIGH);
    delay(500);
    digitalWrite(ciLED_PIN,LOW);
    delay(500);
  }
  // Connection established
  periLCD.clear();
  periLCD.noBlink();
  periLCD.print("G:");
  periLCD.setCursor(4,0);
  periLCD.write(ciCHAR_CELSIUS);
  periLCD.setCursor(18,0);
  periLCD.write(0b11101000); // Char. √
  periLCD.setCursor(12,0);
  periLCD.write('W');
  periLCD.setCursor(0,1);
  periLCD.print("C:");
  periLCD.setCursor(4,1);
  periLCD.write(ciCHAR_CELSIUS);
  periLCD.setCursor(17,1);
  periLCD.write('%');
  periLCD.setCursor(12,1);
  periLCD.write('W');
  periLCD.setCursor(0,2);
  periLCD.print("Mem:");
  periLCD.setCursor(0,3);
  periLCD.print("VRAM:");
  periLCD.setCursor(13,2);
  periLCD.print("MOS:");
  periLCD.setCursor(19,2);
  periLCD.write(ciCHAR_CELSIUS);
  periLCD.setCursor(13,3);
  periLCD.print("SSD:");
  periLCD.setCursor(19,3);
  periLCD.write(ciCHAR_CELSIUS);
}

void loop() {
  while (Serial.available())
  {
    digitalWrite(ciLED_PIN,HIGH);
    uint8_t cMaxLen = 0;
    uint8_t cLetter = Serial.read();
    // Binary search for data type indexing
    if (cLetter==' ') continue;
    if (cLetter<96){
      if (cLetter<'O') switch (cLetter){
        case 'B':
          // Motherboard's MOS Temp.
          periLCD.setCursor(17,2);
          cMaxLen=2;
          break;
        case 'L':
          // CPU Usage %
          periLCD.setCursor(14,1);
          cMaxLen=3;
          break;
        case 'M':
          // System Memory Usage
          periLCD.setCursor(4,2);
          cMaxLen=6;
          break;
        default:
          break;
      }
      else switch (cLetter){
        case 'P':
          // CPU Package Power
          periLCD.setCursor(6,1);
          cMaxLen=6;
          break;
        case 'T':
          // CPU Temp.
          periLCD.setCursor(2,1);
          cMaxLen=2;
          break;
        default:
          break;
      }
    }
    else {
      if (cLetter<'o') switch (cLetter){
        case 'b':
          // SSD Temp.
          periLCD.setCursor(17,3);
          cMaxLen=2;
          break;
        case 'f':
          // GPU Core's Frequency
          periLCD.setCursor(14,0);
          cMaxLen=4;
          break;
        case 'm':
          // VRAM Usage
          periLCD.setCursor(5,3);
          cMaxLen=5;
          break;
        default:
          break;
      }
      else switch (cLetter){
        case 'p':
          // GPU Board Power
          periLCD.setCursor(6,0);
          cMaxLen=6;
          break;
        case 't':
          // GPU Temp.
          periLCD.setCursor(2,0);
          cMaxLen=2;
          break;
        default:
          break;  
      }
    }
    if (cMaxLen>0){
      uint8_t cLen = 0;
      while (cLen<cMaxLen){
        if (Serial.available()){
          char d = Serial.read();
          if (d==' ' && cLen==0) continue;
          if (d==' ' && cLen>0) break;
          cRawData[cLen]=d;
          cLen++;
        }
      }
      cRawData[cLen]='\0';
      for (uint8_t i=0; i<cMaxLen-cLen; i++)
        periLCD.write(' ');
      periLCD.print(cRawData);
    }
  }
  digitalWrite(ciLED_PIN,LOW);
}
