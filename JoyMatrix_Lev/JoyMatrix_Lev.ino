#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9; //Указываем к какому выводу подключен контакт CS
int numHDis = 1; //Количество матриц по горизонтали
int numVDis = 4; //Количество матриц по вертикали
Max72xxPanel matrix = Max72xxPanel(pinCS, numHDis, numVDis);

byte but = 2;
int del = 80;
int x; int y;

const byte lev[16][8] = {
  {B00000000, B11111111, B00000001, B00000001, B00000001, B00000001, B11111111, B00000000},
  {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000},
  {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000},
  {B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000},
  {B11111111, B00000001, B00000001, B00001111, B10000001, B10000001, B10000001, B11111111},
  {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111},
  {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111},
  {B11111111, B00001000, B00001000, B00001000, B10000000, B10000000, B10000000, B11111111},
  {B11111111, B01000001, B01000001, B01001111, B01000001, B01110001, B00000001, B11111111},
  {B11111111, B00000000, B00111111, B00100001, B00101001, B11101011, B10001000, B11111111},
  {B11111111, B00010001, B01010101, B01010101, B01000100, B11111110, B00000000, B11111111},
  {B11111111, B00100001, B10100101, B10111101, B10000101, B11110101, B10000000, B11111111},
  {B11011111, B01000001, B01000001, B01000001, B11111111, B00000001, B00000001, B11011111},
  {B11100100, B00000100, B11111100, B00100000, B00100111, B00100100, B00100100, B00100100},
  {B11011111, B01000100, B01000100, B01000100, B01000100, B01000111, B01100000, B00110000},
  {B10011111, B00010000, B00010000, B10010010, B10010010, B10000010, B10000010, B10000010},
};

const byte startGamer[4][2] = {{3,3},{2,5},{2,2},{3,2}};
int levRow = sizeof(lev) / sizeof(lev[0]); 
int levCol = 8;
byte numLev;
char number[] = {'0','1','2','3','4','5','6','7'};

void setup() {
  Serial.begin(9600);
  matrix.setIntensity(5); //Яркость от 0 до 15
  matrix.setRotation(3); //Направление текста 1, 2, 3, 4
  pinMode(but, INPUT_PULLUP);

  matrix.fillScreen(LOW);
  int levCh = levelChoice();
  numLev = levCh;
  levelDraw(); 
  
}

void(* resetFunc) (void) = 0;

void levelDraw(){
  matrix.fillScreen(LOW);
  if (numLev > levRow/4-1){
     matrix.drawChar(1,1,'E',1,0,1);
     matrix.drawChar(9,1,'N',1,0,1);
     matrix.drawChar(17,1,'D',1,0,1);
     matrix.write();
     delay(5000);
     matrix.fillScreen(LOW);
     resetFunc();
  }
  x = startGamer[numLev][0];
  y = startGamer[numLev][1];
  for (int ii = 0+4*numLev; ii < 4+4*numLev; ii++){
    for (int j = 0; j < 8; j++ ) {            // Передача массива уровня
      for (int i = 0; i < 8; i++ ) {
        matrix.drawPixel((i+8*(ii-4*numLev)), j, lev[ii][j] & (1 << i));
      }
    }
  }
  matrix.drawPixel(x, y, HIGH); matrix.write();
  delay(100);
}

int levelChoice(){
  for (int i = 0; i < levRow/4; i++){
    matrix.drawChar(i*8+1,0,number[i],1,0,1);
  }
  x = 6; y = 6;
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  
  int levCh = Choice();
  return levCh;
}

int Choice(){
  byte b = !digitalRead(but);
  while (b == 0){
    int X = analogRead(A0);
    byte Jx = map(X, 0, 1023, 0, 201);
    if (Jx < 70){
      matrix.drawPixel(x, y, LOW);
      if (x!=6){
        x = x - 8;
      }
    }
    else if (Jx > 130){
      matrix.drawPixel(x, y, LOW);
      if (x!=2*levRow-2){
        x = x + 8;
      }
    }
    matrix.drawPixel(x, y, HIGH);
    matrix.write();
    b = !digitalRead(but);
    delay(300);
  }
  while (b == 1){b = !digitalRead(but);}
  return floor(x/8);
}

int menuLeft(int stepMatrix){
  for (int i = 0; i < levRow/4-stepMatrix; i++){
    matrix.drawChar(i*8+1,0,number[i+stepMatrix],1,0,1);
  }
  stepMatrix += 1;
  return stepMatrix;
}



void ifMove(int *arrCoor){
  int arrXY[] = {arrCoor[0], arrCoor[1]};
  if (arrCoor[2] < 70 && arrCoor[3] < 70){
    moveLeftDiagUp(arrXY);
  }
  else if (arrCoor[2] < 70 && arrCoor[3] > 130){
    moveLeftDiagDown(arrXY);
  }
  else if (arrCoor[2] > 130 && arrCoor[3] > 130){
    moveRightDiagDown(arrXY);
  }
  else if (arrCoor[2] > 130 && arrCoor[3] < 70){
    moveRightDiagUp(arrXY);
  }
  else if (arrCoor[2] < 70){
    moveLeft(arrXY);
  }
  else if (arrCoor[2] > 130){
    moveRight(arrXY);
  }
  else if (arrCoor[3] < 70){
    moveUp(arrXY);
  }
  else if (arrCoor[3] > 130){
    moveDown(arrXY);
  }
  arrCoor[0] = arrXY[0]; arrCoor[1] = arrXY[1];
  delay(del);
  return arrCoor;
}

