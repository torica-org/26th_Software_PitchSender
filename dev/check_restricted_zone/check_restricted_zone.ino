#include <Arduino.h>
#include "check_restricted_zone.h"
#include <TinyGPSPlus.h>
TinyGPSPlus gps;


// アラートを出す閾値（境界線到達までの秒数）
const float TTC_WARNING_SEC = 5.0;

// 必要な変数の宣言
// GPS関連
double data_air_gps_latitude = 0.0;
double data_air_gps_longitude = 0.0;
float data_air_gps_groundspeed_ms = 0.0;
float data_air_gps_heading_deg = 0.0;
// 500msおきに実行のため
unsigned long lastDisplayTime = 0;

void setup() {
  Serial.begin(115200);
  Serial1.setRxBufferSize(8192);
  Serial1.begin(921600, SERIAL_8N1, D7, D6);
  delay(1000);
  Serial.println("GNSS Initialization Complete");
  Serial.println("setup done");
}

void loop() {
  // GNSSからのデータ読み込みとパース
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    gps.encode(c);
  }

  if (millis() - lastDisplayTime > 500) {
    lastDisplayTime = millis();

    data_air_gps_latitude = gps.location.lat();
    data_air_gps_longitude = gps.location.lng();
    data_air_gps_groundspeed_ms = gps.speed.kmph() * 1000 / 3600;
    data_air_gps_heading_deg = gps.course.deg();


    Serial.println("\n--- Current Flight Data ---");
    Serial.print("Lat: ");
    Serial.print(data_air_gps_latitude, 6);
    Serial.print(" | Lon: ");
    Serial.print(data_air_gps_longitude, 6);
    Serial.print(" | Speed: ");
    Serial.print(data_air_gps_groundspeed_ms, 1);
    Serial.print(" m/s");
    Serial.print(" | Heading: ");
    Serial.print(data_air_gps_heading_deg, 1);
    Serial.println(" deg");

    // --------------------------------------------------------
    // 禁止区域内外判定
    // --------------------------------------------------------
    bool isInside = isInsideArea(data_air_gps_latitude, data_air_gps_longitude);

    if (!isInside) {
      // すでにエリア外に出ている場合
      Serial.println(">>> [STATUS] DANGER: 飛行禁止区域！！！");
    } else {
      Serial.println(">>> [STATUS] SAFE: 飛行可能区域内");

      // --------------------------------------------------------
      // 禁止区域外ならば
      // 到達予想時間 (TTC) の計算
      // --------------------------------------------------------
      float ttc = calc_time_to_reach(
        data_air_gps_latitude,
        data_air_gps_longitude,
        data_air_gps_groundspeed_ms,
        data_air_gps_heading_deg);

      // TTCの結果に応じた処理
      if (ttc < 0) {
        Serial.println(">>> [PREDICTION] 境界線から遠ざかっている、または平行に飛行中。");
      } else {
        Serial.print(">>> [PREDICTION] 境界線への最短到達予想時間 (TTC): ");
        Serial.print(ttc, 2);
        Serial.println(" 秒");

        // 警告判定
        if (ttc <= TTC_WARNING_SEC) {
          Serial.println("    !!! ALERT: まもなく境界線に到達します !!!");
          // ここにブザーを鳴らす処理などを追加
        }
      }
    }
  }
}