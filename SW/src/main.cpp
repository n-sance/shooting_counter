#include "main.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"
#include <string>
OneButton button(32, true);
OneButton button_2(34, true);
Adafruit_ADXL345_Unified accel(12345);
Ticker time1(get_data_from_sensors, SENSORS_POLL_FREQ_US, 0, MICROS);
Ticker time2(get_shoots, SENSORS_POLL_FREQ_US, 0, MICROS);
std::deque<int> deque_acc(DEQUE_SIZE, 0);
std::deque<int> deque_piez(DEQUE_SIZE, 0);
ShootDetector shoot_detector(100, 200, 3);
// SSD1306Wire  display(0x3c, 21, 22);
Display display(0x3c, 21, 22);
bool is_finished = false;
void monitor_mode()
{
    is_finished = false;
    time1.start();
}

void count_mode(){
  time2.start();
}

MenuItem* menu = init_menu(monitor_mode, count_mode);

void get_shoots()
{
    sensors_event_t event;
    accel.getEvent(&event);
    int vibro = analogRead(25);
    int sum_acc = get_summ_values_from_acc(event.acceleration.x, event.acceleration.y, event.acceleration.z);
    deque_acc.push_back(sum_acc);
    deque_piez.push_back(vibro);
    deque_acc.pop_front();
    deque_piez.pop_front();

    shoot_detector.potential_shoot_check(deque_acc, deque_piez);
    Serial.print(sum_acc); Serial.print(",");
    Serial.println(vibro);
    display.PrintString(String("Shoots:"), true, false, ArialMT_Plain_24);
    display.PrintString(String(shoot_detector.shoots), false, true, ArialMT_Plain_24, 100);
}


void get_data_from_sensors()
{
    sensors_event_t event;
    static int vibro_max = 0;
    static int acc_max = 0;
    accel.getEvent(&event);
    int vibro = analogRead(25);
    int sum_acc = get_summ_values_from_acc(event.acceleration.x, event.acceleration.y, event.acceleration.z);
    deque_acc.push_back(sum_acc);
    deque_piez.push_back(vibro);
    deque_acc.pop_front();
    deque_piez.pop_front();

    shoot_detector.potential_shoot_check(deque_acc, deque_piez);
    Serial.print(sum_acc); Serial.print(",");
    Serial.println(vibro);
    display.PrintString(String(sum_acc), true, false, ArialMT_Plain_24);
    display.PrintString(String(vibro), false, false, ArialMT_Plain_24, 32);

    if (vibro > vibro_max){
      vibro_max = vibro;
    }
    display.PrintString(String(vibro_max), false, true, ArialMT_Plain_10, 32, 5);
    if (sum_acc > acc_max){
      acc_max = sum_acc;
    }
    display.PrintString(String(acc_max), false, true, ArialMT_Plain_10, 0, 5);
}




// void stop_monitor(void)
// {
//   is_finished = true;
//   time1.stop();
//   Serial.println("stopped");
//     digitalWrite(2,HIGH);
//   delay(500);
//   digitalWrite(2,LOW);
// }

void changeMode(void)
{
  menu = getNext(menu);
  display.PrintString(menu->title, true, true, ArialMT_Plain_16);
  button.attachClick(menu->trigger_on_btn);
}

void setup(void)
{
  pinMode(2,OUTPUT);
  // button.attachDoubleClick(monitor_mode);
  // button.attachClick(stop_monitor);
  button_2.attachClick(changeMode);
  // button.attachClick(prnt1);

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("ERROR: No ADXL345 detected");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);

  dataRate_t rate(ADXL345_DATARATE_3200_HZ);
  accel.setDataRate(rate);

  /* Display some basic information on this sensor */
  displaySensorDetails(accel);

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate(accel);
  displayRange(accel);

  // bool  display_status = display.init();
  if (display.init() == false)
  {
    Serial.println("ERROR: Display not detected");
    while(1);
  }
  // display.flipScreenVertically();
  // display.setFont(ArialMT_Plain_10);
  Serial.println("INFO: Configured. Ready to work");
  display.PrintString("SC ver. 0.2. Please wait..", true, true, ArialMT_Plain_10);
  // drawString(String("Shoot counter ver0.1"));
  sleep(3);
  display.PrintString(menu->title, true, true, ArialMT_Plain_24);
}


void loop() {
  // if (time1.counter() == SENSORS_POL_CNT && !is_finished)
  // {
  //   is_finished = true;
  //   Serial.println("finish");
  //   Serial.print("total shoots: "); Serial.println(f.shoots);
  // }
  time1.update();
  time2.update();
  button.tick();
  button_2.tick();
  // display.clear();
  // drawFontFaceDemo(f.shoots);
  // display.display();
}
