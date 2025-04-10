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
    
    # 置換処理：対象の各セルに対して900以下なら0、900より大きければ1に置換
    for col in columns_to_replace:
        # 数値型として変換し、置換処理を実施
        df[col] = pd.to_numeric(df[col], errors='coerce')
        df[col] = np.where(df[col] <= 900, 0, 1)
    
    # 修正後のデータを新規ファイルとして保存（元ファイル名に "_modified" を追加）
    base, ext = os.path.splitext(csv_file)
    new_file = f"{base}_modified{ext}"
    df.to_csv(new_file, index=False, header=False)
    
    print(f"{csv_file} を {new_file} として保存しました。")
