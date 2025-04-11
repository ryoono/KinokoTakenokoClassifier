import pandas as pd
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score
import joblib
import numpy as np

# CSVファイルのパス（各行：18個のセンサーデータ, ラベル）
CSV_FILE = "data_with_labels.csv"

# CSVデータの読み込み（ヘッダが無い場合はheader=Noneで読み込む）
data = pd.read_csv(CSV_FILE, header=None)

# 特徴量（先頭18列）とラベル（最後の1列）に分割
X = data.iloc[:, 0:18]
y = data.iloc[:, 18]

# 訓練データとテストデータに分割（例：80% 訓練、20% テスト）
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# 線形SVM（LinearSVC）による学習（multi-classはone-vs-restで自動実施）
svm = LinearSVC(max_iter=10000, random_state=42)
svm.fit(X_train, y_train)

# テストデータで推論し、正答率を評価
y_pred = svm.predict(X_test)
acc = accuracy_score(y_test, y_pred)
print("正答率:", acc)
print("分類レポート:")
print(classification_report(y_test, y_pred))

# 学習した係数（クラス数 x 特徴量数）と切片（各クラスごと）
print("係数:\n", svm.coef_)
print("切片:\n", svm.intercept_)

# 学習済みモデルの保存（後で参照・評価する場合などに利用）
joblib.dump(svm, "linear_svm_model.pkl")
print("学習済みモデルを 'linear_svm_model.pkl' として保存しました。")

# ESP32での推論用に、パラメータをC言語のヘッダーファイル形式に出力
coefficients = svm.coef_
intercepts = svm.intercept_

num_classes, num_features = coefficients.shape

header_filename = "model_params.h"
with open(header_filename, "w") as f:
    f.write("#ifndef MODEL_PARAMS_H\n")
    f.write("#define MODEL_PARAMS_H\n\n")
    f.write("// 線形SVMモデルのパラメータ\n")
    f.write("// クラス数: {}\n".format(num_classes))
    f.write("// 特徴量数: {}\n\n".format(num_features))
    
    # 切片の配列
    intercept_str = ", ".join("{:.6f}".format(x) for x in intercepts)
    f.write("const float INTERCEPT[{}] = {{ {} }};\n\n".format(num_classes, intercept_str))
    
    # 係数の配列（2次元配列）
    f.write("const float COEFFICIENTS[{}][{}] = {{\n".format(num_classes, num_features))
    for i in range(num_classes):
        coef_str = ", ".join("{:.6f}".format(x) for x in coefficients[i])
        f.write("    { " + coef_str + " }")
        if i != num_classes - 1:
            f.write(",\n")
        else:
            f.write("\n")
    f.write("};\n\n")
    
    f.write("#endif // MODEL_PARAMS_H\n")

print("ESP32用のパラメータを '{}' に出力しました。".format(header_filename))
