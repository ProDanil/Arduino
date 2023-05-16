#include <IRremote.hpp> 

constexpr uint8_t RECV_PIN {2};             // указываем вывод, к которому подключен ИК приёмник 
constexpr uint8_t BUZZER_PIN {9};           // пин пищалки
constexpr uint8_t RED_PIN {4};
constexpr uint8_t GREEN_PIN {5};
const String pin_code = "1234";
String in_code = "";


uint16_t irReceive() {
  uint16_t received{0};
  
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      received = IrReceiver.decodedIRData.command;
    }
    if (IrReceiver.decodedIRData.protocol == NEC) {
      received = IrReceiver.decodedIRData.command;
      Serial.print("Command: 0x");
      Serial.println(received, HEX);
    }
    IrReceiver.resume();                    // принимаем след команду пульта
  }
  return received;
}


//  Функция генерирующая звуковой сигнал на выводе BUZZER_PIN 
//  i - интенсивность, t - продолжительность
void myTone(uint32_t i, uint32_t t){ 
  t += millis(); 
  analogWrite(BUZZER_PIN, i);
  while (t > millis()){
    // digitalWrite(BUZZER_PIN, HIGH); 
    // delayMicroseconds(500); 
    // digitalWrite(BUZZER_PIN, LOW); 
    // delayMicroseconds(500);
  } 
  analogWrite(BUZZER_PIN, 0);
}


void open(){
  for (int i = 0; i < 10; i++){
    digitalWrite(GREEN_PIN, HIGH);
    myTone(200, 200);
    digitalWrite(GREEN_PIN, LOW);
    delay(100);
  }
}


void close(){
  digitalWrite(RED_PIN, HIGH);
  myTone(200, 1000);
  delay(1000);
  digitalWrite(RED_PIN, LOW);
}


void setup() 
{
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Проверка 
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(RED_PIN, HIGH);
  myTone(100, 500);

  IrReceiver.begin(RECV_PIN);               // запускаем приём

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(RECV_PIN);

  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
}


void loop() 
{
  uint16_t result = irReceive();
  
  if (result) {                             // если данные пришли
    Serial.println(result);
    if (16 <= result && result <= 25){      // работа с нужным диапазоном кнопок 
      result -= 16;
      myTone(100, 100);
      delay(100);
      // Serial.print("Нажата кнопка ");
      // Serial.println(result);
      in_code += String(result);
      Serial.print("ВВОД: ");
      Serial.println(in_code);  
    }
    
    delay(500);
  }

  if (in_code.length() == pin_code.length()){
    if (in_code == pin_code){
      Serial.println("*** Доступ разрешён ***");
      open();
    } else{
      Serial.println("*** Неверный пароль ***");
      close();
    }
    in_code = "";
  }
}
