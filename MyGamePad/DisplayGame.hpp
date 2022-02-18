#ifndef DisplayGame_hpp
#define DisplayGame_hpp

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой

/*
- home() и clear() – первая функция позволяет вернуть курсор в начало экрана, вторая тоже, но при этом удаляет все, что было на мониторе до этого.
- write(ch) – позволяет вывести одиночный символ ch на экран.
- cursor() и noCursor() – показывает/скрывает курсор на экране.
- blink() и noBlink() – курсор мигает/не мигает (если до этого было включено его отображение).
- display() и noDisplay() – позволяет подключить/отключить дисплей.
- scrollDisplayLeft() и scrollDisplayRight() – прокручивает экран на один знак влево/вправо.
- autoscroll() и noAutoscroll() – позволяет включить/выключить режим автопрокручивания. В этом режиме каждый новый символ записывается в одном и том же месте, вытесняя ранее написанное на экране.
- leftToRight() и rightToLeft() – Установка направление выводимого текста – слева направо или справа налево.
- createChar(ch, bitmap) – создает символ с кодом ch (0 – 7), используя массив битовых масок bitmap для создания черных и белых точек.
*/
struct Display{
	unsigned long time;
	int sec;
	bool flag;
	byte select_game_pos;
	byte select_level_pos;
	int sum_step;
	byte game_list = 1;
	char sel_game;

	void init(){
		lcd.init();         // Инициализация дисплея
		lcd.backlight();    // Подключение подсветки
		lcd.cursor();
		lcd.blink();
		lcd.setCursor(0,0); // Установка курсора в начало первой строки
		lcd.print("Hello Everyone!");
	};

	void timer_start(){
        time = millis();
        lcd.clear();
        lcd.noCursor();
        lcd.noBlink();
        lcd.print("00:00");
    }

    void timer(){
        /*if (!flag){
            time = millis();
            flag = true;
        }*/
		if (millis() - time > 1000){
			time = millis();
			sec += 1;
			lcd.setCursor(0,0);
			String min10 = "0";
			String sec10 = "0";

			if (sec/60 > 9){
				min10 = "";
			}
			if (sec - 60*(sec/60) > 9){
				sec10 = "";
			}

			lcd.print(min10 + String(sec/60) + ":" + sec10 + String(sec - 60*(sec/60)));
		}
	};

    void timer_stop(){
        flag = false;
        sec = 0;
    }

	void game_select(byte sum_game, const char *game_names[], int Jx, bool but){
		if(!flag){
			lcd.clear();
			lcd.print("Select game:");
			lcd.setCursor(0,1);
			lcd.print("1:"+ String(game_names[0])+" 2:"+ String(game_names[1]));
			if (sum_game > 2){
				lcd.print(" >");
			};
			lcd.setCursor(0,1);
			flag = true;
			select_game_pos = 0;
		}

		if (Jx < 70){
			if (select_game_pos == 0 && game_list != 1){
				game_list -= 1;
				select_game_pos = 7;
				lcd.setCursor(0,1);
				lcd.print(String(game_list*2-1)+":"+ String(game_names[game_list*2-2])+" "+
						  String(game_list*2) + ":"+ String(game_names[game_list*2-1]));
				lcd.setCursor(0,1);
			} else if (select_game_pos > 0){
				select_game_pos = 0;
			}
		} else if (Jx > 130){
			if (select_game_pos == 7 && sum_game > 2){
				game_list += 1;
				select_game_pos = 0;
				lcd.setCursor(0,1);
				lcd.print(String(game_list*2-1)+":"+ String(game_names[game_list*2-2])+" "+
						  String(game_list*2) + ":"+ String(game_names[game_list*2-1]));
				lcd.setCursor(0,1);
			} else if (select_game_pos == 0){
				select_game_pos += 7;
			}
		}

		lcd.setCursor(select_game_pos,1);

		if (but){
			lcd.clear();
			sel_game = game_names[select_game_pos/7+game_list*2-2];
			select_game_pos = 0;
			game_list = 1;
			flag = false;
		}
	};

	void level_select(byte sum_lev, int Jx, bool but){
		if(!flag){
			lcd.clear();
			lcd.print("Select level:");
			lcd.setCursor(0,1);
			for (int i = 0; i < sum_lev; i++){
				lcd.print(String(i) + " ");
			}
			lcd.setCursor(0,1);
			flag = true;
			select_level_pos = 0;
		}

		if (Jx < 70){
			if (select_level_pos > 0){
				select_level_pos -= 2;
			}
		} else if (Jx > 130){
			if(select_level_pos < (sum_lev-1)*2){
				select_level_pos += 2;
			}
		}

		lcd.setCursor(select_level_pos,1);

		if (but){
			lcd.clear();
			flag = false;
		}
	};

	void step_count(bool step){
		String step10 = "0";
		String step100 = "0";
		if(step){
			sum_step += 1;
		}
		if (sum_step > 9){
			step10 = "";
		}
		if (sum_step > 99){
			step100 = "";
		}
		lcd.setCursor(13,0);
		lcd.print(step100 + step10 + String(sum_step));

	};
};
/*bool fl = false;
bool answer = false;
int Jx;
unsigned long timer;
int sec = 0;

void setup(){
	Serial.begin(9600);
	pinMode(BUT, INPUT_PULLUP);
	lcd.init();         // Инициализация дисплея
	lcd.backlight();    // Подключение подсветки
	lcd.cursor();
	lcd.blink();
	lcd.setCursor(0,0); // Установка курсора в начало первой строки
	lcd.print("Hello!"); // Набор текста на первой строке
	delay(3000);
	lcd.clear();
	lcd.print("0" + String(sec/60) + ":0" + String(sec));
	timer = millis();
}

void loop(){
////// Set timer
	if (millis() - timer > 1000){
		timer = millis();
		sec += 1;
		lcd.setCursor(0,0);
		String min10 = "0";
		String sec10 = "0";

		if (sec/60 > 9){
			min10 = "";
		}
		if (sec - 60*(sec/60) > 9){
			sec10 = "";
		}

		lcd.print(min10 + String(sec/60) + ":" + sec10 + String(sec - 60*(sec/60)));
	}

////// Question with choice of answer
	/*byte b = !digitalRead(BUT);
  	int X = analogRead(A0);
  	Jx = map(X, 0, 1023, 0, 201);
  	delay(30);
  	Serial.println(Jx);

	if (!fl){
		lcd.clear();
		lcd.print("D U want t exit?");
		lcd.setCursor(0,1);
		lcd.print("Y N");
		lcd.setCursor(0,1);
		fl = true;
	}

	if (Jx < 70){
		lcd.setCursor(0,1);
		answer = true;
	} else if (Jx > 130){
		lcd.setCursor(2,1);
		answer = false;
	}

	if (b && answer){
		lcd.clear();
		lcd.print("GoodBye!");
		delay(3000);
		fl = false;
	} else if (b && !answer){
		lcd.clear();
		lcd.print("Hello!");
		delay(3000);
		fl = false;
	}
}
*/
#endif
