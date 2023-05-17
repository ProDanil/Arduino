/*
 * --------------------------------------------------------------------------------------------------------------------
 * Электронный замок с ключ-картой RFID-RC522
 * --------------------------------------------------------------------------------------------------------------------
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * -----------------------------------------------------------------------------------------
 * BUZZER                   3                       D3
 * RED LED                  4                       D4
 * GREEN LED                5                       D5
 * -----------------------------------------------------------------------------------------
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9                  // Configurable, see typical pin layout above
#define SS_PIN          10                 // Configurable, see typical pin layout above
#define BUZZER_PIN      3
#define RED_PIN         4
#define GREEN_PIN       5

MFRC522 mfrc522(SS_PIN, RST_PIN);          // Create MFRC522 instance

const String uid_code = "291285bf";        // Correct UID in HEX
String in_code = "";


//  Функция генерирующая звуковой сигнал на выводе BUZZER_PIN 
//  i - интенсивность, t - продолжительность
void myTone(uint32_t i, uint32_t t){ 
  t += millis(); 
  analogWrite(BUZZER_PIN, i);
  while (t > millis());                    // Do nothing while timer is running
  analogWrite(BUZZER_PIN, 0);
}


void unlock(){
  for (int i = 0; i < 10; i++){
    digitalWrite(GREEN_PIN, HIGH);
    myTone(10, 200);
    digitalWrite(GREEN_PIN, LOW);
    delay(100);
  }
}


void lock(){
  digitalWrite(RED_PIN, HIGH);
  myTone(10, 1000);
  delay(1000);
  digitalWrite(RED_PIN, LOW);
}


void setup() {
  Serial.begin(9600);		                   // Initialize serial communications with the PC

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  while (!Serial);		                     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();			                       // Init SPI bus

  mfrc522.PCD_Init();		                   // Init MFRC522
  delay(4);				                         // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();	     // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}


void loop() {

  static uint32_t rebootTimer = millis();  // Важный костыль против зависания модуля!
  if (millis() - rebootTimer >= 1000) {    // Таймер с периодом 1000 мс
    rebootTimer = millis();                // Обновляем таймер
    digitalWrite(RST_PIN, HIGH);           // Сбрасываем модуль
    delayMicroseconds(2);                  // Ждем 2 мкс
    digitalWrite(RST_PIN, LOW);            // Отпускаем сброс
    mfrc522.PCD_Init();                    // Инициализируем заного
  }

	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  for (uint8_t i = 0; i < 4; i++){
    in_code += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("UID: ");
  Serial.println(in_code);
  Serial.print("CORRECT: ");
  Serial.println(uid_code);
  Serial.println();

  if (in_code == uid_code){
    unlock();
  } else{
    lock();
  }
  in_code = "";

}
