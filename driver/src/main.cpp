#include "main.h"

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

Adafruit_ADXL345_Unified accel(12345);
OneButton button(32, true);
// OneButton button_2(24, true);
Ticker time1(get_data_from_sensors, SENSORS_POLL_FREQ_US, SENSORS_POL_CNT, MICROS);
ShootDetector f(100, 200, 3);

SSD1306Wire  display(0x3c, 21, 22);


std::deque<int> deque_acc(DEQUE_SIZE, 0);
std::deque<int> deque_piez(DEQUE_SIZE, 0);

bool is_finished = false;

int get_summ_values_from_acc(float x, float y, float z)
{
  int x_l = (int)x;
  int y_l = (int)y;
  int z_l = (int)z;
  if (x < 0)
    x_l *= -1;
  if (y < 0)
    y_l *= -1;
  if (z < 0)
    z_l *= -1;

  return (x_l + y_l + z_l);
}

void drawFontFaceDemo(int num) {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, String(num));
}

void get_data_from_sensors()
{
    sensors_event_t event;

    accel.getEvent(&event);

    int vibro = analogRead(34);
    int sum_acc = get_summ_values_from_acc(event.acceleration.x, event.acceleration.y, event.acceleration.z);
    deque_acc.push_back(sum_acc);
    deque_piez.push_back(vibro);
    deque_acc.pop_front();
    deque_piez.pop_front();

    f.potential_shoot_check(deque_acc, deque_piez);
    Serial.print(sum_acc); Serial.print(",");
    Serial.println(vibro);
}


void stop_monitor(void)
{
  is_finished = true;
  time1.stop();
  Serial.println("stopped");
    digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
}


void monitor_mode()
{
    is_finished = false;
    time1.start();
}


void setup(void)
{
  pinMode(2,OUTPUT);
  button.attachDoubleClick(monitor_mode);
  button.attachClick(stop_monitor);
  // button_2.attachClick(prnt);

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
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

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  Serial.println("Configured");
}


void loop() {
  if (time1.counter() == SENSORS_POL_CNT && !is_finished)
  {
    is_finished = true;
    Serial.println("finish");
    Serial.print("total shoots: "); Serial.println(f.shoots);
  }
  time1.update();
  button.tick();
      display.clear();
    drawFontFaceDemo(f.shoots);
    display.display();
}
