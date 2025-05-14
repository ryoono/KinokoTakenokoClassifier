import serial
import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# --- シリアルポート設定 ---
PORT = 'COM18'  # あなたのESP32ポートに変更してください
BAUD_RATE = 115200

# --- 最新データを格納する変数 ---
latest_raw = np.zeros((3, 6))
latest_compressed = np.zeros((3, 6))
latest_class = -1
data_lock = threading.Lock()

# --- シリアル受信スレッド ---
def read_serial():
    global latest_raw, latest_compressed, latest_class
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()

            if not (line.startswith("5,") and line.endswith(",6")):
                continue

            print(f"[Received] {line}")

            # "5," と ",6" を取り除く
            content = line[len("5,"):-len(",6")]

            # 空文字列を除外してから int に変換
            tokens_str = [s for s in content.split(",") if s.strip() != '']
            if len(tokens_str) != 19:
                print(f"[Warning] データ数が19ではなく {len(tokens_str)} 個: {tokens_str}")
                continue

            tokens = list(map(int, tokens_str))
            predicted_class = tokens[0]
            sensor_values = tokens[1:]

            raw = np.array(sensor_values).reshape((3, 6))
            compressed = np.where(raw >= 1000, 10, raw // 100)

            with data_lock:
                latest_raw = raw
                latest_compressed = compressed
                latest_class = predicted_class

        except Exception as e:
            print(f"[Serial Error] {e}")

# --- 描画準備 ---
fig, axs = plt.subplots(1, 2, figsize=(10, 5))
raw_ax, comp_ax = axs

raw_img = raw_ax.imshow(np.zeros((3, 6)), vmin=0, vmax=4095, cmap='YlOrRd')
comp_img = comp_ax.imshow(np.zeros((3, 6)), vmin=0, vmax=10, cmap='YlGnBu')

raw_texts = [[raw_ax.text(j, i, "", ha="center", va="center") for j in range(6)] for i in range(3)]
comp_texts = [[comp_ax.text(j, i, "", ha="center", va="center") for j in range(6)] for i in range(3)]
title = fig.suptitle("", fontsize=32)  # ここでフォントサイズ指定

raw_ax.set_title("Raw Sensor Data (0–4095)")
comp_ax.set_title("Compressed Data (0–10)")
for ax in axs:
    ax.set_xticks([])
    ax.set_yticks([])

# --- 描画更新関数 ---
def update(frame):
    global latest_raw, latest_compressed, latest_class

    with data_lock:
        raw = latest_raw.copy()
        compressed = latest_compressed.copy()
        pred_class = latest_class

    raw_img.set_data(raw)
    comp_img.set_data(compressed)

    for i in range(3):
        for j in range(6):
            raw_texts[i][j].set_text(str(raw[i][j]))
            comp_texts[i][j].set_text(str(compressed[i][j]))

    if np.any(compressed == 0):
        if pred_class in [0, 1]:
            title.set_text("KINOKO")
        elif pred_class in [2, 3]:
            title.set_text("TAKENOKO")
        else:
            title.set_text("推論結果なし")
    else:
        title.set_text("")

# --- シリアルポートを開く ---
try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"[Info] Serial port {PORT} opened.")
except Exception as e:
    print(f"[Error] Failed to open serial port: {e}")
    exit(1)

# --- 受信スレッド開始 ---
thread = threading.Thread(target=read_serial, daemon=True)
thread.start()

# --- アニメーション開始 ---
ani = animation.FuncAnimation(fig, update, interval=200)
plt.tight_layout()
plt.show()