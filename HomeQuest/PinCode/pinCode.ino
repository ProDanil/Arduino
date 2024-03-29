/*
 * --------------------------------------------------------------------------------------------------------------------
 * Электронный замок на матричной клавиатуре 4х4
 * --------------------------------------------------------------------------------------------------------------------
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             Arduino
 *             Uno
 * Signal      Pin
 * -----------------------------------------------------------------------------------------
 * COL 4       11         
 * COL 3       10  
 * COL 2       9
 * COL 1       8
 * ROW 1       7
 * ROW 2       6
 * ROW 3       5
 * ROW 4       4   
 * -----------------------------------------------------------------------------------------
 * SERVO       3
 * RED LED     2
 * GREEN LED   12
 * -----------------------------------------------------------------------------------------
 */

#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4;
const byte COLS = 4;
constexpr uint8_t SERVO_PIN {3};
constexpr uint8_t RED_PIN {2};
constexpr uint8_t GREEN_PIN {12};
const String pin_code = "1234";
String in_code = "";

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {8, 9, 10, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servo;


void unlock(){
  Serial.println("*** Доступ разрешён ***");
  digitalWrite(GREEN_PIN, HIGH);
  servo.write(175);
  delay(3000);
  servo.write(5);
  digitalWrite(GREEN_PIN, LOW);
}


void lock(){
  Serial.println("*** Неверный пароль ***");
  for (int i = 0; i < 3; i++){
    digitalWrite(RED_PIN, HIGH);
    delay(300);
    digitalWrite(RED_PIN, LOW);
    delay(300);
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  delay(1000);
  servo.write(5);
}

void loop() {
  char key = keypad.getKey();
  
  if (key){
    digitalWrite(GREEN_PIN, HIGH);
    in_code += String(key);
    Serial.print("ВВОД: ");
    Serial.println(in_code); 
    delay(100);
    digitalWrite(GREEN_PIN, LOW); 
  }

  if (in_code.length() == pin_code.length()){
    if (in_code == pin_code){
      unlock();
    } else{
      lock();
    }
    in_code = "";
  }
}
