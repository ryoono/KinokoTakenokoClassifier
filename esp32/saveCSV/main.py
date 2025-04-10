import serial
import csv
import time

# シリアルポートと通信設定（必要に応じてポート名を変更してください）
SERIAL_PORT = "COM18"
BAUD_RATE = 115200
TIMEOUT = 1  # 秒

# CSVファイルのパス（追記モードで書き出し）
CSV_FILENAME = "data.csv"

def append_row_to_csv(row, filename=CSV_FILENAME):
    """リストrowをCSVに追記する"""
    with open(filename, "a", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(row)

def process_message(message):
    """
    受信したメッセージからセンサーデータ18個を抽出してリストに変換する。
    メッセージの形式は "5,<d1>,<d2>,...,<d18>,6" とし、
    最初が "5"、最後が "6" で、間に18個のセンサーデータが存在することをチェックする。
    """
    # 文字列をカンマで分割
    tokens = message.split(",")
    
    # 最初と最後のトークンのチェックおよび個数チェック
    if len(tokens) < 2:
        return None
    if tokens[0] != "5" or tokens[-1].strip() != "6":
        return None
    # 期待されるトークン数は 1 (開始) + 18 (データ) + 1 (終了) = 20
    if len(tokens) != 20:
        print("受信トークン数が20個ではありません:", tokens)
        return None

    # 中間の18個をデータとして返す
    sensor_data = tokens[1:-1]
    return sensor_data

def main():
    try:
        # シリアルポートのオープン
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)
        print(f"シリアルポート {SERIAL_PORT} をオープンしました。")
    except Exception as e:
        print("シリアルポートをオープンできませんでした:", e)
        return

    message_buffer = ""
    while True:
        try:
            # シリアルから1行読み取り
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if not line:
                continue

            # 送信開始識別子 "5," で始まる行を検出
            if line.startswith("5,"):
                message_buffer = line
                # 受信ブロックの終了("6")が出るまで受信を続ける
                while "6" not in message_buffer:
                    next_line = ser.readline().decode("utf-8", errors="ignore").strip()
                    if next_line:
                        message_buffer += next_line

                sensor_data = process_message(message_buffer)
                if sensor_data is not None:
                    print("データ受信:", sensor_data)
                    append_row_to_csv(sensor_data)
                else:
                    print("不正なデータを受信:", message_buffer)
            # 少し待機してループ負荷を軽減
            time.sleep(0.1)
        except KeyboardInterrupt:
            print("ユーザーによって終了されました。")
            break
        except Exception as ex:
            print("エラー:", ex)
            continue

if __name__ == '__main__':
    main()
