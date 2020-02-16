#include <L298N.h>
#include <NewPing.h>
// Подклоючаем библиотеку Servo
#include <Servo.h> 

//pin definition
#define ENA 11
#define IN1 12
#define IN2 13

#define ENB 8
#define IN3 9
#define IN4 10

#define PIN_TRIG 6
#define PIN_ECHO 7
// Пин для сервопривода
#define PIN_SERVO 5

//constant
#define MAX_DISTANCE 500 // Константа для определения максимального расстояния, которое мы будем считать корректным.
#define MIN_DISTANCE 20 

#define DELAY_SONAR 10

#define dSonar 2

// константы скорости
#define MAX_SPEED 250
#define MIN_SPEED 80

//create a motorR instance
L298N motorR(ENA, IN1, IN2);
L298N motorL(ENB, IN3, IN4);

// Создаем объект
Servo Servo1;

// Создаем объект, методами которого будем затем пользоваться для получения расстояния.
// В качестве параметров передаем номера пинов, к которым подключены выходы ECHO и TRIG датчика
NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);


int val;
int LED = 13;
int curSpeed = 80;
int cnt = 0;
int curTurnDelay = 200;

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
int oldSpeed = -1;
char curVector = '0';

int m = 1.3; // масса робота 1 кг
int g = 9.8; // ускорение свободного падения
int sTorm;
int curDist = 1000; //текушая дистанция
int curSTorm = 0; // длина тормозного пути
int enableAIStop = 1;

void setup() {

  //used for display information
  Serial.begin(9600);
 
  motorR.setSpeed(curSpeed); // an integer betweENA 0 and 255
  motorL.setSpeed(curSpeed); // an integer betweENA 0 and 255

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.setTimeout(10);
  Serial.println("Для вызова инструкции нажмите H");
  turnServo(0);    
}

void printHelp() {
   Serial.println("************************************************");
   Serial.println("Список команд:");
   Serial.println("************************************************");
   Serial.println("F - вперед");
   Serial.println("B - назад");
   Serial.println("L - налево");
   Serial.println("R - направо");
   Serial.println("S - стоп");
   Serial.println("> - увеличить скорость");
   Serial.println("< - уменьшить скорость");
   Serial.println("#delay#[число] - изменить ожидание при повороте");   
   Serial.println("#changegear#[число] - переключить передачу ( 1 - вверх, 0 - вниз");   
   Serial.println("#policeturn#[число] - полицейский разворот на 180% (1-влево, 0-вправо)");
   Serial.println("#aistop#[число] - останавливаться перед препятствием (1-включить, 0-отключить)");
   Serial.println("#getspeed#[число] - получить скорость моторов (0-скорость всех моторов)");
   Serial.println("#servo#[число] - получить скорость моторов ( -1 - влево, 0 - по центру, 1 - вправо)");
   Serial.println("************************************************");
}

void setSpeed() {
   motorR.setSpeed(curSpeed);
   motorL.setSpeed(curSpeed);
}

void getSpeedInfo() {
  Serial.print("Скорость мотора L: "); Serial.print(motorL.getSpeed());  
  Serial.print("  "); Serial.print("Скорость мотора R: "); Serial.println(motorR.getSpeed());
}

void mF() {
      if ((curDist * 3) <= curSTorm) {
        mStop;
      }
      else {
        setSpeed();
        Serial.println("Motor forward");
        motorR.forward();
        motorL.forward();
        curVector = 'F';
      }
}

void mB() {
     setSpeed();
     Serial.println("Motor backward");
     motorR.backward();
     motorL.backward();
     curVector = 'B';
}

void mStop() {
    Serial.println("Motor STOP");
    motorR.stop();
    motorL.stop();
    curVector = '0';
}

void mTurn(char c) {
    setSpeed();
    Serial.print("Turn: "); Serial.println(c);
    if (c=='L') {
      motorR.forward();
      motorL.backward();
    }
    if (c=='R') {
      motorR.backward();
      motorL.forward();
    }
    delay(curTurnDelay);
    mStop();
}

void chgSpeed(char c, int i) {
    int v = i;
    if (c == '<') v *= -1;
    if (curSpeed >= MAX_SPEED && v > 0)
        curSpeed = MAX_SPEED;
    else if (curSpeed <= MIN_SPEED && v < 0)
        curSpeed = MIN_SPEED;  
    else
        curSpeed += v;
    Serial.print("curSpeed: ");
    Serial.println(curSpeed);
}

