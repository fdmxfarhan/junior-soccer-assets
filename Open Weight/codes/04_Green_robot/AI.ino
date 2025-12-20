String cmd = "";
void update_all() {
  digitalWrite(PC15, digitalRead(PA12));
  robot_angle = gyro.read();
  boost_voltage();
  print_all();

  if (Serial1.available() > 0) {
    rx_data = Serial1.read();
    if (rx_data == 'F') {
      vl1 = -speed;
      vl2 = -speed;
      vr2 = speed;
      vr1 = speed;
    }
    if (rx_data == 'G') {
      vl1 = speed;
      vl2 = speed;
      vr2 = -speed;
      vr1 = -speed;
    }
    if (rx_data == 'L') {
      vl1 = speed;
      vl2 = -speed;
      vr2 = -speed;
      vr1 = speed;
    }
    if (rx_data == 'Q') {
      vl1 = 0;
      vl2 = -speed;
      vr2 = 0;
      vr1 = speed;
    }
    if (rx_data == 'Z') {
      vl1 = speed;
      vl2 = 0;
      vr2 = -speed;
      vr1 = 0;
    }
    if (rx_data == 'R') {
      vl1 = -speed;
      vl2 = speed;
      vr2 = speed;
      vr1 = -speed;
    }
    if (rx_data == 'E') {
      vl1 = -speed;
      vl2 = 0;
      vr2 = speed;
      vr1 = 0;
    }
    if (rx_data == 'C') {
      vl1 = 0;
      vl2 = speed;
      vr2 = 0;
      vr1 = -speed;
    }
    if (rx_data == 'X') {
      digitalWrite(PC15, 1);
      delay(30);
      digitalWrite(PC15, 0);
    }
    if (rx_data == 'S') {
      vl1 = 0;
      vl2 = 0;
      vr2 = 0;
      vr1 = 0;
    }
    if (rx_data == '\n') {

      // SPEED COMMAND: J0 to J255
      if (cmd.charAt(0) == 'J') {
        int val = cmd.substring(1).toInt();  // 0–255

        val = constrain(val, 0, 255);
        speed = map(val, 0, 255, 0, 65535);
      }

      // CLEAR BUFFER
      cmd = "";
    } else {
      cmd += rx_data;
    }
  }
}