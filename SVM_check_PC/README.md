回路図が正しいか確認するためのプロトタイプ  
また、PC上で機械学習による分類ができるかの確認を行う  
将来的にマイコン側で推論したいため、データは極力少なくする  

```bash
.
│  createSVMmodel.py		            # data1-18_label(kino0_take1).csvからSVMモデルを作成するプログラム
│  serial2csv.py			            # Arduinoから送られてくるセンサデータをcsvに保存するプログラム
│  serialKinoTakeclassifier.py		    # Arduinoから送られてくるセンサデータを推論するプログラム
│  svm_model.pkl			            # createSVMmodel.pyで作成した「きのこたけのこ分類モデル」
├─arduino_src
│      arduino_src.ino		            # センサデータを取得しPCに送信するArduinoのプログラム
├─label_data
│      data1-18_label(kino0_take1).csv	# きのことたけのこのセンサデータ(ラベル付き)
├─schematic
│      受光側.jpg			            # 回路図(受光側)
│      投光側.jpg			            # 回路図(投光側)
└─sensor_data
       kinoko_data.csv		            # serial2csv.pyで作成した、きのこのセンサデータ(ラベル無し)
       takenoko_data.csv	            # serial2csv.pyで作成した、たけのこのセンサデータ(ラベル無し)
```
