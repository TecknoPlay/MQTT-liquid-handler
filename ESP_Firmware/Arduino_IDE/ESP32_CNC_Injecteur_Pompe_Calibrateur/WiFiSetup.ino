void GetWiFiJSON() {
  File wifi_file = SPIFFS.open("/wifi.json");
  if (!wifi_file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String  wifi_string = wifi_file.readString();
  deserializeJson(WiFiJSON, wifi_string);
  wifi_file.close();

  ssid = (const char*)WiFiJSON["SSID"];
  password = (const char*)WiFiJSON["PASS"];
  mqtt_server = (const char*)WiFiJSON["SERVER"];
  TopicINFO = (const char*)WiFiJSON["TopicINFO"];
  TopicDEBUG = (const char*)WiFiJSON["TopicDEBUG"];
  TopicCONFIG = (const char*)WiFiJSON["TopicCONFIG"];
  TopicCMD = (const char*)WiFiJSON["TopicCMD"];
  HostName = (const char*)WiFiJSON["HostName"];
  Serial.println("WiFi JSON loaded");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HostName);
  WiFi.begin(ssid, password);
  leds[1] = CRGB( 10, 10, 0);
  FastLED.show();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName)) {
      Serial.println("connected");
      client.subscribe(TopicCMD);
      client.subscribe(TopicCONFIG);
      leds[1] = CRGB( 0, 10, 0);
      FastLED.show();
      //digitalWrite(LED_BUILTIN, LOW);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      //digitalWrite(LED_BUILTIN, HIGH);
      leds[1] = CRGB( 10, 0, 0);
      FastLED.show();
      delay(5000);
    }
  }
}

int GetTimestamp()
{
  struct tm timeinfo;
  time_t now;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    client.publish(TopicDEBUG, "Failed to obtain time");
    leds[0] = CRGB( 10, 0, 0);
    FastLED.show();
    return 0;
  }
  return time(&now);
}
