//Joypad
//Version: 1.0.1
//Author: prodigal

// 库引入
#include "UsbKeyboard.h"
#include <EEPROM.h>

// 按键相关定义
#define KEY_DIGITAL_NUM 10
#define KEY_ANALOG_NUM 2
#define KEY_JOY_NUM 4
#define SHIFT_KEY_NUM 2
#define KEY_PAD_NUM (KEY_DIGITAL_NUM + KEY_ANALOG_NUM)
#define KEY_NUM (KEY_DIGITAL_NUM + KEY_ANALOG_NUM + KEY_JOY_NUM)
#define KEY_NULL 0xff
#define KEY_OFF 1
#define KEY_ON 0
#define ADC_BOUNDARY 500

// 按键指向
#define KEY_ENTER 0x28        // Keyboard Return (ENTER)
#define KEY_ESCAPE 0x29       // Keyboard Escape
#define KEY_BACKSPACE 0x2A    // Keyboard Backspace
#define KEY_SPACE 0x2C        // Keyboard Space
#define KEY_DASH 0x2D         // Keyboard - and _
#define KEY_EQUAL 0x2E        // Keyboard = and +
#define KEY_HOME 0x4A         // Keyboard Home
#define KEY_PAGE_UP 0x4B      // Keyboard Page Up
#define KEY_END 0x4D          // Keyboard End
#define KEY_PAGE_DOWN 0x4E    // Keyboard Page Down
#define KEY_RIGHT_ARROW 0x4F  // Keyboard Right Arrow
#define KEY_LEFT_ARROW 0x50   // Keyboard Left Arrow
#define KEY_DOWN_ARROW 0x51   // Keyboard Down Arrow
#define KEY_UP_ARROW 0x52     // Keyboard Up Arrow
#define KEY_NUM_DASH 0x56     // Keyboard Num Pad -
#define KEY_NUM_PLUS 0x57     // Keyboard Num Pad +

// 按键定义
#define KEYPAD_LIGHT1 KEY_L           // A7
#define KEYPAD_LIGHT2 KEY_O           // A6
#define KEYPAD_LIGHT4 KEY_Y           // D16
#define KEYPAD_LIGHT5 KEY_H           // D15
#define KEYPAD_A KEY_J                // D6
#define KEYPAD_B KEY_K                // D7
#define KEYPAD_X KEY_U                // D8
#define KEYPAD_Y KEY_I                // D9
#define KEYPAD_START KEY_ENTER        // D12
#define KEYPAD_SELECT KEY_SPACE       // D10
#define KEYPAD_MENU KEY_ESCAPE        // D11
#define KEYPAD_LEFT KEY_LEFT_ARROW    // A3
#define KEYPAD_RIGHT KEY_RIGHT_ARROW  // A3
#define KEYPAD_UP KEY_UP_ARROW        // A4
#define KEYPAD_DOWN KEY_DOWN_ARROW    // A4
#define KEYPAD_Z KEY_Z                // D13

// shift组合键定义
#define KEYPAD_LIGHT1_SHIFT KEY_END         // A7 + D19/A0
#define KEYPAD_LIGHT2_SHIFT KEY_PAGE_DOWN   // A6 + D19/A0
#define KEYPAD_LIGHT4_SHIFT KEY_PAGE_UP     // D16 + D19/A0
#define KEYPAD_LIGHT5_SHIFT KEY_HOME        // D15 + D19/A0
#define KEYPAD_A_SHIFT KEY_H                // D6 + D19/A0
#define KEYPAD_B_SHIFT KEY_L                // D7 + D19/A0
#define KEYPAD_X_SHIFT KEY_Y                // D8 + D19/A0
#define KEYPAD_Y_SHIFT KEY_O                // D9 + D19/A0
#define KEYPAD_START_SHIFT KEY_NUM_PLUS     // D12 + D19/A0
#define KEYPAD_SELECT_SHIFT KEY_NUM_DASH    // D10 + D19/A0
#define KEYPAD_MENU_SHIFT KEY_BACKSPACE     // D11 + D19/A0
#define KEYPAD_LEFT_SHIFT KEY_LEFT_ARROW    // A3 + D19/A0
#define KEYPAD_RIGHT_SHIFT KEY_RIGHT_ARROW  // A3 + D19/A0
#define KEYPAD_UP_SHIFT KEY_UP_ARROW        // A4 + D19/A0
#define KEYPAD_DOWN_SHIFT KEY_DOWN_ARROW    // A4 + D19/A0
#define KEYPAD_Z_SHIFT KEY_X                // D13 + D19/A0

// 摇杆灵敏度与摇杆校准相关设置
int RestTime = 5000;                          // 持续按住多少时间后开始校准摇杆，1s = 1000ms
int Stick_sensitivity = 20;                   // 摇杆灵敏度，越小越灵敏
String rest_combo_pins[2] = { "A6", "D16" };  // 摇杆校准组合键，此处为A6和D16即为LK2与LK4

