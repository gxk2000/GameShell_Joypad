#include <EEPROM.h>
#include "UsbKeyboard.h"

// 摇杆校准组合键设置
// A键+B键+X键
#define joy_rest_1 key_a
#define joy_rest_2 key_b
#define joy_rest_3 key_x

// io映射
#define x                A3                //  x               ------>  left , right
#define y                A4                //  y               ------>  up , down
#define z                6                 //  z               ------>  z
#define key_esc          0                 //  Esc/Backspace   ------>  Menu
#define key_shift        0                 //  nul             ------>  Shift
#define key_select       0                //  Space/-          ------>  Select
#define key_start        0                //  Enter/+          ------>  Start
#define key_a            10                //  J/H             ------>  A
#define key_b            11                //  K/L             ------>  B
#define key_x            12                //  U/Y             ------>  X
#define key_y            8                 //  I/O             ------>  Y

// 键值映射
#define KEY_LEFT_ARROW    0x50             // Keyboard Left Arrow
#define KEY_RIGHT_ARROW   0x4F             // Keyboard Right Arrow
#define KEY_DOWN_ARROW    0x51             // Keyboard Down Arrow
#define KEY_UP_ARROW      0x52             // Keyboard Up Arrow
#define KEY_ESCAPE        0x29             // Keyboard Escape Arrow
#define KEY_NULL          0xff             // Keyboard Null Arrow

#define KEYPAD_LEFT       KEY_LEFT_ARROW
#define KEYPAD_RIGHT      KEY_RIGHT_ARROW
#define KEYPAD_DOWN       KEY_DOWN_ARROW
#define KEYPAD_UP         KEY_UP_ARROW
#define KEYPAD_Z          KEY_Z

#define KEYPAD_Y          KEY_I
#define KEYPAD_X          KEY_U
#define KEYPAD_A          KEY_J
#define KEYPAD_B          KEY_K

#define KEYPAD_MENU       KEY_ESCAPE
#define KEYPAD_NULL       KEY_NULL
#define KEYPAD_SELECT     KEY_SPACE
#define KEYPAD_START      KEY_ENTER


int origin_x;
int origin_y;
void setup() {
  Serial.begin(9600);
  //读取eeprom内的x，y轴原点
  origin_x = EEPROM.read(12);
  origin_y = EEPROM.read(13);

  //设置按键相关引脚并拉高
  pinMode(z, INPUT);
  digitalWrite(z, HIGH);
  pinMode(key_a, INPUT);
  digitalWrite(key_a, HIGH);
  pinMode(key_b, INPUT);
  digitalWrite(key_b, HIGH);
  pinMode(key_x, INPUT);
  digitalWrite(key_x, HIGH);
}


int x_value;
int y_value;
bool reset_switch = 1;
unsigned long tmp_time;
int tmp_time_switch = 1;

void loop() {
  UsbKeyboard.update();
  //模拟信号投影格式化
  int x_value = map(analogRead(x), 0, 1023, 0, 255);
  int y_value = map(analogRead(y), 0, 1023, 0, 255);

  // x轴
  if (x_value >= origin_x + 20) {
    UsbKeyboard.press(KEYPAD_LEFT);
    delay(100);
    UsbKeyboard.release(KEYPAD_LEFT);
  };
  if (x_value <= origin_x - 20) {
    UsbKeyboard.press(KEYPAD_RIGHT);
    delay(100);
    UsbKeyboard.release(KEYPAD_RIGHT);
  };
  // y轴
  if (y_value >= origin_y + 20) {
    UsbKeyboard.press(KEYPAD_UP);
    delay(100);
    UsbKeyboard.release(KEYPAD_UP);
  };
  if (y_value <= origin_y - 20) {
    UsbKeyboard.press(KEYPAD_DOWN);
    delay(100);
    UsbKeyboard.release(KEYPAD_DOWN);
  };
  // Z轴
  if (digitalRead(z) == 0) {
    UsbKeyboard.press(KEYPAD_Z);
    delay(100);
    UsbKeyboard.release(KEYPAD_Z);
    //Serial.println("z");
  }


  // A键
  if (digitalRead(key_a) == 0) {
    UsbKeyboard.press(KEYPAD_A);
    delay(100);
    UsbKeyboard.release(KEYPAD_A);
  }

  // B键
  if (digitalRead(key_b) == 0) {
    UsbKeyboard.press(KEYPAD_B);
    delay(100);
    UsbKeyboard.release(KEYPAD_B);
  }

  // X键
  if (digitalRead(key_x) == 0) {
    UsbKeyboard.press(KEYPAD_X);
    delay(100);
    UsbKeyboard.release(KEYPAD_X);
  }

  // Y键
  if (digitalRead(key_y) == 0) {
    UsbKeyboard.press(KEYPAD_Y);
    delay(100);
    UsbKeyboard.release(KEYPAD_Y);
  }

  // MENU键
  if (digitalRead(key_esc) == 0) {
    UsbKeyboard.press(KEYPAD_MENU);
    delay(100);
    UsbKeyboard.release(KEYPAD_MENU);
  }

  // SHIFT键
  if (digitalRead(key_shift) == 0) {
    UsbKeyboard.press(KEYPAD_NULL);
    delay(100);
    UsbKeyboard.release(KEYPAD_NULL);
  }

  // SELECT键
  if (digitalRead(key_select) == 0) {
    UsbKeyboard.press(KEYPAD_SELECT);
    delay(100);
    UsbKeyboard.release(KEYPAD_SELECT);
  }

  // START键
  if (digitalRead(key_start) == 0) {
    UsbKeyboard.press(KEYPAD_START);
    delay(100);
    UsbKeyboard.release(KEYPAD_START);
  }

  //摇杆校准
  //joysitc_reset
  //校准组合键同时按下
  if ((digitalRead(joy_rest_1) == LOW && digitalRead(joy_rest_2) == LOW && digitalRead(joy_rest_3) == LOW) && (reset_switch == 1)) {
    Joyreset();
  }
  //校准组合键完全释放
  else if ((digitalRead(joy_rest_1) == HIGH && digitalRead(joy_rest_2) == HIGH && digitalRead(joy_rest_3) == HIGH) && (reset_switch == 0)) {
    tmp_time_switch = 1;
    reset_switch = 1;
  }
}


void Joyreset() {
  //设置时间戳
  if (tmp_time_switch == 1) {
    tmp_time_switch = 0;
    tmp_time = millis();
  }

  //到达预定时间后将x，y轴原点值写入EEPROM的12，13位
  if (millis() - tmp_time > 2000) {
    //eeprom写入x，y轴原点
    int addr_1 = 12;
    x_value = map(analogRead(x), 0, 1023, 0, 255);
    int addr_2 = 13;
    y_value = map(analogRead(y), 0, 1023, 0, 255);
    EEPROM.write(addr_1, x_value);  //x轴原点写入eeprom
    delay(100);
    EEPROM.write(addr_2, y_value);  //x轴原点写入eeprom
    delay(100);
    //重置当前x，y轴原点
    origin_x = x_value;
    origin_y = y_value;
    reset_switch = 0;
  }
}