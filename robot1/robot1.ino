//#include <L298N.h>
// Двигатель A 
int PIN_ENA = 9;
int PIN_IN1 = 7;
int PIN_IN2 = 8;
// Двигатель B
int PIN_ENB = 3;
int PIN_IN3 = 5;
int PIN_IN4 = 4;
uint8_t power = 50; // Значение ШИМ (или скорости вращения)
int i;

//create a motor instance
//L298N motorL(PIN_ENA, PIN_IN1, PIN_IN2);
//L298N motorR(PIN_ENB, PIN_IN3, PIN_IN4);

void setup() {


  //used for display information
  Serial.begin(9600);

 // motorL.setSpeed(50); // an integer between 0 and 255
//  motorR.setSpeed(50); // an integer between 0 and 255

  // Установка всех управляющих пинов в режим выхода

  pinMode(PIN_ENA, OUTPUT);

  pinMode(PIN_ENB, OUTPUT);

  pinMode(PIN_IN1, OUTPUT);

  pinMode(PIN_IN2, OUTPUT);

  pinMode(PIN_IN3, OUTPUT);

  pinMode(PIN_IN4, OUTPUT);

// Команда остановки двум моторам

  digitalWrite(PIN_IN1, LOW);

  digitalWrite(PIN_IN2, LOW);

  digitalWrite(PIN_IN3, LOW);

  digitalWrite(PIN_IN4, LOW);
  

  //change the initial speed
 // motorL.setSpeed(150);
 // motorR.setSpeed(150);
}

void leftForward(int powr) {

  int pwr = powr;
  while(pwr < 255) {
    
     analogWrite(PIN_ENB, 0); // Устанавливаем скорость 1-го мотора
     digitalWrite(PIN_IN3, LOW);
     digitalWrite(PIN_IN4, LOW);
     
     analogWrite(PIN_ENA, pwr); // Устанавливаем скорость 1-го мотора
     digitalWrite(PIN_IN1, HIGH);
     digitalWrite(PIN_IN2, LOW);
     delay(1000); // Пауза 3 секунды
     pwr += 30; // Увеличиваем скорость
  }
}

void rightForward(int powr) {
  int pwr = powr;
  while(pwr < 255) {
     analogWrite(PIN_ENA, 0); // Устанавливаем скорость 1-го мотора
     digitalWrite(PIN_IN1, LOW);
     digitalWrite(PIN_IN2, LOW);

     analogWrite(PIN_ENB, pwr); // Устанавливаем скорость 1-го мотора
     digitalWrite(PIN_IN3, HIGH);
     digitalWrite(PIN_IN4, LOW);
     
     delay(1000); // Пауза 3 секунды
     pwr += 30; // Увеличиваем скорость
  }
}



void twoForward(int powr) {

  int pwr = powr;
  while(pwr < 255) {
     analogWrite(PIN_ENA, power); // Устанавливаем скорость 1-го мотора
     analogWrite(PIN_ENB, power); // Устанавливаем скорость 2-го мотора
     digitalWrite(PIN_IN1, HIGH);
     digitalWrite(PIN_IN2, LOW);
     digitalWrite(PIN_IN3, HIGH);
     digitalWrite(PIN_IN4, LOW);
     delay(1000); // Пауза 3 секунды
     pwr += 30; // Увеличиваем скорость
  }
}

/*
void forwardAll() {
  //tell the motor to go forward (may depend by your wiring)
  motorL.forward();
  motorR.forward();

  //print the motor satus in the serial monitor
  Serial.print("MotorL.Is moving = ");
  Serial.println(motorL.isMoving());
    Serial.print("MotorR.Is moving = ");
  Serial.println(motorR.isMoving());
  delay(1000);
  motorL.setSpeed(255);
  motorR.setSpeed(255);
}
*/

void loop()
{
  digitalWrite (PIN_IN2, HIGH);
  digitalWrite (PIN_IN1, LOW); 
  digitalWrite (PIN_IN4, HIGH);
  digitalWrite (PIN_IN3, LOW); 
  for (i = 50; i <= 180; ++i)
  {
      analogWrite(PIN_ENA, i);
      analogWrite(PIN_ENB, i);
      delay(300);
  }
  analogWrite (PIN_ENA, 0);
  analogWrite (PIN_ENB, 0);
  delay(1000);
  digitalWrite (PIN_IN1, HIGH);
  digitalWrite (PIN_IN2, LOW); 
  digitalWrite (PIN_IN3, HIGH);
  digitalWrite (PIN_IN4, LOW);
  for (i = 50; i <= 180; ++i)
  {
      analogWrite(PIN_ENA, i);
      analogWrite(PIN_ENB, i);
      delay(300);
  }
  analogWrite (PIN_ENA, 0);
  analogWrite (PIN_ENB, 0);
  delay(2000);
}
