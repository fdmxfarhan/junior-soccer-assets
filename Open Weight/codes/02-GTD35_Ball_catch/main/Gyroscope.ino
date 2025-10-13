void readGyro(){
  i2c.beginTransmission(0x10);
  i2c.write(4);
  i2c.endTransmission();
  i2c.requestFrom(0x10, 18);
  uint8_t data[18];
  for(int i=0; i<18; i++) data[i] = i2c.read();
  int16_t z = (int16_t)data[0] << 8 | data[1];
  robot_angle = z / 100.0;
}