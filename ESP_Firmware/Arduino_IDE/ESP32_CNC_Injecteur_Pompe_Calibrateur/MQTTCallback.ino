
//MQTT CALLBACK=====================================================
void callback(String topic, byte* message, unsigned int length) {
  //(byte*) message to (String)message_temp
  String message_temp;
  for (int i = 0; i < length; i++) {
    message_temp += (char)message[i];
  }
  //Topic = TopicCMD--------------------------------------------------
  if (topic == TopicCMD) {
    //Index0 = X, Y or Z
    String index0 = message_temp.substring(0, 1);
    //Index1 = C/H or (float)CMD
    String index1 = message_temp.substring(1, 2);
    String index_from_1 = message_temp.substring(1);
    String index_from_2 = message_temp.substring(2);
    String debugmqtt;

    //Slot X selected
    if (index0 == "X") {
      //Tool = Syringe
      if (ModulesInfos["X"] == "S") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          Serial.println("Syringe X Calibration");
          client.publish(TopicDEBUG, "Syringe X Calibration");
          SyrHome(index0);
        }
        else {
          //CMD mode
          debugmqtt = "Syringe X CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          SyrInject(index0, atof(index_from_1.c_str()));
          //(atof(index2.c_str()));
        }

      }
      //Tool = pump
      else if (ModulesInfos["X"] == "P") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          //Calibration mode
          Serial.println("Pump X Calibration");
          client.publish(TopicDEBUG, "Pump X Calibration");
          PumpCalib(index0, index_from_2);
        }
        else {
          //CMD mode
          debugmqtt = "Pump X CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          PumpInject(index0, atof(index_from_1.c_str()));
        }
      }
      else {
        Serial.println("No module attached");
        client.publish(TopicDEBUG, "No module attached");
        RedLED(0);
        return;
      }
    }

    //Slot Y selected
    else if (index0 == "Y") {
      //Tool = Syringe
      if (ModulesInfos["Y"] == "S") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          Serial.println("Syringe Y Calibration");
          client.publish(TopicDEBUG, "Syringe Y Calibration");
          SyrHome(index0);
        }
        //CMD mode
        else {
          debugmqtt = "Syringe Y CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          SyrInject(index0, atof(index_from_1.c_str()));
          //(atof(index2.c_str()));
        }
      }
      //Tool = Pump
      else if (ModulesInfos["Y"] == "P") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          Serial.println("Pump Y Calibration");
          client.publish(TopicDEBUG, "Pump Y Calibration");
          PumpCalib(index0, index_from_2);
        }
        //CMD mode
        else {
          debugmqtt = "Pump Y CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          PumpInject(index0, atof(index_from_1.c_str()));
        }
      }
      else {
        Serial.println("No module attached");
        client.publish(TopicDEBUG, "No module attached");
        RedLED(0);
        return;
      }
    }

    //Slot Z selected
    else if (index0 == "Z") {
      //Tool = Syringe
      if (ModulesInfos["Z"] == "S") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          Serial.println("Syringe Z Calibration");
          client.publish(TopicDEBUG, "Syringe Z Calibration");
          SyrHome(index0);
        }
        //CMD mode
        else {
          debugmqtt = "Syringe Z CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          SyrInject(index0, atof(index_from_1.c_str()));
          //(atof(index2.c_str()));
        }
      }
      //Tool = Pump
      else if (ModulesInfos["Z"] == "P") {
        //Calibration mode
        if (index1 == "C" || index1 == "H") {
          Serial.println("Pump Z Calibration");
          client.publish(TopicDEBUG, "Pump Z Calibration");
          PumpCalib(index0, index_from_2);
        }
        //CMD mode
        else {
          debugmqtt = "Pump Z CMD : " + index_from_1;
          Serial.println(debugmqtt);
          client.publish(TopicDEBUG, debugmqtt.c_str());
          PumpInject(index0, atof(index_from_1.c_str()));
        }
      }
      else {
        Serial.println("No module attached");
        client.publish(TopicDEBUG, "No module attached");
        RedLED(0);
        return;
      }
    }
  }
  //Topic = TopicCMD--------------------------------------------------

  //Topic = TopicCONFIG-----------------------------------------------
  if (topic == TopicCONFIG) {
    ConfigModules(message_temp);
  }
}
//Topic = TopicCONFIG-----------------------------------------------

//MQTT CALLBACK=====================================================
