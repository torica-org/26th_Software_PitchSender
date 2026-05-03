# 飛行禁止区域接近警報
## 機能の構成
禁止区域侵入判定と禁止区域到着予想計算で構成されている．

## アルゴリズムとソースコードの解説

### 禁止区域侵入判定
まずは複数の頂点$`\mathrm{B_1}`$～$`\mathrm{B_n}`$ ($`n=3,4,5...`$)を設定し，飛行区域を構成する．また機体の位置を点$`\mathrm{P}`$とする．


<img src="https://raw.githubusercontent.com/TORICA-Org/26th_PitchSender/refs/heads/dev/dev/restricted_zone_alert/figures/fig1.svg"/>


<img src="https://raw.githubusercontent.com/TORICA-Org/26th_PitchSender/refs/heads/dev/dev/restricted_zone_alert/figures/fig2.svg"/>

上図において，任意の境界線ベクトル$`\overrightarrow{\mathrm{B_k B_{k+1}}}`$次式が成立する．

```math
\overrightarrow{\mathrm{B_{k-1} B_{k}}} \times \overrightarrow{\mathrm{B_{k-1}P}} = \left| \overrightarrow{\mathrm{B_{k-1} B_{k}}} \right|  \left| \overrightarrow{\mathrm{B_{k-1}P}} \right| \sin \theta
```
（ただし$`k=1`$のとき$`\mathrm{B_{k-1}}`$は$`\mathrm{B_n}`$とする．）

反時計回りのベクトルで飛行可能区域を囲んでいるので，境界線ベクトルの左側（飛行可能区域）にいるとき$`\theta>0`$．言い換えると飛行可能区域内にいるということは$`\sin \theta>0`$である．

そのため以下の外積計算を行い，その正負によって領域を判定すればよい．

```math
\begin{gather}
\overrightarrow{\mathrm{B_1 B_2}} \times \overrightarrow{\mathrm{B_1 P}} \\
\overrightarrow{\mathrm{B_2 B_3}} \times \overrightarrow{\mathrm{B_2 P}} \\
\vdots\\
\overrightarrow{\mathrm{B_{n-1} B_n}} \times \overrightarrow{\mathrm{B_{n-1} P}} \\
\overrightarrow{\mathrm{B_n B_1}} \times \overrightarrow{\mathrm{B_n P}}
\end{gather}
```

```math
\left\{
\begin{align}
& > 0 \text{（飛行可能区域内）}\\
& = 0 \text{（境界線上）} \\
& < 0 \text{（禁止区域内）}
\end{align}
\right.
```

### 禁止区域到着予想計算
現在地から最も近い境界線までの到達予想時間を計算する．

| GPSデータ等マイコンから取得する値  | 図中のパラメータ |
| ------------- | ------------- |
| `data_air_gps_groundspeed_ms`  | $`v`$  |
| `data_air_gps_heading_deg`  | $`\alpha`$  |

他に定数として境界線用に設定した緯度経度を用いる．



ある境界線（点$`\mathrm{B_{k-1}}`$から点$`\mathrm{B_k}`$に向かう線分）について考える．機体と境界線の位置関係を以下の図に示した．
<img src="https://raw.githubusercontent.com/TORICA-Org/26th_PitchSender/refs/heads/dev/dev/restricted_zone_alert/figures/fig3.svg"/>

機体から境界線までの最短距離は，機体から境界線を延長した直線へ下ろした垂線の長さに等しい．これとGPSから受信した対地速度と方位を組み合わせることで接近速度成分を得ることができ，次式で到達予想時間(time to reach: $`t`$)が求められる。

```math
t = \dfrac{\mathrm{L_{B_k P}} \cdot \sin \phi_k}{v \sin(\alpha - \theta_k)}
```



