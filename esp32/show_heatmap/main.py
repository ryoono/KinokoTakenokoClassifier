import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import re

# --- シリアルポートの設定 ---
PORT = '/dev/tty.usbserial-XXXXX'  # あなたのESP32ポートに変更してください
BAUD_RATE = 115200

# --- ヒートマップ用の行列初期化 ---
raw_data = np.zeros((3, 6))
compressed_data = np.zeros((3, 6))

# --- 描画の準備 ---
fig, axs = plt.subplots(1, 2, figsize=(10, 5))
raw_ax, comp_ax = axs
raw_img = raw_ax.imshow(raw_data, vmin=0, vmax=4095, cmap='YlOrRd')
comp_img = comp_ax.imshow(compressed_data, vmin=0, vmax=10, cmap='YlGnBu')

# 数値表示用テキストオブジェクト
raw_texts = [[raw_ax.text(j, i, "", ha="center", va="center", color="black") for j in range(6)] for i in range(3)]
comp_texts = [[comp_ax.text(j, i, "", ha="center", va="center", color="black") for j in range(6)] for i in range(3)]
title = fig.suptitle("")

raw_ax.set_title("Raw Sensor Data")
comp_ax.set_title("Compressed Sensor Data")

# 軸を非表示
for ax in axs:
    ax.set_xticks([])
    ax.set_yticks([])

# --- データ読み込みと描画更新 ---
def update(frame):
    global raw_data, compressed_data

    line = ser.readline().decode('utf-8').strip()
    if not line.startswith("5,") or not line.endswith(",6"):
        return

    try:
        line = line.strip("5,").strip(",6")
        tokens = list(map(int, line.split(",")))

        predicted_class = tokens[0]
        sensor_values = tokens[1:]

        if len(sensor_values) != 18:
            return

        # 配列に変換（3x6）
        raw_data = np.array(sensor_values).reshape((3, 6))
        compressed_data = np.where(raw_data >= 1000, 10, raw_data // 100)

        # ヒートマップ更新
        raw_img.set_data(raw_data)
        comp_img.set_data(compressed_data)

        # 数値ラベルの更新
        for i in range(3):
            for j in range(6):
                raw_texts[i][j].set_text(str(raw_data[i][j]))
                comp_texts[i][j].set_text(str(compressed_data[i][j]))

        # 推論結果の表示（0が含まれている場合のみ）
        if np.any(compressed_data == 0):
            if predicted_class in [0, 1]:
                title.set_text("きのこの山 🍄")
            elif predicted_class in [2, 3]:
                title.set_text("たけのこの里 🎍")
        else:
            title.set_text("")

    except Exception as e:
        print(f"Error parsing line: {line}\n{e}")

# --- シリアルポートのオープン ---
try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"Serial port {PORT} opened.")
except Exception as e:
    print(f"Failed to open serial port: {e}")
    exit(1)

# --- アニメーションの開始 ---
ani = animation.FuncAnimation(fig, update, interval=100)
plt.tight_layout()
plt.show()
