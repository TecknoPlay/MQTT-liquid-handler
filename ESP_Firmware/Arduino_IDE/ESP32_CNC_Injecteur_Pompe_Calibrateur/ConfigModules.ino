void ConfigModules(String payload) {
  String  config_string;
  //Serialize the current config
  serializeJson(ModulesInfos, config_string);

  //Check the current config----------------------------------------------------------
  if (payload == "INFO") {
    //config_string = serialized ModulesInfos file
    Serial.println("Current Config");
    client.publish(TopicINFO, config_string.c_str());
    //client.publish(TopicDEBUG, config_string.c_str());
    Serial.println(config_string);
    WhiteLED(0);
  }
  //Check the current config----------------------------------------------------------

  //Reset config----------------------------------------------------------------------
  else if (payload == "RESET") {

    ModulesInfos["X"]="N";
    ModulesInfos["XC"]=-1;
    ModulesInfos["Y"]="N";
    ModulesInfos["YC"]=-1;
    ModulesInfos["Z"]="N";
    ModulesInfos["ZC"]=-1;
    //Get and store the current timstamp
    ModulesInfos["Last"] = (int)GetTimestamp();
    //Save the new config to config.json
    SaveConfigModule();
    client.publish(TopicDEBUG, "Config reset");
    Serial.println("Config reset");
    WhiteLED(0);
  }
  //Reset the config------------------------------------------------------------------

  //Modify the config-----------------------------------------------------------------
  else {//Edit the config
    String tool = String(payload[1]);
    //payload[0] = Slot ( X Y or Z )/payload[1] = Tool (S or P)
    if (payload.substring(0, 1) == "X") {
      if (tool == "P" || tool == "S" || tool == "N") {
        ModulesInfos["X"] = tool;
        ModulesInfos["XC"] = -1;
        String output = tool + " attached to slot X";
        Serial.println(output);
        client.publish(TopicDEBUG, output.c_str());
        WhiteLED(0);
      }
      else {
        Serial.println("ERROR ! Tool identifier not recognized");
        client.publish(TopicDEBUG, "ERROR ! Tool identifier not recognized");
        RedLED(0);
        return;
      }

    }
    else if (payload.substring(0, 1) == "Y") {
      if (tool == "P" || tool == "S" || tool == "N") {
        ModulesInfos["Y"] = tool;
        ModulesInfos["YC"] = -1;
        String output = tool + " attached to slot Y";
        Serial.println(output);
        client.publish(TopicDEBUG, output.c_str());
        WhiteLED(0);
      }
      else {
        Serial.println("ERROR ! Tool identifier not recognized");
        client.publish(TopicDEBUG, "ERROR ! Tool identifier not recognized");
        RedLED(0);
        return;
      }

    }
    else if (payload.substring(0, 1) == "Z") {
      if (tool == "P" || tool == "S" || tool == "N") {
        ModulesInfos["Z"] = tool;
        ModulesInfos["ZC"] = -1;
        String output = tool + " attached to slot Z";
        Serial.println(output);
        client.publish(TopicDEBUG, output.c_str());
        WhiteLED(0);
      }
      else {
        Serial.println("ERROR ! Tool identifier not recognized");
        client.publish(TopicDEBUG, "ERROR ! Tool identifier not recognized");
        RedLED(0);
        return;
      }
    }
    else {
      Serial.println("Error. Check if the command start by X Y or Z");
      client.publish(TopicDEBUG, "Error. Check if the command start by X Y or Z");
      RedLED(0);
      return;
    }
    //Get and store the current timstamp
    ModulesInfos["Last"] = (int)GetTimestamp();
    //Save the new config to config.json
    SaveConfigModule();
  }
  //Modify the config-----------------------------------------------------------------
}

void GetConfigModule() {
  //Load the config.JSOn file to an ArduinoJson file
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    RedLED(0);
    return;
  }
  File config_file = SPIFFS.open("/config.json");
  if (!config_file) {
    Serial.println("Failed to open file for reading");
    RedLED(0);
    return;
  }
  String  config_string = config_file.readString();
  deserializeJson(ModulesInfos, config_string);
  config_file.close();
  Serial.println("Config JSON loaded");
  WhiteLED(0);
}

void SaveConfigModule() {
  String config_string;
  //Open the config.json file (Write Mode)
  File config_file = SPIFFS.open("/config.json", "w");
  if (!config_file) {
    Serial.println("Failed to open file (Writing mode)");
    RedLED(0);
    return;
  }

  //Serialize the ModulesInfos file to a JSON string
  serializeJson(ModulesInfos, config_string);

  //Save and close the JSON file
  if (config_file.println(config_string)) {
    Serial.println("New Config");
    Serial.println(config_string);
    //client.publish(TopicDEBUG, config_string.c_str());
  } else {
    Serial.println("File write failed");
    client.publish(TopicDEBUG, "File write failed");
    RedLED(0);
  }
  config_file.close();
}
