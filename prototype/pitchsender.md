# 胴体桁基板(Pitchsender)用プログラム
## 胴体桁基板(Pitchsender)が担う役割
- テール保持者にBluetoothイヤホン経由でPitchを伝える．
- BNO055で姿勢角を取得する．→ Roll, Pitch, Yawを計算
- BMP390で気温・気圧を取得．→ 気圧高度を計算
- MicroSDにデータを記録．
- スピーカーを通してPに速度や高度，飛行禁止区域への接近を伝達．

## Pitchsenderの構成
- ESP32-WROOM-32E
- 9軸センサーモジュール BNO055
- 気圧気温センサー BMP390
- アンプ・スピーカー
- MicroSDスロット

## プログラムの構成
### 飛行禁止区域接近警報について
Under construction