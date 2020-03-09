void PumpCalib(String motor, String calvol) {
  digitalWrite(HOLD, HIGH);
  bool ldr_mode = false;
  int caldiv = 2;
  int tresh = 500;
  int calvol_int = calvol.toInt();
  int calstep = 0;
  int ldr_calib = 0;
  float calval = 0;
  EndStop = true;
  String debug = "Pump" + motor + "calibrated";

  //Check if Calvol is > 10ml
  if (calvol_int < 10) {
    Serial.println("ERROR ! Calibration volume is too small");
    client.publish(TopicDEBUG, "ERROR ! Calibration volume is too small");
    digitalWrite(HOLD, LOW);
    RedLED(0);
    return;
  }

  //Proceed to LDR calibration if ldr_mode == true
  if (ldr_mode == true) {
    //LDR calibration
    Serial.println("LDR calibration");
    for (int i = 0; i < 5; i++) {
      ldr_calib += analogRead(RESUME);
      delay(250);
      Serial.println(ldr_calib);
    }
    ldr_calib = ldr_calib / 5;
    Serial.println("LDR calibrated. Performing pump calibration");
    client.publish(TopicDEBUG, "LDR calibrated. Performing pump calibration");
  }

  while (EndStop == true) {
    OrangeLED(0);
    if (motor == "X") {
      StepperX.enable();
      StepperX.move(200 / caldiv);
    }
    else if (motor == "Y") {
      StepperY.enable();
      StepperY.move(200 / caldiv);
    }
    else if (motor == "Z") {
      StepperZ.enable();
      StepperZ.move(200 / caldiv);
    }
    if (ldr_mode == true) {
      if (analogRead(RESUME) < ldr_calib - tresh) {
        EndStop = false;
      }
    }
    else {
      if (digitalRead(SPNEN) == LOW) {
        EndStop = false;
      }
    }
    calstep += 1;
  }

  GreenLED(0);

  //Calval (in ml/turn) = calvol/(calstep/caldiv)
  calval = float(calvol_int) / (float(calstep) / float(caldiv));
  String output = "Calibration performed. Val = " + String(calval) + "ml/turn";
  Serial.println(output);
  client.publish(TopicDEBUG, output.c_str());
  digitalWrite(HOLD, LOW);

  //Save calval in the memory
  if (motor == "X") {
    ModulesInfos["XC"] = float(calval);
    SaveConfigModule();
  }
  else if (motor == "Y") {
    ModulesInfos["YC"] = float(calval);
    SaveConfigModule();
  }
  else if (motor == "Z") {
    ModulesInfos["ZC"] = float(calval);
    SaveConfigModule();
  }
}

void PumpInject(String motor, float volume) {
  float calval = 0;

  if (motor == "X") {
    calval = ModulesInfos["XC"];
    if (calval < 0) {
      Serial.println("ERROR ! Pump not calibrated");
      client.publish(TopicDEBUG, "ERROR ! Pump not calibrated");
      RedLED(0);
      return;
    }
    else {
      BlueLED(0);
      StepperX.enable();
      StepperX.move((volume / calval) * 200);
    }
  }
  else if (motor == "Y") {
    calval = ModulesInfos["YC"];
    if (calval < 0) {
      Serial.println("ERROR ! Pump not calibrated");
      client.publish(TopicDEBUG, "ERROR ! Pump not calibrated");
      RedLED(0);
      return;
    }
    else {
      BlueLED(0);
      StepperY.enable();
      StepperY.move((volume / calval) * 200);
    }
  }
  else if (motor == "Z") {
    calval = ModulesInfos["ZC"];
    if (calval < 0) {
      Serial.println("ERROR ! Pump not calibrated");
      client.publish(TopicDEBUG, "ERROR ! Pump not calibrated");
      RedLED(0);
      return;
    }
    else {
      BlueLED(0);
      StepperZ.enable();
      StepperZ.move((volume / calval) * 200);
    }
  }
  Serial.println("Done");
  client.publish(TopicDEBUG, "Done");

}
