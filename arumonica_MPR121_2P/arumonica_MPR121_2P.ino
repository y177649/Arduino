#include <Wire.h>
#include <Adafruit_MPR121.h>

// MPR121インスタンスの作成
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();

// タッチデータ変数
String TouchData = "";

void setup() {
  Serial.begin(9600);

  // MPR121の初期設定
  if (!cap1.begin(0x5A)) {
    Serial.println("MPR121a not found, check wiring?");
    while (1);
  }
  if (!cap2.begin(0x5B)) {
    Serial.println("MPR121b not found, check wiring?");
    while (1);
  }
}

void loop() {
  // タッチデータのリセット
  TouchData = "";

  // センサー1の状態の読み取り
  for (uint8_t i = 0; i < 12; i++) {
    if (cap1.touched() & (1 << i)) {
      TouchData += "1";
    } else {
      TouchData += "0";
    }
  }

  // センサー2の状態の読み取り
  for (uint8_t i = 0; i < 12; i++) {
    if (cap2.touched() & (1 << i)) {
      TouchData += "1";
    } else {
      TouchData += "0";
    }
  }

  // タッチデータの出力
  Serial.println(TouchData);

  // 0.1秒待機
  delay(100);
}
