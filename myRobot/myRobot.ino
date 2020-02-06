#include <L298N.h>
#include <NewPing.h>

//pin definition
#define ENA 11
#define IN1 12
#define IN2 13

#define ENB 8
#define IN3 9
#define IN4 10

#define PIN_TRIG 6
#define PIN_ECHO 7
#define MAX_DISTANCE 300 // Константа для определения максимального расстояния, которое мы будем считать корректным.
#define MIN_DISTANCE 20 

#define DELAY_SONAR 50

#define dSonar 2


//create a motorR instance
L298N motorR(ENA, IN1, IN2);
L298N motorL(ENB, IN3, IN4);

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

int m = 1; // масса робота 1 кг
int g = 9.8; // ускорение свободного падения
int sTorm;
int curDist = 1000; //текушая дистанция
int curSTorm = 0; // длина тормозного пути

void setup() {

  //used for display information
  Serial.begin(9600);
 
  motorR.setSpeed(curSpeed); // an integer betweENA 0 and 255
  motorL.setSpeed(curSpeed); // an integer betweENA 0 and 255

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.setTimeout(10);
  Serial.println("Для вызова инструкции нажмите H");

      Serial.print("0 curSpeed: "); Serial.println(curSpeed);
      
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
   Serial.println("#policeturn#[число] - полицейский разворот на 180% (1-влево, 0-вправо)");
   Serial.println("************************************************");
}

void setSpeed() {
   motorR.setSpeed(curSpeed);
   motorL.setSpeed(curSpeed);
}

void mF() {
      if ((curDist * 3) <= curSTorm) mStop;
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
  //Serial.print("Расстояние до объекта: "); Serial.print(distance); Serial.println(" см");  
  curSTorm = getSTorm();
  if (curDist <= curSTorm) {
    
    Serial.print("Длина тормозного пути: "); Serial.print(curSTorm); Serial.print("см ");
    Serial.print("До объекта: "); Serial.print(curDist); Serial.println("см");
         
    setdSonar(1, (200 - (curDist*8)));
    if (curVector == 'F') {
        Serial.print("Скорость мотора L: "); Serial.println(motorL.getSpeed());
        Serial.print("Скорость мотора R: "); Serial.println(motorR.getSpeed());
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
      setdSonar(0, 0);
 }

}


void setdSonar(int v, int power) {
  if (v == 1) 
    analogWrite(dSonar, power);
  else
    digitalWrite(dSonar, LOW);
}

void changeSpeed(int v) {
  Serial.print("Переключить передачу "); Serial.println(v == 1 ? "вверх" : "вниз");
  switch (v) {
    case 1 :
      curSpeed += 50;
      break;
    case 0 :
      curSpeed -= 50;
      break;
  }
  setSpeed();
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
    if (val == 'H') printHelp();
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
      if (str.compareTo("s") == 0 ) {
        changeSpeed(x);
      }
      
  }
  
  delay(100);
}
