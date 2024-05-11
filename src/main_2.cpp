// #include <Arduino.h>
// #include <PubSubClient.h>
// #include <ESP8266WiFi.h>
// #include <DHTesp.h>
// #include <LiquidCrystal_I2C.h>

// WiFiClient wifiClient;
// PubSubClient mqttClient(wifiClient);
// const char *ssid = "VA Iphone";
// const char *password = "vietanh123";
// #define COOL_PIN D3
// #define DRY_PIN D0

// #define MSG_BUFFER_SIZE (50)    // dung lượng ký tự trong message
// char msg_temp[MSG_BUFFER_SIZE]; // mảng dữ liệu message
// char msg_humi[MSG_BUFFER_SIZE]; // mảng dữ liệu message

// int temperature, humidity;
// bool autoMode = true;
// void callback(char *topic, byte *payload, unsigned int length);

// void setup()
// {
//   pinMode(COOL_PIN, OUTPUT);
//   pinMode(DRY_PIN, OUTPUT);
//   Serial.begin(9600);

//   // -----------------  CONNECTING WIFI -----------------------
//   Serial.println("Setup Connecting Wifi!");
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(1000);
//     Serial.print("+");
//   }
//   Serial.println("WiFi Connected!");

//   // ------------------ CONNECTING MQTT SERVER ----------------
//   Serial.println("Wifi connected => setup Mqqt");
//   mqttClient.setServer("broker.emqx.io", 1883); // TCP Port - 1883
//   mqttClient.setCallback(callback);             // dang ky ham callback de nhan trigger neu co mgs push len topic da duoc subscribe
//   Serial.println("MQTT connected");
// }

// void loop()
// {
//   if (!mqttClient.connected())
//   {
//     // Loop until we're reconnected
//     while (!mqttClient.connected())
//     {
//       Serial.print("Attempting MQTT connection...");
//       // Create a random client ID
//       String clientId = "ESP8266Client-6969";
//       // clientId += String(random(0xffff), HEX);
//       // Attempt to connect
//       if (mqttClient.connect(clientId.c_str()))
//       {
//         Serial.println("connected");
//         mqttClient.publish("Status", "Sensors Device connected!");
//       }
//       else
//       {
//         Serial.print("failed, rc=");
//         Serial.print(mqttClient.state());
//         // Wait 5 seconds before retrying
//         delay(5000);
//       }
//     }
//   }
//   mqttClient.subscribe("VA_Temperature");
//   mqttClient.subscribe("VA_Humidity");
//   // mqttClient.subscribe("VA_Air_Remote");
//   mqttClient.subscribe("VA_Air_Manual");
//   mqttClient.loop();
//   if (autoMode)
//   {
//     if (temperature > 32)
//     {
//       digitalWrite(COOL_PIN, HIGH);
//       mqttClient.publish("VA_Air_Status", "Auto Mode : Turn on COOL Air");
//       // Serial.println("Auto Mode : Turn on COOL Air");
//     }
//     if (temperature <= 32)
//     {
//       digitalWrite(COOL_PIN, LOW);
//       mqttClient.publish("VA_Air_Status", "Auto Mode : Turn off COOL Air");
//       // Serial.println("Auto Mode : Turn off COOL Air");
//     }
//     if (humidity > 89)
//     {
//       digitalWrite(DRY_PIN, HIGH);
//       mqttClient.publish("VA_Air_Status", "Auto Mode : Turn on Dry Air");
//       // Serial.println("Auto Mode : Turn on Dry Air");
//     }
//     if (humidity <= 89)
//     {
//       digitalWrite(DRY_PIN, LOW);
//       mqttClient.publish("VA_Air_Status", "Auto Mode : Turn off Dry Air");
//       // Serial.println("Auto Mode : Turn off Dry Air");
//     }
//   }
//   delay(100);
// }

// // neu co bat ky mgs gui toi topic ma client nay da subscribe, ham callback se duoc goi
// void callback(char *topic, byte *payload, unsigned int length)
// {
//   String strTopic = String(topic);
//   Serial.print("Message arrived [");
//   Serial.print(strTopic);
//   Serial.print("] ");
//   String strBuffer;
//   for (unsigned int i = 0; i < length; i++)
//   {
//     Serial.print((char)payload[i]);
//     strBuffer += (char)payload[i];
//   }
//   Serial.println();
//   // Switch on the LED if an 1 was received as first character
//   if (strTopic == "VA_Temperature")
//   {
//     temperature = strBuffer.toInt();
//     Serial.print("Value Temp : ");
//     Serial.println(temperature);
//   }
//   else if (strTopic == "VA_Humidity")
//   {
//     // humidity = (int)payload[0] * 10 + (int)payload[1];
//     humidity = strBuffer.toInt();
//     Serial.print("Value Humid : ");
//     Serial.println(humidity);
//   }
//   // else if (strTopic == "VA_Air_Remote") {
//   //   if((char)payload[0] == '1') {
//   //     //ON AUTO_MOD
//   //     autoMode = true;
//   //     mqttClient.publish("VA_Air_Status", "Auto Mode is turn on!");
//   //     Serial.println("Auto Mode is turn on!");
//   //   }
//   //   if((char)payload[0] == '0') {
//   //     //OFF AUTO_MOD
//   //     autoMode = false;
//   //     mqttClient.publish("VA_Air_Status", "Auto Mode is turn off!");
//   //     Serial.println("Auto Mode is turn off!");
//   //   }
//   // } else if (strTopic == "VA_Air_Manual") {
//   //   if(autoMode == false && (char)payload[0] == '0') {
//   //     //TURN ON COOL
//   //     digitalWrite(COOL_PIN, LOW);
//   //     mqttClient.publish("VA_Air_Status", "Manual Mode : Turn off COOL Air");
//   //     Serial.println("Manual Mode : Turn off COOL Air");
//   //   }
//   //   if(autoMode == false && (char)payload[0] == '1') {
//   //     //TURN OFF COOL
//   //     digitalWrite(COOL_PIN, HIGH);
//   //     mqttClient.publish("VA_Air_Status", "Manual Mode : Turn on COOL Air");
//   //     Serial.println("Manual Mode : Turn on COOL Air");
//   //   }
//   //   if(autoMode == false && (char)payload[0] == '2') {
//   //     //TURN OFF COOL
//   //     digitalWrite(DRY_PIN, LOW);
//   //     mqttClient.publish("VA_Air_Status", "Manual Mode : Turn off Dry Air");
//   //     Serial.println("Manual Mode : Turn off Dry Air");
//   //   }
//   //   if(autoMode == false && (char)payload[0] == '3') {
//   //     //TURN OFF COOL
//   //     digitalWrite(DRY_PIN, HIGH);
//   //     mqttClient.publish("VA_Air_Status", "Manual Mode : Turn on Dry Air");
//   //     Serial.println("Manual Mode : Turn on Dry Air");
//   //   }
//   // }
// }