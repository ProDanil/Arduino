//  TODO: more level, big level with move screen

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define BUTTON_PIN 8
#define NUMBER_OF_LEVELS 5
#define DELAY_BLINK 500 // время мерцания игрока

int pinCS = 9; //Указываем к какому выводу подключен контакт CS
int numHDis = 1; //Количество матриц по горизонтали
int numVDis = 4; //Количество матриц по вертикали
Max72xxPanel matrix = Max72xxPanel(pinCS, numHDis, numVDis);

int del = 90; //задержка для задания скорости Игрока
int x, y; //координаты Игрока
int Jx, Jy; //координаты Джойстика
byte levelMap[4][8]; //массив уровня
char number[] = {'0','1','2','3','4','5','6','7','8','9'};
bool go_menu = false; // флаг для перехода в меню
long blink_timer = 0;
bool gamer_high = true;

enum NumLev {
		START,
		LEV0,
		LEV1,
		LEV2,
		LEV3,
		LEV4,
		//LEV5
		END
	} num_lev;

void setup()
{
	Serial.begin(9600);
  matrix.setIntensity(1); //Яркость от 0 до 15
  matrix.setRotation(3); //Направление текста 1, 2, 3, 4
  pinMode(BUTTON_PIN, INPUT_PULLUP);
	matrix.fillScreen(LOW);
  matrix.write();
	num_lev = START;
}

class Level {
public:
  Level(){};
  byte startGamer[2];

  void next_level(){
    num_lev = num_lev+1;
  }

  void prev_level(){
    num_lev = num_lev-1;
  }

  void mapping(){ // создание карты уровня
    if (num_lev == 1){
      byte levelMap1[4][8] = 
      {
        {B00000000, B11111111, B00000001, B00000001, B00000001, B00000001, B11111111, B00000000},
        {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000},
        {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000},
        {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000}
      };
      startGamer[0] = 3;
      startGamer[1] = 3;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
          levelMap[i][j] = levelMap1[i][j];
        }
      }
    } else if (num_lev == 2){
        byte levelMap1[4][8] = 
      {
        {B11111111, B00000001, B00000001, B00001111, B10000001, B10000001, B10000001, B11111111},
        {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111},
        {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111},
        {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111}
      };
      startGamer[0] = 2;
      startGamer[1] = 5;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
          levelMap[i][j] = levelMap1[i][j];
        }
      }
    } else if (num_lev == 3){
      byte levelMap1[4][8] = 
      {
        {B11111111, B01000001, B01000001, B01001111, B01000001, B01110001, B00000001, B11111111},
        {B11111111, B00000000, B00111111, B00100001, B00101001, B11101011, B10001000, B11111111},
        {B11111111, B00010001, B01010101, B01010101, B01000100, B11111110, B00000000, B11111111},
        {B11111111, B00100001, B10100101, B10111101, B10000101, B11110101, B10000000, B11111111}
      };
      startGamer[0] = 2;
      startGamer[1] = 2;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
          levelMap[i][j] = levelMap1[i][j];
        }
      }
    } else if (num_lev == 4){
      byte levelMap1[4][8] = 
      {
        {B11100001, B11100001, B11100001, B00000001, B00000001, B00000001, B11100001, B11100001},
        {B00111000, B00111000, B10111000, B10000111, B00000111, B00000111, B10111000, B10111000},
        {B01100110, B01100110, B00011001, B00011001, B01100110, B01100110, B00011001, B00011001},
        {B00001010, B00010101, B00001010, B00010101, B00001010, B00010101, B00001010, B00010101}
       };
      startGamer[0] = 2;
      startGamer[1] = 3;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
          levelMap[i][j] = levelMap1[i][j];
        }
      }
    } else if (num_lev == 5){
      byte levelMap1[4][8] = 
      {
        {B11011111, B01000001, B01000001, B01000001, B11111111, B00000001, B00000001, B11011111},
        {B11100100, B00000100, B11111100, B00100000, B00100111, B00100100, B00100100, B00100100},
        {B11011111, B01000100, B01000100, B01000100, B01000100, B01000111, B01100000, B00110000},
        {B10011111, B00010000, B00010000, B10010010, B10010010, B10000010, B10000010, B10000010}
       };
      startGamer[0] = 3;
      startGamer[1] = 2;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
          levelMap[i][j] = levelMap1[i][j];
        }
      }
    }
    
    if (num_lev != 0){
      drow();
    }
  }

  void restart(){
    unsigned long timer = millis();
    matrix.drawPixel(x, y, LOW);
    x = startGamer[0];
    y = startGamer[1];
    matrix.drawPixel(x, y, HIGH);
    matrix.write();
    while(!digitalRead(BUTTON_PIN)==1){
      if (millis()-timer >= 3000){
        go_menu = true;
        return;
      }
    }
  }

