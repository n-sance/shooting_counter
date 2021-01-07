import serial
import csv
import time
import datetime
import matplotlib.pyplot as plt
import argparse



BAUDRATE = '9600'

#CHANGE NAME OF YOUR COM PORt BEFORE USING
COM = '/dev/cu.SLAB_USBtoUART'

class ComConnector():
    def __init__(self):
        self.session = None
        self.buffer_size = 1024

    def _connect(self, com, baudrate):
        self.session = serial.Serial(port=com, baudrate=baudrate)
        print(f'waiting for output from {COM} on {BAUDRATE}')
        text = self.session.readline(self.buffer_size)
        text_str = text.decode("utf-8")
        return self.session.portstr + ":  " + text_str

    def read_from_device(self, path_to_output_file=None):
        line = self.session.readline(self.buffer_size)
        print('after line')
        with open(path_to_output_file, 'w') as f:
            f.write("a,p\n")
            while line and ("finish" not in str(line) and "stopped" not in str(line)):
                line = self.session.readline(self.buffer_size)
                if ("finish" not in str(line)) and ("stopped" not in str(line)):
                    f.write(line.decode("utf-8").strip() + '\n')
            line = self.session.readline(self.buffer_size)
            print(line.decode('utf-8'))
            print('it finished')

def parse_raw_data(path):
    output = {'a': [], 'piezo': []}
    with open(path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            output['a'].append(row['a'])
            output['piezo'].append(row['p'])
    return output

def handle_args():
    timestamp = str(time.time())
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('--filename', dest='name', default=timestamp, help='namestamp of output file with measurements')
    args = arg_parser.parse_args()
    return args

if __name__ == "__main__":
    args = handle_args()
    path_to_measurement = f'/Users/admin/my_projects/shoot_counter/Shoot_counter/test/measurements/{args.name}.csv'
    conn = ComConnector()
    conn._connect(COM, BAUDRATE)
    conn.read_from_device(path_to_measurement)

    result = parse_raw_data(path_to_measurement)

    fig, axs = plt.subplots(2)
    hor_axis = [x for x in range(len(result['a']))]

    axs[0].plot(hor_axis, result['a'])
    axs[1].plot(hor_axis, result['piezo'])

    plt.show()


    print('ok')
