import matplotlib.pyplot as plt
import numpy as np

# 1751,1939,432,0,0,1285
# 1933,1627,101,0,0,0
# 58,57,54,0,0,993

# 1793,1918,286,0,0,1750
# 2252,2725,0,0,0,35
# 1813,2320,351,0,0,50
# 0,0,0,741,2143,2161
# 0,0,0,61,401,1430
# 846,0,1,139,36,0

# 1809,2007,2221,848,7,0
# 2255,2786,469,0,0,0
# 1780,436,0,0,0,0
# 1475,176,9,973,2257,2229
# 0,0,0,146,3173,2132
# 0,0,0,21,2180,1995
# 1729,1856,1073,0,80,2022
# 1979,653,0,0,0,1617
# 33,0,0,0,0,1610

# 1586,168,0,654,1862,2081
# 1477,0,0,0,125,1371
# 1194,0,0,0,2,784

# --- 仮の3x6生データ（ラベル0〜3） ---
label_data = {
    0: np.array([[1751,1939,432,0,0,1285],
                 [1933,1627,101,0,0,0],
                 [58,57,54,0,0,993]]),
    1: np.array([[0,0,0,741,2143,2161],
                 [0,0,0,61,401,1430],
                 [846,0,1,139,36,0]]),
    2: np.array([[586,168,0,654,1862,2081],
                 [477,0,0,0,125,1371],
                 [194,0,0,0,2,784]]),
    3: np.array([[1729,1856,1073,0,80,2022],
                 [1979,653,0,0,0,1617],
                 [33,0,0,0,0,1610]])
}

label_names = {
    0: "Label 0 - KINOKO",
    1: "Label 1 - KINOKO",
    2: "Label 2 - TAKENOKO",
    3: "Label 3 - TAKENOKO"
}

# --- A4横のキャンバス 2x2 グリッド ---
fig, axs = plt.subplots(2, 2, figsize=(11.69, 8.27))  # A4 landscape

for ax, label in zip(axs.flat, range(4)):
    data = label_data[label]
    im = ax.imshow(data, vmin=0, vmax=4095, cmap='YlOrRd')

    # 数値をセルに描画
    for i in range(3):
        for j in range(6):
            ax.text(j, i, str(data[i, j]), ha="center", va="center", fontsize=8, color="black")

    ax.set_title(label_names[label], fontsize=14)
    ax.set_xticks([])
    ax.set_yticks([])

# --- レイアウト調整・保存 ---
plt.tight_layout()
plt.savefig("heatmap_labels.png", dpi=300)
print("画像保存完了：heatmap_labels.png")