private:
  void drow(){
  	matrix.fillScreen(LOW);
    x = startGamer[0];
    y = startGamer[1];
    for (int ii = 0; ii < 4; ii++){
      for (int j = 0; j < 8; j++ ) {            // Передача массива уровня
        for (int i = 0; i < 8; i++ ) {
          matrix.drawPixel((i+8*ii), j, levelMap[ii][j] & (1 << i));
        }
      }
    }
    matrix.drawPixel(x, y, HIGH);
    matrix.write();
    delay(100);
  }
};

Level level;

class Menu {
public:
	Menu(){};

  void opening(){
    matrix.fillScreen(LOW);
		for (int i = 0; i < NUMBER_OF_LEVELS; i++){
    	matrix.drawChar(i*8+1,0,number[i],1,0,1);
  	}

  	level.next_level();
  	x = 6;
  	y = 6;
  	matrix.drawPixel(x, y, HIGH);
  	matrix.write();
  	choice();
	};

private:
	void choice(){
    delay(1000);
		byte b = !digitalRead(BUTTON_PIN);
		int num_list = 0;
  		
  	while (b == 0){
    	int X = analogRead(A0);
    	Jx = map(X, 0, 1023, 0, 201);

    	if (Jx < 70){
    		matrix.drawPixel(x, y, LOW);
      		
      	if (x!=6){
       		x = x - 8;
       		level.prev_level();
      	} else if(num_list!=0){
      		matrix.fillScreen(LOW);
      		num_list-=1;
      		x = 30;
      		level.prev_level();
      		for (int i = 0+num_list*4; i < NUMBER_OF_LEVELS; i++){
    				matrix.drawChar((i-4*num_list)*8+1,0,number[i],1,0,1);
  				}
      	}
    		
    	} else if (Jx > 130){
    		matrix.drawPixel(x, y, LOW);

      	if (x!=6+(NUMBER_OF_LEVELS-1-4*num_list)*8 && x!=30){
       		x = x + 8;
       		level.next_level();
      	} else if (x!=6+(NUMBER_OF_LEVELS-1-4*num_list)*8){
      		matrix.fillScreen(LOW);
      		num_list+=1;
      		x = 6;
      		level.next_level();
      		for (int i = 0+num_list*4; i < NUMBER_OF_LEVELS; i++){
    				matrix.drawChar((i-4*num_list)*8+1,0,number[i],1,0,1);
  				}
      	}
    	}

    	matrix.drawPixel(x, y, HIGH);
    	matrix.write();
   		b = !digitalRead(BUTTON_PIN);
    	delay(200);
  	}

 		while (b == 1){
 			b = !digitalRead(BUTTON_PIN);
 		}
 		level.mapping();

	}

};

Menu menu;

class GamerMove {
public: 
	Move();

	void go(){

  		if (Jx < 70 && Jy < 70){
    		left_diag_up();

  		} else if (Jx < 70 && Jy > 130){
    		left_diag_down();

  		} else if (Jx > 130 && Jy > 130){
  		  	right_diag_down();

  		} else if (Jx > 130 && Jy < 70){
  		  	right_diag_up();

  		} else if (Jx < 70){
  		  	left();

  		} else if (Jx > 130){
  		  	right();

  		} else if (Jy < 70){
  		  	up();

  		} else if (Jy > 130){
  		  	down();
  		}

  		delay(del);

	}

private:

	void left_diag_up(){

		matrix.drawPixel(x, y, LOW);
		int matrNum, numBit, height;

		if (y == 0){
			height = 7;
		} else {
			height = y-1;
		}

  		if ((x % 8) == 0){
  			matrNum = floor((x-1)/8); 
  			numBit = 7;
  		} else {
  			matrNum = floor(x/8); 
  			numBit = x-1-8*matrNum;
  		}

  		if (!bitRead(levelMap[matrNum][height],numBit)){
  		  x = x - 1; 
  		  y = y - 1;
  		}
  		
  		if (x < 0){
  			x = 31;
  		} 

  		if (y < 0){
  			y = 7;
  		}

  		matrix.drawPixel(x, y, HIGH);
  		matrix.write();
	}

	void left_diag_down(){

  		matrix.drawPixel(x, y, LOW);
  		int matrNum, numBit, height; 

 		if (y == 7){
 			height = 0;
 		} else {
 			height = y+1;
 		}
  		
  		if ((x % 8) == 0){
  			matrNum = floor((x-1)/8); 
  			numBit = 7;
  		} else {
  			matrNum = floor(x/8); 
  			numBit = x-1-8*matrNum;
  		}
  
  		if (!bitRead(levelMap[matrNum][height],numBit)){
    		x = x - 1; 
    		y = y + 1;
  		}
  
  		if (x < 0){
  			x = 31;
  		} 

  		if (y > 7){
  			y = 0;
  		}

  		matrix.drawPixel(x, y, HIGH);
  		matrix.write();
	}

