# 飛行禁止区域接近警報
## 機能の構成
禁止区域侵入判定と禁止区域到着予想計算で構成されている．

## アルゴリズムとソースコードの解説

### 禁止区域侵入判定
まずは複数の頂点$`\mathrm{B_1}`$～$`\mathrm{B_n}`$ ($`n=3,4,5...`$)を設定し，飛行区域を構成する．また機体の位置を点$`\mathrm{P}`$とする．


<img src="https://raw.githubusercontent.com/TORICA-Org/26th_PitchSender/refs/heads/dev/dev/restricted_zone_alert/figures/fig1.svg"/>


<img src="https://raw.githubusercontent.com/TORICA-Org/26th_PitchSender/refs/heads/dev/dev/restricted_zone_alert/figures/fig2.svg"/>

上図において次式が成立する．
```math
\overrightarrow{\mathrm{B_{k-1} B_{k}}} \times \overrightarrow{\mathrm{B_{k-1}P}} = |\overrightarrow{\mathrm{B_{k-1} B_{k}}}||\overrightarrow{\mathrm{B_{k-1}P}}| \sin \theta
```
反時計回りのベクトルで飛行禁止区域を囲んでいるので，境界線ベクトルの左側（飛行可能区域）にいるとき$`\theta>0`$．言い換えると飛行可能区域内にいるということは$`\sin \theta>0`$である．

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

使用するGPSデータ：
`data_air_gps_groundspeed_ms`, `data_air_gps_heading_deg`．

到達予想時間(time to reach)は
```math
t = \dfrac{L_{\mathrm{B_{k-1} B_k }} \sin \theta_{\mathrm{B_k P} }}{v \sin(\alpha - \beta)}
```
である．



