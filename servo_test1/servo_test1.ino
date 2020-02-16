// Подклоючаем библиотеку Servo
#include <Servo.h> 

// Пин для сервопривода
int servoPin = 3;
// Создаем объект
Servo Servo1;
int val;
char chgTurn;


void setup() {
  // Нам нужно подключить сервопривод к используемому номеру пина
  Servo1.attach(servoPin);
  val = 90;
  chgTurn = 'L';
  Serial.begin(9600);
  Servo1.write(val);
}

void loop(){
  // 0 градусов
  /*
  Servo1.write(10);
  delay(1000);
  // 90 градусов
  Servo1.write(90);
  delay(1000);
  // 180 градусов
  Servo1.write(170);
  delay(1000);
    // 90 градусов
  Servo1.write(90);
  delay(1000);

 */
  
  //Serial.print("val: ");  Serial.print(val); Serial.print(" chgTurn: ");  Serial.println(chgTurn);
  /*

  if (val < 110 && chgTurn == 'L')
   {
    val+=5;
   }
  else {
    chgTurn = 'R';
  }

   if (val > 0 && chgTurn == 'R')
   {
    val-=5;
   }
  else {
    chgTurn = 'L';
  }
 
  Servo1.write(val);
   
  delay(100);
  */
   Servo1.write(90);
  
}
