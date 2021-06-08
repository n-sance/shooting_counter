#include "main.h"

Adafruit_ADXL345_Unified accel(ACC_ID);

bool accelerometer_init() {
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("ERROR: No ADXL345 detected");
    return false;
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
  return true;
}
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

void displayDataRate(Adafruit_ADXL345_Unified& accel)
{
  Serial.print  ("Data Rate:    ");

  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(Adafruit_ADXL345_Unified& accel)
{
  Serial.print  ("Range:         +/- ");

  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}

void displaySensorDetails(Adafruit_ADXL345_Unified& accel)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

sensors_data_sample_t get_data_from_sensors()
{
    sensors_event_t event;
    sensors_data_sample_t measurement;
    //static int vibro_max = 0;
    //static int acc_max = 0;  #maybe static needed
    accel.getEvent(&event);
    measurement.piezo = analogRead(PIEZO_ADC_GPIN);
    measurement.acc = get_summ_values_from_acc(event.acceleration.x, event.acceleration.y, event.acceleration.z);

    Serial.print(measurement.piezo); Serial.print(",");
    Serial.println(measurement.acc);

    return measurement;
}
