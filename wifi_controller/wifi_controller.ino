/**
 * Template for connecting ESP8622 to WiFi and publishing a demo topic to a MQTT server
 * 
 * Optionally print to Serial and/or OLED for Wifi Kit 8
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#define WIFI_KIT /* comment if not using Wifi Kit 8 */

#define wifi_ssid "FBI Van #825"
#define wifi_password "************"

#define mqtt_server "10.0.0.188"
#define mqtt_user "homeassistant"
#define mqtt_password "************"

/* pause sensor using button on pin 0 */
#define pause_topic "controller/pause"
const int pauseButtonPin = 5;
int pauseButtonState = 0;
int prevPauseButtonState;

/* next sensor using button on pin 0 */
#define next_topic "controller/next"
const int nextButtonPin = 4;
int nextButtonState = 0;
int prevNextButtonState;

/* screen setup */
#ifdef WIFI_KIT
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4); 
#endif

/* MQTT Client */
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  #ifdef WIFI_KIT
  u8g2.begin();
  #endif
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  pinMode(pauseButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  
  prevPauseButtonState = digitalRead(pauseButtonPin);
  prevNextButtonState = digitalRead(nextButtonPin);
}

long lastMsg = 0;
long now;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  now = millis();
  if (now - lastMsg > 300) {
    lastMsg = now;
    
    /* read sensor state */
    pauseButtonState = digitalRead(pauseButtonPin);
    nextButtonState = digitalRead(nextButtonPin);

    if (pauseButtonState != prevPauseButtonState) {
      client.publish(pause_topic, String(pauseButtonState).c_str(), true);
      
      prevPauseButtonState = pauseButtonState;
    }

    if (nextButtonState != prevNextButtonState) {
      client.publish(next_topic, String(nextButtonState).c_str(), true);
      
      prevNextButtonState = nextButtonState;
    }
  }
}

/** Connect to WiFi */
void setup_wifi() {
  delay(10);

  #ifdef WIFI_KIT
  u8g2.clearBuffer();
  u8g2_prepare();
  u8g2.setCursor(0, 0);
  u8g2.print("Connecting to ");
  u8g2.setCursor(0, 10);
  u8g2.print(wifi_ssid);
  u8g2.setCursor(0, 20);
  u8g2.sendBuffer();
  delay(500);
  #endif
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    #ifdef WIFI_KIT
    u8g2.print(".");
    u8g2.sendBuffer();
    #endif
    delay(500);
  }

  #ifdef WIFI_KIT
  u8g2.clearBuffer();
  u8g2_prepare();
  u8g2.setCursor(0, 0);
  u8g2.print("WiFi connected");
  u8g2.setCursor(0, 10);
  u8g2.print("IP: ");
  u8g2.print(WiFi.localIP());
  u8g2.sendBuffer();
  delay(500);
  #endif

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/** Connect to WQTT Broker */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    #ifdef WIFI_KIT
    u8g2.setCursor(0, 20);
    u8g2.print("MQTT reconnect");
    u8g2.sendBuffer();
    delay(500);
    #endif
    
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      
      #ifdef WIFI_KIT
      u8g2.setCursor(0, 20);
      u8g2.print("MQTT connected           ");
      u8g2.sendBuffer();
      delay(500);
      #endif
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      #ifdef WIFI_KIT
      u8g2.print(".");
      u8g2.sendBuffer();
      #endif
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

#ifdef WIFI_KIT
void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
#endif
