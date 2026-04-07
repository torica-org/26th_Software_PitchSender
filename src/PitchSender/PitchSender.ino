// ----- ESP32がCOMポートで認識されない -----
// 1. デバイスマネージャを確認
// 2. `cp2102n usb to uart bridge controller`を開くとドライバが無い
// 3. `https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads`にアクセス
// 4. `CP210x Windows Drivers`をダウンロード
// 5. 解凍し`CP210xVCPInstaller_x64.exe`を実行（Windowsの場合）

#include "MyBluetooth.h"
#include "Frequency.h"
#include "MyIMU.h"

// 許容誤差（±TOLERANCE[deg]まで許容する）.
constexpr float TOLERANCE = 3.0; // degree[°]

enum {
  TAIL_UP,
  LEVEL,
  TAIL_DOWN
} attitude = LEVEL;

void setup() {
  Serial.begin(115200);
  bt_init("C2");
  imu_init();
}

void loop() {

  imu_refresh_euler();

  String status;

  if (angles.pitch < -1 * TOLERANCE) {
    attitude = TAIL_UP;
    status = "TAIL_UP";
  }
  else if (angles.pitch > TOLERANCE) {
    attitude = TAIL_DOWN;
    status = "TAIL_DOWN";
  }
  else {
    attitude = LEVEL;
    status = "LEVEL";
  }

  float freq = 0.0;
  float interval = 0.0;

  switch (attitude) {
    case TAIL_UP: {
      freq = frequency_get("G5");
      interval = 0.05;
      break;
    }
    case LEVEL: {
      freq = frequency_get("C5");
      interval = 0.5;
      break;
    }
    case TAIL_DOWN: {
      freq = frequency_get("A4");
      interval = 0.05;
      break;
    }
  }

  bt_set_sound(freq, interval);

  static unsigned long prev = 0;
  unsigned long cur = millis();
  if (cur - prev > 1000) {
    prev = cur;
    Serial.printf("[%s | %s]  (%.5f, %.5f, %.5f)\n", bt_status, status, angles.roll, angles.pitch, angles.yaw);
  }

  delayMicroseconds(10);
}

