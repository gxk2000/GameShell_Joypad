// digital
#define BUTTON_A 6
#define BUTTON_B 7
#define BUTTON_X 8
#define BUTTON_Y 9
#define BUTTON_MENU 11
#define BUTTON_SELECT 10
#define BUTTON_START 12
#define BUTTON_Z 13
#define BUTTON_LK3 19
#define BUTTON_LK4 16
#define BUTTON_LK5 15


// analog
#define BUTTON_LK1 A7
#define BUTTON_LK2 A6
#define BUTTON_SHIFT A0

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(BUTTON_A, INPUT);
  digitalWrite(BUTTON_A, HIGH);
  pinMode(BUTTON_B, INPUT);
  digitalWrite(BUTTON_B, HIGH);
  pinMode(BUTTON_X, INPUT);
  digitalWrite(BUTTON_X, HIGH);
  pinMode(BUTTON_Y, INPUT);
  digitalWrite(BUTTON_Y, HIGH);
  pinMode(BUTTON_MENU, INPUT);
  digitalWrite(BUTTON_MENU, HIGH);
  pinMode(BUTTON_SELECT, INPUT);
  digitalWrite(BUTTON_SELECT, HIGH);
  pinMode(BUTTON_START, INPUT);
  digitalWrite(BUTTON_START, HIGH);
  pinMode(BUTTON_Z, INPUT);
  digitalWrite(BUTTON_Z, HIGH);
  pinMode(BUTTON_LK3, INPUT);
  digitalWrite(BUTTON_LK3, HIGH);
  pinMode(BUTTON_LK4, INPUT);
  digitalWrite(BUTTON_LK4, HIGH);
  pinMode(BUTTON_LK5, INPUT);
  digitalWrite(BUTTON_LK5, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Analog port buttons
  int LK1;
  int LK2;
  int SHIFT;
  LK1 = analogRead(BUTTON_LK1);
  if (LK1 < 100) {
    Serial.println("LK1");
  }
  LK2 = analogRead(BUTTON_LK2);
  if (LK2 < 100) {
    Serial.println("LK2");
  }
  SHIFT = analogRead(SHIFT);
  if (SHIFT < 100) {
    Serial.println("SHIFT");
  }

  // Digital port buttons
  if (digitalRead(BUTTON_A) == 0) {
    Serial.println("A");
  }
  if (digitalRead(BUTTON_B) == 0) {
    Serial.println("B");
  }
  if (digitalRead(BUTTON_X) == 0) {
    Serial.println("X");
  }
  if (digitalRead(BUTTON_Y) == 0) {
    Serial.println("Y");
  }
  if (digitalRead(BUTTON_MENU) == 0) {
    Serial.println("MENU");
  }
  if (digitalRead(BUTTON_SELECT) == 0) {
    Serial.println("SELECT");
  }
  if (digitalRead(BUTTON_START) == 0) {
    Serial.println("START");
  }
  if (digitalRead(BUTTON_Z) == 0) {
    Serial.println("Z");
  }
  if (digitalRead(BUTTON_LK3) == 0) {
    Serial.println("LK3");
  }
  if (digitalRead(BUTTON_LK4) == 0) {
    Serial.println("LK4");
  }
  if (digitalRead(BUTTON_LK5) == 0) {
    Serial.println("LK5");
  }

  // Joystick port
  int JoyStick_X = 4;  // x
  int JoyStick_y = 3;  // y
  int x;
  int y;
  x = analogRead(JoyStick_X);
  y = analogRead(JoyStick_y);
  if (x <= 250) {
    Serial.println("DOWN");
  }
  if (x >= 400) {
    Serial.println("UP");
  }
  if (y <= 300) {
    Serial.println("RIGHT");
  }
  if (y >= 550) {
    Serial.println("LEFT");
  }
}