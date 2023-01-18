#define key_z 13        // z
int JoyStick_X = 4;     // x
int JoyStick_y = 3;     // y
void setup() 
{
  Serial.begin(9600); // 9600 bps
  pinMode(key_z, INPUT);
  digitalWrite(key_z, HIGH);
}
void loop() 
{
  int x;
  int y;
  x=analogRead(JoyStick_X);
  y=analogRead(JoyStick_y);
  Serial.print("x: ");
  Serial.print(x ,DEC);
  Serial.print("  |  y: ");
  Serial.print(y ,DEC);
  Serial.print("  |  z: ");
  Serial.println(digitalRead(key_z));
  delay(100);
}