	void right_diag_down(){

  		matrix.drawPixel(x, y, LOW);
  		int matrNum, numBit, height;

  		if (x == 31){
    		level.next_level();
        level.mapping(); 
    		return;
    	}

  		if (y == 7){
  			height = 0;
  		} else {
  			height = y+1;
  		}

  		if (((x+1) % 8) == 0){
  			matrNum = floor((x+1)/8); 
  			numBit = 0;
  		} else {
  			matrNum = floor(x/8); 
  			numBit = x+1-8*matrNum;
  		}
  		
  		if (!bitRead(levelMap[matrNum][height],numBit)){
    		x = x + 1; 
    		y = y + 1;
  		}

  		if (y > 7){
  			y = 0;
  		}
  		
  		matrix.drawPixel(x, y, HIGH);
		matrix.write();
	}

	void right_diag_up(){

		matrix.drawPixel(x, y, LOW);
	  	int matrNum, numBit, height;

	  	if (x == 31){
	    	level.next_level(); 
	    	level.mapping();
	    	return;
	  	}

	  	if (y == 0){
	  		height = 7;
	  	} else {
	  		height = y-1;
	  	}
	  	
	  	if (((x+1) % 8) == 0){
	  		matrNum = floor((x+1)/8); 
	  		numBit = 0;
	  	} else {
	  		matrNum = floor(x/8); 
	  		numBit = x+1-8*matrNum;
	  	}

	  	if (!bitRead(levelMap[matrNum][height],numBit)){
	    	x = x + 1; 
	    	y = y - 1;
	  	}

	  	if (y < 0){
	  		y = 7;
	  	}
	  
	  	matrix.drawPixel(x, y, HIGH);
	  	matrix.write();
	}

	void left(){
		
		matrix.drawPixel(x, y, LOW);
  		int matrNum, numBit;
  		
  		if ((x % 8) == 0){
  			matrNum = floor((x-1)/8);
  			numBit = 7;
  		} else {
  			matrNum = floor(x/8); 
  			numBit = x-1-8*matrNum;
  		}

  		//Serial.println(String(x)+' '+String(x % 8)+' '+String(matrNum)+' '+String(numBit));
  
  		if (!bitRead(levelMap[matrNum][y],numBit)){
    		x = x - 1;
  		}
  
  		if (x < 0){
  			x = 31;
  		}
  		
  		matrix.drawPixel(x, y, HIGH);
  		matrix.write();
	}

	void right(){

  		matrix.drawPixel(x, y, LOW);
  		int matrNum, numBit;
  
  		if (x == 31){
    		level.next_level();
    		level.mapping();
    		return;
    	}

  		if (((x+1) % 8) == 0){
  			matrNum = floor((x+1)/8); 
  			numBit = 0;
  		} else {
  			matrNum = floor(x/8); 
  			numBit = x+1-8*matrNum;
  		}

  		if (!bitRead(levelMap[matrNum][y],numBit)){
    		x = x + 1;
  		}

	  	matrix.drawPixel(x, y, HIGH);
	  	matrix.write();
	}

	void up(){
  		
  		matrix.drawPixel(x, y, LOW);
  		int matrNum = floor(x/8), height;
  		
  		if (y == 0){
  			height = 7;
  		} else {
  			height = y-1;
  		}
  
  		if (!bitRead(levelMap[matrNum][height],x-8*matrNum)){
    		y = y - 1;
  		}

  		if (y < 0){
  			y = 7;
  		}

  		matrix.drawPixel(x, y, HIGH);
  		matrix.write();
	}

	void down(){
  
  		matrix.drawPixel(x, y, LOW);
  		int matrNum = floor(x/8), height;

  		if (y == 7){
  			height = 0;
  		} else {
  			height = y+1;
  		}
  
  		if (!bitRead(levelMap[matrNum][height],x-8*matrNum)){
    		y = y + 1;
  		}
  		
  		if (y > 7){
  			y = 0;
  		}
  
  		matrix.drawPixel(x, y, HIGH);
  		matrix.write();
	}

};
GamerMove move;

void loop()
{
	if (num_lev == START){
		menu.opening();
  }

  if (!digitalRead(BUTTON_PIN)==1){
    level.restart();
    if (go_menu){
      go_menu = false;
      num_lev = START;
    }
  }

	int X = analogRead(A0);
  int Y = analogRead(A1);
  Jx = map(X, 0, 1023, 0, 201);
  Jy = map(Y, 0, 1023, 0, 201);
  delay(30);

  if ((millis() - blink_timer >= DELAY_BLINK) && !gamer_high){
    matrix.drawPixel(x, y, HIGH);
    matrix.write();
    gamer_high = true;
    blink_timer = millis();
  }

  if ((millis() - blink_timer >= DELAY_BLINK) && gamer_high){
    matrix.drawPixel(x, y, LOW);
    matrix.write();
    gamer_high = false;
    blink_timer = millis();
  }

 	if (Jx < 70 || Jx > 130 || Jy < 70 || Jy > 130){
   	move.go();
 	}

  if (num_lev == END){
  	num_lev = START;
  	matrix.fillScreen(LOW);
  	matrix.drawChar(1,1,'E',1,0,1);
   	matrix.drawChar(9,1,'N',1,0,1);
   	matrix.drawChar(17,1,'D',1,0,1);
   	matrix.write();
   	delay(5000);
   	matrix.fillScreen(LOW);
  }
}