void chgTurnDelay(int i) {
    curTurnDelay = i;
    Serial.print("curTurnDelay: ");
    Serial.println(curTurnDelay);
}

void policeTurn(int v) {
  Serial.println("Выполняю полицейский разворот " + v == '1' ? "влево" : "вправо");
  int tmpSpeed = curSpeed;
  int tmpTurnDelay = curTurnDelay;
  curSpeed = 100;
  curTurnDelay = 900;
  switch (v) {
    case 1 :
      mTurn('L');
      break;
    case 0 :
      mTurn('R');
      break;
  }
  curSpeed = tmpSpeed;
  curTurnDelay = tmpTurnDelay;
}

// вычисление тормозного пути
int getSTorm() {
  sTorm = ((curSpeed^2)/(m*g)) *2.5;
  return sTorm;
}

void checkSonar() {
  // Стартовая задержка, необходимая для корректной работы.
  delay(DELAY_SONAR);
  // Получаем значение от датчика расстояния и сохраняем его в переменную
  // unsigned int distance = sonar.ping_cm();
  curDist = sonar.ping_cm();
  // Печатаем расстояние в мониторе порта
  //Serial.print("Расстояние до объекта: "); Serial.print(curDist); Serial.println(" см");  
  curSTorm = getSTorm();
  if (curDist <= curSTorm) {
    
    //Serial.print("Длина тормозного пути: "); Serial.print(curSTorm); Serial.print("см ");
    //Serial.print("До объекта: "); Serial.print(curDist); Serial.println("см");
         
    setLedSonar(1, (200 - (curDist*8)));
    if (enableAIStop == 1 && curVector == 'F') {
        if (oldSpeed == -1)
          oldSpeed = curSpeed;

        while (curSpeed > 0) {
          chgSpeed('<', 50);
        }
        mStop();
        curSpeed = oldSpeed;
        oldSpeed = -1;
    }
    else {
      if (oldSpeed >= 0 && curSpeed <= 0) {
        curSpeed = oldSpeed;
        oldSpeed = -1;
      }
    }
  }
  else {
      setLedSonar(0, 0);
  }
}


void setLedSonar(int v, int power) {
  if (v == 1) 
    analogWrite(dSonar, power);
  else
    digitalWrite(dSonar, LOW);
}

void changeGear(int v) {
  Serial.print("Переключить передачу "); Serial.println(v == 1 ? "вверх" : "вниз");
  switch (v) {
    case 1 :
      curSpeed += 50;
      if (curSpeed >= MAX_SPEED) 
        curSpeed = MAX_SPEED;
      break;
    case 0 :
      curSpeed -= 50;
      if (curSpeed <= MIN_SPEED)
        curSpeed = MIN_SPEED;  
      break;
  }
  setSpeed();
  getSpeedInfo();
}

void turnServo(int v) {
  Servo1.attach(PIN_SERVO);
  String s;
  int iv = 0;
  if (v == -1) {
     iv = 0;
     s = "вправо";  
  }
  else if (v == 1) {
     iv = 180;   
     s = "влево";
  }
  else if (v == 0) {
     iv = 90;   
     s = "центр";
  }
  Serial.print("turnServo: "); Serial.print(s);   Serial.print(" ( ");  Serial.print(iv);  Serial.println(" )");  
  Servo1.write(iv); 
  delay(500);
  Servo1.detach();
}

void loop() {
  
  checkSonar();
  if (Serial.available())
  {
    val = Serial.read();
    if (val == 'F') mF();
    if (val == 'B') mB();
    if (val == 'L' || val == 'R') mTurn(val);
    if (val == 'S') mStop();
    if (val == '>' || val == '<') chgSpeed(val, 10);
    if (val == 'H' || val == 'h') printHelp();
  }

  if(Serial.available() > 0)
  {
      int x;
      String str;
      //пример команды #delay#500
      str = Serial.readStringUntil('#');
      x = Serial.parseInt();
      if (str.compareTo("delay") == 0) {
        chgTurnDelay(x);
      }
      if (str.compareTo("policeturn") == 0) {
        policeTurn(x);
      }
      if (str.compareTo("changegear") == 0 ) {
        changeGear(x);
      }
      if (str.compareTo("aistop") == 0 ) {
        Serial.print("enableAIStop: "); Serial.println(x);
        enableAIStop = x;
      }
      if (str.compareTo("getspeed") == 0 ) {
        getSpeedInfo();
      }
      if (str.compareTo("servo") == 0 ) {
        turnServo(x);
      }
      
  }
  
  delay(100);
}
