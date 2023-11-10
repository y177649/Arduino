#include <SPI.h>
int value; // MAX6675からのデータ格納変数

// LEDのピン番号
const int ledPin = 3;

// ヒーター制御関連の変数
const int heaterPin = D3; // ヒーター制御用のピン番号
bool heaterState = false; // ヒーターのON/OFF状態
unsigned long lastHeaterOnTime = 0; // 最後にヒーターをONにした時刻
const unsigned long heaterOnDuration = 10000; // ヒーターをONにする時間(10秒)

void setup() {
  // SPI通信の初期設定
  SPI.setBitOrder(MSBFIRST);           // 上位ビットから送信
  SPI.setClockDivider(SPI_CLOCK_DIV4); // 分周期1/4指定 16MHz/4=4MHz(MAX6675仕様：Max 4.3MHz)
  SPI.setDataMode(SPI_MODE0);          // アイドル時Low、立上がりエッジでデータ取得
  SPI.begin();                         // SPI通信の初期化、有効化
  Serial.begin(9600);                  // シリアル通信のデータ転送レートを9600bpsで指定

  // LEDピンを出力モードに設定
  pinMode(ledPin, OUTPUT);
  
  // ヒーターピンを出力モードに設定
  pinMode(heaterPin, OUTPUT);
}

void loop() { 

  // 温度データ読み出し
  delay(500);                       // MAX6675仕様(Conversion Time > 220ms)
  digitalWrite(SS, LOW);            // 制御デバイス選択(Enable)
  value = SPI.transfer(0x00) << 8;  // 上位ビット8bit読み出し後8bit左にシフト
  value |= SPI.transfer(0x00);      // 下位ビット8bit読み出し後上位ビットとOR
  digitalWrite(SS, HIGH);           // 制御デバイス選択(Disable)

  // 熱電対のopen check(bit<2> = 1ならopen)
  if ((value & 0x0004) != 0){
    Serial.println("Open Error");
    
  // 読み出した温度の表示 bit<14:3>が温度数値格納bit bit<15>=0(dummy bit)
  } else {
    float temperature = (value >> 3) * 0.25; // 温度を計算、MAX6675仕様(分解能 0.25℃)
    Serial.print(temperature); // 温度を表示
    Serial.println(" degC");
    
    // ヒーター制御ロジック
    if (temperature >= 200.0) {
      unsigned long currentTime = millis();
      // 200度以上でヒーターをONにする
      if (!heaterState) {
        digitalWrite(heaterPin, HIGH); // ヒーターをON
        heaterState = true;
        lastHeaterOnTime = currentTime;
      } else {
        // 200度以上でヒーターがONの場合、10秒間ヒーターをONにしたらOFFにする
        if (currentTime - lastHeaterOnTime >= heaterOnDuration) {
          digitalWrite(heaterPin, LOW); // ヒーターをOFF
          heaterState = false;
        }
      }
    } else if (temperature < 170.0) {
      // 170度未満でヒーターをOFFにする
      digitalWrite(heaterPin, LOW); // ヒーターをOFF
      heaterState = false;
    }

    // 温度が200度以上ならLEDをON、それ以下ならOFF
    if (temperature >= 200.0) {
      digitalWrite(ledPin, HIGH); // LEDをON
    } else {
      digitalWrite(ledPin, LOW); // LEDをOFF
    }
  } 
}