#include "main.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"
#include <string>
OneButton button(BUTTON_1_GPIO, true);
OneButton button_2(BUTTON_2_GPIO, true);
Ticker time1(render_sensdata, SENSORS_POLL_FREQ_US, 0, MICROS);
Ticker time2(render_shoots, SENSORS_POLL_FREQ_US, 0, MICROS);
Display display(DISPLAY_I2C_ADDRESS, DISPLAY_I2C_SDA_GPIO, DISPLAY_I2C_SCK_GPIO);
MenuItem* menu = init_menu(debug_mode, count_mode);

void debug_mode()
{
    time1.start();
}

void count_mode(){
  time2.start();
}

void render_shoots()
{
    int shoots = calculate_shoots();
    display.PrintString(String("Shoots:"), true, false, ArialMT_Plain_24);
    display.PrintString(String(shoots), false, true, ArialMT_Plain_24, 100);
}

void render_sensdata()
{

    int piezo_max = 0;
    int acc_max = 0;
    sensors_data_sample_t measurement = get_data_from_sensors();

    display.PrintString(String(measurement.acc), true, false, ArialMT_Plain_24);
    display.PrintString(String(measurement.piezo), false, false, ArialMT_Plain_24, 32);

    if (measurement.piezo > piezo_max){
      piezo_max = measurement.piezo;
    }
    display.PrintString(String(piezo_max), false, true, ArialMT_Plain_10, 32, 5);
    if (measurement.acc > acc_max){
      acc_max = measurement.acc;
    }
    display.PrintString(String(acc_max), false, true, ArialMT_Plain_10, 0, 5);
}

void changeMode(void)
{
  menu = getNext(menu);
  display.PrintString(menu->title, true, true, ArialMT_Plain_24);
  button.attachClick(menu->trigger_on_btn);
}

void setup(void)
{
  pinMode(2,OUTPUT);
  button_2.attachClick(changeMode);

  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  if (!accelerometer_init()) {
    while(1);
  }

  // bool  display_status = display.init();
  if (display.init() == false)
  {
    Serial.println("ERROR: Display not detected");
    while(1);
  }
  // display.flipScreenVertically();
  Serial.println("INFO: Configured. Ready to work");
  display.PrintString("SC ver. 0.3. Wait..", true, true, ArialMT_Plain_10);
  sleep(3);
  display.PrintString(menu->title, true, true, ArialMT_Plain_24);
}


void loop() {
  time1.update();
  time2.update();
  button.tick();
  button_2.tick();
}
