import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC

# CSVファイルの読み込み
df = pd.read_csv("data_with_labels_0~1.csv", header=None)

# 特徴量とラベルに分割
X = df.iloc[:, :18].values
y = df.iloc[:, 18].values

# 学習用とテスト用に分割
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# モデル定義（順番維持）
models = [
    ("Logistic Regression", LogisticRegression(max_iter=200)),
    ("Decision Tree", DecisionTreeClassifier()),
    ("Naive Bayes", GaussianNB()),
    ("K-Nearest Neighbors", KNeighborsClassifier(n_neighbors=3)),
    ("Tiny Neural Network", MLPClassifier(hidden_layer_sizes=(16,), max_iter=300)),
    ("Random Forest", RandomForestClassifier(n_estimators=50)),
    ("SVM (Linear)", SVC(kernel="linear")),
    ("SVM (RBF)", SVC(kernel="rbf"))
]

# 結果保存リスト
results = []

# 学習・評価
for name, model in models:
    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)
    acc = accuracy_score(y_test, y_pred)
    results.append((name, acc))

# 表示・Excel用フォーマット出力
print("モデル名,精度")
for name, acc in results:
    print(f"{name},{acc:.4f}")
