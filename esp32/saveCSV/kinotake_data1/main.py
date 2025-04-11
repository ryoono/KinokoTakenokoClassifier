import os
import glob
import pandas as pd
import numpy as np

# CSVファイルが保存されているディレクトリのパスを指定してください
directory = "./"  # 例: "C:/data/csvfiles" または "/home/user/csvfiles"

# 指定ディレクトリ内の全CSVファイルを取得
csv_files = glob.glob(os.path.join(directory, "*.csv"))

for csv_file in csv_files:
    # CSVファイルをヘッダーなしで読み込む（ExcelでS列は19個目のデータに該当）
    df = pd.read_csv(csv_file, header=None, delimiter=",")
    
    # 置換対象の列は全列から19番目（インデックス18）を除外した列
    columns_to_replace = df.columns.tolist()
    if 18 in columns_to_replace:
        columns_to_replace.remove(18)
    
    # 置換処理：各セルの値に対して範囲に応じた整数へ変換
    for col in columns_to_replace:
        df[col] = pd.to_numeric(df[col], errors='coerce')  # 数値型に変換
        df[col] = df[col].apply(lambda x: int(min(x // 100, 10)) if pd.notna(x) else x)
    
    # 修正後のデータを新規ファイルとして保存（元ファイル名に "_modified" を追加）
    base, ext = os.path.splitext(csv_file)
    new_file = f"{base}_modified{ext}"
    df.to_csv(new_file, index=False, header=False)
    
    print(f"{csv_file} を {new_file} として保存しました。")