// 按键相关变量
const int pins[KEY_NUM] = { 6, 7, 8, 9, 12, 10, 11, 16, 13, 15, 7, 6, 3, 3, 4, 4 };
const int shift_pins[SHIFT_KEY_NUM] = { 0, 19 };
const int keys[KEY_NUM] = {
  KEYPAD_A, KEYPAD_B, KEYPAD_X, KEYPAD_Y, KEYPAD_START, KEYPAD_SELECT, KEYPAD_MENU,
  KEYPAD_LIGHT4, KEYPAD_Z, KEYPAD_LIGHT5, KEYPAD_LIGHT1, KEYPAD_LIGHT2, KEYPAD_LEFT,
  KEYPAD_RIGHT, KEYPAD_DOWN, KEYPAD_UP
};
const int shift_keys[KEY_NUM] = {
  KEYPAD_A_SHIFT, KEYPAD_B_SHIFT, KEYPAD_X_SHIFT, KEYPAD_Y_SHIFT, KEYPAD_START_SHIFT, KEYPAD_SELECT_SHIFT, KEYPAD_MENU_SHIFT,
  KEYPAD_LIGHT4_SHIFT, KEYPAD_Z_SHIFT, KEYPAD_LIGHT5_SHIFT, KEYPAD_LIGHT1_SHIFT, KEYPAD_LIGHT2_SHIFT, KEYPAD_LEFT_SHIFT,
  KEYPAD_RIGHT_SHIFT, KEYPAD_DOWN_SHIFT, KEYPAD_UP_SHIFT
};
int old_keys[KEY_NUM];

// 摇杆相关变量
int addrx = 12;  // 设定x原点数据储存于eeprom的地址
int addry = 13;  // 设定y原点数据储存于eeprom的地址
int read_x;
int read_y;
unsigned long NowMillis;
int rest_flag_1 = KEY_ON;
int rest_flag_2 = KEY_ON;
int rest_flag_3 = KEY_OFF;
int rest_led_stat = KEY_OFF;

// 键盘初始化相关变量
int one_set_flag = KEY_ON;

// 检测摇杆是否运动
bool joystic_press(int i) {
  if (i >= KEY_NUM - KEY_JOY_NUM and i < KEY_NUM - KEY_JOY_NUM + 2) {
    if (map(analogRead(pins[i]), 0, 1023, 0, 255) > read_x + Stick_sensitivity or map(analogRead(pins[i]), 0, 1023, 0, 255) < read_x - Stick_sensitivity) {
      return KEY_ON;
    } else {
      return KEY_OFF;
    }
  } else if (i >= KEY_NUM - KEY_JOY_NUM + 2) {
    if (map(analogRead(pins[i]), 0, 1023, 0, 255) > read_y + Stick_sensitivity or map(analogRead(pins[i]), 0, 1023, 0, 255) < read_y - Stick_sensitivity) {
      return KEY_ON;
    } else {
      return KEY_OFF;
    }
  }
}

// 检测shift键是否被按下
bool shift_key_pressed() {
  for (int i = 0; i < SHIFT_KEY_NUM; i++) {
    if (i == 0) {
      if (analogRead(shift_pins[i]) / ADC_BOUNDARY == KEY_ON) {
        return true;
      } else {
        return false;
      }
    } else {
      if (digitalRead(shift_pins[i]) == KEY_ON) {
        return true;
      } else {
        return false;
      }
    }
  }
}

// 检测第一个摇杆校准组合键是否被按下
bool RestJudgment_Key1() {
  if (rest_combo_pins[0].substring(0, 1) == "D") {
    return digitalRead(rest_combo_pins[0].substring(1).toInt());
  } else {
    return analogRead(rest_combo_pins[0].substring(1).toInt()) / ADC_BOUNDARY;
  }
}

// 检测第二个摇杆校准组合键是否被按下
bool RestJudgment_Key2() {
  if (rest_combo_pins[1].substring(0, 1) == "D") {
    return digitalRead(rest_combo_pins[1].substring(1).toInt());
  } else {
    return analogRead(rest_combo_pins[1].substring(1).toInt()) / ADC_BOUNDARY;
  }
}

// 复位状态灯
void rest_led(int rest_led_stat) {
  if (rest_led_stat == KEY_ON) {
    digitalWrite(PD0, LOW);
    digitalWrite(PD1, LOW);
    delay(500);
    digitalWrite(PD0, HIGH);
    digitalWrite(PD1, HIGH);
    delay(500);
  }
}

// 上电后延时1秒防止主板检测不到键盘
void one_delay() {
  if (one_set_flag == KEY_ON) {
    delay(1000);
    one_set_flag = KEY_OFF;
  }
}

