#include <NewPing.h> // библиотека УЗ датчика

// пины УЗ датчика
#define PIN_TRIG 10
#define PIN_ECHO 11

#define PIN_VIBRO 9

#define MAX_DISTANCE 400 // максимальная дистанция

// создание объекта sonar
NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

float dist_arr_3[3] = {0.0, 0.0, 0.0}; // массив для хранения трёх последних измерений
float dist_middle, middle, dist_filtered, dist_aver, dist;
byte i;
byte vibro_val;
unsigned long sensTimer; // переменная для задержки

// медианный фильтр 3 порядка
float middle_of_3(float a, float b, float c){
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

// фильтр бегущее среднее
float moving_average(float filtered, float mid){
	float k, delta;
	delta = abs(filtered - mid);             // изменение значения
	if (delta > 1) k = 0.7;                  // если большое - резкий коэффициент
	else k = 0.1;                            // если маленькое - плавный

	filtered = mid * k + filtered * (1 - k); // фильтр

	return filtered;
}

void setup() {
	Serial.begin(9600);
	Serial.println("noFilt,middle,aver,filtered");
	pinMode(PIN_VIBRO, OUTPUT);
}

void loop() {
	// Стартовая задержка 50 мс
	if (millis() - sensTimer > 50){
		
		// счётчик для массива значений dist_arr_3
		if (i > 1) i = 0;
		else i++;

		dist_arr_3[i] = (float)sonar.ping()/57.5;                               // получить расстояние в текущую ячейку массива
    dist = dist_arr_3[i];
		dist_middle = middle_of_3(dist_arr_3[0], dist_arr_3[1], dist_arr_3[2]); // фильтрация медианным фильтром 3 порядка

		dist_aver = moving_average(dist_aver, dist_arr_3[i]);                   // фильтрация только бегущим средним
   
		dist_filtered = moving_average(dist_filtered, dist_middle);             // фильтрация бегущим средним

		// вывод сигнала на вибромотор
		//byte vibro_val;
		if (dist_filtered < 2) vibro_val = 0;
		else vibro_val = map(dist_filtered, 2, 400, 255, 0);
		analogWrite(PIN_VIBRO, vibro_val);

		sensTimer = millis();                                                   // сброс таймера
	}

	// Получаем значение с датчика
	// unsigned int distance = sonar.ping_cm();

	// Вывод значения
	Serial.print(dist);
	Serial.print("\t");
  Serial.print(dist_middle);
  Serial.print("\t");
  Serial.print(dist_aver);
  Serial.print("\t");
	Serial.println(dist_filtered);
	//Serial.println(dist_arr_3[i]);
//	Serial.println(" cm");
}
