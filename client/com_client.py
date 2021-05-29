import serial
import csv
import time
import matplotlib.pyplot as plt
import argparse


BAUDRATE = "9600"

# CHANGE NAME OF YOUR COM PORt BEFORE USING
# COM_DEVICE_NAME = "/dev/cu.SLAB_USBtoUART"
COM_DEVICE_NAME = "/dev/ttyUSB0"
RESULT_PATH = f"../measurements"


class ComClient:
    def __init__(self):
        self.session = None
        self.buffer_size = 1024

    def connect(self, com, baudrate):
        self.session = serial.Serial(port=com, baudrate=baudrate)

        print(f"waiting for output from {COM_DEVICE_NAME} on {BAUDRATE}")

        text = self.session.readline(self.buffer_size)
        text_str = text.decode("utf-8")

        return self.session.portstr + ":  " + text_str

    def read_from_device(self, path_to_output_file=None):
        line = self.session.readline(self.buffer_size)
        print("after line")

        with open(path_to_output_file, "w") as f:
            f.write("a,p\n")
            while line and ("finish" not in str(line) and "stopped" not in str(line)):
                line = self.session.readline(self.buffer_size)

                if ("finish" not in str(line)) and ("stopped" not in str(line)):
                    f.write(line.decode("utf-8").strip() + "\n")

            line = self.session.readline(self.buffer_size)

            print(line.decode("utf-8"))
            print("it finished")


def parse_raw_data(path):
    output = {"a": [], "piezo": []}
    with open(path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            output["a"].append(row["a"])
            output["piezo"].append(row["p"])
    return output


def handle_args() -> str:
    timestamp = str(time.time())
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument(
        "--filename",
        dest="name",
        default=timestamp,
        help="namestamp of output file with measurements",
    )
    args = arg_parser.parse_args()

    return args.name


if __name__ == "__main__":
    file_name = handle_args()

    conn = ComClient()
    conn.connect(COM_DEVICE_NAME, BAUDRATE)

    file_path = f"{RESULT_PATH}/{file_name}.csv"
    conn.read_from_device(file_path)

    result = parse_raw_data(file_path)

    fig, axs = plt.subplots(2)
    hor_axis = [x for x in range(len(result["a"]))]

    axs[0].plot(hor_axis, result["a"])
    axs[1].plot(hor_axis, result["piezo"])

    plt.show()

    print("ok")