void setup() {
  // put your setup code here, to run once:
  // TIMSK0 &= !(1 << TOIE0);
  Serial.begin(115200);
  // 将除了D19之外的所有数字按键申明为输入且拉高
  for (int i = 0; i < KEY_NUM; i++) {
    if (i < KEY_DIGITAL_NUM) {
      pinMode(pins[i], INPUT);
      digitalWrite(pins[i], HIGH);
    }
    old_keys[i] = KEY_NULL;
  }

  // 申明D19为输入且拉高
  pinMode(19, INPUT);
  digitalWrite(19, HIGH);

  // 申明PD0,PD1为输出且拉低作为指示灯
  pinMode(PD0, OUTPUT);
  pinMode(PD1, OUTPUT);
  digitalWrite(PD0, HIGH);
  digitalWrite(PD1, HIGH);

  // 读取eeprom中地址为12和13的数据
  read_x = EEPROM.read(addrx);
  read_y = EEPROM.read(addry);
  //Serial.println("setup completes !");
}

void loop() {
  int on_off, key;
  UsbKeyboard.update();
  one_delay();
  for (int i = 0; i < KEY_NUM; i++) {
    // 检测三个部分（数字按键，模拟按键，摇杆按键）中是否有按键被按下
    if (i < KEY_DIGITAL_NUM) {
      on_off = digitalRead(pins[i]);
    } else if (i >= KEY_NUM - KEY_JOY_NUM - KEY_ANALOG_NUM and i < KEY_NUM - KEY_JOY_NUM) {
      on_off = analogRead(pins[i]) / ADC_BOUNDARY;
    } else {
      on_off = joystic_press(i);
    }

    // 判断按键是否按下
    if (on_off == KEY_OFF) {
      if (old_keys[i] != KEY_NULL) {
        UsbKeyboard.release(old_keys[i]);
        old_keys[i] = KEY_NULL;
        //Serial.println("key release1");
        // Serial.println(old_keys[i]);
      }
    } else {
      //判断shift（SHIFT键或LIGHTKEY3）是否被按下
      if (shift_key_pressed()) {
        key = shift_keys[i];
      } else {
        key = keys[i];
      }

      // 判断哪个按键被按下
      if (key != old_keys[i]) {
        // 释放先前的按键
        if (old_keys[i] != KEY_NULL) {
          UsbKeyboard.release(old_keys[i]);
          //Serial.println("key release2");
          // Serial.println(old_keys[i]);
        }

        // Joystic
        // 摇杆方向键检测
        // X轴
        if (i == KEY_NUM - KEY_JOY_NUM and map(analogRead(pins[i]), 0, 1023, 0, 255) > read_x + Stick_sensitivity) {
          UsbKeyboard.press(key);
          old_keys[i] = key;
          //Serial.println("LEFT");
          // Serial.println(key);
        }
        if (i == KEY_NUM - KEY_JOY_NUM + 1 and map(analogRead(pins[i]), 0, 1023, 0, 255) < read_x - Stick_sensitivity) {
          UsbKeyboard.press(key);
          old_keys[i] = key;
          //Serial.println("RIGHT");
          // Serial.println(key);
        }
        // y轴
        if (i == KEY_NUM - KEY_JOY_NUM + 2 and map(analogRead(pins[i]), 0, 1023, 0, 255) < read_y - Stick_sensitivity) {
          UsbKeyboard.press(key);
          old_keys[i] = key;
          //Serial.println("DOWN");
          // Serial.println(key);
        }
        if (i == KEY_NUM - KEY_JOY_NUM + 3 and map(analogRead(pins[i]), 0, 1023, 0, 255) > read_y + Stick_sensitivity) {
          UsbKeyboard.press(key);
          old_keys[i] = key;
          //Serial.println("UP");
          // Serial.println(key);
        }

        // Key
        // 除方向键外的按键检测
        if (i < KEY_NUM - KEY_JOY_NUM) {
          UsbKeyboard.press(key);
          old_keys[i] = key;
          // Serial.println("key press");
          // Serial.println(key);
        }
      }
    }
  }
  // Joystick calibration
  // 摇杆校准
  if (RestJudgment_Key1() == KEY_ON and RestJudgment_Key2() == KEY_ON) {
    // 设置时间戳
    if (rest_flag_1 == KEY_ON) {
      Serial.end();            // 停用串口通讯
      rest_led_stat = KEY_ON;  //指示灯闪烁
      NowMillis = millis();
      rest_flag_1 = KEY_OFF;
      rest_flag_2 = KEY_ON;
      //Serial.println("begin...");
    }
    // 比较时间戳并将原始x与y轴数据存入eeprom
    if (millis() - NowMillis >= RestTime and rest_flag_2 == KEY_ON) {
      int ox = map(analogRead(3), 0, 1023, 0, 255);
      int oy = map(analogRead(4), 0, 1023, 0, 255);
      EEPROM.write(addrx, ox);
      delay(200);
      EEPROM.write(addry, oy);
      delay(200);
      read_x = ox;
      read_y = oy;
      rest_flag_2 = KEY_OFF;
      rest_led_stat = KEY_OFF;
    }
    rest_flag_3 = KEY_ON;
  } else {
    if (rest_flag_3 == KEY_ON) {
      rest_flag_1 = KEY_ON;
      Serial.begin(115200);
      rest_flag_3 = KEY_OFF;
    }
  }
  rest_led(rest_led_stat);
}