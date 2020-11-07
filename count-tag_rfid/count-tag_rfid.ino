#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
 
#define SS_PIN 53
#define RST_PIN 5
 
#define BEEP 6
#define BUTTON 7
 
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Adafruit_PCD8544 display = Adafruit_PCD8544(22, 23, 24, 25, 26);
 
bool resetMode = false;
 
void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; // Key used is FF FF FF FF FF FF
  }
 
  // Init LCD
  display.begin();
  display.setContrast(50);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setRotation(2); // using it upside down
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
 
  pinMode(BEEP, OUTPUT);
  pinMode(BUTTON, INPUT);
  display.println("Press the bu-tton to reset the counter ");
  display.display();
}
 
void loop() {
 
  if ( digitalRead(BUTTON) == 1) {
    //reset mode
    display.clearDisplay();
    display.println("Swipe the ca-rd to reset  the counter ");
    display.display();
    resetMode = true;
    return;
  }
 
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // Verify if the UID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 
  // Check for compatibility
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
          &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
          &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("This only works with MIFARE Classic cards."));
    return;
  }
 
  // Use sector #1 (Block #4 - #7) to store data, to be specific Block #4
  byte sector = 1;
  byte blockAddr = 4;
  byte trailerBlock = 7;
 
  // Initialize data to write
  byte dataBlock[16];
  for (int i = 0; i < 16; i++)
    dataBlock[i] = 0x00;
 
  // Initialize array to store the result
  byte buffer[18];
  byte size = sizeof(buffer);
 
  // Initialize variable for storing counter
  byte count = 0;
 
  MFRC522::StatusCode status;
 
  // Using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode) rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }
 
  if (resetMode) {
    // Reset mode
    status = (MFRC522::StatusCode) rfid.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
    }
  } else {
    // Counter mode
 
    //Read the number first
    status = (MFRC522::StatusCode) rfid.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
    }
    count = buffer[15];
 
    //INC
    count++;
 
    //Write the number back
    dataBlock[15] = count;
    status = (MFRC522::StatusCode) rfid.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
    }
  }
 
  //Read the number again
  status = (MFRC522::StatusCode) rfid.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(rfid.GetStatusCodeName(status));
  }
  count = buffer[15];
 
  //Turn on the buzzer
  digitalWrite(BEEP, HIGH);
 
  // Print the data to serial
  SerialPrintHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.print(" ");
  Serial.println(count);
 
  // Print the data to LCD
  display.clearDisplay();
  display.println("Press the bu-tton to reset the counter ");
  display.print("UID: ");
  lcdPrintHex(rfid.uid.uidByte, rfid.uid.size);
  display.println();
  display.print("Counter: ");
  display.println(count);
  display.display();
 
  //Turn off the buzzer
  digitalWrite(BEEP, LOW);
 
  //Beep twice if is in reset mode
  if (resetMode) {
    delay(50);
    digitalWrite(BEEP, HIGH);
    delay(100);
    digitalWrite(BEEP, LOW);
    resetMode = false;
  }
  // Halt PICC
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
 
// Function for print byte array in hex
void SerialPrintHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}
 
void lcdPrintHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    display.print(buffer[i] < 0x10 ? "0" : "");
    display.print(buffer[i], HEX);
    display.display();
  }
}