void moveLeftDiagUp(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit; int height;
  if (y == 0){height = 7;} else {height = y-1;}
  if ((x % 8) == 0){matrNum = floor((x-1)/8); numBit = 7;}
  else {matrNum = floor(x/8); numBit = x-1-8*matrNum;}
  if (!bitRead(lev[matrNum+4*numLev][height],numBit)){
    x = x - 1; y = y - 1;
  }
  if (x < 0){x = 31;} if (y < 0){y = 7;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x; arrXY[1] = y;
  return arrXY;
}

void moveLeftDiagDown(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit; int height;
  if (y == 7){height = 0;} else {height = y+1;}
  if ((x % 8) == 0){matrNum = floor((x-1)/8); numBit = 7;}
  else {matrNum = floor(x/8); numBit = x-1-8*matrNum;}
  if (!bitRead(lev[matrNum+4*numLev][height],numBit)){
    x = x - 1; y = y + 1;
  }
  if (x < 0){x = 31;} if (y > 7){y = 0;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x; arrXY[1] = y;
  return arrXY;
}

void moveRightDiagDown(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit; int height;
  if (x == 31){
    numLev+=1; 
    levelDraw();
    arrXY[0] = x;
    arrXY[1] = y; 
    return arrXY;
  }
  if (y == 7){height = 0;} else {height = y+1;}
  if (((x+1) % 8) == 0){matrNum = floor((x+1)/8); numBit = 0;}
  else {matrNum = floor(x/8); numBit = x+1-8*matrNum;}
  if (!bitRead(lev[matrNum+4*numLev][height],numBit)){
    x = x + 1; y = y + 1;
  } 
  if (y > 7){y = 0;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x; arrXY[1] = y;
  return arrXY;
}

void moveRightDiagUp(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit; int height;
  if (x == 31){
    numLev+=1; 
    levelDraw();
    arrXY[0] = x;
    arrXY[1] = y; 
    return arrXY;
  } 
  if (y == 0){height = 7;} else {height = y-1;}
  if (((x+1) % 8) == 0){matrNum = floor((x+1)/8); numBit = 0;}
  else {matrNum = floor(x/8); numBit = x+1-8*matrNum;}
  if (!bitRead(lev[matrNum+4*numLev][height],numBit)){
    x = x + 1; y = y - 1;
  }
  if (y < 0){y = 7;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x; arrXY[1] = y;
  return arrXY;
}

void moveLeft(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit;
  if ((x % 8) == 0){matrNum = floor((x-1)/8); numBit = 7;}
  else {matrNum = floor(x/8); numBit = x-1-8*matrNum;}
  Serial.println(String(x)+' '+String(x % 8)+' '+String(matrNum)+' '+String(numBit));
  if (!bitRead(lev[matrNum+4*numLev][y],numBit)){
    x = x - 1;
  }
  if (x < 0){x = 31;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x;
  return arrXY;
}

void moveRight(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum; int numBit;
  if (x == 31){
    numLev+=1; 
    levelDraw();
    arrXY[0] = x;
    arrXY[1] = y; 
    return arrXY;
    }
  if (((x+1) % 8) == 0){matrNum = floor((x+1)/8); numBit = 0;}
  else {matrNum = floor(x/8); numBit = x+1-8*matrNum;}
  if (!bitRead(lev[matrNum+4*numLev][y],numBit)){
    x = x + 1;
  }
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[0] = x; 
  return arrXY;
}

void moveUp(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum = floor(x/8);
  int height;
  if (y == 0){height = 7;}
  else {height = y-1;}
  if (!bitRead(lev[matrNum+4*numLev][height],x-8*matrNum)){
    y = y - 1;
  }
  if (y < 0){y = 7;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[1] = y;
  return arrXY;
}

void moveDown(int *arrXY){
  x = arrXY[0]; y = arrXY[1];
  matrix.drawPixel(x, y, LOW);
  int matrNum = floor(x/8);
  int height;
  if (y == 7){height = 0;}
  else {height = y+1;}
  if (!bitRead(lev[matrNum+4*numLev][height],x-8*matrNum)){
    y = y + 1;
  }
  if (y > 7){y = 0;}
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  arrXY[1] = y;
  return arrXY;
}

void Restart(){
  matrix.drawPixel(x, y, LOW);
  x = startGamer[numLev][0];
  y = startGamer[numLev][1];
  matrix.drawPixel(x, y, HIGH);
  matrix.write();
  byte b = !digitalRead(but);
  while (b==1){b = !digitalRead(but);}
}

void loop() {
  byte b = !digitalRead(but);
  if (b==1){Restart();}
  int X = analogRead(A0);
  int Y = analogRead(A1);
  byte Jx = map(X, 0, 1023, 0, 201);
  byte Jy = map(Y, 0, 1023, 0, 201);
  delay(30);
  if (Jx < 70 || Jx > 130 || Jy < 70 || Jy > 130){
    int arrCoor[] = {x, y, Jx, Jy};
    ifMove(arrCoor);
    x = arrCoor[0]; y = arrCoor[1];
  }
  
}
