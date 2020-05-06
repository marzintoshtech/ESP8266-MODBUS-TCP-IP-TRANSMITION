void ReadTemp() {
  float temp_0;
  float temp_1;
  float temp_2;
  float temp_3;
  float temp_4;
  DS18B20.requestTemperatures();
  temp_0 = DS18B20.getTempCByIndex(0); // Sensor 0 will capture Temp in Celcius
  temp_1 = DS18B20.getTempCByIndex(1); // Sensor 1 will capture Temp in Celcius
  temp_2 = DS18B20.getTempCByIndex(2); // Sensor 2 will capture Temp in Celcius
  temp_3 = DS18B20.getTempCByIndex(3); // Sensor 3 will capture Temp in Celcius
  temp_4 = DS18B20.getTempCByIndex(4); // Sensor 4 will capture Temp in Celcius
  int moltiplicator = 10;
  int moltiplicator1 = 100;
  TempBuffer1 = temp_0 * moltiplicator;
  TempBuffer2 = temp_1 * moltiplicator;
  TempBuffer3 = temp_2 * moltiplicator;
  TempBuffer4 = temp_3 * moltiplicator;
  TempBuffer5 = temp_4 * moltiplicator;
  TempBuffer6 = temp_0 * moltiplicator1;
  TempBuffer7 = temp_1 * moltiplicator1;


}
