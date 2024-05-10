#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient wifiClient;
PubSubClient mqttClient;

#define LED_PIN 2

#define MSG_BUFFER_SIZE	(50)      // dung lượng ký tự trong message
char msg_temp[MSG_BUFFER_SIZE];   // mảng dữ liệu message
char msg_humi[MSG_BUFFER_SIZE];   // mảng dữ liệu message

void wait_wifi_available();
void setup_mqtt();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void send_mqtt_broker(uint8_t temp, uint8_t humi);
void get_mqtt_broker(uint8_t temp, uint8_t humi);


void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  wait_wifi_available();
  setup_mqtt();
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();  // check MQTT cache for send or receive data
  mqttClient.publish("VA", "viet anh hello");
  delay(1000);
}

void wait_wifi_available() {
  WiFi.begin("KhanhHa", "0912166066");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi Connected!");
}

void setup_mqtt() {
  mqttClient.setServer("broker.emqx.io", 1883);    // TCP Port - 1883
  mqttClient.setCallback(callback);                // dang ky ham callback de nhan trigger neu co mgs push len topic da duoc subscribe
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("vietanhTopic", "hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// neu co bat ky mgs gui toi topic ma client nay da subscribe, ham callback se duoc goi
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}