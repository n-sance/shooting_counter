import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras import Input
from tensorflow.data import Dataset
import numpy as np
import itertools


def data_generator(acc_treshold=30, piezo_treshold=100):
	while (True):
		# x = np.random.uniform()
		# y = np.random.uniform()
		# z = np.random.uniform()
		p = np.random.uniform(0.0, 1000.0)
		acc = np.random.uniform(0.0, 1000.0)
		# our input data is an array containing 4 numbers
		X = [p, acc]
		# our label is 1 or 0
		# Y = 1 if (x > acc_treshold or y > acc_treshold or z > acc_treshold) and p > piezo_treshold else 0
		Y = 1 if (acc > acc_treshold) and p > piezo_treshold else 0
		# our generator should return the input data and the label
		yield X, [Y]


# create a dataset from our generator
train_dataset = tf.data.Dataset.from_generator(
	data_generator,
	output_types=(tf.float32, tf.int32),
	output_shapes=((2), (1)) #nb
)

print('train dataset: ', train_dataset)

train_dataset = train_dataset.batch(batch_size=30)

model = Sequential([
    Input(shape=(2)),
    Dense(5, activation='relu'),
    Dense(1, activation='sigmoid')
])

print(model)

model.compile(optimizer='adam',
              loss=tf.keras.losses.BinaryCrossentropy(),
              metrics=['accuracy'])

model.summary()

model.fit(
    train_dataset,
    steps_per_epoch=1000,
    epochs=4
)

test_X = np.array([
    [300, 500],
    [0.3, 0.4],
    [12, 90000],
    [0.7, 0.2]
])
Y = model.predict_on_batch(test_X)
np.set_printoptions(formatter={'float': lambda x: "{0:0.2f}".format(x)})
print(Y)


converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
def representative_dataset_gen():
    for _ in range(10000):
        yield [
            np.array(
                [np.random.uniform(), np.random.uniform()]
            , dtype=np.float32)
        ]
converter.representative_dataset = representative_dataset_gen
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
tflite_quant_model = converter.convert()
open("converted_model.tflite", "wb").write(tflite_quant_model)