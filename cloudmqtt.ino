#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = D7, TXPin = D8;
static const uint32_t GPSBaud = 9600;
const char* ssid = "FTIM INTLAB FiWi";
const char* password = ""; 
const char* userMQTT = "vawetfuo";
const char* passwordMQTT = "SoHR1xR97Ldw";
const char* mqtt_server = " m10.cloudmqtt.com";
unsigned long interval = 3600000;
SoftwareSerial ss(RXPin,TXPin);
TinyGPSPlus gps;
WiFiClient espClient; 
PubSubClient client(espClient);
void setup_wifi() { 
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }
  static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do 
  {
    pinMode(D7, INPUT);
    while (ss.available())
    gps.encode(ss.read());
  } while (millis() - start < ms);
    pinMode(D7, OUTPUT);
}
void reconnect() {
  // Loop until we're reconnected
  setup_wifi();
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect("BUS2","ytlxebxo","KTDUKgjPltyC")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String message = "" + String(gps.location.lat(),6)+"," + String(gps.location.lng(),6);
      smartDelay(1000);
      client.publish("BUS2", String(message).c_str(), true);
      // ... and resubscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 16406);
  if (client.connect("BUS2",userMQTT,passwordMQTT)) {
    Serial.println("MQTT connected");
  }
}
void loop() {
  if(millis() >= interval) {
    ESP.reset();
  }
  if (client.connected() != 1 || WiFi.status() != WL_CONNECTED){
    reconnect(); 
  }
  String message = "" + String(gps.location.lat(),6)+"," + String(gps.location.lng(),6);
  smartDelay(1000); 
  Serial.println(message);
  client.publish("BUS2",String(message).c_str(), true); 
  client.loop();
}
