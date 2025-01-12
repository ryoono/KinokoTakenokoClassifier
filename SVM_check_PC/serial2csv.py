import serial
import csv
import time

# シリアルポートと通信速度の設定
SERIAL_PORT = "COM3"  # 実際のポート番号に変更してください
BAUD_RATE = 9600
CSV_FILE = "kinoko_data.csv"

def main():
    try:
        # シリアル通信を初期化
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} bps")
            
            # CSVファイルをオープン
            with open(CSV_FILE, mode="w", newline="") as file:
                writer = csv.writer(file)
                writer.writerow(["Sensor Data"])  # ヘッダー行
                
                while True:
                    # シリアルポートからデータを受信
                    line = ser.readline().decode("utf-8").strip()
                    if line:
                        # データをカンマで分割
                        data = line.split(",")
                        
                        # 正しいフォーマットか確認
                        if len(data) == 20 and data[0] == "5" and data[-1] == "6":
                            # センサデータ部分をCSVに書き込む
                            writer.writerow(data[1:-1])
                            print("Received and saved:", data[1:-1])
                        else:
                            print("Invalid data:", line)
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
