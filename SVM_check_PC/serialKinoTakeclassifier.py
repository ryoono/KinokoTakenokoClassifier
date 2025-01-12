import serial
import joblib
import numpy as np

# モデルの読み込み
svm_model = joblib.load("svm_model.pkl")
print("Model loaded.")

# シリアルポート設定
SERIAL_PORT = "COM3"  # 実際のポート番号に変更
BAUD_RATE = 9600

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Listening on {SERIAL_PORT} at {BAUD_RATE} bps.")
            while True:
                # データをシリアルポートから受信
                line = ser.readline().decode('utf-8').strip()
                if line:
                    # データを処理
                    data = line.split(",")
                    if len(data) == 20 and data[0] == "5" and data[-1] == "6":
                        # センサデータ部分を抽出して分類
                        sensor_data = np.array(data[1:-1], dtype=int).reshape(1, -1)
                        prediction = svm_model.predict(sensor_data)
                        group = "きのこの山" if prediction[0] == 0 else "たけのこの里"
                        print(f"Sensor Data: {sensor_data[0]}, Predicted Group: {group}")
                    else:
                        print("Invalid data format:", line)
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
