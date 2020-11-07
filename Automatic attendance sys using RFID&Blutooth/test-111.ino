

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
 
#define SS_PIN 10
#define RST_PIN 9
const int buzzer = 8; //buzzer to arduino pin 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(3, 5);//Create a serial connection with TX and RX on these pins

//SoftwareSerial serial_connection2(7, 8);//Create a serial connection with TX and RX on these pins


#define BUFFER_SIZE 64//This will prevent buffer overruns.

char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.

char inChar=-1;//Initialie the first character as nothing

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, A4, 4, A3, A2);
int total = 0;
int c=0; 
boolean flag;
void setup() 
{
   serial_connection.begin(9600);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //Serial.println("Approximate your card to the reader...");
  Serial.println();
  serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.

  Serial.println("Started");//Tell the serial monitor that the sketch has started.
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print(" Scan YOUR RFID ");
  // Print a message to the LCD.
 // lcd.print("hello, world!");

}
void loop() 
{ // byte byte_count=serial_connection.available();
 // lcd.setCursor(0,1);
 // lcd.print(millis()/1000);
   // Turn off the display:
  //lcd.noDisplay();
  //delay(500);
  // Turn on the display:
  lcd.display();
  delay(500);
  
  // Look for new cards
 
  
 // if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
 while(true)
{  
 
   if (digitalRead(A0) == LOW)
  {
    while (digitalRead(A1) == HIGH) { }
    Serial.print("Person entered there are ");
    total++;
    //show();
    Serial.print(total);
    Serial.println(" people in room.");
    lcd.setCursor(1,1);
    lcd.print("N:");
    lcd.print(total);
    delay(1000);
  } 
  
  if (digitalRead(A1) == LOW) 
  {
    while (digitalRead(A0) == HIGH) {}
    Serial.print("Person exited there are ");
    total--;
    c--;
    //show();
    if (total >-1|| c>-1)
     {
      Serial.print(total);
     Serial.println(" people in room.");
     Serial.print("number of rfid : ");
     Serial.println(c);
     lcd.setCursor(1,1);
    lcd.print("N:");
    lcd.print(total);
    delay(1000);
     }
    else
      {
     total=0;
     c=0;
     Serial.print(total);
     Serial.println(" people in room.");
     Serial.print("number of rfid : ");
     Serial.println(c);
     lcd.setCursor(1,1);
    lcd.print("N:");
    lcd.print(total);
      delay(1000);
   
      }
  } 

   if (c!=total)
 {  
  //delay(1000);
  Serial.println("error");
  tone(buzzer, 5000); // Send 1KHz sound signal...
  //delay(1000);        // ...for 1 sec
  //noTone(buzzer);     // Stop sound...
 // delay(1000);        // ...for 1sec
  lcd.print("Error");
  flag=false;
   
 }
  else if (c==total){
    noTone(buzzer);
     flag=true;
  }

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  lcd.setCursor(0,0);
  Serial.print("ID: ");
  lcd.print("ID: ");
  String content= "";
  byte letter;
  Serial.println(c);
  Serial.println(total);
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     if (c==total && c!=0 ){
       serial_connection.println(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
       serial_connection.println(mfrc522.uid.uidByte[i], HEX);
     }
  //   serial_connection.println("11");
     lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
     lcd.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  //Serial.print("Message : ");
  lcd.print("       ");
  c++;
 
  if (c>-1)
  {
  Serial.print("number of rfid : ");
  Serial.println(c);
    
   }
   else
   {
    c=0;
    Serial.print("number of rfid : ");
    Serial.println(c);
    }
  content.toUpperCase();
  // delay(5000);


  delay(3000);
  }
} 
