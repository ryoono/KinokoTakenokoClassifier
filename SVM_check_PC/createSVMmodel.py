from sklearn.svm import SVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import pandas as pd
import joblib

# データの読み込み
data = pd.read_csv("./label_data/data1-18_label(kino0_take1).csv")  # ラベル付きセンサデータ
X = data.iloc[:, :-1].values  # 特徴量
y = data.iloc[:, -1].values   # ラベル

# データの分割
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# SVMモデルの作成
svm_model = SVC(kernel='linear', random_state=42)

# モデルの訓練
svm_model.fit(X_train, y_train)

# モデルの評価
y_pred = svm_model.predict(X_test)
print("Accuracy:", accuracy_score(y_test, y_pred))

# モデルを保存
joblib.dump(svm_model, "svm_model.pkl")
print("Model saved as svm_model.pkl")
