#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
LiquidCrystal_I2C lcd(0x27,16,2); // 0x27 - address, LCD (16 column, 2 row)
DHTesp dhtObject;                 // Initialize DHT Object
int temperature, humidity;
const char *ssid = "VA Iphone";
const char *password = "vietanh123";
#define DHT_PIN D3

#define MSG_BUFFER_SIZE (50)    // dung lượng ký tự trong message
char msg_temp[MSG_BUFFER_SIZE]; // mảng dữ liệu message
char msg_humi[MSG_BUFFER_SIZE]; // mảng dữ liệu message

void callback(char *topic, byte *payload, unsigned int length);
void read_dht(int &temp, int &humi);
void show_data(int temp, int data);

void setup()
{
  Serial.begin(9600);
  // ----------------- SETUP DHT Sensor  ---------------------
  pinMode(D3, INPUT);
  dhtObject.setup(DHT_PIN, DHTesp::DHT22);      // pass param into object

  // ----------------- SETUP LCD1602 -------------------------
  lcd.init();             // initialize the lcd 
  lcd.backlight();        // turn on backlight in lcd
  lcd.setCursor(0,0);     // move cursor to col 0, row 0
  lcd.print("Anh.PLV232001M");
  lcd.setCursor(0,1);     // move cursor to col 0, row 1
  lcd.print("My IoT System");
  delay(2000);
  // ----------------- CONNECTING WIFI -----------------------
  lcd.setCursor(0,1);
  lcd.print("Connecting Wifi!");
  Serial.println("Setup Connecting Wifi!");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("+");
  }
  Serial.println("WiFi Connected!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected!");
  delay(1000);
  // ------------------ CONNECTING MQTT SERVER ----------------
  Serial.println("Connecting MQTT!");
  lcd.setCursor(0,1);
  lcd.print("Connecting MQTT!");
  mqttClient.setServer("broker.emqx.io", 1883); // TCP Port - 1883
  mqttClient.setCallback(callback);             // dang ky ham callback de nhan trigger neu co mgs push len topic da duoc subscribe
  Serial.println("MQTT connected!");
  lcd.setCursor(0,1);
  lcd.print("MQTT connected!");
  delay(1000);
}

void loop()
{
  if (!mqttClient.connected())
  {
    // Loop until we're reconnected
    while (!mqttClient.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (mqttClient.connect(clientId.c_str()))
      {
        Serial.println("connected");
        mqttClient.publish("Status", "Sensors Device connected!");
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
  // check MQTT cache for send or receive data
  mqttClient.loop();           
  // Get data from DHT Sensor                 
  read_dht(temperature, humidity);
  show_data(temperature, humidity);
  // Now is pushing data to Topic on MQTT Cloud Server
  snprintf (msg_temp, MSG_BUFFER_SIZE, "%d", temperature);  // chuyen ky tu so sang mang char
  mqttClient.publish("VA_Temperature", msg_temp);                    // publish data len server theo topic VA_TEMP
  snprintf (msg_humi, MSG_BUFFER_SIZE, "%d", humidity);
  mqttClient.publish("VA_Humidity", msg_humi);                    // publish data len server theo topic VA_HUMI
  delay(1000);
}


void read_dht(int &temp, int &humi) {
  // TempAndHumidity data = dhtObject.getTempAndHumidity();
  // temp = data.temperature;
  // humi = data.humidity;
  temp = dhtObject.getTemperature();
  humi = dhtObject.getHumidity();
}

void show_data(int temp, int humi) {
  String stemp = String(temp) + "C";
  String shumid = String(humi) + "%";
  Serial.print("Temperature: " + stemp);
  Serial.println(", Humidity: " + shumid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp : ");   lcd.print(stemp);
  lcd.setCursor(0, 1);
  lcd.print("Humi : ");   lcd.print(shumid);
}


// If any message push to correct Topic which this device subscribe on -> this callback will be triggered
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}