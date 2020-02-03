int IN1 = 5; // Input1 подключен к выводу 5 
int IN2 = 6;
int IN3 = 7;
int IN4 = 8;
int EN1 = 1;
int EN2 = 2;
int i;
void setup()
{
  pinMode (EN1, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (EN2, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (IN3, OUTPUT);
}
void loop()
{
  digitalWrite (IN2, HIGH);
  digitalWrite (IN1, LOW); 
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
  for (i = 50; i <= 180; ++i)
  {
      analogWrite(EN1, i);
      analogWrite(EN2, i);
      delay(30);
  }
  analogWrite (EN1, 0);
  analogWrite (EN2, 0);
  delay(500);
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW); 
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  for (i = 50; i <= 180; ++i)
  {
      analogWrite(EN1, i);
      analogWrite(EN2, i);
      delay(30);
  }
  analogWrite (EN1, 0);
  analogWrite (EN2, 0);
  delay(8000);
}
