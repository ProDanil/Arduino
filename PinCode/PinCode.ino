#define DECODE_NEC 1
#define IDC IrReceiver.decodedIRData.command
#include <IRremote.h>
#include <Servo.h>
//#include <LiquidCrystal_I2C.h>
//#include <Wire.h>

/* CH-(0x45)  CH(0x46)   CH+(0x47)
   Prev(0x44) Next(0x40) Play(0x43)
   -(0x7)     +(0x15)    EQ(0x9)
   0(0x16)    100+(0x19) 200+(0xD)
   1(0xC)     2(0x18)    3(0x5E)
   4(0x8)     5(0x1C)    6(0x5A)
   7(0x42)    8(0x52)    9(0x4A)*/ 

//LiquidCrystal_I2C lcd(0x27,16,2);
int ReceivePin = 11;
String pinCode = "13529";
String codeIn = "";
bool f = 0;
int red = 7;
int green = 6;
int buzzer = 9;
int servoPin = 3;
Servo Servo1;

void setup() {
    Servo1.attach(servoPin);
    Serial.begin(19200);
    // Запуск приемника, включение обратной связи LED  и снятие вывода обратной связи LED с внутренних плат.
    IrReceiver.begin(ReceivePin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    Serial.println(F("Введите пин-код "));
    //Serial.println(ReceivePin);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(buzzer, OUTPUT);
    Servo1.write(0);
    /*lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.blink();
    lcd.setCursor(17,0);
    lcd.print("Enter code");*/
}

void button(){
  if (!IrReceiver.decode()) {
    button();
  }
}

void buzz(){
  digitalWrite(buzzer, 100);
  delay(100);
  digitalWrite(buzzer, 0);
}

void greenLed(){
  digitalWrite(green, HIGH);
  Servo1.write(179);
  for (int j=1; j<=2; j++){
    digitalWrite(buzzer, 100);
    delay(2000);
    digitalWrite(buzzer, 0);
    delay(500);
  }
  digitalWrite(green, LOW);
  Servo1.write(0);
}

void redLed(){
  digitalWrite(red, HIGH);
  for (int j=1; j<=3; j++){
    digitalWrite(buzzer, 100);
    delay(200);
    digitalWrite(buzzer, 0);
    delay(200);
  }
  delay(3800);
  digitalWrite(red, LOW);
}

void loop() {
    /*lcd.setCursor(23,0);
    lcd.print("*****");
    lcd.setCursor(23,0);*/
    for (int i=1; i<=pinCode.length(); i++){
      button();
      switch (IDC) {
        case 0x45:
          Serial.println("CH-");
        case 0x46:
          Serial.println("CH");
        case 0x47:
          Serial.println("CH+");
        case 0x44:
          codeIn = ""; i = 6;
          f = 1; buzz(); delay(100);
          Serial.println();
          Serial.println("Введите пин-код");
          //lcd.print("*****"); lcd.setCursor(23,0);
          break;
        case 0x40:
          Serial.println("Next");
        case 0x43:
          Serial.println("Play");
        case 0x7:
          Serial.println("-");
        case 0x15:
          Serial.println("+");
        case 0x9:
          Serial.println("EQ");
        case 0x16:
          Serial.print("0"); //lcd.print("0");
          codeIn += 0;  break;
        case 0x19:
          Serial.println("100+");
        case 0xD:
          Serial.println("200+");
        case 0xC:
          Serial.print("1"); //lcd.print("1");
          codeIn += 1; break;
        case 0x18:
          Serial.print("2"); //lcd.print("2");
          codeIn += 2; break;
        case 0x5E:
          Serial.print("3"); //lcd.print("3");
          codeIn += 3; break;
        case 0x8:
          Serial.print("4"); //lcd.print("4");
          codeIn += 4; break;
        case 0x1C:
          Serial.print("5"); //lcd.print("5");
          codeIn += 5; break;
        case 0x5A:
          Serial.print("6"); //lcd.print("6");
          codeIn += 6; break;
        case 0x42:
          Serial.print("7"); //lcd.print("7");
          codeIn += 7; break;
        case 0x52:
          Serial.print("8"); //lcd.print("8");
          codeIn += 8; break;
        case 0x4A:
          Serial.print("9"); //lcd.print("9");
          codeIn += 9; break;
      }
      buzz();
      delay(900);
      IrReceiver.resume();
    }
    
    if (String(codeIn) == pinCode){
      Serial.println(" Открыто"); //lcd.setCursor(29,0); lcd.print("OPEN"); 
      codeIn = "";
      greenLed();
    } else if (!f){ Serial.println(" Закрыто"); codeIn = ""; redLed();} //дlcd.setCursor(29,0); lcd.print("CLOSE");
    f = 0;
    
}
