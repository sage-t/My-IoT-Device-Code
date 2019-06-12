/**
 * Relays MQTT messages to LED Board via pwm signal
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "FBI Van #825"
#define wifi_password "************"

#define mqtt_server "10.0.0.189"
#define mqtt_user "homeassistant"
#define mqtt_password "************"

/* demo sensor using button on pin 0 */
#define status_topic "board/status"

#define RELAY_PIN 4

/* MQTT Client */
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    reconnect();
  }
  client.subscribe(status_topic); // use http://10.0.0.188:8123/dev-mqtt for testing

  /* relay pin */
  pinMode(RELAY_PIN, OUTPUT);
}

long lastMsg = 0;
long now;
int prev_status = 5;
int curr_status = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  now = millis();
  if (now - lastMsg > 200) {
    lastMsg = now;

    if (curr_status != prev_status) {
      prev_status = curr_status;

      Serial.print("Updating relay signal: ");
      Serial.println(curr_status);
      analogWrite(RELAY_PIN, curr_status);
    } else {
      curr_status = 5;
    }
  }
}

/** Connect to WiFi */
void setup_wifi() {
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

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
    
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  String inString = "";    // string to hold input
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    
    if (isDigit(payload[i])) {
      inString += (char)payload[i];
    }
  }
  curr_status = inString.toInt();
  if (curr_status > 255) {
    curr_status = 255;
  } else if (curr_status < 0) {
    curr_status = 0;
  }
  Serial.println();
  Serial.print("Value:");
  Serial.print(curr_status);
 
  Serial.println();
  Serial.println("-----------------------");
}

