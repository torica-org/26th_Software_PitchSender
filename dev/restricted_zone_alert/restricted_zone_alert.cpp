#include "restricted_zone_alert.h"
#include "parameters.h"
#include <TinyGPS++.h>

struct Point {
    double lat; // 緯度
    double lon; // 経度
}

const Point boundary[] = {
  {35.00000001, 136.0000001}, // B1: プラットフォーム側
  {35.00000002, 136.0000002}, // B2: 観覧席側
  {35.00000003, 136.0000003}, // B3: 沖側1
  {35.00000004, 136.0000004}  // B4: 沖側2
};

const int NUM_VERTICES = sizeof(boundary) / sizeof(boundary[0]);


// 飛行可能範囲かどうか判定
/* 仕組み：外積を使って計算 */
bool checkInsideArea(Point p) {
  // すべての辺に対して外積を計算
  for (int i = 0; i < NUM_VERTICES; i++) {
    // 余りを活用して Bn の次は B1(index:0) に戻るようにする
    int next_i = (i + 1) % NUM_VERTICES; 

    // B_k から B_k+1 へのベクトル
    double bx_ax = boundary[next_i].lon - boundary[i].lon;
    double by_ay = boundary[next_i].lat - boundary[i].lat;
    
    // B_k から 機体P へのベクトル
    double px_ax = p.lon - boundary[i].lon;
    double py_ay = p.lat - boundary[i].lat;

    // 外積 Z = (Bx-Ax)(Py-Ay) - (By-Ay)(Px-Ax)
    double z = (bx_ax * py_ay) - (by_ay * px_ax);

    // Z < 0 が一つでもあれば、多角形の右側（外）に出ている
    if (z < 0) {
      return false; 
    }
  }
  // 全ての辺で Z >= 0 ならエリア内
  return true;
}

// 禁止エリアまでの予想時間を計算
void calc_time_to_reach(){

}