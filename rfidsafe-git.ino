/*
 * NFC Safe by Levi Pinkard
 * 
 * The Adafruit_PN532 ibrary and some of the code in this file is from Adafruit Industries, from the iso14443a_uid file.
 * 
 * Software License Agreement (BSD License)
 * Copyright (c) 2012, Adafruit Industries
 * All rights reserved.
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a SPI connection:
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  //Pin conncected to gate of MOSFET (connect through ~220 ohm resistor to ensure it doesn't try to pull too much current
  pinMode(9, OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(10); // for Leonardo/Micro/Zero
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    boolean correctRight = true;
    boolean correctLeft = true;
    boolean correctSpare = true;
    boolean correctVivo = true;
    uint8_t rightHand[] = {0x2, 0xAF ,0xF3 ,0x12 ,0xB2 ,0x80 ,0x12};
    uint8_t leftHand[] = {0x2, 0xAF ,0xF3 ,0x12 ,0xB2 ,0x80 ,0x12};
    uint8_t vivoHand[] = {0x2, 0xAF ,0xF3 ,0x12 ,0xB2 ,0x80 ,0x12};
    uint8_t spareCard[] = {0x4,0xCA ,0xF1 ,0x45};
    for (uint8_t i=0; i < uidLength; i++) 
    { if (uidLength >= 7) {
        correctSpare = false;
        if (rightHand[i] != uid[i]) correctRight = false;
        if (leftHand[i] != uid[i]) correctLeft = false;
        if (vivoHand[i] != uid[i]) correctVivo = false;
      } else if (uidLength >= 4) { 
        correctRight = false;
        correctLeft = false;
        correctVivo = false;
        if (spareCard[i] != uid[i]) correctSpare = false;
      } else {
        correctRight = false;
        correctLeft = false;
        correctVivo = false;
        correctSpare = false;
      }
      Serial.print(" 0x");Serial.print(uid[i], HEX); 
    }
    if(correctLeft || correctVivo || correctRight || correctSpare){
      Serial.print("\nCorrect read");
      digitalWrite(9, HIGH);
      //Allow user to open safe before relocking
      delay(4000);
      digitalWrite(9, LOW);
    }
    Serial.println("");
  // Wait 1 second before continuing
  delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}
