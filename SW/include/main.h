#pragma once


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "OneButton.h"
#include "configs.h"
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "Ticker.h"
#include <deque>

typedef struct {
	int acc;
	int piezo;
} sensors_data_sample_t;

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

class Display : public SSD1306Wire{
public:
	Display(uint8_t _address, int _sda, int _scl, OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64);
	void PrintString(String text = "default text", bool preclear = false, bool render = true, const uint8_t* font = NULL, int16_t x = 0, int16_t y = 26, OLEDDISPLAY_TEXT_ALIGNMENT align = TEXT_ALIGN_LEFT);
};

class MenuItem {
public:
	char *title;
	void (*trigger_on_btn)();
	MenuItem *next;
};

void displaySensorDetails(Adafruit_ADXL345_Unified& accel);
void displayRange(Adafruit_ADXL345_Unified& accel);
void displayDataRate(Adafruit_ADXL345_Unified& accel);
int get_summ_values_from_acc(float x, float y, float z);
void debug_mode();
void count_mode();
// void get_data_from_sensors();
// void get_shoots();
int calculate_shoots();
void render_shoots();
void render_sensdata();
bool accelerometer_init();
sensors_data_sample_t get_data_from_sensors();
MenuItem *init_menu(void (*function1)(), void (*function2)());
MenuItem *getNext(MenuItem *current);
