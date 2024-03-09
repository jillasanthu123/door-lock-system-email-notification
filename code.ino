#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>
#include <EMailSender.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Wi-Fi credentials
const char* ssid ="Andhrajyothi";
const char* password ="AP28BK7961";

// RFID PINOUT
#define SS_PIN 5
#define RST_PIN 14

// servo
Servo myservo; 
const int servoPin = 2; 

// LCD 
const int sdaPin = 21; 
const int sclPin = 22;

//relay
const int relayPin = 12;

// buzzer
const int buzzerPin = 13; 

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

// mail id pass
EMailSender emailSend("20ag1a6926@gmail.com", "mrog omyw oyhr wbuc"); //mail



void setup() {
  Serial.begin(9600);   // Initialize serial communications
  SPI.begin();          // Initialize SPI bus
  
  //relay pin
  pinMode(relayPin,OUTPUT);
  
  // buzzer pin
  pinMode(buzzerPin, OUTPUT);

  //servo PIN
  myservo.attach(servoPin);


  // LCD START
  Wire.begin(sdaPin, sclPin); // LCD INITIALIZE
  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, INPUT_PULLUP);
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print("Connecting to wifi:");
 
  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi connectted"); 
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Wifi connectted");
  
  //RFID INITIALIZE
  mfrc522.PCD_Init();    // Initialize MFRC522
  Serial.println("Ready to read RFID cards/n");


}

void loop() {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("WELCOME");
  lcd.setCursor(0,1);
  lcd.print("PLACE YOUR CARD");
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { // If a new card is present and read successfully
    Serial.print("Card UID: ");   // Print UID
    byte uid[4];
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid[i] = mfrc522.uid.uidByte[i];
      Serial.print(uid[i], HEX);
      Serial.print("");
    }

    
// MESSAGES THAT SHOLD BE SENT WITH THE SUBJECT AFTER DETECTION OF THE CARD
    EMailSender::EMailMessage message1;
    message1.subject = "DOOR LOCK SYSTEM";
    message1.message = "DOOR HAS BEEN UNLOCKED BY 1ST CARD";

    EMailSender::EMailMessage message2;
    message2.subject = "DOOR LOCK SYSTEM";
    message2.message = "DOOR HAS BEEN UNLOCKED BY 2ND CARD";

    EMailSender::EMailMessage message3;
    message3.subject = "DOOR LOCK SYSTEM";
    message3.message = "DOOR HAS BEEN UNLOCKED BY 3ND CARD";

    EMailSender::EMailMessage message4;
    message4.subject = "DOOR LOCK SYSTEM";
    message4.message = "UNAUTHORIZED PERSON WANT TO UNLOCK THE DOOR PLEASE CHECK THE PERSON";

    
    //Define expected UIDs of rfid
    byte a[]={0x93,0xA5,0x55,0xED};
    byte b[]={0x23,0x5C,0xAC,0xFD};
    byte c[]={0x7A,0xA7,0x9A,0x80};


    //COMPARE UID WITH EXPECTED
    
    if (compareUID(uid, a)) {
      EMailSender::Response resp1 = emailSend.send("jillasanthosh0852@gmail.com", message1);
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("HELLO");
      lcd.setCursor(2,1);
      lcd.print("USER NAME 1");
      Serial.println("Sending status: /n");
      Serial.println(resp1.status);
      digitalWrite(relayPin, HIGH);
      delay(100);
      myservo.write(100);
      delay(5000);
      myservo.write(0);
      delay(100);
      digitalWrite(relayPin, LOW);
      lcd.clear();
    }
    
     else if (compareUID(uid, b)) {
      EMailSender::Response resp2 = emailSend.send("jillasanthosh0852@gmail.com", message2);
      lcd.clear();
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("HELLO");
      lcd.setCursor(2,1);
      lcd.print("USER NAME 2");    
      Serial.println("Sending status: ");
      Serial.println(resp2.status);
      digitalWrite(relayPin, HIGH);
      delay(100);
      myservo.write(100);
      delay(5000);
      myservo.write(0);
      delay(100);
      digitalWrite(relayPin, LOW);
      lcd.clear();
      }
      
     else if (compareUID(uid, c)) {
      EMailSender::Response resp3 = emailSend.send("jillasanthosh0852@gmail.com", message3);
      lcd.clear();
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("HELLO");
      lcd.setCursor(2,1);
      lcd.print("USER NAME 3");      
      Serial.println("Sending status: ");
      Serial.println(resp3.status);
      digitalWrite(relayPin, HIGH);
      delay(100);
      myservo.write(100);
      delay(5000);
      myservo.write(0);
      delay(100);
      digitalWrite(relayPin, LOW);
      lcd.clear();
    }
     else {
      EMailSender::Response resp4 = emailSend.send("jillasanthosh0852@gmail.com", message4);
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("UNAUTHORIZED");    
      lcd.setCursor(0,1);
      lcd.print("TRY AGAIN");
      Serial.println("Sending status: ");
      Serial.println(resp4.status);
      digitalWrite(buzzerPin, HIGH);
      delay(2000);
      digitalWrite(buzzerPin, LOW);
    }
  }
  delay(1000); // Wait 1 second before trying again
}
bool compareUID(byte* uid1, byte* uid2) {
  for (byte i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
return true;
}
