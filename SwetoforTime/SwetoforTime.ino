#define A 8
#define B 7
#define C 6
#define D 5
#define E 4
#define F 3
#define G 2

int red = 10;
int yellow = 11;
int green = 12;
bool i = 1;
int buzzer = 9;

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  clearNum();
}

void loop() {
  if (i == 1){digitalWrite(red, HIGH); i = !i;}
  else {digitalWrite(green, HIGH); analogWrite(buzzer,10); i = !i;}
  timeLow();
  num3(); if (i){analogWrite(buzzer,10);}; delay(500);
  clearNum(); delay(500);
  num2(); if (i){digitalWrite(green, HIGH); analogWrite(buzzer,10);} delay(500);
  clearNum(); delay(500);
  num1(); if (i){digitalWrite(green, HIGH); analogWrite(buzzer,10);} delay(500);
  clearNum(); delay(500);
  num0(); if (i){digitalWrite(green, HIGH); analogWrite(buzzer,10);} delay(500);
  clearNum(); delay(500);
  digitalWrite(yellow, HIGH);
  delay(2000);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);

}

void timeLow(){
  num9(); delay(1000); 
  num8(); if (i){analogWrite(buzzer,0);}; delay(1000);
  num7(); if (i){analogWrite(buzzer,10);}; delay(1000);
  num6(); if (i){analogWrite(buzzer,0);}; delay(1000);
  num5(); if (i){analogWrite(buzzer,10);}; delay(1000);
  num4(); if (i){analogWrite(buzzer,0);}; delay(1000);
}

void clearNum(){
  digitalWrite(A, LOW); 
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(green, LOW);
  analogWrite(buzzer,0);
}

void num1(){
  digitalWrite(A, LOW); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void num2(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void num3(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void num4(){
  digitalWrite(A, LOW); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void num5(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void num6(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void num7(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void num8(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void num9(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void num0(){
  digitalWrite(A, HIGH); 
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
}
