//REM : Couper le connecteur de la PIN12 et Jump PIN12 et 13 sur le Shield
#include <Wire.h>
#include <Arduino.h>
#include <ArduinoJson.h> //ArduinoJSON6
#include "SPIFFS.h"
#include <FastLED.h>
//#include "FS.h"
//#include "FFat.h"
#include "time.h"
#include "DRV8825.h" //V1.1.3
long unsigned int PrevMil = 0;
long unsigned int PrevMil2 = 0;
int StepOffDelay = 5000;
int LEDOffDelay = 1000;

DynamicJsonDocument ModulesInfos(2048);
DynamicJsonDocument WiFiJSON(2048);

#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

String SMot = "N";
String Cmd = "0";
volatile bool EndStop = false;

#define NUM_LEDS 3
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

#define ZPLUS 23 //(VSPI MOSI)
#define YPLUS 5 //(VSPI CS0)
#define SPNEN 19 //(VSPI MISO)
#define SPNDIR 18 //(VSPI CLK)
#define COOEN 34 //(InputOnly/RTC_GPIO4/ADC1_CH6)
#define ABORT 2 //(ADC2_CH1/Touch2/RTC_GPIO12)
#define HOLD 4 // (ADC2_CH0/Touch0/RTC_GPIO10)
#define RESUME 35 //(InputOnly/RTC_GPIO5/ADC1_CH7)
#define SDA 39 //(InputOnly/Sensor_VN/RTC_GPIO3/ADC1_CH3)
#define SCL 35 //(InputOnly/RTC_GPIO5/ADC1_CH7)
#define RST 0 //(ADC2_CH1/Touch1/RTC_GPIO11)
//#define LED_BUILTIN 2

//WiFi, MQTT settings
const char *ssid, *password, *mqtt_server, *TopicINFO, *TopicDEBUG, *TopicCONFIG, *TopicCMD, *HostName;
const char* ntpServer = "pool.ntp.org";

#define RPM 200
//MOTOR_STEPS, DIR, STEP, ENABLE, MS1, MS2, MS3
DRV8825 StepperX(200, 16, 26, 13, 0, 0, 0);
DRV8825 StepperY(200, 27, 25, 13, 0, 0, 0);
DRV8825 StepperZ(200, 14, 17, 13, 0, 0, 0);

//Syringe EndStop interrupt (button connected to ZPLus)
void IRAM_ATTR HomeEnd() {
  StepperX.disable();
  EndStop = false;
  StepperX.enable();
  leds[2] = CRGB( 0, 0, 50);
  PrevMil2 = millis() + LEDOffDelay;
}
//SETUP===========================================================
void setup() {
  Serial.begin(9600);
  Serial.println();

  GetConfigModule();
  GetWiFiJSON();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  configTime(3600, 3600, ntpServer);

  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ZPLUS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ZPLUS), HomeEnd, FALLING);
  pinMode(HOLD, OUTPUT);
  pinMode(RESUME, INPUT);
  pinMode(SPNEN, INPUT_PULLUP);

  StepperX.begin(RPM);
  StepperX.enable();
  StepperY.begin(RPM);
  StepperY.enable();
  StepperZ.begin(RPM);
  StepperZ.enable();

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
}
//SETUP===========================================================

//LOOP============================================================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  yield();

  if (millis() - PrevMil > StepOffDelay) {
    PrevMil = millis();
    StepperX.disable();
    StepperY.disable();
    StepperZ.disable();
  }

  if (millis() > PrevMil2) {
    //PrevMil2 = millis();
    leds[0] = CRGB( 0, 0, 0);
    leds[2] = CRGB( 0, 0, 0);
    FastLED.show();
  }
  FastLED.show();

}
//LOOP============================================================
