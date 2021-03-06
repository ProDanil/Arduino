#include <NewPing.h> // библиотека УЗ датчика

#define MAX_DISTANCE 400 // максимальная дистанция

/*-------------------------ПИНЫ--------------------------*/ 
// УЗ датчики                  № 1   2   3   4   5   6
const unsigned int pin_trig[] = {12, 11, 9, 7, 5, 3};
const unsigned int pin_echo[] = {12, 10, 8, 6, 4, 2};

// Вибромоторы                 № 1  2  3  4  5  6
//const unsigned int pin_vibro[] = {2, 3, 4, 5, 6, 7};
/*-------------------------------------------------------*/

// УЗ датчики                  № 1   2   3   4   5   6
//const unsigned int pin_trig[] = {22, 24, 26, 28, 30, 32};
//const unsigned int pin_echo[] = {23, 25, 27, 29, 31, 33};

// Вибромоторы                 № 1  2  3  4  5  6
//const unsigned int pin_vibro[] = {2, 3, 4, 5, 6, 7};
// создание объекта sonar
NewPing sonar[6] = {
                    NewPing(pin_trig[0], pin_echo[0], MAX_DISTANCE),
                    NewPing(pin_trig[1], pin_echo[1], MAX_DISTANCE),
                    NewPing(pin_trig[2], pin_echo[2], MAX_DISTANCE),
                    NewPing(pin_trig[3], pin_echo[3], MAX_DISTANCE),
                    NewPing(pin_trig[4], pin_echo[4], MAX_DISTANCE),
                    NewPing(pin_trig[5], pin_echo[5], MAX_DISTANCE),
                   };

// массив для хранения трёх последних измерений для 6 датчиков
float dist_arr_3[6][3] = {
                          {0.0, 0.0, 0.0},  // датчик №1
                          {0.0, 0.0, 0.0},  // датчик №2
                          {0.0, 0.0, 0.0},  // датчик №3
                          {0.0, 0.0, 0.0},  // датчик №4
                          {0.0, 0.0, 0.0},  // датчик №5
                          {0.0, 0.0, 0.0},  // датчик №6
                         }; 
float dist_filtered[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float dist_middle;
byte i, j;
byte vibro_val;
unsigned long sensTimer; // переменная для задержки


// медианный фильтр 3 порядка
float middle_of_3(float a, float b, float c){
  float middle;
  if ((a <= b) && (a <= c)){
    middle = (b <= c) ? b : c;
  
  } else {
    if ((b <= a) && (b <= c)){
      middle = (a <= c) ? a : c;
    
    } else {
      middle = (a <= b) ? a : b;
    }
  }
  
  return middle;
}


// фильтр скользящее среднее
float moving_average(float filtered, float mid){
  float k, delta;
  delta = abs(filtered - mid);             // изменение значения
  if (delta > 1) k = 0.7;                  // если большое - резкий коэффициент
  else k = 0.1;                            // если маленькое - плавный

  filtered = mid * k + filtered * (1 - k); // фильтр

  return filtered;
}


void setup() {
  Serial.println("1,2,3,4,5,6");
  Serial.begin(9600);
  for (int k = 0; k < 6; k++){
    //pinMode(pin_vibro, OUTPUT);  
  }
}


void loop() {
  // Стартовая задержка 10 мс
  if (millis() - sensTimer > 10){
    
    // счётчик для массива значений dist_arr_3
    if (i > 4) {i = 0; j++;}
    else i++;
    if (j > 1) j = 0;

    // получить расстояние с датчика i в текущую ячейку массива j
    dist_arr_3[i][j] = (float)sonar[i].ping()/57.5;

    // фильтрация i датчика
    dist_middle = middle_of_3(dist_arr_3[i][0], dist_arr_3[i][1], dist_arr_3[i][2]);  // медианным фильтром 3 порядка
    dist_filtered[i] = moving_average(dist_filtered[i], dist_middle);                       // скользящим средним

    //Serial.print(i);
    //Serial.print("     ");
    //Serial.println(dist_filtered[i]);
    // вывод сигнала на вибромотор
    //if (dist_filtered[i] < 2) vibro_val = 0;
    //else vibro_val = map(dist_filtered[i], 2, 400, 255, 0);
    //analogWrite(pin_vibro[i], vibro_val);
    Serial.print(millis());
    Serial.print(",");
    Serial.print(dist_filtered[0]);
    Serial.print(",");
    Serial.print(dist_filtered[1]);
    Serial.print(",");
    Serial.print(dist_filtered[2]);
    Serial.print(",");
    Serial.print(dist_filtered[3]);
    Serial.print(",");
    Serial.print(dist_filtered[4]);
    Serial.print(",");
    Serial.println(dist_filtered[5]);  
    // сброс таймера
    sensTimer = millis();
  }
}
