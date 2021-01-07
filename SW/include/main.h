#pragma once


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "OneButton.h"
#include "Ticker.h"
#include <deque>

#define SENSORS_POLL_FREQ_US 10
#define SENSORS_POL_CNT 3000
#define DEQUE_SIZE 10

void displaySensorDetails(Adafruit_ADXL345_Unified& accel);
void displayRange(Adafruit_ADXL345_Unified& accel);
void displayDataRate(Adafruit_ADXL345_Unified& accel);
void get_data_from_sensors(void);


class ShootDetector {
public:
	int acc_threshold;
	int piezo_threshold;
	int shoots;
	int timeout;
	int timeout_cycles_left;
	bool potential_shoot;
	bool acc_thr_exceeded;
	bool piezo_thr_exceeded;
	ShootDetector(int acc_thr, int piezo_thr, int timeout);
	void potential_shoot_check(std::deque<int>& a, std::deque<int>& p);
};
