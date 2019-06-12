/**
 * Template for connecting ESP8622 to WiFi and publishing a demo topic to a MQTT server
 * 
 * Optionally print to Serial and/or OLED for Wifi Kit 8
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "FBI Van #825"
#define wifi_password "************"

#define mqtt_server "10.0.0.188"
#define mqtt_user "homeassistant"
#define mqtt_password "************"

/* demo sensor using button on pin 0 */
#define pause_topic "controller/pause"
#define next_topic "controller/next"
#define prev_topic "controller/prev"
#define shuffle_topic "controller/shuffle"
#define volume_up_topic "controller/volume_up"
#define volume_down_topic "controller/volume_down"
const int button1Pin = 12;
const int button2Pin = 15;
const int button3Pin = 13;

int button1State = 0;
int button2State = 0;
int button3State = 0;

int prevButton1State;
int prevButton2State;
int prevButton3State;

/* screen setup */
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

/* MQTT Client */
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  u8g2.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  /* buttons */
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);

  prevButton1State = digitalRead(button1Pin);
  prevButton2State = digitalRead(button2Pin);
  prevButton3State = digitalRead(button3Pin);
}

long lastMsg = 0;
long now;
int menu_selection = 1;
int page_selection = 1;
int curr_button = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;

    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
    button3State = digitalRead(button3Pin);
    
    /* read sensor states */
    if (button1State != prevButton1State) {
      prevButton1State = button1State;

      if (button1State) {
        curr_button = 1;
      }
    } else if (button2State != prevButton2State) {
      prevButton2State = button2State;

      if (button2State) {
        curr_button = 2;
        menu_selection -= 1;
      }
    } else if (button3State != prevButton3State) {
      prevButton3State = button3State;

      if (button3State) {
        curr_button = 3;
        menu_selection += 1;
      }
    } else {
      curr_button = 0;
    }

    /* limit menu selection number */
    if (menu_selection < 1) {
      menu_selection = 1;
    } else if (menu_selection > 3) {
      menu_selection = 3;
    }

    /* display current page */
    if (page_selection == 1) {
      main_menu(menu_selection, (curr_button == 1));
    } else if (page_selection == 2) {
      music_menu(curr_button);
    } else if (page_selection == 3) {
      music_menu_2(curr_button);
    }
    
  }
}

/** Connect to WiFi */
void setup_wifi() {
  delay(10);
  int wifi_x = 120;
  int wifi_y = 15;

//  u8g2.setFont(u8g2_font_unifont_t_symbols);
//  u8g2.drawGlyph(wifi_x, wifi_y, 0x25cb);
//  u8g2.sendBuffer();
  delay(500);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  int conn_count = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    if (conn_count == 7) {
      conn_count = 0;
    } else {
      conn_count += 1;
    }
    
//    if (conn_count == 0) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25cb);
//    } else if (conn_count == 1) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d4);
//    } else if (conn_count == 2) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d1);
//    } else if (conn_count == 3) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d5);
//    } else if (conn_count == 4) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25cf);
//    } else if (conn_count == 5) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d5);
//    } else if (conn_count == 6) {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d1);
//    } else {
//      u8g2.drawGlyph(wifi_x, wifi_y, 0x25d4);
//    }
//    u8g2.sendBuffer();

    delay(100);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  u8g2.drawGlyph(wifi_x, wifi_y, 0x25cf);
  u8g2.sendBuffer();
}

/** Connect to WQTT Broker */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(120, 30, 0x25cb);
    u8g2.sendBuffer();

    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");

      u8g2.drawGlyph(120, 30, 0x25cf);
      u8g2.sendBuffer();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void main_menu(int selection, bool select) {
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);

  if (selection == 1) {
    u8g2.setDrawColor(0);
    u8g2.drawGlyph(0, 32, 0x00e1);
    u8g2.setDrawColor(1);
    u8g2.drawFrame(0,0,32,32);
  } else {
    u8g2.setDrawColor(1);
    u8g2.drawGlyph(0, 32, 0x00e1);
  }

  if (selection == 2) {
    u8g2.setDrawColor(0);
    u8g2.drawGlyph(32 + 8, 32, 0x0103);
    u8g2.setDrawColor(1);
    u8g2.drawFrame(32 + 8,0,32,32);
  } else {
    u8g2.setDrawColor(1);
    u8g2.drawGlyph(32 + 8, 32, 0x0103);
  }

  if (selection == 3) {
    u8g2.setDrawColor(0);
    u8g2.drawGlyph(64 + 16, 32, 0x00de);
    u8g2.setDrawColor(1);
    u8g2.drawFrame(64 + 16,0,32,32);
  } else {
    u8g2.setDrawColor(1);
    u8g2.drawGlyph(64 + 16, 32, 0x00de);
  }

  if (select) {
    if (selection == 1) {
      page_selection = 2;
    } else if (selection == 2) {
      page_selection = 1; // TODO
    } else if (selection == 3) {
      page_selection = 1; // TODO
    }
  }
  
  u8g2.sendBuffer();
}

void music_menu(int button) {
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);

  u8g2.setDrawColor(0);
  u8g2.drawBox(0,0,112,32);

  u8g2.setDrawColor(1);
  u8g2.drawGlyph(0, 32, 0x00d3);
  u8g2.drawGlyph(32 + 8, 32, 0x00d7);
  u8g2.drawGlyph(64 + 16, 32, 0x00d8);
  u8g2.sendBuffer();

  if (button == 1) {
    long start = millis();
    while (digitalRead(button1Pin)) {
      if (millis() - start > 1000) {
        page_selection = 1;
        menu_selection = 1;
        return;
      }
    }

    client.publish(pause_topic, "1", true);
    client.publish(pause_topic, "0", true);
  } else if (button == 2) {
    client.publish(prev_topic, "1", true);
    client.publish(prev_topic, "0", true);
  } else if (button == 3) {
    long start = millis();
    while (digitalRead(button3Pin)) {
      if (millis() - start > 1000) {
        page_selection = 3;
        return;
      }
    }
    
    client.publish(next_topic, "1", true);
    client.publish(next_topic, "0", true);
  }
}

void music_menu_2(int button) {
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);

  u8g2.setDrawColor(0);
  u8g2.drawBox(0,0,112,32);

  u8g2.setDrawColor(1);
  u8g2.drawGlyph(0, 32, 0x00d3);
  u8g2.drawGlyph(32 + 8, 32, 0x0116);
  u8g2.drawGlyph(64 + 16, 32, 0x0115);
  u8g2.sendBuffer();

  if (button == 1) {
    long start = millis();
    while (digitalRead(button1Pin)) {
      if (millis() - start > 1000) {
        page_selection = 2;
        return;
      }
    }

    client.publish(pause_topic, "1", true);
    client.publish(pause_topic, "0", true);
  } else if (button == 2) {
    client.publish(volume_down_topic, "1", true);
    client.publish(volume_down_topic, "0", true);
  } else if (button == 3) {
    client.publish(volume_up_topic, "1", true);
    client.publish(volume_up_topic, "0", true);
  }
}

//void u8g2_prepare(void) {
//  u8g2.setFont(u8g2_font_6x10_tf);
//  u8g2.setFontRefHeightExtendedText();
//  u8g2.setDrawColor(1);
//  u8g2.setFontPosTop();
//  u8g2.setFontDirection(0);
//}
