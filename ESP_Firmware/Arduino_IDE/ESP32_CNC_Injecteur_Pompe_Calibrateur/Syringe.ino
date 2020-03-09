void SyrHome(String motor) {
  //Number of step after home button activaton (to prevent infinite interrupt)
  int backpush = 1000;
  EndStop = true;
  String debug = "Syringe " + motor + " calibrated";
  //Homing until home button is pressed
  while (EndStop == true) {
    OrangeLED(0);
    if (motor == "X") {
      StepperX.enable();
      StepperX.move(-200);
    }
    else if (motor == "Y") {
      StepperY.enable();
      StepperY.move(-200);
    }
    else if (motor == "Z") {
      StepperZ.enable();
      StepperZ.move(-200);
    }
  }

  BlueLED(0);

  //Slot X selected
  if (motor == "X") {
    StepperX.enable();
    StepperX.move(backpush);
    ModulesInfos["XC"] = 0;
    SaveConfigModule();
  }
  //Slot Y selected
  else if (motor == "Y") {
    StepperY.enable();
    StepperY.move(backpush);
    ModulesInfos["YC"] = 0;
    SaveConfigModule();
  }
  //Slot Z selected
  else if (motor == "Z") {
    StepperZ.enable();
    StepperZ.move(backpush);
    ModulesInfos["ZC"] = 0;
    SaveConfigModule();
  }

  GreenLED(0);

  Serial.println(debug);
  client.publish(TopicDEBUG, debug.c_str());
}

void SyrInject(String motor, float volume) {
  float calval = ModulesInfos["SC"];
  float pistpos = -1;
  float pistmax = ModulesInfos["SM"];
  //Slot X selected
  if (motor == "X") {
    pistpos = ModulesInfos["XC"];
    //Check if the sysringe has been homed
    if (pistpos < 0) {
      Serial.println("ERROR ! Syringe not homed");
      client.publish(TopicDEBUG, "ERROR ! Syringe not homed");
      RedLED(0);
      return;
    }
    else {
      //Check current and future piston position
      if (pistpos + ((volume / calval) * 200) <= pistmax) {
        BlueLED(0);
        StepperX.enable();
        StepperX.move((volume / calval) * 200);
        ModulesInfos["XC"] = pistpos + ((volume / calval) * 200);
        SaveConfigModule();
      }
      else {
        Serial.println("ERROR ! Maximum volume exceeded");
        client.publish(TopicDEBUG, "ERROR ! Maximum volume exceeded");
        RedLED(0);
        return;
      }
    }
  }
  else if (motor == "Y") {
    //Slot Y selected
    pistpos = ModulesInfos["YC"];
    //Check if the syringe has  been homed
    if (pistpos < 0) {
      Serial.println("ERROR ! Syringe not homed");
      client.publish(TopicDEBUG, "ERROR ! Syringe not homed");
      RedLED(0);
      return;
    }
    else {
      //Check the current and future piston position
      if (pistpos + ((volume / calval) * 200) <= pistmax) {
        BlueLED(0);
        StepperY.enable();
        StepperY.move((volume / calval) * 200);
        ModulesInfos["YC"] = pistpos + ((volume / calval) * 200);
        SaveConfigModule();
      }
      else {
        Serial.println("ERROR ! Maximum volume exceeded");
        client.publish(TopicDEBUG, "ERROR ! Maximum volume exceeded");
        RedLED(0);
        return;
      }
    }
  }
  else if (motor == "Z") {
    //Slot Z selected
    pistpos = ModulesInfos["ZC"];
    if (pistpos < 0) {
      //Check if the syringe has been homed
      Serial.println("ERROR ! Syringe not homed");
      client.publish(TopicDEBUG, "ERROR ! Syringe not homed");
      RedLED(0);
      return;
    }
    else {
      //Check the current and future piston position then proceed to injection
      if (pistpos + ((volume / calval) * 200) <= pistmax) {
        BlueLED(0);
        StepperZ.enable();
        StepperZ.move((volume / calval) * 200);
        ModulesInfos["ZC"] = pistpos + ((volume / calval) * 200);
        SaveConfigModule();
      }
      else {
        Serial.println("ERROR ! Maximum volume exceeded");
        client.publish(TopicDEBUG, "ERROR ! Maximum volume exceeded");
        RedLED(0);
        return;
      }
    }
  }

  Serial.println("Done");
  client.publish(TopicDEBUG, "Done");

}
