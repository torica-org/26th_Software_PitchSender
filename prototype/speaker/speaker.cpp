#include "speaker.h"
#include <Arduino.h>

int O_SPK = 15;

static float float_map(float x, float in_min, float in_max, float out_min, float out_max) {
  if (in_max == in_min) {
    return out_min;
  }
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// speed_level 0=FAST 1=NORMAL 2=SLOW
static int calc_speed_level(float airspeed) {
  //FAST(10.8m/s~), NORMAL(9.5~10.8), SLOW(~9.5)
  if (airspeed > 10.8f) {
    return 0; //FAST
  } else if (9.5f <= airspeed && airspeed <= 10.8f) {
    return 1; //NORMAL
  } else {
    return 2;
  }
}

// flight_phase 0=PLATFORM 1=HIGH_LEVEL 2=MID_LEVEL 3=LOW_LEVEL
static int calc_flight_phase(float altitude) {
  // HIGH_LEVEL(超音波高度1m~), MID_LEVEL(0.3~1m), LOW_LEVEL(0.3m未満想定)
  if (altitude > 1.0f) {
    return 1; //HIGH_LEVEL
  } else if (0.3f <= altitude && altitude <= 1.0f) {
    return 2; //MID_LEVEL
  } else if (0.0f < altitude && altitude < 0.3f) {
    return 3; //LOW_LEVEL
  } else {
    return 4; //PLATFORM
  }
}

void speaker_init() {
  pinMode(O_SPK, OUTPUT);

  // 念のため停止
  noTone(O_SPK);

  // 初期状態を明示したい場合は、ここで初期値を入れてもOK
  // （speaker()内でstatic管理しているので必須ではない）
}

void speaker(float airspeed, float altitude) {

  static int sound_freq = 440; // サウンド周波数
  static int spk_flag = 0;     // 音が出ているかのフラグ
  static uint32_t speaker_last_change_time = 0;
  static const uint32_t sound_duration = 100; // 音が出ている時間(ms)

  // 初回呼び出し時の時刻初期化
  if (speaker_last_change_time == 0) {
    speaker_last_change_time = millis();
  }

  int speed_level_local = calc_speed_level(airspeed);
  int flight_phase_local = calc_flight_phase(altitude);


  switch (speed_level_local) {
    case 0: // FAST
      sound_freq = 440;
      break;
    case 1: // NORMAL
      sound_freq = 880;
      break;
    case 2: // SLOW
      sound_freq = 1320;
      break;
    default:
      break;
  }

  // インターバル決定
  float interval;
  const float altitude_max = 1.0f;
  const float altitude_min = 0.0f;

  switch (flight_phase_local) {
    case 0: // PLATFORM
      interval = 1000; // 実質鳴らさないので値は任意
      break;
    case 1: // HIGH_LEVEL
      interval = 900;
      break;
    case 2: // MID_LEVEL
      interval = float_map(altitude, altitude_min, altitude_max, 125, 700);
      break;
    case 3: // LOW_LEVEL
      interval = float_map(altitude, altitude_min, altitude_max, 125, 700);
      break;
    default:
      interval = 0;
      break;
  }

  // intervalが小さすぎる/不正な場合の保護
  if (interval < (float)sound_duration) {
    interval = (float)sound_duration;
  }

  uint32_t current_time = millis();
  uint32_t off_duration = (uint32_t)interval - sound_duration;

  // PLATFORMでない＝離陸後なら鳴らす
  if (flight_phase_local != 0) {
    if (spk_flag == 0 && (current_time - speaker_last_change_time) > off_duration) {
      tone(O_SPK, sound_freq);
      speaker_last_change_time = current_time;
      spk_flag = 1;
    } else if (spk_flag == 1 && (current_time - speaker_last_change_time) > sound_duration) {
      noTone(O_SPK);
      speaker_last_change_time = current_time;
      spk_flag = 0;
    }
  } else {
    // PLATFORM中は確実に停止（状態もリセットしたければここで spk_flag=0 等）
    if (spk_flag != 0) {
      noTone(O_SPK);
      spk_flag = 0;
      speaker_last_change_time = current_time;
    }
  }
}